#pragma once

#include <cstdint>
#include <fstream>
#include <vector>

namespace NAMESPACE_NAME {

std::vector<std::pair<int64_t, int64_t>> parse_input(std::ifstream &s);

int64_t part1(const std::vector<std::pair<int64_t, int64_t>> &in);
int64_t part2(const std::vector<std::pair<int64_t, int64_t>> &in);

}
