#pragma once

#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/algorithm/string.hpp>
#include <cstdint>
#include <fstream>
#include <vector>

namespace NAMESPACE_NAME {

struct map
{
    // clang-format off
    enum class entity_t { nothing, guard_n, guard_e, guard_s, guard_w, obstacle };
    [[nodiscard]] static entity_t parse_field_entity (const char c)
    {
        switch (c)
        {
            case '.': return entity_t::nothing;
            case '#': return entity_t::obstacle;
            case '^': return entity_t::guard_n;
            case '>': return entity_t::guard_e;
            case 'v': return entity_t::guard_s;
            case '<': return entity_t::guard_w;
            default: return entity_t::nothing;
        }
    }

    static std::pair<int64_t, int64_t> get_next_pos (
        const std::pair<int64_t, int64_t> &pos, const entity_t e)
    {
        const auto [row, col] = pos;
        switch (e)
        {
            case entity_t::guard_n: return { row - 1, col };
            case entity_t::guard_e: return { row, col + 1 };
            case entity_t::guard_s: return { row + 1, col };
            case entity_t::guard_w: return { row, col - 1 };
            default: return { -1, -1 };
        }
    }
    // clang-format on

    explicit map (const std::string &map_data)
    {
        std::vector<std::string> lines;
        boost::split(lines, map_data, boost::is_any_of("\n"));
        _rows = static_cast<int64_t>(lines.size());
        _cols = static_cast<int64_t>(lines[0].size());
        _field.reserve(_rows * _cols);
        for (const auto &l : lines)
            for (const auto &c : l)
                _field.emplace_back(parse_field_entity(c));
    }

    std::pair<int64_t, int64_t> get_guard_pos () const
    {
        for (int64_t row = 0; row < _rows; ++row)
            for (int64_t col = 0; col < _cols; ++col)
                if (boost::algorithm::any_of_equal(
                        std::array<entity_t, 4> { entity_t::guard_n, entity_t::guard_e,
                                                  entity_t::guard_s,
                                                  entity_t::guard_w },
                        _field[row * _cols + col]))
                    return { row, col };
        return { -1, -1 };
    }

    bool guard_pos_oob (const std::pair<int64_t, int64_t> &pos) const
    {
        if (const auto [row, col] = pos;
            row < 0 || row >= _rows || col < 0 || col >= _cols)
            return true;
        return false;
    }
    bool guard_pos_valid (const std::pair<int64_t, int64_t> &pos) const
    {
        if (const auto [row, col] = pos;
            row < 0 || row >= _rows || col < 0 || col >= _cols)
            return false;
        return get_entity(pos) == entity_t::nothing;
    }

    std::pair<int64_t, int64_t> advance_guard_pos ()
    {
        const auto guard_type        = get_entity(get_guard_pos());
        const auto current_guard_pos = get_guard_pos();
        const auto next_pos
            = get_next_pos(current_guard_pos, get_entity(current_guard_pos));
        if (guard_pos_oob(next_pos))
            return { -1, -1 };
        if (guard_pos_valid(next_pos))
        {
            get_entity(current_guard_pos) = entity_t::nothing;
            get_entity(next_pos)          = guard_type;
            return next_pos;
        }

        if (guard_type == entity_t::guard_e)
            get_entity(current_guard_pos) = entity_t::guard_s;
        else if (guard_type == entity_t::guard_s)
            get_entity(current_guard_pos) = entity_t::guard_w;
        else if (guard_type == entity_t::guard_w)
            get_entity(current_guard_pos) = entity_t::guard_n;
        else
            get_entity(current_guard_pos) = entity_t::guard_e;

        return current_guard_pos;
    }

    entity_t get_entity (const std::pair<int64_t, int64_t> &pos) const
    {
        const auto [row, col] = pos;
        return _field[row * _cols + col];
    }
    entity_t &get_entity (const std::pair<int64_t, int64_t> &pos)
    {
        const auto [row, col] = pos;
        return _field[row * _cols + col];
    }

    int64_t               _rows, _cols;
    std::vector<entity_t> _field;
};

std::string parse_input(std::ifstream &s);

int64_t part1(const std::string &in);
int64_t part2(const std::string &in);

}
