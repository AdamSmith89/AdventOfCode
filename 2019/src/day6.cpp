#include "day6.h"

#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <numeric>
#include <memory>
#include <algorithm>

namespace day6
{
    class SpaceObject
    {
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

        // int DistanceToAncestor(std::string const& ancestorName)
        // {
        //     if (m_parent && m_parent->GetName() == ancestorName)
        //         return 0;

        //     if (m_parent)
        //         return 1 + m_parent->DistanceToAncestor(ancestorName);

        //     return 999; // Shouldn't get here?
        // }

        std::vector<std::shared_ptr<SpaceObject>> GetParents()
        {
            std::vector<std::shared_ptr<SpaceObject>> parents;

            if (m_parent)
            {
                parents.push_back(m_parent);
                auto grandParents = m_parent->GetParents();
                parents.insert(parents.end(), grandParents.begin(), grandParents.end());
            }

            return parents;
        }

        void AddParent(std::shared_ptr<SpaceObject> parent)
        {
            m_parent = parent;
        }

        void AddOrbiter(std::shared_ptr<SpaceObject> orbiter)
        {
            m_orbiters.push_back(orbiter);
        }

    private:
        std::string m_name;
        std::vector<std::shared_ptr<SpaceObject>> m_orbiters;
        std::shared_ptr<SpaceObject> m_parent;
        int m_numOrbits = -1;
    };

    std::shared_ptr<SpaceObject> FindSpaceObjectByName(std::vector<std::shared_ptr<SpaceObject>> const& spaceObjects, std::string const& name)
    {
        std::shared_ptr<SpaceObject> spaceObject1;
        auto iter1 = std::find_if(std::begin(spaceObjects), std::end(spaceObjects), [&name](auto const& spaceObject) { return spaceObject->GetName() == name; });
        if (iter1 != spaceObjects.cend())
        {
            spaceObject1 = *iter1;
        }

        return spaceObject1;
    }

    int Calculate()
    {
        std::ifstream ifs("resources/day6.txt");

        std::vector<std::shared_ptr<SpaceObject>> spaceObjects;
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

        int totalOrbits = 0;
        for (auto const& spaceObject : spaceObjects)
        {
            totalOrbits += spaceObject->CalculateNumOrbits();
        }

        auto youSpaceObject = FindSpaceObjectByName(spaceObjects, "YOU");
        auto sanSpaceObject = FindSpaceObjectByName(spaceObjects, "SAN");

        auto youParents = youSpaceObject->GetParents();
        auto sanParents = sanSpaceObject->GetParents();

        auto fnCmpNames = [](auto const& lhs, auto const& rhs)
            {
                return lhs->GetName() < rhs->GetName();
            };

        std::sort(youParents.begin(), youParents.end(), fnCmpNames);
        std::sort(sanParents.begin(), sanParents.end(), fnCmpNames);

        std::vector<std::shared_ptr<SpaceObject>> commonParents;
        std::set_intersection(youParents.begin(), youParents.end(), sanParents.begin(), sanParents.end(),
                                std::back_inserter(commonParents), fnCmpNames);

        auto fnCmpOrbits = [](auto const& lhs, auto const& rhs)
            {
                return lhs->GetName() < rhs->GetName();
            };
        auto result = std::max_element(commonParents.begin(), commonParents.end(), fnCmpOrbits);
        auto closestSharedAncestor = *result;

        auto stepsFromYouToAncestor = youSpaceObject->GetNumOrbits() - closestSharedAncestor->GetNumOrbits() - 1;
        auto stepsFromSanToAncestor = sanSpaceObject->GetNumOrbits() - closestSharedAncestor->GetNumOrbits() - 1;

        // Answer came back as 626 - too high apparently
        return stepsFromYouToAncestor + stepsFromSanToAncestor;
    }
}