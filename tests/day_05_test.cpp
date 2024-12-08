#include HEADER_NAME

#include <gtest/gtest.h>

TEST(Day04, Part1)
{
    std::ifstream input_file(INPUT_FILE_PATH);
    if (!input_file)
        std::cerr << "Failed to open input file " << INPUT_FILE_PATH << std::endl;

    const auto input = NAMESPACE_NAME::parse_input(input_file);
    EXPECT_EQ(NAMESPACE_NAME::part1(input), 5374);
}

TEST(Day04, Part2)
{
    std::ifstream input_file(INPUT_FILE_PATH);
    if (!input_file)
        std::cerr << "Failed to open input file " << INPUT_FILE_PATH << std::endl;

    const auto input = NAMESPACE_NAME::parse_input(input_file);
    EXPECT_EQ(NAMESPACE_NAME::part2(input), 4260);
}
