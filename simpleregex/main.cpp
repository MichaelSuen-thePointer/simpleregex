#include <vld.h>
#include <iostream>
#include <iterator>
#include "tokenizergenerator.h"

using namespace pl;
using namespace pl::regex;

int main()
{
    lex::Generator generator("lex.txt");

    generator.generate_file();
}