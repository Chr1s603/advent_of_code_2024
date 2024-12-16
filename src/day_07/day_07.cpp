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

std::string
parse_input (std::ifstream &s)
{
    return { istreambuf_iterator<char>(s), {} };
}

struct equation
{
    // clang-format off
    enum class op { add, mul, concat };
    // clang-format on
    explicit equation (const std::string &in_unf)
    {
        auto                in { trim_copy(in_unf) };
        std::vector<string> result_and_values;
        split(result_and_values, in, is_any_of(":"));
        _result = strtoll(result_and_values[0].c_str(), nullptr, 10);

        trim(result_and_values[1]);
        std::vector<string> values;
        split(values, result_and_values[1], is_any_of(" "));
        for (const auto &v : values)
            _values.emplace_back(strtoll(trim_copy(v).c_str(), nullptr, 10));
    }

    [[nodiscard]] int64_t eval (const std::vector<op> &ops,
                                const unsigned long    until) const
    {
        int64_t res = _values.front();
        for (const auto &[i, o] : views::enumerate(ops))
            switch (o)
            {
                case op::add:
                    res += _values[i + 1];
                    break;
                case op::mul:
                    res *= _values[i + 1];
                    break;
                case op::concat: {
                    string left  = to_string(res);
                    string right = to_string(_values[i + 1]);
                    res          = stoll(left + right);
                    break;
                }
                default:;
            }
        return res;
    }

    std::optional<vector<op>> try_solve_eq (vector<op> &current_ops,
                                            const bool  con) const
    {
        const auto current_op_cnt = current_ops.size();
        if (const auto target_op_cnt = _values.size() - 1;
            current_op_cnt == target_op_cnt)
        {
            if (eval(current_ops, current_op_cnt) == _result)
                return current_ops;
            return nullopt;
        }

        auto next_ops = current_ops;
        next_ops.emplace_back(op::mul);
        if (eval(next_ops, current_op_cnt + 1) <= _result)
            if (auto opt = try_solve_eq(next_ops, con); opt.has_value())
                return opt;

        next_ops[current_op_cnt] = op::add;
        if (eval(next_ops, current_op_cnt + 1) <= _result)
            if (auto opt = try_solve_eq(next_ops, con); opt.has_value())
                return opt;

        next_ops[current_op_cnt] = op::concat;
        if (con)
            if (eval(next_ops, current_op_cnt + 1) <= _result)
                if (auto opt = try_solve_eq(next_ops, con); opt.has_value())
                    return opt;

        return nullopt;
    }

    int64_t              _result;
    std::vector<int64_t> _values;
};

std::vector<equation>
parse_equations (string in)
{
    std::vector<string> eq_strings;
    split(eq_strings, in, is_any_of("\n"));
    std::vector<equation> res;
    for (auto &s : eq_strings)
        res.emplace_back(s);
    return res;
}

int64_t
part1 (const string &in)
{
    auto eqs { parse_equations(in) };

    int64_t res { 0 };
    for (const auto &[i, eq] : views::enumerate(eqs))
    {
        std::vector<equation::op> ops;
        ops.reserve(eq._values.size());
        if (eq.try_solve_eq(ops, false).has_value())
            res += eq._result;
    }
    return res;
}

int64_t
part2 (const string &in)
{
    auto eqs { parse_equations(in) };

    int64_t res { 0 };
    for (const auto &[i, eq] : views::enumerate(eqs))
    {
        std::vector<equation::op> ops;
        ops.reserve(eq._values.size());
        if (eq.try_solve_eq(ops, true).has_value())
            res += eq._result;
    }
    return res;
}

}
