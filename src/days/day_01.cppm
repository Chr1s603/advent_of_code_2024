export module aoc.day01;

import std;
import util.enumerable;
import util.parse;
import util.types;

export namespace day01 {

struct Day01
{
    static constexpr s64 number = 1;
    static constexpr sv  name{"Day 01: Historian Hysteria"};

    using LocationPair = pair<s64, s64>;
    using Parsed       = vec<LocationPair>;

    // Input is the research result(a list) from each of the two groups of historians:
    // List0_Index0 List1Index0
    // List0_Index1 List1Index1
    //      ...        ...
    static Parsed parse (sv input)
    {
        return util::parse::parse_whitespace_pairs<s64>(input);
    }

    // Part 1: Total distance between left/right lists after sorting.
    static s64 part1 (const Parsed& pairs)
    {
        auto left  = util::enumerable::get_nth_of_each_element<0>(pairs);
        auto right = util::enumerable::get_nth_of_each_element<1>(pairs);

        // "Pair up the smallest number in the left list with the smallest number in the
        // right list, then the second-smallest left number with the second-smallest
        // right number, and so on." -> Straight up sorting.
        std::ranges::sort(left);
        std::ranges::sort(right);

        s64 total_distance = 0;
        for (u64 i = 0; i < std::min(left.size(), right.size()); ++i)
            total_distance += std::abs(left[i] - right[i]);

        return total_distance;
    }

    // Part 2: Similarity score by multiplying left IDs by occurrences in right list.
    static s64 part2 (const Parsed& pairs)
    {
        auto left  = util::enumerable::get_nth_of_each_element<0>(pairs);
        auto right = util::enumerable::get_nth_of_each_element<1>(pairs);

        // Store the frequency / occurence-count of each location on the right:
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
