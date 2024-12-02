#include HEADER_NAME

#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/count.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <boost/range/combine.hpp>
#include <boost/tuple/tuple.hpp>

namespace
NAMESPACE_NAME {
    std::vector<std::vector<int64_t> > parse_input(std::ifstream &s) {
        std::vector<std::vector<int64_t> > res;
        std::string line;

        while (std::getline(s, line)) {
            std::istringstream iss(line);
            int64_t value;
            std::vector<int64_t> row;
            while (iss >> value)
                row.push_back(value);

            res.push_back(std::move(row));
        }

        return res;
    }

    std::vector<int64_t> get_err_idxs(const std::vector<int64_t> &row, const bool ascending) {
        std::vector<int64_t> res;
        if (row.size() >= 2) {
            for (int64_t i = 0; i < row.size() - 1; ++i) {
                const int64_t diff = std::abs(row[i] - row[i + 1]);
                const bool to_far_apart = diff < 1 || diff > 3;
                const bool should_be_ascending_but_is_not = ascending && row[i] > row[i + 1];
                const bool should_be_descending_but_is_not = !ascending && row[i] < row[i + 1];
                if (to_far_apart ||
                    should_be_ascending_but_is_not ||
                    should_be_descending_but_is_not) {
                    res.push_back(i);
                    res.push_back(i + 1);
                }
            }
        }
        return std::move(res);
    }

    int64_t
    part1(const std::vector<std::vector<int64_t> > &in) {
        int64_t safe_count{0};
        for (const auto &row: in)
            if (get_err_idxs(row, true).empty() || get_err_idxs(row, false).empty()) safe_count++;
        return safe_count;
    }

    int64_t part2(const std::vector<std::vector<int64_t> > &in) {
        int64_t safe_count{0};
        for (const auto &row: in) {
            std::cout << "- ROW ----------------" << std::endl;
            const auto idxs_asc = get_err_idxs(row, true);
            const auto idxs_desc = get_err_idxs(row, false);
            if (idxs_asc.empty() || idxs_desc.empty()) {
                safe_count++;
                std::cout << "VALID" << std::endl;
            } else {
                bool dampen_helped{false};
                for (const auto &idx: idxs_asc) {
                    std::vector<int64_t> row2 = row;
                    row2.erase(row2.begin() + idx);

                    if (get_err_idxs(row2, true).empty()) {
                        std::cout << "VALID" << std::endl;
                        dampen_helped = true;
                        break;
                    }
                }
                for (const auto &idx: idxs_desc) {
                    std::vector<int64_t> row2 = row;
                    row2.erase(row2.begin() + idx);

                    if (get_err_idxs(row2, false).empty()) {
                        std::cout << "VALID" << std::endl;
                        dampen_helped = true;
                        break;
                    }
                }
                if (dampen_helped) safe_count++;
            }
        }
        return safe_count;
    }
}
