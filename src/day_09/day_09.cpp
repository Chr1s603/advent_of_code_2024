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

string
parse_input (ifstream &s)
{
    return { istreambuf_iterator<char>(s), {} };
}

using disk_t = vector<int64_t>;

disk_t
parse_disk (const string &dmap)
{
    disk_t  res;
    int64_t file_id { 0 };
    for (const auto &[i, c] : views::enumerate(dmap))
    {
        const auto is_file_len { i % 2 == 0 };
        res.resize(res.size() + (c - '0'), is_file_len ? file_id++ : -1);
    }
    return res;
}

disk_t
compact_disk (const disk_t &disk)
{
    int64_t current_end { static_cast<int64_t>(disk.size()) };
    auto    dec_until_non_empty_block = [&] () -> int64_t {
        do
            current_end--;
        while (disk[current_end] < 0 && current_end >= 0);
        return disk[current_end];
    };

    disk_t res;
    for (int64_t i = 0; i < disk.size(); i++)
    {
        if (i >= current_end)
            res.emplace_back(-1);
        else if (disk[i] >= 0)
            res.emplace_back(disk[i]);
        else
            res.emplace_back(dec_until_non_empty_block());
    }

    return res;
}

disk_t
compact_disk_no_fragmentation (const disk_t &disk)
{
    std::map<int64_t, int64_t> file_lengths;
    for (const auto ch : disk)
        if (ch >= 0)
            file_lengths[ch]++;

    disk_t res;
    for (int64_t i = static_cast<int64_t>(file_lengths.size()) - 1; i > 0; i--)
    {
    }

    return res;
}

int64_t
calc_checksum (const disk_t &disk)
{
    int64_t checksum { 0 };
    for (const auto &[i, val] : views::enumerate(disk))
        if (val >= 0)
            checksum += val * i;
    return checksum;
}

int64_t
part1 (const string &in)
{
    const string &disk_map { in };
    const auto    disk { parse_disk(disk_map) };
    const auto    compacted_disk { compact_disk(disk) };
    return calc_checksum(compacted_disk);
}

int64_t
part2 (const string &in)
{
    const string &disk_map { in };
    const auto    disk { parse_disk(disk_map) };
    const auto    compacted_disk { compact_disk_no_fragmentation(disk) };
    return calc_checksum(compacted_disk);
}

}
