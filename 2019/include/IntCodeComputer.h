#pragma once

#include <vector>
#include <deque>
#include <istream>
#include <ostream>
#include <iostream>

enum class ParameterMode
{
    Position,
    Immediate
};

class IntCodeComputer
{
    // Overload of stream operator to default output log data to nowhere
    class NullStream : public std::ostream
    {
        class NullBuffer : public std::streambuf
        {
        public:
            int overflow(int c) { return c; }
        } m_nb;

    public:
        NullStream() : std::ostream(&m_nb) {}
    };

public:
    IntCodeComputer(std::string const& name, std::istream &input, std::ostream &output, std::ostream &log = NullStream())
        : m_name(name), m_input(input), m_output(output), m_log(log)
    {
    }

    std::vector<int> Run(std::vector<int> const &rawInstructions);

private:
    int GetNextParameter();

    template<typename Func>
    void BinaryOp(Func func, std::string const& opName);
    template<typename Func>
    void JumpOp(Func funcCmp, std::string const& cmpName);
    template<typename Func>
    void CmpOp(Func funcCmp, std::string const& cmpName);
    void SaveOp();
    void OutOp();

    std::istream &m_input;
    std::ostream &m_output;
    std::ostream &m_log;
    std::vector<int> m_rawInstructions;

    int m_ip = -1;
    std::deque<ParameterMode> m_modeStack;

    std::string m_name;
};