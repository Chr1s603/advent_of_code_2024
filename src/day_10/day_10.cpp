#include <vector>
#include <string>
#include <set>
#include <numeric>
#include <fstream>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

namespace NAMESPACE_NAME {

string
parse_input (ifstream &s)
{
    return { istreambuf_iterator<char>(s), {} };
}

// clang-format off
using pos_t = pair<int64_t, int64_t>;
int64_t x (const pos_t &p) { return get<0>(p); }
int64_t y (const pos_t &p) { return get<1>(p); }
// clang-format on

struct topographical_map
{
    explicit topographical_map(const string &in)
    {
        vector<string> lines;
        split(lines, in, is_any_of("\n"));

        height = static_cast<int64_t>(lines.size());
        width = static_cast<int64_t>(lines[0].size());
        map.resize(width * height);
        for (int64_t y = 0; y < height; ++y)
            for (int64_t x = 0; x < width; ++x)
            {
                map[y * width + x] = lines[y][x] - '0';
                if (lines[y][x] == '0')
                    trailheads.emplace_back(x, y);
            }
    }

    [[nodiscard]] int64_t at(const int64_t x, const int64_t y) const { return map[y * width + x]; }
    [[nodiscard]] int64_t at(const pos_t &p) const { return at(x(p), y(p)); }

    int64_t width, height;
    vector<int64_t> map;
    vector<pos_t> trailheads;
};

int64_t
calc_trailhead_common_impl(const pos_t &p, const int64_t last,
    const topographical_map &map, set<pos_t> &visited,
    set<pos_t> *reachable_nines)
{
    if (x(p) < 0 || x(p) >= map.width || y(p) < 0 || y(p) >= map.height)
        return 0;

    if (visited.contains(p))
        return 0;

    const auto current = map.at(p);
    if (current != last + 1)
        return 0;
    visited.insert(p);

    if (current == 9)
    {
        if (reachable_nines)
            reachable_nines->insert(p);

        visited.erase(p);

        return 1;
    }

    const pos_t n{x(p), y(p) - 1};
    const pos_t s{x(p), y(p) + 1};
    const pos_t w{x(p) - 1, y(p)};
    const pos_t e{x(p) + 1, y(p)};

    int64_t result = 0;
    result += calc_trailhead_common_impl(n, current, map, visited, reachable_nines);
    result += calc_trailhead_common_impl(s, current, map, visited, reachable_nines);
    result += calc_trailhead_common_impl(w, current, map, visited, reachable_nines);
    result += calc_trailhead_common_impl(e, current, map, visited, reachable_nines);

    visited.erase(p); // Backtrack: unmark the node for other paths

    return result;
}

int64_t
calc_trailhead_score(const pos_t &th, const topographical_map &map)
{
    set<pos_t> visited;
    set<pos_t> reachable_nines;
    calc_trailhead_common_impl(th, -1, map, visited, &reachable_nines);
    return static_cast<int64_t>(reachable_nines.size());
}

int64_t
calc_trailhead_rating(const pos_t &th, const topographical_map &map)
{
    set<pos_t> visited;
    return calc_trailhead_common_impl(th, -1, map, visited, nullptr);
}

int64_t
part1 (const string &in)
{
    const topographical_map map { in };
    return accumulate(map.trailheads.begin(), map.trailheads.end(), 0LL,
        [&](int64_t sum, const auto& th) { return sum + calc_trailhead_score(th, map); });
}

int64_t
part2(const string &in)
{
    const topographical_map map{in};
    return accumulate(map.trailheads.begin(), map.trailheads.end(), 0LL,
                      [&](int64_t sum, const auto &th) { return sum + calc_trailhead_rating(th, map); });
}

}
