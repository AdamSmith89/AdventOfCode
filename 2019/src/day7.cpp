#include "day7.h"
#include "IntCodeComputer.h"

#include <sstream>
#include <fstream>
#include <algorithm>
#include <thread>
#include <functional>
#include <mutex>

namespace day7
{
    void OutputVectorAsCsv(std::vector<int> const& vec)
    {
        for (auto const& entry : std::vector<int>(vec.begin(), vec.end()-1))
        {
            std::cout << entry << ", ";
        }
        std::cout << vec.back();
    }

    int Calculate()
    {
        std::vector<int> instructions;

        std::ifstream ifs("resources/day7.txt");
        std::string digit;
        while (std::getline(ifs, digit, ','))
        {
            instructions.push_back(std::atoi(digit.c_str()));
        }

        // std::stringstream ampEtoA, ampAtoB, ampBtoC, ampCtoD, ampDtoE;
        // IntCodeComputer ampA("AmpA", ampEtoA, ampAtoB, std::cerr);
        // IntCodeComputer ampB("AmpB", ampAtoB, ampBtoC, std::cerr);
        // IntCodeComputer ampC("AmpC", ampBtoC, ampCtoD, std::cerr);
        // IntCodeComputer ampD("AmpD", ampCtoD, ampDtoE, std::cerr);
        // IntCodeComputer ampE("AmpE", ampDtoE, ampEtoA, std::cerr);

        // std::vector<int> phaseSettings = {9, 8, 7, 6, 5};
        // ampEtoA << phaseSettings[0] << " " << 0;
        // ampAtoB << phaseSettings[1];
        // ampBtoC << phaseSettings[2];
        // ampCtoD << phaseSettings[3];
        // ampDtoE << phaseSettings[4];

        // std::vector<std::thread> ampThreads(5);
        // ampThreads.push_back(std::thread(&IntCodeComputer::Run, &ampA, instructions));
        // ampThreads.push_back(std::thread(&IntCodeComputer::Run, &ampB, instructions));
        // ampThreads.push_back(std::thread(&IntCodeComputer::Run, &ampC, instructions));
        // ampThreads.push_back(std::thread(&IntCodeComputer::Run, &ampD, instructions));
        // ampThreads.push_back(std::thread(&IntCodeComputer::Run, &ampE, instructions));

        // for (auto& thread : ampThreads)
        // {
        //     if (thread.joinable())
        //         thread.join();
        // }

        // int thrusterSignal = 0;
        // ampEtoA >> thrusterSignal;

        // return thrusterSignal;
        std::vector<int> phaseSettings = {5, 6, 7, 8, 9};

        int maxThrusterSignal = 0;
        do
        {
            std::cout << "Testing permutation {";
            OutputVectorAsCsv(phaseSettings);
            std::cout << "}" << std::endl;

            std::stringstream ampEtoA, ampAtoB, ampBtoC, ampCtoD, ampDtoE;
            IntCodeComputer ampA("AmpA", ampEtoA, ampAtoB, std::cerr);
            IntCodeComputer ampB("AmpB", ampAtoB, ampBtoC, std::cerr);
            IntCodeComputer ampC("AmpC", ampBtoC, ampCtoD, std::cerr);
            IntCodeComputer ampD("AmpD", ampCtoD, ampDtoE, std::cerr);
            IntCodeComputer ampE("AmpE", ampDtoE, ampEtoA, std::cerr);

            ampEtoA << phaseSettings[0] << " " << 0;
            ampAtoB << phaseSettings[1];
            ampBtoC << phaseSettings[2];
            ampCtoD << phaseSettings[3];
            ampDtoE << phaseSettings[4];

            std::vector<std::thread> ampThreads(5);
            ampThreads.push_back(std::thread(&IntCodeComputer::Run, &ampA, instructions));
            ampThreads.push_back(std::thread(&IntCodeComputer::Run, &ampB, instructions));
            ampThreads.push_back(std::thread(&IntCodeComputer::Run, &ampC, instructions));
            ampThreads.push_back(std::thread(&IntCodeComputer::Run, &ampD, instructions));
            ampThreads.push_back(std::thread(&IntCodeComputer::Run, &ampE, instructions));

            for (auto& thread : ampThreads)
            {
                if (thread.joinable())
                    thread.join();
            }

            int thrusterSignal = 0;
            ampEtoA >> thrusterSignal;
            std::cout << "ThrusterSignal = " << thrusterSignal << std::endl;

            if (thrusterSignal > maxThrusterSignal)
            {
                maxThrusterSignal = thrusterSignal;
            }
        } while (std::next_permutation(phaseSettings.begin(), phaseSettings.end()));

        return maxThrusterSignal;
    }
} // namespace day7