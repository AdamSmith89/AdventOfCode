#include "IntCodeComputer.h"
#include "doctest.h"

#include <sstream>
#include <algorithm>
#include <map>
#include <functional>

namespace
{
    enum class OpCode
    {
        Add = 1,
        Mul = 2,
        Save = 3,
        Output = 4,
        Ret = 99
    };

    struct Instruction
    {
        OpCode m_opCode = OpCode::Ret;
        std::deque<ParameterMode> m_opModes;
    };

    Instruction ParseInstruction(int rawInstruction)
    {
        Instruction instruction;
        instruction.m_opCode = static_cast<OpCode>(rawInstruction % 100);
        rawInstruction /= 100;

        // Add & Mul have 3 parameters - the two operands and the result
        if (instruction.m_opCode == OpCode::Add || instruction.m_opCode == OpCode::Mul)
        {
            for (int i = 0; i < 3; ++i)
            {
                instruction.m_opModes.push_back(static_cast<ParameterMode>(rawInstruction % 10));
                rawInstruction /= 10;
            }
        }
        else if (instruction.m_opCode == OpCode::Save || instruction.m_opCode == OpCode::Output)
        {
            instruction.m_opModes.push_back(static_cast<ParameterMode>(rawInstruction % 10));
            rawInstruction /= 10;
        }

        return instruction;
    }

    TEST_CASE("ParseInstruction_Basic")
    {
        int inputInstruction;
        OpCode expectedOp;
        int expectedModeCount;

        // Don't really like this way of defining "value-parameterized" tests but it's the only supported way so far
        SUBCASE("Add") { inputInstruction = 1;      expectedOp = OpCode::Add;       expectedModeCount = 3; }
        SUBCASE("Mul") { inputInstruction = 2;      expectedOp = OpCode::Mul;       expectedModeCount = 3; }
        SUBCASE("Save") { inputInstruction = 3;     expectedOp = OpCode::Save;      expectedModeCount = 1; }
        SUBCASE("Output") { inputInstruction = 4;   expectedOp = OpCode::Output;    expectedModeCount = 1; }
        SUBCASE("Ret") { inputInstruction = 99;     expectedOp = OpCode::Ret;       expectedModeCount = 0; }

        CAPTURE(inputInstruction);
        CAPTURE(expectedOp);
        CAPTURE(expectedModeCount);

        auto inst = ParseInstruction(inputInstruction);
        CHECK(inst.m_opCode == expectedOp);
        CHECK(inst.m_opModes.size() == expectedModeCount);
        CHECK(std::all_of(inst.m_opModes.begin(), inst.m_opModes.end(), [](auto mode) { return mode == ParameterMode::Position; }));
    }

    TEST_CASE("ParseInstruction_Complex")
    {
        auto inst = ParseInstruction(11002);
        CHECK(inst.m_opCode == OpCode::Mul);
        CHECK(inst.m_opModes.size() == 3);
        CHECK(inst.m_opModes[0] == ParameterMode::Position);
        CHECK(inst.m_opModes[1] == ParameterMode::Immediate);
        CHECK(inst.m_opModes[2] == ParameterMode::Immediate);
    }
} // namespace

std::vector<int> IntCodeComputer::Run(std::vector<int> const &instructions)
{
    m_rawInstructions = instructions;

    std::map<OpCode, std::function<void()>> const OP_CODE_HANDLERS =
    {
        { OpCode::Add, std::bind(&IntCodeComputer::ExecAdd, this) },
        { OpCode::Mul, std::bind(&IntCodeComputer::ExecMul, this) },
        { OpCode::Save, std::bind(&IntCodeComputer::ExecSave, this) },
        { OpCode::Output, std::bind(&IntCodeComputer::ExecOutput, this) }
    };

    while (true)
    {
        auto instruction = ParseInstruction(m_rawInstructions[++m_ip]);

        m_log << "Processing ";
        for (int i = 0; i < 4; ++i)
            m_log << m_rawInstructions[m_ip + i] << ", ";
        m_log << "\n";

        if (instruction.m_opCode == OpCode::Ret)
            break;

        m_modeStack = instruction.m_opModes;
        OP_CODE_HANDLERS.at(instruction.m_opCode)();
    }

    return m_rawInstructions;
}

int IntCodeComputer::GetNextParameter()
{
    auto parameterMode = m_modeStack.front();
    m_modeStack.pop_front();
    
    if (parameterMode == ParameterMode::Position)
    {
        return m_rawInstructions[m_rawInstructions[++m_ip]];
    }
    else if (parameterMode == ParameterMode::Immediate)
    {
        return m_rawInstructions[++m_ip];
    }

    return -1;
}

void IntCodeComputer::ExecAdd()
{
    int operand1 = GetNextParameter();
    int operand2 = GetNextParameter();
    int result = operand1 + operand2;
    
    // Output parameters will always be position mode
    int saveIndex = m_rawInstructions[++m_ip];
    m_rawInstructions[saveIndex] = result;
    m_log << "Put " << operand1 << " + " << operand2 << "(" << result << ") in " << saveIndex << "\n";
}

void IntCodeComputer::ExecMul()
{
    int operand1 = GetNextParameter();
    int operand2 = GetNextParameter();
    int result = operand1 * operand2;

    // Output parameters will always be position mode
    int saveIndex = m_rawInstructions[++m_ip];
    m_rawInstructions[saveIndex] = result;
    m_log << "Put " << operand1 << " * " << operand2 << "(" << result << ") in " << saveIndex << "\n";
}

void IntCodeComputer::ExecSave()
{
    // Output parameters will always be position mode
    int saveInputIndex = m_rawInstructions[++m_ip];
    m_input >> m_rawInstructions[saveInputIndex];

    m_log << "Put input " << m_rawInstructions[saveInputIndex] << " into " << saveInputIndex << "\n";
}

void IntCodeComputer::ExecOutput()
{
    // Output opcode is essentially position mode
    int parameter = GetNextParameter();
    m_output << parameter;

    m_log << "\nOutput " << parameter << "\n";
}

#pragma region Tests
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

    std::stringstream ss;
    ss << 1;
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
    std::vector<int> input = {1002, 4, 3, 4, 33};

    std::stringstream ss;
    std::ostringstream oss;
    IntCodeComputer icc(ss, oss);
    auto output = icc.Run(input);

    std::vector<int> expected = {1002, 4, 3, 4, 99};
    CHECK(output == expected);
}

TEST_CASE("Run_InvalidToValid")
{
    // Mul (02) position(0) (4) and immediate(1) (3) then store at position(0) (4)
    std::vector<int> input = {1101, 100, -1, 4, 0};

    std::stringstream ss;
    std::ostringstream oss;
    IntCodeComputer icc(ss, oss);
    auto output = icc.Run(input);

    std::vector<int> expected = {1101, 100, -1, 4, 99};
    CHECK(output == expected);
}
#pragma endregion