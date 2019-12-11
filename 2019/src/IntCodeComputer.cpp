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
        Out = 4,
        JmpT = 5,
        JmpF = 6,
        Less = 7,
        Eq = 8,
        Ret = 99
    };

    std::map<OpCode, int> OpParams = 
    {
        { OpCode::Add, 3 },
        { OpCode::Mul, 3 },
        { OpCode::Save, 1 },
        { OpCode::Out, 1 },
        { OpCode::JmpT, 2 },
        { OpCode::JmpF, 2 },
        { OpCode::Less, 3 },
        { OpCode::Eq, 3 },
        { OpCode::Ret, 0 }
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

        for (int i = 0; i < OpParams[instruction.m_opCode]; ++i)
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
        SUBCASE("Out") { inputInstruction = 4;      expectedOp = OpCode::Out;       expectedModeCount = 1; }
        SUBCASE("JmpT") { inputInstruction = 5;     expectedOp = OpCode::JmpT;      expectedModeCount = 2; }
        SUBCASE("JmpF") { inputInstruction = 6;     expectedOp = OpCode::JmpF;      expectedModeCount = 2; }
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

    while (true)
    {
        auto instruction = ParseInstruction(m_rawInstructions[++m_ip]);

        m_log << "Processing ";
        for (int i = 0; i <= OpParams[instruction.m_opCode]; ++i)
            m_log << m_rawInstructions[m_ip + i] << ", ";
        m_log << "\n";

        if (instruction.m_opCode == OpCode::Ret)
            break;

        m_modeStack = instruction.m_opModes;

        if (instruction.m_opCode == OpCode::Add)
        {
            BinaryOp(std::plus<int>(), "+");
        }
        else if (instruction.m_opCode == OpCode::Mul)
        {
            BinaryOp(std::multiplies<int>(), "*");
        }
        else if (instruction.m_opCode == OpCode::Save)
        {
            SaveOp();
        }
        else if (instruction.m_opCode == OpCode::Out)
        {
            OutOp();
        }
        else if (instruction.m_opCode == OpCode::JmpT)
        {
            JumpOp(std::not_equal_to<int>(), "!=");
        }
        else if (instruction.m_opCode == OpCode::JmpF)
        {
            JumpOp(std::equal_to<int>(), "==");
        }
        else if (instruction.m_opCode == OpCode::Less)
        {
            CmpOp(std::less<int>(), "<");
        }
        else if (instruction.m_opCode == OpCode::Eq)
        {
            CmpOp(std::equal_to<int>(), "==");
        }
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

template<typename Func>
void IntCodeComputer::BinaryOp(Func func, std::string const& opName)
{
    int parameter1 = GetNextParameter();
    int parameter2 = GetNextParameter();
    int result = func(parameter1, parameter2);
    
    // Output parameters will always be position mode
    int saveIndex = m_rawInstructions[++m_ip];
    m_rawInstructions[saveIndex] = result;
    m_log << "Put " << parameter1 << " " << opName << " " << parameter2 << "(" << result << ") in " << saveIndex << "\n";
}

template<typename Func>
void IntCodeComputer::JumpOp(Func funcCmp, std::string const& cmpName)
{
    int parameter1 = GetNextParameter();
    int parameter2 = GetNextParameter();

    m_log << "Testing " << parameter1 << " " << cmpName << " 0\n";
    if (funcCmp(parameter1, 0))
    {
        m_ip = parameter2 - 1; // Main loop will advance by 1 so work-around...
        m_log << "Passed. Set ip to " << parameter2 << "\n";
    }
    else
    {
        m_log << "Failed. Leaving ip as-is\n";
    }
}

template<typename Func>
void IntCodeComputer::CmpOp(Func funcCmp, std::string const& cmpName)
{
    int parameter1 = GetNextParameter();
    int parameter2 = GetNextParameter();
    int resultIndex = m_rawInstructions[++m_ip];

    m_log << "Testing " << parameter1 << " " << cmpName << " " << parameter2 << "\n";
    m_rawInstructions[resultIndex] = funcCmp(parameter1, parameter2) ? 1 : 0;
    m_log << "Set position " << resultIndex << " to " << m_rawInstructions[resultIndex] << "\n";
}

void IntCodeComputer::SaveOp()
{
    // Output parameters will always be position mode
    int saveInputIndex = m_rawInstructions[++m_ip];
    m_input >> m_rawInstructions[saveInputIndex];

    m_log << "Put input " << m_rawInstructions[saveInputIndex] << " into " << saveInputIndex << "\n";
}

void IntCodeComputer::OutOp()
{
    // Output opcode is essentially position mode
    int parameter = GetNextParameter();
    m_output << parameter;

    m_log << "\nOutput " << parameter << "\n";
}

#pragma region Tests
namespace std
{
std::ostream& operator<< (std::ostream& os, std::vector<int> const& vec) {
    std::for_each(vec.begin(), vec.end() - 1, [&os](auto const& entry) { os << entry << ", "; });
    os << vec.back();
    return os;
}
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

TEST_CASE("Run_JmpT_InputIsZero")
{
    // Save (3) input to position (3), JmpT (1105) if immediate value (-1 [set by input]) is non-zero to immediate position (9).
    // It is zero so Add (1101) immediate value (0) and immediate value (0), then store in position (12).
    // Output (4) value in position (12). Then ret (99)
    //                        0  1    2    3  4     5  6  7   8  9  10  11  12
    std::vector<int> input = {3, 3, 1105, -1, 9, 1101, 0, 0, 12, 4, 12, 99, 1};

    std::stringstream ss;
    ss << 0;
    std::ostringstream oss;
    IntCodeComputer icc(ss, oss);
    auto output = icc.Run(input);

    std::vector<int> expected = {3, 3, 1105, 0, 9, 1101, 0, 0, 12, 4, 12, 99, 0};
    CHECK(output == expected);
    CHECK(oss.str() == "0");
}

TEST_CASE("Run_JmpT_InputIsNonZero")
{
    // Save (3) input to position (3), JmpT (1105) if immediate value (-1 [set by input]) is non-zero to immediate position (9).
    // Output (4) value in position (12). Then ret (99)
    //                        0  1    2    3  4     5  6  7   8  9  10  11  12
    std::vector<int> input = {3, 3, 1105, -1, 9, 1101, 0, 0, 12, 4, 12, 99, 1};

    std::stringstream ss;
    ss << 8;
    std::ostringstream oss;
    IntCodeComputer icc(ss, oss);
    auto output = icc.Run(input);

    std::vector<int> expected = {3, 3, 1105, 8, 9, 1101, 0, 0, 12, 4, 12, 99, 1};
    CHECK(output == expected);
    CHECK(oss.str() == "1");
}

TEST_CASE("Run_JmpF_InputIsZero")
{
    // Save (3) input to position (12), JmpF (6) if position (12) is zero to index at position (15).
    // Output (4) value in position (13). Then ret (99)
    //                        0   1  2   3   4  5   6   7   8  9  10  11  12 13 14 15
    std::vector<int> input = {3, 12, 6, 12, 15, 1, 13, 14, 13, 4, 13, 99, -1, 0, 1, 9};

    std::stringstream ss;
    ss << 0;
    std::ostringstream oss;
    IntCodeComputer icc(ss, oss);
    auto output = icc.Run(input);

    std::vector<int> expected = {3, 12, 6, 12, 15, 1, 13, 14, 13, 4, 13, 99, 0, 0, 1, 9};
    CHECK(output == expected);
    CHECK(oss.str() == "0");
}

TEST_CASE("Run_JmpF_InputIsNonZero")
{
    // Save (3) input to position (12), JmpF (6) if position (12) is zero to index at position (15).
    // It's non-zero so Add (1) value at position (13) and value at position (14), then store at (13).
    // Output (4) value in position (13). Then ret (99)
    //                        0   1  2   3   4  5   6   7   8  9  10  11  12 13 14 15
    std::vector<int> input = {3, 12, 6, 12, 15, 1, 13, 14, 13, 4, 13, 99, -1, 0, 1, 9};

    std::stringstream ss;
    ss << 8;
    std::ostringstream oss;
    IntCodeComputer icc(ss, oss);
    auto output = icc.Run(input);

    std::vector<int> expected = {3, 12, 6, 12, 15, 1, 13, 14, 13, 4, 13, 99, 8, 1, 1, 9};
    CHECK(output == expected);
    CHECK(oss.str() == "1");
}

TEST_CASE("Run_LessThan8_InputIs0")
{
    // Save (3) input to position (9), Check Less (7), is value at position (9) < than value at position (10).
    // It will be so store 1 at position (9). Output (4) value in position (9). Then ret (99)
    //                        0  1  2  3   4  5  6  7   8   9  10
    std::vector<int> input = {3, 9, 7, 9, 10, 9, 4, 9, 99, -1, 8};

    std::stringstream ss;
    ss << 0;
    std::ostringstream oss;
    IntCodeComputer icc(ss, oss);
    auto output = icc.Run(input);

    std::vector<int> expected = {3, 9, 7, 9, 10, 9, 4, 9, 99, 1, 8};
    CHECK(output == expected);
    CHECK(oss.str() == "1");
}

TEST_CASE("Run_LessThan8_InputIs9")
{
    // Save (3) input to position (9), Check Less (7), is value at position (9) < than value at position (10).
    // It won't be so store 0 at position (9). Output (4) value in position (9). Then ret (99)
    //                        0  1  2  3   4  5  6  7   8   9  10
    std::vector<int> input = {3, 9, 7, 9, 10, 9, 4, 9, 99, -1, 8};

    std::stringstream ss;
    ss << 9;
    std::ostringstream oss;
    IntCodeComputer icc(ss, oss);
    auto output = icc.Run(input);

    std::vector<int> expected = {3, 9, 7, 9, 10, 9, 4, 9, 99, 0, 8};
    CHECK(output == expected);
    CHECK(oss.str() == "0");
}

TEST_CASE("Run_EqualTo8_InputIs8")
{
    // Save (3) input to position (3), Check Equal (7), is immediate value (-1 [set by input]) == immediate value (8).
    // It will be so store 1 at position (3). Output (4) value in position (3). Then ret (99)
    //                        0  1     2   3  4  5  6  7   8
    std::vector<int> input = {3, 3, 1108, -1, 8, 3, 4, 3, 99};

    std::stringstream ss;
    ss << 8;
    std::ostringstream oss;
    IntCodeComputer icc(ss, oss);
    auto output = icc.Run(input);

    std::vector<int> expected = {3, 3, 1108, 1, 8, 3, 4, 3, 99};
    CHECK(output == expected);
    CHECK(oss.str() == "1");
}

TEST_CASE("Run_EqualTo8_InputIs9")
{
    // Save (3) input to position (3), Check Equal (7), is immediate value (-1 [set by input]) == immediate value (8).
    // It won't be so store 0 at position (3). Output (4) value in position (3). Then ret (99)
    //                        0  1     2   3  4  5  6  7   8
    std::vector<int> input = {3, 3, 1108, -1, 8, 3, 4, 3, 99};

    std::stringstream ss;
    ss << 9;
    std::ostringstream oss;
    IntCodeComputer icc(ss, oss);
    auto output = icc.Run(input);

    std::vector<int> expected = {3, 3, 1108, 0, 8, 3, 4, 3, 99};
    CHECK(output == expected);
    CHECK(oss.str() == "0");
}
#pragma endregion