//#include <vld.h>
#include <iostream>
#include <iterator>

#include "tokenizergenerator.h"

using namespace pl;
using namespace pl::regex;

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        lex::Generator generator(argv[1]);
        generator.generate_file();
    }
    else
    {
        std::cout << "require file";
    }
}