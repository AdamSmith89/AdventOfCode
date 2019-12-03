#include "day1.h"
#include <cmath>
#include <fstream>

namespace day1
{
    int Calculate()
    {
        std::ifstream ifs("resources/day1.txt");

        int totalFuel = 0;
        int mass = 0;
        while (ifs >> mass)
        {
            totalFuel += CalculateFuelForMass(mass);
        }
        
        return totalFuel;
    }

    int CalculateFuelForMass(int const& mass)
    {
        return std::floor(mass / 3) - 2;
    }
}