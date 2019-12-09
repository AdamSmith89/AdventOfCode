#include "IntCodeComputer.h"
#include "doctest.h"

std::vector<int> IntCodeComputer::Run(std::vector<int> instructions)
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

    return instructions;
}

TEST_CASE("Run_OpAdd")
{
    // Add (1) position 0 (0) and position 0 (0) then store at position 0 (0)
    std::vector<int> input = {1, 0, 0, 0, 99};
    IntCodeComputer icc;
    auto output = icc.Run(input);

    std::vector<int> expected = {2, 0, 0, 0, 99};
    CHECK(input == expected);
}