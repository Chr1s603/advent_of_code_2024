// src/days/day_01.cppm
export module aoc.day01;

import std;
import util.types;
import util.parse;

export namespace day01 {

struct Day01
{
    static constexpr s64 number = 1;

    static consteval sv name ()
    {
        return "Day 01: Historian Hysteria";
    }

    using LocationPair = pair<s64, s64>;
    using Parsed       = vec<LocationPair>;

    // Parse input into left/right location ID pairs
    static Parsed parse (sv input)
    {
        return util::parse::parse_whitespace_pairs<s64>(input);
    }

    // Extract left or right location IDs
    static vec<s64> left_list (const Parsed& pairs)
    {
        auto view = pairs | std::ranges::views::transform([] (auto& p) { return p.first; });
        return vec<s64>(view.begin(), view.end());
    }

    static vec<s64> right_list (const Parsed& pairs)
    {
        auto view = pairs | std::ranges::views::transform([] (auto& p) { return p.second; });
        return vec<s64>(view.begin(), view.end());
    }

    // Part 1: total distance between left/right lists after sorting
    static s64 part1 (const Parsed& pairs)
    {
        auto left  = left_list(pairs);
        auto right = right_list(pairs);

        std::ranges::sort(left);
        std::ranges::sort(right);

        s64 total_distance = 0;
        for (u64 i = 0; i < std::min(left.size(), right.size()); ++i)
        {
            total_distance += std::abs(left[i] - right[i]);
        }

        return total_distance;
    }

    // Part 2: similarity score by multiplying left IDs by occurrences in right list
    static s64 part2 (const Parsed& pairs)
    {
        auto left  = left_list(pairs);
        auto right = right_list(pairs);

        // Frequency map to drop complexity and improve runtime
        std::unordered_map<s64, s64> freq;
        for (auto r : right)
            freq[r]++;

        s64 similarity_score = 0;
        for (auto l : left)
            similarity_score += l * freq[l];

        return similarity_score;
    }

    static constexpr pair<s64, s64> expected ()
    {
        return {1882714, 19437052};
    }
};

} // namespace day01
