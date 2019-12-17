#include "day5.h"
#include "doctest.h"
#include "IntCodeComputer.h"

#include <fstream>
#include <string>
#include <iostream>

namespace day5
{
int Calculate()
{
    std::vector<int> instructions;

    std::ifstream ifs("resources/day5.txt");
    std::string digit;
    while (std::getline(ifs, digit, ','))
    {
        instructions.push_back(std::atoi(digit.c_str()));
    }

    IntCodeComputer icc("ICC", std::cin, std::cout, std::cerr);
    icc.Run(instructions);

    std::cout << std::endl;
    return 0;
}
} // namespace day5