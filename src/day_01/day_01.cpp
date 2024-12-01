#include HEADER_NAME

#include <boost/algorithm/string.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/count.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/combine.hpp>
#include <boost/tuple/tuple.hpp>

namespace NAMESPACE_NAME {

std::vector<std::pair<int64_t, int64_t>>
parse_input (std::ifstream &s)
{
    int64_t                                  a, b;
    std::vector<std::pair<int64_t, int64_t>> res;
    while (s >> a >> b)
        res.emplace_back(std::pair<int64_t, int64_t> { a, b });
    return res;
}

int64_t
part1 (const std::vector<std::pair<int64_t, int64_t>> &in)
{
    std::vector<int64_t> first_values;
    boost::push_back(
        first_values,
        in | boost::adaptors::transformed([] (const auto &p) { return p.first; }));
    boost::sort(first_values);

    std::vector<int64_t> second_values;
    boost::push_back(
        second_values,
        in | boost::adaptors::transformed([] (const auto &p) { return p.second; }));
    boost::sort(second_values);

    int64_t res { 0 };
    for (const auto &pair : boost::combine(first_values, second_values))
    {
        const auto first  = boost::get<0>(pair);
        const auto second = boost::get<1>(pair);
        res += std::abs(first - second);
    }

    return res;
}

int64_t
part2 (const std::vector<std::pair<int64_t, int64_t>> &in)
{
    auto first_values
        = in | boost::adaptors::transformed([] (const auto &p) { return p.first; });
    auto second_values
        = in | boost::adaptors::transformed([] (const auto &p) { return p.second; });

    int64_t res { 0 };
    for (const auto &first : first_values)
        res += first * boost::count(second_values, first);

    return res;
}

}
