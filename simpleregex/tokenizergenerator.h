#pragma once

#ifndef TOKENIZER_GENERATOR_H
#define TOKENIZER_GENERATOR_H

#include <fstream>
#include <algorithm>    
#include "simpleregexfsm.h"
#include "lexfilereader.h"

namespace pl
{
namespace lex
{

using namespace regex;

class Generator
{
protected:
    LexReader reader;
    FSM fsm;
    vector<string> enumEntry;

protected:
    void output_header(std::ofstream& file)
    {
        output_preprocess_statement(file);
        
        output_class_structure(file);

        output_preprocess_statement_end(file);
    }
    void output_cpp(std::ofstream& file)
    {
        output_cpp_include(file);
        output_cpp_static_member(file);
        output_cpp_member_function(file);
    }
    void output_preprocess_statement(std::ofstream& file)
    {
        file << "#pragma once\n";
        file << "#ifndef " << reader.class_name_upper_case() << "_H\n";
        file << "#define " << reader.class_name_upper_case() << "_H\n";

        file << "#include <fstream>\n";
        file << "#include <string>\n";
        file << "#include <map>\n";
    }
    void output_preprocess_statement_end(std::ofstream& file)
    {
        file << "#endif";
    }
    void output_class_structure(std::ofstream& file)
    {
        output_exception_structure(file);
        output_state_info_structure(file);

        file << "class " << get_class_name();
        file << "{" << "\n";
        file << "public:\n";
        
        output_enum_structure(file);
        output_token_structure(file);

        file << "protected:" << "\n";
        file << "    static int _stateMachine[" << fsm.state_count() << "][256];\n";
        file << "    static int _invalidState;\n";
        file << "    static std::map<int, int> _endStates;\n";
        file << "    int _lastState;\n";
        file << "    int _line, _column;\n";
        file << "    std::ifstream _file;\n";
        file << "    Token _prefetch;\n";
        file << "public:\n";
        file << "    " << get_class_name() << "(const std::string& file)\n";
        file << "    : _file(file)\n";
        file << "    {\n";
        file << "        prefetch();\n";
        file << "    }\n";
        file << "    Token get();\n";
        file << "    Token peek();\n";
        file << "private:\n";
        file << "    void prefetch();\n";
        file << "};\n";
    }
    void output_enum_structure(std::ofstream& file)
    {
        file << "    enum TokenKind\n";
        file << "    {\n";
        //file << "       END = 0";
        for (size_t i = 0; i != enumEntry.size(); i++)
        {
            file << "        " << enumEntry[i] << " = " << i << ",\n";
        }
        file.seekp(-2, file.cur);
        file << "\n   };\n";
    }
    void output_token_structure(std::ofstream& file)
    {
        file << "    struct Token\n";
        file << "    {\n";
        file << "        TokenKind kind;\n";
        file << "        std::string content;\n";
        file << "        int line, column;\n";
        file << "    };\n";
    }
    void output_exception_structure(std::ofstream& file)
    {
        file << R"__(class BadToken: std::runtime_error
{
public:
    int line, column;
    std::string content;
    BadToken(const std::string& _content, int _line, int _column)
        : std::runtime_error("bad token")
        , line(_line)
        , column(_column)
        , content(_content)
    {
    }
    BadToken(char _content, int _line, int _column)
        : std::runtime_error("bad token")
        , line(_line)
        , column(_column)
        , content{_content}
    {
    }
};)__";
    }
    void output_state_info_structure(std::ofstream& file)
    {
        file << R"__(
struct StateInfo
{
    int label;
    std::string name;
    StateInfo(int _label = -1, std::string _name = "")
        : label(_label)
        , name(_name)
    {
    }
};
)__";
    }
    void output_cpp_include(std::ofstream& file)
    {
        file << "#include \"" << reader.class_name() << ".h\"\n";
    }
    void output_cpp_static_member(std::ofstream& file)
    {
        file << "int " << get_class_name() << "::_stateMachine[" << fsm.state_count() << "][256] = {\n";
        for (auto& line : fsm.state_machine())
        {
            file << "{";
            std::copy(line.begin(), line.end(), std::ostream_iterator<int>(file, ","));
            file.seekp(-1, file.cur);
            file << "},\n";
        }
        file.seekp(-3, file.cur);
        file << "\n};\n";

        file << "int " << get_class_name() << "::_invalidState = " << fsm.invalid_state() << ";\n";

        file << "std::map<int, int> " << get_class_name() << "::_endStates =\n";
        file << "{\n";
        for (auto& pair : fsm.end_states())
        {
            file << "{" << pair.first << ", " << pair.second.label << "},\n";
        }
        file.seekp(-3, file.cur);
        file << "\n};\n";
    }
    void output_cpp_member_function(std::ofstream& file)
    {
        output_memfunc_prefetch(file);
        output_memfunc_peek(file);
        output_memfunc_get(file);
    }
    void output_memfunc_prefetch(std::ofstream& file)
    {
        file <<"void " <<  get_class_name() << "::prefetch()";
        file << R"__(
{
    std::string matched;
    while (_file)
    {
        char ch = _file.peek();
        if (ch == '\n')
        {
            _column++;
            _line = 0;
            continue;
        }
        _line++;
        if (_stateMachine[_lastState][ch] != _invalidState)
        {
            _lastState = _stateMachine[_lastState][ch];
            matched.push_back(ch);
        }
        else if (matched.length() == 0)
        {
            throw BadToken(ch, _line, _column);
        }
        else
        {
            if(_endStates.find(_lastState) == _endStates.end())
            {
                throw BadToken(matched, _line, _column);
            }
            _lastState = 0;
            _file.get();
            int kind = _endStates[_lastState];
            _prefetch = Token{TokenKind(kind), matched, _line, _column};
            return;
        }
    }
    if (matched.length())
    {
        throw BadToken(matched, _line, _column);
    }
    _prefetch = Token{TokenKind::End, matched, _line, _column};
}

)__";
    }
    void output_memfunc_peek(std::ofstream& file)
    {
        file << get_class_name() << "::Token " << get_class_name() << "::peek()";
        file << R"__(
{
    return _prefetch;
}
)__";
    }
    void output_memfunc_get(std::ofstream& file)
    {
        file << get_class_name() << "::Token " << get_class_name() << "::get()";
        file << R"__(
{
    Token got = _prefetch;
    prefetch();
    return got;
}
)__";
    }
    const string& get_class_name()
    {
        return reader.class_name();
    }
public:
    Generator(const string& fileAddr)
    {
        EpsilonNFA eNfa;
        reader.read_file(fileAddr);
        enumEntry.insert(enumEntry.end(), reader.rules().size() + 1, string());
        enumEntry[0] = "END";
        for (auto& entry : reader.rules())
        {
            eNfa.combine_regex(entry.regex, StateInfo(entry.label, entry.name));
            enumEntry[entry.label] = entry.name;
        }
        fsm = FSM(DFA(NFA(eNfa)));
    }
    void generate_file()
    {
        std::ofstream hfile(reader.class_name() + ".h");
        std::ofstream cppfile(reader.class_name() + ".cpp");

        output_header(hfile);
        output_cpp(cppfile);
    }



};

}
}
#endif // !TOKENIZER_GENERATOR_H
