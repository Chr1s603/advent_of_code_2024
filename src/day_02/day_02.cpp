#include HEADER_NAME

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <iostream>
#include <sstream>

namespace NAMESPACE_NAME {
using Row    = std::vector<int64_t>;
using Matrix = std::vector<Row>;

Matrix
parse_input (std::ifstream &s)
{
    Matrix      res;
    std::string line;

    while (std::getline(s, line))
    {
        std::istringstream iss(line);
        res.emplace_back(std::istream_iterator<int64_t>(iss),
                         std::istream_iterator<int64_t>());
    }

    return res;
}

Row
get_err_idxs (const Row &row, bool ascending)
{
    Row res;
    if (row.size() < 2)
        return res;

    for (size_t i = 0; i < row.size() - 1; ++i)
    {
        int64_t diff = std::abs(row[i] - row[i + 1]);
        if (diff < 1 || diff > 3 || (ascending && row[i] > row[i + 1])
            || (!ascending && row[i] < row[i + 1]))
        {
            res.push_back(i);
            res.push_back(i + 1);
        }
    }
    return res;
}

int64_t
part1 (const Matrix &in)
{
    return boost::count_if(in, [] (const Row &row) {
        return get_err_idxs(row, true).empty() || get_err_idxs(row, false).empty();
    });
}

int64_t
part2 (const Matrix &in)
{
    return boost::count_if(in, [] (const Row &row) {
        auto try_dampen = [&] (const Row &indices, const bool ascending) {
            return std::ranges::any_of(indices, [&] (const int64_t idx) {
                Row row2 = row;
                row2.erase(row2.begin() + idx);
                return get_err_idxs(row2, ascending).empty();
            });
        };

        const auto idxs_asc  = get_err_idxs(row, true);
        const auto idxs_desc = get_err_idxs(row, false);

        return idxs_asc.empty() || idxs_desc.empty() || try_dampen(idxs_asc, true)
               || try_dampen(idxs_desc, false);
    });
}

}
