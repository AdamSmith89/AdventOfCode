#include "day3.h"

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

namespace day3
{
    char const UP = 'U';
    char const DOWN = 'D';
    char const RIGHT = 'R';
    char const LEFT = 'L';

    struct Step
    {
        char direction;
        int amount;
    };

    struct Path
    {
        std::vector<Step> steps;
    };

    std::vector<Path> GetInput()
    {
        std::vector<Path> paths;

        std::ifstream ifs("resources/day3.txt");
        std::string path;

        while (std::getline(ifs, path))
        {
            Path newPath;

            std::stringstream pathStream(path);
            std::string step;
            while (std::getline(pathStream, step, ','))
            {
                Step newStep;
                newStep.direction = step.front();
                
                step.erase(step.begin());
                newStep.amount = std::atoi(step.c_str());

                newPath.steps.push_back(newStep);
            }
            paths.push_back(newPath);
        }

        return paths;
    }

    std::vector<std::pair<int, int>> TracePath(Path const& path)
    {
        std::vector<std::pair<int, int>> tracedPath;

        int x = 0;
        int y = 0;
        for (auto const& step : path.steps)
        {
            for (int i = 0; i < step.amount; ++i)
            {
                if (step.direction == UP)
                    y++;
                else if (step.direction == DOWN)
                    y--;
                else if (step.direction == RIGHT)
                    x++;
                else if (step.direction == LEFT)
                    x--;
    
                // For some reason vscode seems to fail here for big input files - has errors when inserting > 498 pairs in the vector...
                tracedPath.push_back({x, y});
            }
        }

        return tracedPath;
    }

    int Calculate()
    {
        std::vector<Path> paths = GetInput();
        std::vector<std::pair<int, int>> tracedPathOne = TracePath(paths[0]);
        std::vector<std::pair<int, int>> tracedPathTwo = TracePath(paths[1]);

        auto sortedTracedPathOne = tracedPathOne;
        auto sortedTracedPathTwo = tracedPathTwo;

        std::sort(sortedTracedPathOne.begin(), sortedTracedPathOne.end());
        std::sort(sortedTracedPathTwo.begin(), sortedTracedPathTwo.end());

        std::vector<std::pair<int, int>> intersections;
        std::set_intersection(sortedTracedPathOne.begin(), sortedTracedPathOne.end(), sortedTracedPathTwo.begin(), sortedTracedPathTwo.end(), std::back_inserter(intersections));

        // Part 1 - calculate which intersectioni is the closest to centre based on manhattan distance
        // std::vector<int> distances(intersections.size());
        // for (int i = 0; i < intersections.size(); ++i)
        // {
        //     distances[i] = std::abs(intersections[i].first) + std::abs(intersections[i].second);
        // }
        // std::sort(distances.begin(), distances.end());

        // Part 2 - Work out which intersection takes the least amount of steps to get to
        std::pair<int, int> lowestStepIntersection;
        int lowestStepCount = 0;
        for (auto const& intersection : intersections)
        {
            auto stepsOnPathOne = std::distance(tracedPathOne.begin(), std::find(tracedPathOne.begin(), tracedPathOne.end(), intersection)) + 1;
            auto stepsOnPathTwo = std::distance(tracedPathTwo.begin(), std::find(tracedPathTwo.begin(), tracedPathTwo.end(), intersection)) + 1;
            auto intersectionStepCount = stepsOnPathOne + stepsOnPathTwo;

            if (lowestStepCount == 0 || intersectionStepCount < lowestStepCount)
            {
                lowestStepCount = intersectionStepCount;
                lowestStepIntersection = intersection;
            }
        }

        return lowestStepCount;
    }
} // namespace day3