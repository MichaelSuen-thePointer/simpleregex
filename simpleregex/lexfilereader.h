#pragma once
#ifndef LEX_FILE_READER
#define LEX_FILE_READER

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
namespace pl
{
using std::string;
using std::vector;

string operator""_s(const char* str, size_t)
{
    return string(str);
}
class LexReader
{
public:
    struct Rule
    {
        int label;
        string name;
        string regex;
    };
protected:
    string _className;
    string _classNameUpperCase;
    vector<Rule> _rules;

public:
    const string& class_name() { return _className; }
    const string& class_name_upper_case() { return _classNameUpperCase; }
    const vector<Rule>& rules() { return _rules; }

    void read_file(const string& filePath)
    {
        std::ifstream lexFile(filePath);
        constexpr size_t bufsize = 256;
        char lineBuf[bufsize];
        while (!lexFile.eof())
        {

            lexFile.getline(lineBuf, bufsize);

            size_t length = strlen(lineBuf);
            if (length == 0)
            {
                continue;
            }

            std::istringstream line(lineBuf);

            line.getline(lineBuf, bufsize, ':');
            if (lineBuf == "ClassName"_s)
            {
                line.getline(lineBuf, bufsize);
                _className = lineBuf;
                _classNameUpperCase = lineBuf;
                std::transform(_className.begin(), _className.end(), _classNameUpperCase.begin(), ::toupper);
            }
            else if (lineBuf == "Rules"_s)
            {
                read_rules(lexFile);
                return;
            }
        }
    }
    void read_rules(std::ifstream& lexFile)
    {
        constexpr size_t bufsize = 256;
        char lineBuf[bufsize];

        while (!lexFile.eof())
        {
            lexFile.getline(lineBuf, bufsize);
            if (strlen(lineBuf) == 0)
            {
                continue;
            }
            std::istringstream sstream(lineBuf);
            sstream.getline(lineBuf, bufsize, ':');
            string regex(lineBuf);
            sstream.getline(lineBuf, bufsize);
            string name(lineBuf);

            _rules.push_back(Rule{static_cast<int>(_rules.size() + 1), name, regex});
        }
    }
};
}

#endif // !LEX_FILE_READER
