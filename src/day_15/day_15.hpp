#pragma once

#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>
#include <boost/algorithm/string.hpp>
#include <cstdint>
#include <fstream>
#include <ranges>
#include <vector>

namespace NAMESPACE_NAME {

enum class cmd { up, down, right, left };
using cmd_vec = std::vector<cmd>;

inline cmd_vec
parse_cmds(const std::string& in)
{
    std::string only_cmds { in };
    boost::remove_erase_if(only_cmds, [](char c) {
        return c != '<' && c != '>' && c != '^' && c != 'v';
    });

    cmd_vec result;
    result.reserve(only_cmds.size());
    for (const auto& c : only_cmds)
        switch (c)
        {
            case '<': result.emplace_back(cmd::left);  break;
            case '>': result.emplace_back(cmd::right); break;
            case '^': result.emplace_back(cmd::up);    break;
            default: result.emplace_back(cmd::down);   break;
        }

    return result;
}

struct map
{
    // clang-format off
    enum class entity_t { nothing, robot, wall, box };
    [[nodiscard]] static entity_t parse_field_entity (const char c)
    {
        switch (c)
        {
            case '.': return entity_t::nothing;
            case 'O': return entity_t::box;
            case '@': return entity_t::robot;
            case '#': return entity_t::wall;
            default: return entity_t::nothing;
        }
    }
    // clang-format on

    explicit map (const std::string &map_data)
    {
        std::vector<std::string> lines;
        boost::split(lines, map_data, boost::is_any_of("\n"));
        for (_rows = 0; _rows < lines.size(); ++_rows)
            if (lines[_rows].empty()) break;
        _cols = static_cast<int64_t>(lines[0].size());
        _field.reserve(_rows * _cols);
        for (int32_t row = 0; row < _rows; row++)
            for (int32_t col = 0; col < _cols; col++)
            {
                const auto entity { parse_field_entity(lines[row][col]) };
                _field.emplace_back(entity);
                if (entity == entity_t::robot)
                    _robot_pos = {row, col};
            }
    }

    static std::pair<int64_t, int64_t> get_next_pos (
        const std::pair<int64_t, int64_t> &pos, const cmd c)
    {
        const auto [row, col] = pos;
        switch (c)
        {
            case cmd::up: return { row, col - 1 };
            case cmd::right: return { row + 1, col };
            case cmd::down: return { row, col + 1 };
            case cmd::left: return { row - 1, col };
            default: return { -1, -1 };
        }
    }

    bool
    try_move_boxes(std::pair<int64_t, int64_t>o, std::pair<int64_t, int64_t> n)
    {
        const auto xdiff { n.first - o.first };
        const auto ydiff { n.second - o.second };
        if (xdiff > 0)
        {
            for (int i = n.first + 1; i < _rows; ++i)
                if (get_entity({i, n.second}) == entity_t::wall)
                    return false;
                else if (get_entity({i, n.second}) == entity_t::nothing)
                {
                    get_entity({i, n.second}) = entity_t::box;
                    get_entity(o) = entity_t::nothing;
                    return true;
                }
        }
        else if (xdiff < 0)
        {
            for (int i = n.first - 1; i >= 0; --i)
                if (get_entity({i, n.second}) == entity_t::wall)
                    return false;
                else if (get_entity({i, n.second}) == entity_t::nothing)
                {
                    get_entity({i, n.second}) = entity_t::box;
                    get_entity(o) = entity_t::nothing;
                    return true;
                }
        }
        if (ydiff > 0)
        {
            for (int i = n.second + 1; i < _cols; ++i)
                if (get_entity({n.first, i}) == entity_t::wall)
                    return false;
                else if (get_entity({n.first, i}) == entity_t::nothing)
                {
                    get_entity({n.first, i}) = entity_t::box;
                    get_entity(o) = entity_t::nothing;
                    return true;
                }
        }
        else if (ydiff < 0)
        {
            for (int i = n.second - 1; i >= 0; --i)
                if (get_entity({n.first, i}) == entity_t::wall)
                    return false;
                else if (get_entity({n.first, i}) == entity_t::nothing)
                {
                    get_entity({n.first, i}) = entity_t::box;
                    get_entity(o) = entity_t::nothing;
                    return true;
                }
        }
        return false;
    }

    void
    move_robot(const cmd c)
    {
        auto old_pos { _robot_pos };
        auto new_pos { get_next_pos(_robot_pos, c) };
        auto ent = get_entity(new_pos);
        switch (ent)
        {
            case entity_t::nothing: _robot_pos = new_pos; return;
            case entity_t::wall:  return;
            case entity_t::robot: return;
            case entity_t::box:
                if (try_move_boxes(old_pos, new_pos))
                    _robot_pos = new_pos;
                return;
        }
    }

    entity_t &get_entity (const std::pair<int64_t, int64_t> &pos)
    {
        const auto [row, col] = pos;
        return _field[row * _cols + col];
    }

    std::pair<int, int>   _robot_pos;
    int64_t               _rows, _cols;
    std::vector<entity_t> _field;
};

std::string parse_input(std::ifstream &s);

int64_t part1(const std::string &in);
int64_t part2(const std::string &in);

}
