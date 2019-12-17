#pragma once

#include <sstream>
#include <ostream>

class AtomicLogger
{
public:
    explicit AtomicLogger(std::ostream& output)
        : m_output(output)
    {}

    ~AtomicLogger()
    {
        m_output << m_internal.str();
    }

    template<typename T>
    AtomicLogger& operator << (T const& t)
    {
        m_internal << t;
        return *this;
    }

private:
    std::ostringstream m_internal;
    std::ostream& m_output;
};