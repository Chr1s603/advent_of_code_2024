#pragma once

#include <cstdint>
#include <fstream>

namespace NAMESPACE_NAME {

std::string parse_input(std::ifstream &s);

int64_t part1(const std::string &in);
int64_t part2(const std::string &in);

}
