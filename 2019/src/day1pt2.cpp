#include <fstream>
#include <algorithm>
#include <iostream>

#include "day1pt2.h"
#include "day1.h"

namespace day1pt2
{
    int CalculateFuelForFuel(int const& fuelMass)
    {
        return std::max<int>(0, day1::CalculateFuelForMass(fuelMass));
    }

    int Calculate()
    {
        std::ifstream ifs("resources/day1.txt");

        int totalFuel = 0;
        int mass = 0;
        while (ifs >> mass)
        {
            std::cout << "Calculating fuel for " << mass << std::endl;

            int moduleFuel = day1::CalculateFuelForMass(mass);

            std::cout << "  Module:" << std::endl;
            std::cout << "    -> " << moduleFuel << std::endl;

            std::cout << "  Extra:" << std::endl;
            int extraFuel = moduleFuel;
            while((extraFuel = CalculateFuelForFuel(extraFuel)) > 0)
            {
                std::cout << "    -> " << extraFuel << std::endl;
                moduleFuel += extraFuel;
            }

            totalFuel += moduleFuel;
            std::cout << "  Total: " << moduleFuel << std::endl;
            std::cout << "Running Total: " << totalFuel << std::endl << std::endl;
        }
        
        return totalFuel;
    }
}