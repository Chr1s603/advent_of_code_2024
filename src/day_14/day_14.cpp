#include <cmath>
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>
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

struct patrol_robot
{
    explicit patrol_robot(const string& s)
    {
        auto parse_coords = [](const string& part) {
            const auto start = part.find('=') + 1;
            const auto comma = part.find(',');
            return pos_t{stoll(part.substr(start, comma - start)), stoll(part.substr(comma + 1))};
        };

        const size_t split_pos = s.find("v=");
        pos = parse_coords(s.substr(0, split_pos));
        vel = parse_coords(s.substr(split_pos));
    }

    pos_t pos;
    pos_t vel;
};

void move_robot(patrol_robot& r, const int64_t room_w, const int64_t room_h)
{
    r.pos = { x(r.pos) + x(r.vel), y(r.pos) + y(r.vel) };
    r.pos = { (x(r.pos) % room_w + room_w) % room_w, (y(r.pos) % room_h + room_h) % room_h };
}


int64_t part1 (const string &in)
{
    vector<string> robot_descriptions;
    split(robot_descriptions, in, is_any_of("\n"));

    vector<patrol_robot> robots;
    for (const auto& d : robot_descriptions)
        robots.emplace_back(d);

    constexpr int64_t w { 101 };
    constexpr int64_t h { 103 };
    for (int64_t seconds = 0; seconds < 100; seconds++)
        for (auto& r : robots)
            move_robot(r, w, h);

    int64_t nw = 0, ne = 0, sw = 0, se = 0;
    for (const auto& r : robots)
        if (x(r.pos) < w / 2 && y(r.pos) < h / 2)
            nw++;
        else if (x(r.pos) > w / 2 && y(r.pos) < h / 2)
            ne++;
        else if (x(r.pos) < w / 2 && y(r.pos) > h / 2)
            sw++;
        else if (x(r.pos) > w / 2 && y(r.pos) > h / 2)
            se++;

    return nw * ne * sw * se;
}

int64_t part2 (const string &in) { return 0; }

}
