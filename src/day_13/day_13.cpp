#include <cmath>
#include <regex>
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

struct claw_machine
{
    static int64_t
    cost(const int64_t times_button_a_pressed, const int64_t times_button_b_pressed)
    {
        return times_button_a_pressed * 3 + times_button_b_pressed;
    }

    pos_t buttonA;
    pos_t buttonB;
    pos_t prize;
};

vector<claw_machine>
parse_claw_machines(const std::string& input) {
    const regex line_regex(
        R"(Button A: X\+(\d+), Y\+(\d+)\nButton B: X\+(\d+), Y\+(\d+)\nPrize: X=(\d+), Y=(\d+))"
    );

    const auto begin = sregex_iterator(input.begin(), input.end(), line_regex);
    const auto end = sregex_iterator();

    vector<claw_machine> machines;
    for (auto it = begin; it != end; ++it) {
        const smatch& match = *it;
        claw_machine machine{
                { stoll(match[1]), stoll(match[2]) },
                { stoll(match[3]), stoll(match[4]) },
                { stoll(match[5]), stoll(match[6]) }
        };

        machines.push_back(machine);
    }

    return machines;
}

constexpr auto invalid_cost { numeric_limits<int64_t>::max() };

int64_t
try_win_claw_machine(const claw_machine& m) {
    int64_t min_cost = invalid_cost;

    for (int64_t a = 0; a <= 100; a++)
    {
        for (int64_t b = 0; b <= 100; b++)
        {
            const auto x_move = a * x(m.buttonA) + b * x(m.buttonB);
            const auto y_move = a * y(m.buttonA) + b * y(m.buttonB);

            if (x_move == x(m.prize) && y_move == y(m.prize))
            {
                const auto cost = claw_machine::cost(a, b);
                min_cost = min(min_cost, cost);
            }
        }
    }

    return min_cost;
}

int64_t part1 (const string &in)
{
    const auto claws { parse_claw_machines(in) };

    int64_t result { 0 };
    for (const auto& m : claws)
        if (const auto min_cost {try_win_claw_machine(m) }; min_cost != invalid_cost)
            result += min_cost;
    return result;
}

int64_t part2 (const string &in) { return 0; }

}
