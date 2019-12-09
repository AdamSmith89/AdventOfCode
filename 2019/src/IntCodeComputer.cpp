#include "IntCodeComputer.h"
#include "doctest.h"

#include <sstream>

struct OpCode
{
    int code;
    int count;
};

std::vector<int> IntCodeComputer::Run(std::vector<int> const& instructions)
{
    m_instructions = instructions;

    int const OP_ADD = 1;
    int const OP_MUL = 2;
    int const OP_SAV = 3;
    int const OP_OUT = 4;
    int const OP_RET = 99;

    while (m_instructions[m_ip] != OP_RET)
    {
        // 1002
        // op % 100 will give last two digits - 02
        //  - that's the actual opcode
        // % 10 to get each of the next digits in turn (the amount is based on the code, for a mul we need 3 but for a out we only need 1)
        //  - 0 = position mode
        //  - 1 = immediate mode

        /*
        int wholeOp = m_instructions[m_ip];
        m_op = wholeOp % 100;
        wholeOp /= 100;
        for (int i = 0; i < OPS[m_op].NumOperands; ++i)
        {
            operandModes.push_back(wholeOp % 10);
            wholeOp /= 10;
        }
        */

        m_op = m_instructions[m_ip];
        if (m_op == OP_RET)
            break;

        if (m_op == OP_ADD)
        {
            ExecAdd();
        }
        else if (m_op == OP_MUL)
        {
            ExecMul();
        }
        else if (m_op == OP_SAV)
        {
            ExecSav();
        }
        else if (m_op == OP_OUT)
        {
            ExecOut();
        }

        m_ip++;
    }

    return m_instructions;
}

void IntCodeComputer::ExecAdd()
{
    int operand1 = m_instructions[m_instructions[++m_ip]];
    int operand2 = m_instructions[m_instructions[++m_ip]];
    m_instructions[m_instructions[++m_ip]] = operand1 + operand2;
}

void IntCodeComputer::ExecMul()
{
    int operand1 = m_instructions[m_instructions[++m_ip]];
    int operand2 = m_instructions[m_instructions[++m_ip]];
    m_instructions[m_instructions[++m_ip]] = operand1 * operand2;
}

void IntCodeComputer::ExecSav()
{
    int saveInputIndex = m_instructions[++m_ip];
    m_input >> m_instructions[saveInputIndex];
}

void IntCodeComputer::ExecOut()
{
    int outputIndex = m_instructions[++m_ip];
    m_output << m_instructions[outputIndex];
}

TEST_CASE("Run_OpAdd")
{
    // Add (1) position (0) and position (0) then store at position (0)
    std::vector<int> input = {1, 0, 0, 0, 99};
    
    std::stringstream ss;
    std::ostringstream oss;
    IntCodeComputer icc(ss, oss);
    auto output = icc.Run(input);

    std::vector<int> expected = {2, 0, 0, 0, 99};
    CHECK(output == expected);
}

TEST_CASE("Run_OpMul")
{
    // Mul (2) position (3) and position (0) then store at position (3)
    std::vector<int> input = {2, 3, 0, 3, 99};

    std::stringstream ss;
    std::ostringstream oss;
    IntCodeComputer icc(ss, oss);
    auto output = icc.Run(input);

    std::vector<int> expected = {2, 3, 0, 6, 99};
    CHECK(output == expected);
}

TEST_CASE("Run_OpSav")
{
    // Save (3) from input into position (0)
    std::vector<int> input = {3, 0, 99};

    std::stringstream ss;
    ss << 1;
    std::ostringstream oss;
    IntCodeComputer icc(ss, oss);
    auto output = icc.Run(input);

    std::vector<int> expected = {1, 0, 99};
    CHECK(output == expected);
}

TEST_CASE("Run_OpOut")
{
    // Output (4) value at position (0)
    std::vector<int> input = {4, 0, 99};
    
    std::stringstream ss;
    std::ostringstream oss;
    IntCodeComputer icc(ss, oss);
    auto output = icc.Run(input);

    std::vector<int> expected = {4, 0, 99};
    CHECK(output == expected);
    CHECK(oss.str() == "4");
}

TEST_CASE("Run_OutputTheInput")
{
    // Save (3) from input into position (0) then Output (4) value at position (0)
    std::vector<int> input = {3, 0, 4, 0, 99};

    std::stringstream ss; ss << 1;
    std::ostringstream oss;
    IntCodeComputer icc(ss, oss);
    auto output = icc.Run(input);

    std::vector<int> expected = {1, 0, 4, 0, 99};
    CHECK(output == expected);
    CHECK(oss.str() == "1");
}

TEST_CASE("Run_MixedModes")
{
    // Mul (02) position(0) (4) and immediate(1) (3) then store at position(0) (4)
    std::vector<int> input = {1002,4,3,4,33};

    std::stringstream ss;
    std::ostringstream oss;
    IntCodeComputer icc(ss, oss);
    auto output = icc.Run(input);

    std::vector<int> expected = {1002,4,3,4,99};
    CHECK(output == expected);
}