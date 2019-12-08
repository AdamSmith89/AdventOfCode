#include "day4.h"
#include "doctest.h"

#include <vector>
#include <algorithm>
#include <iostream>

namespace day4
{
std::vector<int> IntToVec(int num)
{
    std::vector<int> digits;
    while (num > 0)
    {
        digits.push_back(num % 10);
        num /= 10;
    }

    std::reverse(digits.begin(), digits.end());
    return digits;
}

bool HasAdjacentDigits(std::vector<int> const &codeDigits)
{
    bool hasAdjacentDigits = false;

    auto adjacentMatch = std::adjacent_find(codeDigits.begin(), codeDigits.end());
    while (adjacentMatch != codeDigits.end())
    {
        hasAdjacentDigits = true;

        // Find the next adjacent match starting from the 2nd entry in this match
        auto nextAdjacentMatch = std::adjacent_find(adjacentMatch + 1, codeDigits.end());

        // If it is a triplet match then it will find the location we started from
        if (nextAdjacentMatch == adjacentMatch + 1)
        {
            // More than 2 adjacent digits is an invalid match...
            hasAdjacentDigits = false;

            // So find the next digit that isn't part of this adjacent set...
            auto nextNotMatchingDigit = std::find_if_not(nextAdjacentMatch, codeDigits.end(), [&nextAdjacentMatch](auto const &entry) { return *nextAdjacentMatch == entry; });
            
            // Then continue the process from there.
            adjacentMatch = std::adjacent_find(nextNotMatchingDigit, codeDigits.end());
        }
        else
        {
            break;
        }
    }

    return hasAdjacentDigits;
}

int Calculate()
{
    int const rangeMin = 183564;
    int const rangeMax = 657474;

    int numCodesMatch = 0;
    for (int code = rangeMin; code <= rangeMax; ++code)
    {
        // It is a 6 digit number - a given so far...
        // Within range provided - a given so far...

        auto codeDigits = IntToVec(code);

        // Left-to-right, the digits don't decrease
        if (!std::is_sorted(codeDigits.begin(), codeDigits.end()))
            continue;

        // Two adjacent digits ONLY are the same
        if (!HasAdjacentDigits(codeDigits))
            continue;

        numCodesMatch++;
    }

    return numCodesMatch;
}
} // namespace day4

TEST_CASE("HasAdjacentDigits")
{
    CHECK(day4::HasAdjacentDigits({1, 1, 2, 2, 3, 3}) == true);
    CHECK(day4::HasAdjacentDigits({1, 2, 3, 4, 4, 4}) == false);
    CHECK(day4::HasAdjacentDigits({1, 1, 1, 1, 2, 2}) == true);
    CHECK(day4::HasAdjacentDigits({1, 1, 1, 1, 2, 3}) == false);
    CHECK(day4::HasAdjacentDigits({1, 1, 2, 4, 4}) == true);
    CHECK(day4::HasAdjacentDigits({1, 1, 1, 4, 4}) == true);
    CHECK(day4::HasAdjacentDigits({1, 1, 4, 4, 4}) == true);
    CHECK(day4::HasAdjacentDigits({1, 1, 4, 4, 5}) == true);
    CHECK(day4::HasAdjacentDigits({1, 2, 4, 4, 5}) == true);
}