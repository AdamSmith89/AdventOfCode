#include "day6.h"

#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <numeric>

namespace day6
{
    // TODO: Damn input isn't in order....
    // So 2R1 for example appears first as the orbitted object,
    // and then later as the orbiter object.
    // Probably going to have to build that tree after all!
    int Calculate()
    {
        std::ifstream ifs("resources/day6.txt");

        std::map<std::string, int> objects = 
        {
            { "COM", 0 }
        };
        std::string orbit;
        while (ifs >> orbit)
        {
            auto delimeterIndex = orbit.find(')');
            std::string object1 = orbit.substr(0, delimeterIndex);
            std::string object2 = orbit.substr(delimeterIndex + 1);

            if (objects.find(object1) == objects.cend())
            {
                objects.insert({object1, 0});
            }

            // Objects can only orbit one thing, so each object 2 will be a new one.
            objects.insert({object2, objects[object1] + 1});
        }

        return std::accumulate(objects.cbegin(), objects.cend(), 0,  [] (int value, const std::map<std::string, int>::value_type& p)
                   {
        return value + p.second; }
               );
    }
}