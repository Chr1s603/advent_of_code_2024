export module aoc.day15;

import std;
import util.types;

export namespace day15 {

struct Day15
{
    static constexpr s64 number = 15;
    static consteval sv  name ()
    {
        return "Day 15: Example";
    }

    using Pair   = pair<s64, s64>;
    using Parsed = vec<Pair>;

    // Simple whitespace parser for pairs of integers
    static Parsed parse (sv s)
    {
        Parsed      out;
        const char* p = s.data();
        const char* e = s.data() + s.size();

        while (p < e)
        {
            while (p < e && std::isspace(static_cast<unsigned char>(*p)))
                ++p;
            if (p >= e)
                break;

            s64 a{}, b{};
            auto [q1, ec1] = std::from_chars(p, e, a);
            if (ec1 != std::errc{})
                break;
            p = q1;

            while (p < e && std::isspace(static_cast<unsigned char>(*p)))
                ++p;
            if (p >= e)
                break;

            auto [q2, ec2] = std::from_chars(p, e, b);
            if (ec2 != std::errc{})
                break;
            p = q2;

            out.emplace_back(a, b);
        }

        return out;
    }

    // Helper to extract first or second elements
    static vec<s64> project (const Parsed& v, bool first)
    {
        vec<s64> out;
        out.reserve(v.size());
        for (const auto& p : v)
        {
            out.push_back(first ? p.first : p.second);
        }
        return out;
    }

    // Helper to combine two vectors element-wise
    static vec<Pair> combine (const vec<s64>& a, const vec<s64>& b)
    {
        vec<Pair> out;
        out.reserve(std::min(a.size(), b.size()));
        for (u64 i = 0; i < out.capacity(); ++i)
            out.emplace_back(a[i], b[i]);

        return out;
    }

    // Part 1: sum of absolute differences after sorting
    static s64 part1 (const Parsed& in)
    {
        auto first_values  = project(in, true);
        auto second_values = project(in, false);

        std::sort(first_values.begin(), first_values.end());
        std::sort(second_values.begin(), second_values.end());

        s64  res      = 0;
        auto combined = combine(first_values, second_values);
        for (const auto& [f, s] : combined)
            res += std::abs(f - s);

        return res;
    }

    // Part 2: sum of first_values multiplied by occurrences in second_values
    static s64 part2 (const Parsed& in)
    {
        auto first_values  = project(in, true);
        auto second_values = project(in, false);

        s64 res = 0;
        for (const auto& f : first_values)
            res += f * std::count(second_values.begin(), second_values.end(), f);

        return res;
    }

    static constexpr pair<s64, s64> expected ()
    {
        return {1882714, 19437052}; // replace with real answers
    }
};
} // namespace day01
