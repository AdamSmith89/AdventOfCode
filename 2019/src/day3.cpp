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

    struct Intersection
    {
        int x = 0;
        int y = 0;
        int distanceFromCentralPort = 0;
    };

    std::vector<Path> GetInput()
    {
        std::vector<Path> paths;

        std::ifstream ifs("resources/day3_test3.txt");
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

        std::sort(tracedPathOne.begin(), tracedPathOne.end());
        std::sort(tracedPathTwo.begin(), tracedPathTwo.end());

        std::vector<std::pair<int, int>> intersections;
        std::set_intersection(tracedPathOne.begin(), tracedPathOne.end(), tracedPathTwo.begin(), tracedPathTwo.end(), std::back_inserter(intersections));

        std::vector<int> distances(intersections.size());
        for (int i = 0; i < intersections.size(); ++i)
        {
            distances[i] = std::abs(intersections[i].first) + std::abs(intersections[i].second);
        }

        std::sort(distances.begin(), distances.end());
        return distances[0];
    }
} // namespace day3