#include HEADER_NAME

#include <boost/algorithm/string.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/unordered_map.hpp>
#include <iostream>
#include <ranges>

using namespace std;
using namespace boost;

namespace NAMESPACE_NAME {

std::string
parse_input (std::ifstream &s)
{
    return { istreambuf_iterator<char>(s), {} };
}

boost::unordered_map<std::pair<int64_t, int64_t>, int64_t>
get_distinct_visited_positions (map &m)
{
    boost::unordered_map<std::pair<int64_t, int64_t>, int64_t> visited;
    const auto initial_guard_pos { m.get_guard_pos() };
    auto       current_guard_pos { initial_guard_pos };
    do
    {
        const auto next_guard_pos = m.advance_guard_pos();
        visited.insert({ next_guard_pos, 1 });
        current_guard_pos = next_guard_pos;
    } while (current_guard_pos != std::pair<int64_t, int64_t> { -1, -1 });

    return visited;
}

int64_t
part1 (const std::string &in)
{
    map m { in };
    return get_distinct_visited_positions(m).size();
}

int64_t
part2 (const std::string &in)
{
    map        m { in };
    const auto initial_guard_pos { m.get_guard_pos() };
    const auto distinct_positions { get_distinct_visited_positions(m) };

    int64_t obstacles_that_result_in_a_loop { 0 };
    for (const auto &obstacle : std::views::keys(distinct_positions))
    {
        if (m.guard_pos_oob(obstacle))
            continue;
        if (obstacle == initial_guard_pos)
            continue;

        map m_w_obstacle { in };
        m_w_obstacle.get_entity(obstacle) = map::entity_t::obstacle;

        boost::unordered_map<std::pair<int64_t, int64_t>, map::entity_t> visited;
        bool guard_oob { false };
        do
        {
            const auto current_guard_pos  = m_w_obstacle.advance_guard_pos();
            const auto current_guard_type = m_w_obstacle.get_entity(current_guard_pos);
            if (visited.contains(current_guard_pos)
                && visited[current_guard_pos] == current_guard_type)
                break;
            visited.insert({ current_guard_pos, current_guard_type });

            guard_oob = current_guard_pos == std::pair<int64_t, int64_t> { -1, -1 };
        } while (!guard_oob);

        if (!guard_oob)
            obstacles_that_result_in_a_loop++;
    }

    return obstacles_that_result_in_a_loop;
}

}
