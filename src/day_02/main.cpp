#include HEADER_NAME

#include <fstream>
#include <iostream>

using namespace NAMESPACE_NAME;

int main() {
    std::ifstream input_file(INPUT_FILE_PATH);
    if (!input_file)
    {
        std::cerr << "Failed to open input file " << INPUT_FILE_PATH << std::endl;
        return 1;
    }

    const auto input = parse_input(input_file);
    std::cout << "Part 1: " << part1(input) << std::endl;
    std::cout << "Part 2: " << part2(input) << std::endl;

    return 0;
}
