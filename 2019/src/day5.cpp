#include "day5.h"
#include "doctest.h"

#include <vector>
#include <fstream>
#include <string>

namespace day5
{
void ProcessInstructions(std::vector<int> &instructions)
{
    int const OP_ADD = 1;
    int const OP_MUL = 2;
    int const OP_RET = 99;

    for (size_t index = 0; index < instructions.size(); ++index)
    {
        int const opcode = instructions[index];
        if (opcode == OP_RET)
            break;

        int operand1 = instructions[instructions[++index]];
        int operand2 = instructions[instructions[++index]];
        int resultIndex = instructions[++index];

        if (opcode == OP_ADD)
        {
            instructions[resultIndex] = operand1 + operand2;
        }
        else if (opcode == OP_MUL)
        {
            instructions[resultIndex] = operand1 * operand2;
        }
    }
}

int Calculate()
{
    std::vector<int> instructions;

    std::ifstream ifs("resources/day5.txt");
    std::string digit;
    while (std::getline(ifs, digit, ','))
    {
        instructions.push_back(std::atoi(digit.c_str()));
    }

    return 0;
}
} // namespace day5

TEST_CASE("ProcessInstructions")
{
    
}