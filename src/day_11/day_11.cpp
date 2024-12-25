#include <cmath>
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

namespace NAMESPACE_NAME {

string
parse_input (ifstream &s)
{
    return { istreambuf_iterator<char>(s), {} };
}

// Use a map to track the frequency of each stone (many duplicates, saves huge amounts of memory)
using stone_map = unordered_map<int64_t, int64_t>;

constexpr int64_t
get_digits(int64_t val, const int64_t exp = 10)
{
    int64_t digits { 0 };
    while (val != 0)
    {
        val /= exp;
        digits ++;
    }
    return digits;
}

constexpr pair<int64_t, int64_t>
split_number(const int64_t val, const int64_t pos)
{
    int64_t a = floor(static_cast<double>(val) / pow(10, pos));
    int64_t b = val - a * static_cast<int64_t>(pow(10, pos));
    return {a, b};
}

void
blink(stone_map& stones)
{
    stone_map next_stones;
    for (const auto& [val, count] : stones)
    {
        const int64_t digits { get_digits(val) };

        if (val == 0)
            next_stones[1] += count;
        else if (digits % 2)
            next_stones[val * 2024] += count;
        else
        {
            const auto [a, b] = split_number(val, digits >> 1);
            next_stones[a] += count;
            next_stones[b] += count;
        }
    }

    stones = move(next_stones);
}

int64_t
get_stone_count(const string& in, const int64_t blinks)
{
    vector<string> tokens;
    split(tokens, in, is_any_of(" "));

    stone_map stones;
    for (const auto& token : tokens)
        stones[stoll(token)]++;

    for (int64_t b = 0; b < blinks; b++)
        blink(stones);

    int64_t total_stones = 0;
    for (const auto& [stone, count] : stones)
        total_stones += count;

    return total_stones;
}

int64_t part1 (const string &in) { return get_stone_count(in, 25); }
int64_t part2 (const string &in) { return get_stone_count(in, 75); }

}
