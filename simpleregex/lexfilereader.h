#pragma once
#ifndef LEX_FILE_READER
#define LEX_FILE_READER

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "LexReaderTokenizer.h"
namespace pl
{
using std::string;
using std::vector;

class ParseError: public std::runtime_error
{
public:
    ParseError(const string& msg)
        : std::runtime_error(msg)
    {
    }
};

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

    void expect(const LexReaderTokenizer::Token& token, const LexReaderTokenizer::TokenKind kind, const string& description)
    {
        if (token.kind != kind)
        {
            throw ParseError("expect token: \'" + description + "\'");
        }
    }
public:
    const string& class_name() { return _className; }
    const string& class_name_upper_case() { return _classNameUpperCase; }
    const vector<Rule>& rules() { return _rules; }

    void read_file(const string& filePath)
    {
        LexReaderTokenizer tokenizer(filePath);
        while (tokenizer.peek().kind != tokenizer.End)
        {
            auto token = tokenizer.get();
            if (token.kind == tokenizer.ClassName)
            {
                token = tokenizer.get();
                expect(token, tokenizer.Colon, ":");
                token = tokenizer.get();
                expect(token, tokenizer.Regex, "a name");
                _className = token.content;
                _classNameUpperCase = _className;
                std::transform(_className.begin(), _className.end(), _classNameUpperCase.begin(), [](char ch) {return ::toupper(ch); });
            }
            if (token.kind == tokenizer.Rules)
            {
                token = tokenizer.get();
                expect(token, tokenizer.Colon, ":");
                read_rules(tokenizer);
            }
        }
    }
    void read_rules(LexReaderTokenizer& tokenizer)
    {
        while (tokenizer.peek().kind != tokenizer.End)
        {
            auto regex = tokenizer.get();
            expect(tokenizer.get(), tokenizer.Colon, ":");
            auto ruleName = tokenizer.get();

            _rules.push_back(Rule{static_cast<int>(_rules.size() + 1), ruleName.content, regex.content});
        }
    }
};
}

#endif // !LEX_FILE_READER
