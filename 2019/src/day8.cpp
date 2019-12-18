#include "day8.h"
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <iterator>

namespace day8
{
    int const WIDTH = 25;
    int const HEIGHT = 6;

    struct Layer
    {
        std::vector<int> pixels;
    };

    std::vector<Layer> ParseIntoLayers(std::vector<int> const& input)
    {
        size_t const pixelsPerLayer = WIDTH * HEIGHT;

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

    void PrettyPrint(Layer const& layer)
    {
        for (int i = 0; i < WIDTH; ++i)
        {
            for (int j = 0; j < HEIGHT; ++j)
            {
                std::cout << layer.pixels[i*HEIGHT+j];
            }
            std::cout << std::endl;
        }
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

        auto layers = ParseIntoLayers(input);

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
        int const BLACK = 0; int const WHITE = 1; int const TRANSPARENT = 2;

        Layer mergedLayers;
        mergedLayers.pixels.resize(layers[0].pixels.size(), TRANSPARENT);
        for (auto const& layer : layers)
        {
            for (int i = 0; i < layer.pixels.size(); ++i)
            {
                if (mergedLayers.pixels[i] == TRANSPARENT && layer.pixels[i] != TRANSPARENT)
                {
                    mergedLayers.pixels[i] = layer.pixels[i];
                }
            }
        }

        PrettyPrint(mergedLayers);  // TODO: Actually print the image!

        std::copy(mergedLayers.pixels.begin(), mergedLayers.pixels.end(), std::ostream_iterator<int>(std::cout));
        std::cout << std::endl;

        return 0;
    }
}