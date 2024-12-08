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

struct equation
{
    enum class op
    {
        add,
        mul
    };
    explicit equation (const std::string &in_unf)
    {
        auto                in { boost::trim_copy(in_unf) };
        std::vector<string> result_and_values;
        split(result_and_values, in, is_any_of(":"));
        _result = std::strtoll(result_and_values[0].c_str(), nullptr, 10);

        boost::trim(result_and_values[1]);
        std::vector<string> values;
        split(values, result_and_values[1], is_any_of(" "));
        for (const auto &v : values)
            _values.emplace_back(
                std::strtoll(boost::trim_copy(v).c_str(), nullptr, 10));
    }
    int64_t              _result;
    std::vector<int64_t> _values;
};

std::vector<equation>
parse_equations (std::string in)
{
    std::vector<string> eq_strings;
    split(eq_strings, in, is_any_of("\n"));
    std::vector<equation> res;
    for (auto &s : eq_strings)
        res.emplace_back(s);
    return res;
}

int64_t
part1 (const std::string &in)
{
    auto    eqs { parse_equations(in) };
    int64_t res { 0 };
    return res;
}

int64_t
part2 (const std::string &in)
{
    auto    eqs { parse_equations(in) };
    int64_t res { 0 };
    return res;
}

}
