#include HEADER_NAME

#include <boost/algorithm/string_regex.hpp>
#include <boost/range/adaptors.hpp>
#include <cmath>
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
    map<int64_t, pair<size_t, size_t>> file_lengths; // file ID -> [start, length]
    for (const auto &[i, block] : views::enumerate(disk))
    {
        if (block >= 0)
        {
            if (!file_lengths.contains(block))
                file_lengths[block] = make_pair(i, 1);
            else
                file_lengths[block].second++;
        }
    }

    auto calc_free_blocks = [] (const disk_t &d) {
        vector<pair<size_t, size_t>> free_blocks;
        size_t                       i = 0;
        while (i < d.size())
        {
            if (d[i] == -1)
            {
                size_t start  = i;
                size_t length = 0;
                while (i < d.size() && d[i] == -1)
                {
                    ++i;
                    ++length;
                }
                free_blocks.emplace_back(start, length);
            }
            else
                ++i;
        }
        return free_blocks;
    };

    disk_t result = disk;
    for (const auto &[file_id, start_len] : ranges::reverse_view(file_lengths))
    {
        const size_t file_start = start_len.first;
        const size_t file_len   = start_len.second;

        for (const auto free_blocks = calc_free_blocks(result);
             const auto &[start, length] : free_blocks)
        {
            if (start >= file_start)
                continue;

            if (length >= file_len)
            {
                for (size_t i = 0; i < file_len; ++i)
                    result[start + i] = file_id;

                for (int64_t l = 0; l < file_len; ++l)
                    result[file_start + l] = -1;

                break;
            }
        }
    }

    return result;
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
