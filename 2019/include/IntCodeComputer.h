#pragma once

#include <vector>
#include <istream>
#include <ostream>

class IntCodeComputer
{
public:
    IntCodeComputer(std::istream& input, std::ostream& output)
    : m_input(input), m_output(output)
    {}
    
    std::vector<int> Run(std::vector<int> const& instructions);

private:
    void ExecAdd();
    void ExecMul();
    void ExecSav();
    void ExecOut();

    std::istream& m_input;
    std::ostream& m_output;
    std::vector<int> m_instructions;
    int m_op = 0;
    int m_ip = 0;
};