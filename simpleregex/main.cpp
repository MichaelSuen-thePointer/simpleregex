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
        try
        {
            lex::Generator generator(argv[1]);
            generator.generate_file();
        }
        catch (...)
        {
            std::cout << "error occured." << "\npress any key\n";
            std::cin.get();
        }
    }
    else
    {
        std::cout << "require file";
    }
}