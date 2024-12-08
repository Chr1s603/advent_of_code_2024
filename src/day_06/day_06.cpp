#include HEADER_NAME

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string_regex.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm/count_if.hpp>
#include <boost/unordered_map.hpp>
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

int64_t
part1 (const std::string &in)
{
    map m { in };

    boost::unordered_map<std::pair<int64_t, int64_t>, int64_t> visited;
    const auto initial_guard_pos { m.get_guard_pos() };
    auto       current_guard_pos { initial_guard_pos };
    do
    {
        const auto next_guard_pos = m.advance_guard_pos();
        visited.insert({ next_guard_pos, 1 });
        current_guard_pos = next_guard_pos;
    } while (current_guard_pos != std::pair<int64_t, int64_t> { -1, -1 });

    return visited.size();
}

int64_t
part2 (const std::string &in)
{
    const map m { in };
    int64_t   res { 0 };
    return res;
}

}
