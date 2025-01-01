#include HEADER_NAME

#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/fusion/container/map/map_fwd.hpp>

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

int64_t part1 (const string &in)
{
    map m { in };
    const auto cmds { parse_cmds(in) };
    for (const auto& c : cmds)
        m.move_robot(c);

    return 0;
}

int64_t part2 (const string &in) { return 0; }

}
