#include <iostream>
#include "..\simpleregex\Tokenizer.h"

int main()
{
    Tokenizer lex("keywordTest.txt");

    try
    {
        while (lex.peek().kind != lex.EndToken)
        {

            auto token = lex.get();

            std::cout << token.kind << ' ' << token.content << ' ' << token.line << ' ' << token.column << '\n';
        }
    }
    catch (BadToken bd)
    {
        std::cout << bd.line << ' ' << bd.column << ' ' << bd.content << '\n';
    }
}