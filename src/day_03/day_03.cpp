#include HEADER_NAME

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/find_iterator.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/combine.hpp>
#include <iostream>

using namespace std;
using namespace boost;

namespace NAMESPACE_NAME {
std::string
parse_input (std::ifstream &s)
{
    return { istreambuf_iterator<char>(s), {} };
}

int64_t
part1 (const std::string &in)
{
    int64_t res = 0;

    for (auto it = make_find_iterator(in, first_finder("mul", is_iequal()));
         it != decltype(it)(); ++it)
    {
        if (auto start = it->end(); start != in.end() && *start == '(')
        {
            ++start;
            std::string first;

            auto read_digits = [&start, &in] (std::string &target) {
                while (target.size() < 3 && start != in.end() && isdigit(*start))
                    target += *start++;
            };

            read_digits(first);

            if (start != in.end() && *start++ == ',')
            {
                std::string second;
                read_digits(second);
                if (start != in.end() && *start == ')')
                    res += stoi(first) * stoi(second);
            }
        }
    }

    return res;
}

struct do_dont_entry
{
    size_t index;
    bool   is_do_cmd;
};

vector<do_dont_entry>
parse_do_and_dont_entries (const std::string &input)
{
    vector<do_dont_entry> results;
    for (auto range = make_iterator_range(input); !range.empty();)
    {
        auto doRange   = find_first(range, "do()");
        auto dontRange = find_first(range, "don't()");

        if (!doRange.empty()
            && (dontRange.empty() || doRange.begin() < dontRange.begin()))
        {
            results.emplace_back(doRange.begin() - input.begin(), true);
            range = make_iterator_range(doRange.end(), range.end());
        }
        else if (!dontRange.empty())
        {
            results.emplace_back(dontRange.begin() - input.begin(), false);
            range = make_iterator_range(dontRange.end(), range.end());
        }
        else
        {
            break;
        }
    }

    return results;
}

int64_t
part2 (const std::string &in)
{
    const auto  do_and_dont_entries = parse_do_and_dont_entries(in);
    std::string modified            = in;

    size_t prev_idx       = 0;
    bool   is_mul_enabled = true;

    for (const auto &[index, is_do_cmd] : do_and_dont_entries)
    {
        if (!is_mul_enabled)
        {
            auto section = in.substr(prev_idx, index - prev_idx);
            replace_all(section, "mul", "666");
            modified.replace(prev_idx, index - prev_idx, section);
        }

        prev_idx       = index;
        is_mul_enabled = is_do_cmd;
    }

    return part1(modified);
}

}
