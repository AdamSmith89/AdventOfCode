#include "day2.h"

#include <vector>
#include <fstream>
#include <string>

namespace day2
{
    int Calculate()
    {
        int const OP_ADD = 1;
        int const OP_MUL = 2;
        int const OP_RET = 99;

        std::vector<int> opcodes;

        std::ifstream ifs("resources/day2.txt");
        std::string digit;
        while (std::getline(ifs, digit, ','))
        {
            opcodes.push_back(std::atoi(digit.c_str()));
        }

        for (size_t index = 0; index < opcodes.size(); ++index)
        {
            int const opcode = opcodes[index];
            if (opcode == OP_RET)
                break;

            int operand1 = opcodes[opcodes[++index]];
            int operand2 = opcodes[opcodes[++index]];
            int resultIndex = opcodes[++index];

            if (opcode == OP_ADD)
            {
                opcodes[resultIndex] = operand1 + operand2;
            }
            else if (opcode == OP_MUL)
            {
                opcodes[resultIndex] = operand1 * operand2;
            }
        }

        return opcodes[0];
    }
}