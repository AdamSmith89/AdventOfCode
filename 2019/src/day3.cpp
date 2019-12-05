#include "day3.h"

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <thread>
#include <iostream>

using namespace std::chrono_literals;

namespace day3
{
    auto const UI_DELAY = 0.25s;
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
        char id;
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

        std::ifstream ifs("resources/day3_test1.txt");
        std::string path;
        char id = '1';
        while (std::getline(ifs, path))
        {
            Path newPath;
            newPath.id = id++;  // Sneaky addition for a char to up the number

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
    
    void PrintGrid(std::vector<std::vector<char>> const& grid)
    {
        for (auto const& row : grid)
        {
            for (auto const& col : row)
            {
                std::cout << col << " ";
            }
            std::cout << std::endl;
        }
    }

    int Calculate()
    {
        std::vector<Path> paths = GetInput();

        std::vector<std::vector<char>> grid = { { '0' } }; // rows by columns (Y by X)
        int centralX = 0;   // X is the column number
        int centralY = 0;   // Y is the row number

        PrintGrid(grid);

        std::vector<Intersection> intersections;

        for (auto const& path : paths)
        {
            int pathCurrentX = centralX;
            int pathCurrentY = centralY;

            for (auto const& step : path.steps)
            {
                for (int i = 1; i <= step.amount; ++i)
                {
                    if (step.direction == UP && --pathCurrentY < 0)
                    {
                        pathCurrentY = 0;   // New grid row will be inserted so keep our position at 0
                        
                        // As we've inserted a new grid row we need to update the central position & all intersection positions
                        centralY++;
                        for (auto& intersection : intersections)
                        {
                            intersection.y++;
                        }

                        std::vector<char> newRow(grid[0].size(), '-');
                        grid.insert(grid.begin(), newRow);
                    }
                    else if (step.direction == DOWN && ++pathCurrentY >= grid.size())
                    {
                        std::vector<char> newRow(grid[0].size(), '-');
                        grid.push_back(newRow);
                    }
                    else if (step.direction == RIGHT && ++pathCurrentX >= grid[pathCurrentY].size())
                    {
                        int const newSize = grid[0].size() + 1;
                        for (auto& row : grid)
                        {
                            row.resize(newSize, '-');
                        }
                    }
                    else if (step.direction == LEFT && --pathCurrentX < 0)
                    {
                        pathCurrentX = 0;   // New grid column will be inserted so keep our position at 0
                        
                        // As we've inserted a new grid row we need to update the central position & all intersection positions
                        centralX++;
                        for (auto& intersection : intersections)
                        {
                            intersection.x++;
                        }

                        for (auto& row : grid)
                        {
                            row.insert(row.begin(), '-');
                        }
                    }

                    if (grid[pathCurrentY][pathCurrentX] == '-')
                    {
                        grid[pathCurrentY][pathCurrentX] = path.id;
                    }
                    else if (grid[pathCurrentY][pathCurrentX] != path.id)
                    {
                        grid[pathCurrentY][pathCurrentX] = '+';

                        Intersection newIntersection;
                        newIntersection.x = pathCurrentX;
                        newIntersection.y = pathCurrentY;
                        newIntersection.distanceFromCentralPort = std::abs(newIntersection.x - centralX) + std::abs(newIntersection.y - centralY);
                        
                        intersections.push_back(newIntersection);
                    }

                    system("cls");
                    std::cout << step.direction << step.amount << " (" << i << ")" << std::endl;
                    PrintGrid(grid);
                    std::cout << std::endl;
                    std::cout << "C: " << centralX << ", " << centralY << std::endl;
                    for (auto const& intersection : intersections)
                    {
                        std::cout << "+: " << intersection.x << ", " << intersection.y << " <-> " << intersection.distanceFromCentralPort << std::endl;
                    }
                    std::this_thread::sleep_for(UI_DELAY);
                }
            }
        }

        system("cls");
        PrintGrid(grid);
        std::cout << std::endl;
        std::cout << "C: " << centralX << ", " << centralY << std::endl;
        for (auto const& intersection : intersections)
        {
            std::cout << "+: " << intersection.x << ", " << intersection.y << " <-> " << intersection.distanceFromCentralPort << std::endl;
        }
        system("pause");

        return 0;
    }
} // namespace day3