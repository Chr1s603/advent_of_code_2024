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
try_win_claw_machine(const claw_machine& m, const int64_t max_btn_press = 100)
{
    // Cramer's Rule:
    //
    // a * x_1 + b * x_2 = c
    // d * x_1 + e * x_2 = f
    //
    // -> x_1 = det(c,b;f,e) / det(a,b;d,e) = (c*e - f*b) / (a*e - d*b)
    // -> x_2 = det(a,c;d,f) / det(a,b;d,e) = (d*c - a*f) / (a*e - d*b)

    const auto a { static_cast<double>(m.buttonA.first) };
    const auto b { static_cast<double>(m.buttonB.first) };
    const auto c { static_cast<double>(m.prize.first) };
    const auto d { static_cast<double>(m.buttonA.second) };
    const auto e { static_cast<double>(m.buttonB.second) };
    const auto f { static_cast<double>(m.prize.second) };

    const double det_A = a * e - d * b;
    constexpr double epsilon = 1e-9;
    if (std::abs(det_A) < epsilon)
        return invalid_cost;

    const double x1 = (c * e - f * b) / det_A;
    const double x2 = (a * f - d * c) / det_A;
    if (std::abs(x1 - std::round(x1)) < epsilon && std::abs(x2 - std::round(x2)) < epsilon)
    {
        const auto rounded_x1 { static_cast<int64_t>(round(x1)) };
        const auto rounded_x2 { static_cast<int64_t>(round(x2)) };
        if (rounded_x1 <= max_btn_press && rounded_x2 <= max_btn_press && rounded_x1 >= 0 && rounded_x2 >= 0)
            return rounded_x1 * 3 + rounded_x2;
    }

    return invalid_cost;
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

int64_t part2 (const string &in)
{
    auto claws { parse_claw_machines(in) };

    int64_t result { 0 };
    for (auto& m : claws)
    {
        constexpr int64_t offset { 10000000000000 };
        m.prize = { m.prize.first + offset, m.prize.second + offset };
        if (const auto min_cost {try_win_claw_machine(m, numeric_limits<int64_t>::max()) }; min_cost != invalid_cost)
            result += min_cost;
    }
    return result;
}

}
