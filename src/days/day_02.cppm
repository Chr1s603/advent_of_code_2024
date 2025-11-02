export module aoc.day02;

import std;
import util.types;
import util.parse;

export namespace day02 {

struct Day02
{
    static constexpr s64 number = 2;
    static constexpr sv  name{"Day 02: Red-Nosed Reports"};

    using Report  = vec<s64>;
    using Reports = vec<Report>;

    static Reports parse (csv s)
    {
        const auto reports = util::parse::split(s, sv("\n"))
                             | std::views::filter([] (sv line) { return !line.empty(); })
                             | std::ranges::to<vec<sv>>();
        return reports //
               | std::views::transform([] (sv line) {
                     return util::parse::split(line, sv(" ")) //
                            | util::parse::to_numbers<s64>()  //
                            | std::ranges::to<vec<s64>>();
                 })
               | std::ranges::to<vec<vec<s64>>>();
    }

    static bool is_safe (const Report& report)
    {
        s64        last          = report[0];
        const bool is_increasing = report[0] < report[1];
        for (s64 sample_idx = 1; sample_idx < cs64(report.size()); sample_idx++)
        {
            cs64 current = report[sample_idx];
            if (const auto diff = std::abs(current - last); diff == 0 || diff > 3)
                return false;
            if (is_increasing && (current < last))
                return false;
            if (!is_increasing && (current > last))
                return false;
            last = current;
        }
        return true;
    }

    static bool can_be_safe_with_dampener (const Report& report)
    {
        if (is_safe(report))
            return true; // already safe

        // Try removing each element in turn
        for (s64 i = 0; i < report.size(); ++i)
        {
            Report copy;
            copy.reserve(report.size() - 1);
            for (s64 j = 0; j < report.size(); ++j)
                if (i != j)
                    copy.push_back(report[j]);

            if (is_safe(copy))
                return true;
        }

        return false;
    }

    // Part 1: Count of safe reports.
    static s64 part1 (const Reports& in)
    {
        const auto safe_flags
            = in | std::views::transform([] (const Report& line) { return is_safe(line); });
        return std::ranges::count(safe_flags, true);
    }

    // Part 2: Count of safe reports with up to 1 erased number.
    static s64 part2 (const Reports& in)
    {
        const auto safe_flags = in | std::views::transform([] (const Report& line) {
                                    return can_be_safe_with_dampener(line);
                                });
        return std::ranges::count(safe_flags, true);
    }

    static constexpr pair<s64, s64> expected ()
    {
        return {236, 308}; // replace with real answers
    }
};
} // namespace day01
