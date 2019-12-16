#include "day6.h"

#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <numeric>
#include <memory>
#include <algorithm>
#include <iostream>

namespace day6
{
    class SpaceObject
    {
        using SpaceObjectPtr = std::shared_ptr<SpaceObject>;
        using SpaceObjectPtrList = std::vector<SpaceObjectPtr>;

    public:
        SpaceObject(std::string const& name)
            : m_name(name)
        {}

        std::string GetName() const { return m_name; }
        int GetNumOrbits() const { return m_numOrbits; }

        int CalculateNumOrbits()
        {
            if (m_numOrbits != -1)
                return m_numOrbits;
            
            if (m_parent)
                m_numOrbits = 1 + m_parent->CalculateNumOrbits();
            else
                m_numOrbits = 0;

            return m_numOrbits;
        }

        SpaceObjectPtrList GetParents()
        {
            SpaceObjectPtrList parents;

            if (m_parent)
            {
                parents.push_back(m_parent);
                auto grandParents = m_parent->GetParents();
                parents.insert(parents.end(), grandParents.begin(), grandParents.end());
            }
            
            return parents;
        }

        void AddParent(SpaceObjectPtr parent)
        {
            m_parent = parent;
        }

        void AddOrbiter(SpaceObjectPtr orbiter)
        {
            m_orbiters.push_back(orbiter);
        }

    private:
        std::string m_name;
        SpaceObjectPtrList m_orbiters;
        SpaceObjectPtr m_parent;
        int m_numOrbits = -1;
    };

    using SpaceObjectPtr = std::shared_ptr<SpaceObject>;
    using SpaceObjectPtrList = std::vector<SpaceObjectPtr>;

    SpaceObjectPtr FindSpaceObjectByName(SpaceObjectPtrList const& spaceObjects, std::string const& name)
    {
        SpaceObjectPtr spaceObject1;
        auto iter1 = std::find_if(std::begin(spaceObjects), std::end(spaceObjects), [&name](auto const& spaceObject) { return spaceObject->GetName() == name; });
        if (iter1 != spaceObjects.cend())
        {
            spaceObject1 = *iter1;
        }

        return spaceObject1;
    }

    SpaceObjectPtrList FindCommonParents(SpaceObjectPtr const& object1, SpaceObjectPtr const& object2)
    {
        auto object1Parents = object1->GetParents();
        auto object2Parents = object2->GetParents();

        auto fnCmpNames = [](auto const& lhs, auto const& rhs)
            {
                return lhs->GetName() < rhs->GetName();
            };

        std::sort(object1Parents.begin(), object1Parents.end(), fnCmpNames);
        std::sort(object2Parents.begin(), object2Parents.end(), fnCmpNames);

        SpaceObjectPtrList commonParents;
        std::set_intersection(object1Parents.begin(), object1Parents.end(), object2Parents.begin(), object2Parents.end(),
                                std::back_inserter(commonParents), fnCmpNames);
        return commonParents;
    }

    SpaceObjectPtrList GetInput()
    {
        std::ifstream ifs("resources/day6.txt");

        SpaceObjectPtrList spaceObjects;
        std::string orbit;
        while (ifs >> orbit)
        {
            auto delimeterIndex = orbit.find(')');
            std::string objectName1 = orbit.substr(0, delimeterIndex);
            std::string objectName2 = orbit.substr(delimeterIndex + 1);

            auto spaceObject1 = FindSpaceObjectByName(spaceObjects, objectName1);
            if (!spaceObject1)
            {
                spaceObject1 = std::make_shared<SpaceObject>(objectName1);
                spaceObjects.push_back(spaceObject1);
            }

            auto spaceObject2 = FindSpaceObjectByName(spaceObjects, objectName2);
            if (!spaceObject2)
            {
                spaceObject2 = std::make_shared<SpaceObject>(objectName2);
                spaceObjects.push_back(spaceObject2);
            }

            spaceObject1->AddOrbiter(spaceObject2);
            spaceObject2->AddParent(spaceObject1);
        }

        return spaceObjects;
    }

    int Calculate()
    {
        SpaceObjectPtrList spaceObjects = GetInput();

        // Part 1
        int totalOrbits = 0;
        for (auto const& spaceObject : spaceObjects)
        {
            totalOrbits += spaceObject->CalculateNumOrbits();
        }
        std::cout << "Total Orbits = " << totalOrbits << std::endl;

        // Part 2
        // Find common parent which orbits the most things, then work out how many steps from YOU & SAN to that ancestor.
        auto youSpaceObject = FindSpaceObjectByName(spaceObjects, "YOU");
        auto sanSpaceObject = FindSpaceObjectByName(spaceObjects, "SAN");
        SpaceObjectPtrList commonParents = FindCommonParents(youSpaceObject, sanSpaceObject);

        auto fnCmpOrbits = [](auto const& lhs, auto const& rhs) { return lhs->GetNumOrbits() < rhs->GetNumOrbits(); };
        auto closestSharedAncestor = *std::max_element(commonParents.begin(), commonParents.end(), fnCmpOrbits);

        auto stepsFromYouToAncestor = youSpaceObject->GetNumOrbits() - closestSharedAncestor->GetNumOrbits() - 1;
        auto stepsFromSanToAncestor = sanSpaceObject->GetNumOrbits() - closestSharedAncestor->GetNumOrbits() - 1;        

        return stepsFromYouToAncestor + stepsFromSanToAncestor;
    }
}