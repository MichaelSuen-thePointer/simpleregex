#include <vld.h>
#include <iostream>
#include <iterator>
#include "simpleregex.h"

using namespace pl;
using namespace pl::regex;

int main()
{
    Regex regex1("abdce");
    Regex regex2("abdce");
    auto result1 = regex1 + regex2;

    auto result2 = regex1 | regex2;

    auto result3 = *regex1;

    regex1 += regex1 + regex2;
}