#include "day3.h"

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <thread>
#include <iostream>

using namespace std::chrono_literals;
auto const UI_DELAY = 0.25s;

namespace day3
{
    enum class Direction
    {
        Up,
        Down,
        Right,
        Left
    };

    struct Step
    {
        Direction direction;
        int amount;
    };

    struct Path
    {
        std::vector<Step> steps;
        char id;
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
                switch (step.front())
                {
                case 'U':
                    newStep.direction = Direction::Up;
                    break;
                case 'D':
                    newStep.direction = Direction::Down;
                    break;
                case 'R':
                    newStep.direction = Direction::Right;
                    break;
                case 'L':
                    newStep.direction = Direction::Left;
                    break;
                }

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

        std::cout << std::endl;
    }

    int Calculate()
    {
        std::vector<Path> paths = GetInput();

        std::vector<std::vector<char>> grid = { { '0' } }; // rows by columns (Y by X)
        int centralX = 0;   // X is the column number
        int centralY = 0;   // Y is the row number

        PrintGrid(grid);

        for (auto const &path : paths)
        {
            int pathCurrentX = centralX;
            int pathCurrentY = centralY;

            for (auto const &step : path.steps)
            {
                if (step.direction == Direction::Up)
                {
                    for (int i = 1; i <= step.amount; ++i)
                    {
                        if (--pathCurrentY >= 0)
                        {
                            if (grid[pathCurrentY][pathCurrentX] == '-')
                            {
                                grid[pathCurrentY][pathCurrentX] = path.id;
                            }
                            else if (grid[pathCurrentY][pathCurrentX] != path.id)
                            {
                                grid[pathCurrentY][pathCurrentX] = '+';
                            }
                        }
                        else
                        {
                            pathCurrentY = 0;   // New grid row will be inserted so keep our position at 0
                            centralY++; // As we've inserted a new grid row we need to update the central position for the next path

                            std::vector<char> newRow(grid[0].size(), '-');
                            newRow[pathCurrentX] = path.id;
                            grid.insert(grid.begin(), newRow);
                        }

                        system("cls");
                        std::cout << static_cast<int>(step.direction) << " : " << step.amount << "(" << i << ")" << std::endl;
                        PrintGrid(grid);
                        std::this_thread::sleep_for(UI_DELAY);
                    }
                }
                else if (step.direction == Direction::Down)
                {
                    for (int i = 1; i <= step.amount; ++i)
                    {
                        if (++pathCurrentY < grid.size())
                        {
                            if (grid[pathCurrentY][pathCurrentX] == '-')
                            {
                                grid[pathCurrentY][pathCurrentX] = path.id;
                            }
                            else if (grid[pathCurrentY][pathCurrentX] != path.id)
                            {
                                grid[pathCurrentY][pathCurrentX] = '+';
                            }
                        }
                        else
                        {
                            std::vector<char> newRow(grid[0].size(), '-');
                            newRow[pathCurrentX] = path.id;
                            grid.push_back(newRow);
                        }

                        system("cls");
                        std::cout << static_cast<int>(step.direction) << " : " << step.amount << "(" << i << ")" << std::endl;
                        PrintGrid(grid);
                        std::this_thread::sleep_for(UI_DELAY);
                    }
                }
                else if (step.direction == Direction::Right)
                {
                    for (int i = 1; i <= step.amount; ++i)
                    {
                        if (++pathCurrentX < grid[pathCurrentY].size())
                        {
                            if (grid[pathCurrentY][pathCurrentX] == '-')
                            {
                                grid[pathCurrentY][pathCurrentX] = path.id;
                            }
                            else if (grid[pathCurrentY][pathCurrentX] != path.id)
                            {
                                grid[pathCurrentY][pathCurrentX] = '+';
                            }
                        }
                        else
                        {
                            grid[pathCurrentY].push_back(path.id);

                            for (auto& row : grid)
                            {
                                row.resize(grid[pathCurrentY].size(), '-');
                            }
                        }
                        
                        system("cls");
                        std::cout << static_cast<int>(step.direction) << " : " << step.amount << "(" << i << ")" << std::endl;
                        PrintGrid(grid);
                        std::this_thread::sleep_for(UI_DELAY);
                    }
                }
                else if (step.direction == Direction::Left)
                {
                    for (int i = 1; i <= step.amount; ++i)
                    {
                        if (--pathCurrentX >= 0)
                        {
                            if (grid[pathCurrentY][pathCurrentX] == '-')
                            {
                                grid[pathCurrentY][pathCurrentX] = path.id;
                            }
                            else if (grid[pathCurrentY][pathCurrentX] != path.id)
                            {
                                grid[pathCurrentY][pathCurrentX] = '+';
                            }
                        }
                        else
                        {
                            pathCurrentX = 0;     // New grid column will be inserted so keep our position at 0
                            centralX++; // As we've inserted a new grid column we need to update the central position for the next path

                            for (auto& row : grid)
                            {
                                row.insert(row.begin(), '-');
                            }

                            grid[pathCurrentY][pathCurrentX] = path.id;
                        }
                        
                        system("cls");
                        std::cout << static_cast<int>(step.direction) << " : " << step.amount << "(" << i << ")" << std::endl;
                        PrintGrid(grid);
                        std::this_thread::sleep_for(UI_DELAY);
                    }
                }
            }
        }

        system("cls");
        PrintGrid(grid);
        system("pause");

        return 0;
    }
} // namespace day3