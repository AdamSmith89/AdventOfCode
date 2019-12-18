#include "day8.h"
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

namespace day8
{
    struct Layer
    {
        std::vector<int> pixels;
    };

    std::vector<Layer> ParseIntoLayers(std::vector<int> const& input, int const width, int const height)
    {
        size_t const pixelsPerLayer = width * height;

        std::vector<Layer> layers;

        auto layerStart = input.begin();
        while (layerStart < input.end())
        {
            Layer newLayer;
            std::copy_n(layerStart, std::min<size_t>(pixelsPerLayer, std::distance(layerStart, input.end())), std::back_inserter(newLayer.pixels));
            layers.push_back(newLayer);

            layerStart += pixelsPerLayer;
        }

        return layers;
    }

    size_t CountValues(std::vector<int> const& vec, int const value)
    {
        return std::count(vec.begin(), vec.end(), value);
    }

    int Calculate()
    {
        std::vector<int> input;
        std::ifstream ifs("resources/day8.txt");
        
        char ch;
        while(ifs.get(ch))
        {
            input.push_back(std::atoi(&ch));
        }

        int const width = 25;
        int const height = 6;

        auto layers = ParseIntoLayers(input, width, height);

        Layer lowestZeroDigitsLayer;
        int lowestZeroDigitsCount = -1;
        for (auto const& layer : layers)
        {
            size_t const numZeroDigits = CountValues(layer.pixels, 0);

            if (lowestZeroDigitsCount == -1 || numZeroDigits < lowestZeroDigitsCount)
            {
                lowestZeroDigitsCount = numZeroDigits;
                lowestZeroDigitsLayer = layer;
            }
        }

        // Part 1
        int const numOneDigits = CountValues(lowestZeroDigitsLayer.pixels, 1);
        int const numTwoDigits = CountValues(lowestZeroDigitsLayer.pixels, 2);
        //return numOneDigits * numTwoDigits;

        // Part 2
        
    }
}