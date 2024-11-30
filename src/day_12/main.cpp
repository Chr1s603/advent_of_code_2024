#include HEADER_NAME

#include <fstream>
#include <iostream>

using namespace NAMESPACE_NAME;

int main() {
    std::ifstream input(INPUT_FILE_PATH);
    if (!input) {
        std::cerr << "Failed to open input file " << INPUT_FILE_PATH << std::endl;
        return 1;
    }

    std::cout << "Part 1: " << part1() << std::endl;
    std::cout << "Part 2: " << part2() << std::endl;

    return 0;
}
