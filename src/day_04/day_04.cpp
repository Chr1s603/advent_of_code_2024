#include HEADER_NAME

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/algorithm/string/find_iterator.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm/count_if.hpp>
#include <boost/range/combine.hpp>
#include <iostream>

using namespace std;
using namespace boost;
using namespace boost::numeric::ublas;

namespace NAMESPACE_NAME {
std::string
parse_input (std::ifstream &s)
{
    return { istreambuf_iterator<char>(s), {} };
}

int64_t
count_xmas_occurences_and_backwards (const std::string &str)
{
    std::vector<boost::iterator_range<std::string::const_iterator>> results_fw,
        results_bw;
    find_all(results_fw, str, "XMAS");
    find_all(results_bw, str, "SAMX");
    return static_cast<int64_t>(results_fw.size() + results_bw.size());
}

int64_t
part1 (const std::string &in)
{
    std::vector<string> lines;
    split(lines, in, is_any_of("\n"));
    const auto rows = static_cast<int64_t>(lines.size());
    const auto cols = static_cast<int64_t>(lines[0].size());

    int64_t fw_diag_occurences = 0;
    for (int64_t d = 0; d < rows + cols - 1; ++d)
    {
        std::string diagonal;
        for (int64_t i = 0; i < rows; ++i)
            if (const int64_t j = d - i; j >= 0 && j < cols)
                diagonal += lines[i][j];
        if (!diagonal.empty())
            fw_diag_occurences += count_xmas_occurences_and_backwards(diagonal);

        diagonal.clear();
        for (int64_t i = 0; i < rows; ++i)
            if (const int64_t j = i + d - (cols - 1); j >= 0 && j < cols)
                diagonal += lines[i][j];
        if (!diagonal.empty())
            fw_diag_occurences += count_xmas_occurences_and_backwards(diagonal);
    }

    int64_t vertical_occurences   = 0;
    int64_t horizontal_occurences = 0;
    for (int64_t col = 0; col < rows; ++col)
    {
        std::string vertical;
        for (auto &line : lines)
        {
            vertical.push_back(line[col]);
            horizontal_occurences
                += col == 0 ? count_xmas_occurences_and_backwards(line) : 0;
        }
        vertical_occurences += count_xmas_occurences_and_backwards(vertical);
    }

    return horizontal_occurences + vertical_occurences + fw_diag_occurences;
}

int64_t
part2 (const std::string &in)
{
    std::vector<string> lines;
    split(lines, in, is_any_of("\n"));
    const auto rows { static_cast<int64_t>(lines.size()) };
    const auto cols { static_cast<int64_t>(lines[0].size()) };

    // Iterate over all 3x3 blocks.
    int64_t xmas_count { 0 };
    for (int64_t row = 0; row < rows - 2; ++row)
        for (int64_t col = 0; col < cols - 2; ++col)
            if (lines[row + 1][col + 1] == 'A')
                xmas_count
                    += 2 == //
                       (lines[row][col] == 'M' && lines[row + 2][col + 2] == 'S')
                           + (lines[row][col + 2] == 'M' && lines[row + 2][col] == 'S')
                           + (lines[row + 2][col] == 'M' && lines[row][col + 2] == 'S')
                           + (lines[row + 2][col + 2] == 'M' && lines[row][col] == 'S');

    return xmas_count;
}

}
