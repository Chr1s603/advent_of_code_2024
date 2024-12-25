#include HEADER_NAME

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string_regex.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm/count_if.hpp>
#include <cmath>
#include <iostream>

using namespace std;
using namespace boost;

namespace NAMESPACE_NAME {

std::string
parse_input (std::ifstream &s)
{
    return { istreambuf_iterator<char>(s), {} };
}

struct por
{
    bool follows_rule (std::vector<int64_t> &values, bool fix = false) const
    {
        int64_t first_second_idx { -1 };
        int64_t last_first_idx { -1 };
        for (int64_t idx = static_cast<int64_t>(values.size()) - 1; idx >= 0; --idx)
            if (values[idx] == first)
                last_first_idx = idx;
            else if (values[idx] == second)
                first_second_idx = first_second_idx >= 0 ? first_second_idx : idx;
        if (first_second_idx == -1 || last_first_idx == -1)
            return true; // One or both elements are not found
        if (last_first_idx > first_second_idx)
        {
            if (!fix)
                return false;
            std::swap(values[last_first_idx], values[first_second_idx]);
            return follows_rule(values, true);
        }
        return true;
    }

    int64_t first;
    int64_t second;
};

std::vector<por>
parse_pors (std::string &in)
{
    std::vector<string> por_strings;
    split(por_strings, in, is_any_of("\n"));

    std::vector<por> res;
    for (auto &s : por_strings)
    {
        std::vector<string> first_second;
        split(first_second, s, is_any_of("|"));
        res.emplace_back(atoll(first_second[0].c_str()),
                         atoll(first_second[1].c_str()));
    }
    return res;
}

using ptp_t = std::vector<int64_t>;
std::vector<ptp_t>
parse_ptps (std::string &in)
{
    trim(in);
    std::vector<string> ptp_strings;
    split(ptp_strings, in, is_any_of("\n"));

    std::vector<ptp_t> res;
    for (auto &s : ptp_strings)
    {
        std::vector<string> values;
        split(values, s, is_any_of(","));
        std::vector<int64_t> ptp;
        ranges::transform(values, std::back_inserter(ptp),
                          [] (const std::string &x) { return std::stoll(x); });
        res.emplace_back(ptp);
    }
    return res;
}

std::pair<std::vector<por>, std::vector<ptp_t>>
parse_pors_and_ptps (const std::string &in)
{
    std::vector<string> por_ptp; // PageOrderingRules PagesToProduce
    split_regex(por_ptp, in, regex("\n\n"));
    return { parse_pors(por_ptp[0]), parse_ptps(por_ptp[1]) };
}

int64_t
part1 (const std::string &in)
{
    auto [pors, ptps] { parse_pors_and_ptps(in) };

    int64_t res { 0 };
    for (auto &ptp : ptps)
    {
        bool ok { true };
        for (const auto &por : pors)
            if (!por.follows_rule(ptp))
                ok = false;
        if (ok)
            res += ptp[std::floor(ptp.size() / 2.0)];
    }

    return res;
}

int64_t
part2 (const std::string &in)
{
    auto [pors, ptps] { parse_pors_and_ptps(in) };

    int64_t res { 0 };
    for (auto &ptp : ptps)
    {
        bool valid = true;
        bool retry;
        do
        {
            retry = false;
            for (const auto &por : pors)
            {
                if (!por.follows_rule(ptp))
                {
                    valid = false;
                    retry = true;
                    por.follows_rule(ptp, true);
                    assert(por.follows_rule(ptp));
                }
            }
        } while (retry);
        if (!valid)
            res += ptp[std::floor(ptp.size() / 2.0)];
    }

    return res;
}

}
