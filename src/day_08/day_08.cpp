#include HEADER_NAME

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string_regex.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm/count_if.hpp>
#include <cmath>
#include <iostream>
#include <ranges>

using namespace std;
using namespace boost;

namespace NAMESPACE_NAME {

string
parse_input (ifstream &s)
{
    return { istreambuf_iterator<char>(s), {} };
}

using pos_t         = pair<int64_t, int64_t>;
using antenna_map_t = map<char, vector<pos_t>>;
using antinodes_t   = set<pos_t>;

struct antenna_map
{
    int64_t       rows, cols;
    antenna_map_t map;
};

antenna_map
parse_antennas (const string &s)
{
    vector<string> lines;
    split(lines, s, is_any_of("\n"));

    antenna_map_t res;
    const int64_t rows { static_cast<int64_t>(lines.size()) };
    const int64_t cols { static_cast<int64_t>(lines[0].size()) };
    for (int64_t x = 0; x < rows; x++)
        for (int64_t y = 0; y < cols; y++)
            if (lines[x][y] != '.')
            {
                if (const char c { lines[x][y] }; res.contains(c))
                    res[c].emplace_back(x, y);
                else
                    res.insert({ c, { { x, y } } });
            }

    return { rows, cols, res };
}

// clang-format off
int64_t x (const pos_t &p) { return get<0>(p); }
int64_t y (const pos_t &p) { return get<1>(p); }
// clang-format on

int64_t
in_bounds (const pos_t &p, const int64_t rows, const int64_t cols)
{
    return x(p) >= 0 && x(p) < cols && //
           y(p) >= 0 && y(p) < rows;
}

antinodes_t
gen_resonance_nodes (const pos_t &pos_a, const pos_t &pos_b, const int64_t rows,
                     const int64_t cols, const bool resonance = false)
{
    const pos_t dist { x(pos_b) - x(pos_a), y(pos_b) - y(pos_a) };

    antinodes_t res;
    if (resonance)
    {
        res.emplace(pos_a);
        res.emplace(pos_b);
    }

    const int64_t limit { resonance ? max(rows, cols) : 2ll };
    for (int64_t i = 1; i < limit; i++)
    {
        const pos_t antinode_a { x(pos_a) - i * x(dist), y(pos_a) - i * y(dist) };
        const pos_t antinode_b { x(pos_b) + i * x(dist), y(pos_b) + i * y(dist) };
        const bool  a_in_bounds = in_bounds(antinode_a, rows, cols);
        const bool  b_in_bounds = in_bounds(antinode_b, rows, cols);
        if (a_in_bounds)
            res.emplace(antinode_a);
        if (b_in_bounds)
            res.emplace(antinode_b);

        if (!a_in_bounds && !b_in_bounds)
            break;
    }

    return res;
}

antinodes_t
gen_antinodes_inner (const vector<pos_t> &antenna_positions, const int64_t rows,
                     const int64_t cols, const bool resonance = false)
{
    antinodes_t res;
    for (int64_t i = 0; i < antenna_positions.size(); i++)
        for (int64_t j = i + 1; j < antenna_positions.size(); j++)
        {
            const pos_t pos_a { antenna_positions[i] };
            const pos_t pos_b { antenna_positions[j] };
            const pos_t dist { x(pos_b) - x(pos_a), y(pos_b) - y(pos_a) };
            res.merge(gen_resonance_nodes(pos_a, pos_b, rows, cols, resonance));
        }

    return res;
}

antinodes_t
gen_antinodes (const antenna_map &antennas, const bool resonance = false)
{
    antinodes_t res;
    for (const auto &a : antennas.map)
        res.merge(
            gen_antinodes_inner(get<1>(a), antennas.rows, antennas.cols, resonance));
    return res;
}

int64_t
part1 (const string &in)
{
    const auto antennas { parse_antennas(in) };
    return static_cast<int64_t>(gen_antinodes(antennas).size());
}

int64_t
part2 (const string &in)
{
    const auto antennas { parse_antennas(in) };
    return static_cast<int64_t>(gen_antinodes(antennas, true).size());
}

}
