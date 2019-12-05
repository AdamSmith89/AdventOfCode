#include "day2.h"

#include <vector>
#include <fstream>
#include <string>
#include <iostream>

//#define pt1
#define pt2

namespace day2
{
    void ProcessInstructions(std::vector<int>& instructions)
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

        std::ifstream ifs("resources/day2.txt");
        std::string digit;
        while (std::getline(ifs, digit, ','))
        {
            instructions.push_back(std::atoi(digit.c_str()));
        }

#ifdef pt1
        instructions[1] = 12;
        instructions[2] = 2;
        
        ProcessInstructions(instructions);
        return instructions[0];
#endif

#ifdef pt2
        int const TARGET_RESULT = 19690720;
        
        for (int noun=0; noun < 100; ++noun)
        {
            for (int verb=0; verb < 100; ++verb)
            {
                std::cout << "Testing noun = " << noun << ", verb = " << verb << std::endl;
                std::vector<int> tempInstructions = instructions;
                tempInstructions[1] = noun;
                tempInstructions[2] = verb;

                ProcessInstructions(tempInstructions);

                std::cout << "    Result = " << tempInstructions[0] << std::endl;
                if (tempInstructions[0] == TARGET_RESULT)
                {
                    return (100 * noun) + verb;
                }
            }
        }
#endif
    }
}