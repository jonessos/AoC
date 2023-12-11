#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>

typedef unsigned long int u64;

auto is_all_empty_space = [](const std::string &line)
{
    return std::all_of(line.begin(), line.end(), [](char c) { return c == '.'; });
};

u64 solve(const std::string &filename, const uint &expansion_width)
{
    std::ifstream               file(filename);
    std::string                 line;
    u64                         result      = 0;
    std::vector<std::string>    map;
    std::vector<u64>            empty_rows;
    std::vector<u64>            empty_columns;
    u64                         line_num    = 0;

    while (std::getline(file, line)) {
        map.push_back(line);

        if (is_all_empty_space(line))
            empty_rows.push_back(line_num);

        ++line_num;
    }

    for (u64 column = 0; column < map[0].size(); ++column) {
        std::string column_chars;

        for (u64 row = 0; row < map.size(); ++row)
            column_chars += map[row][column];

        if (is_all_empty_space(column_chars))
            empty_columns.push_back(column);
    }

    std::vector<std::pair<u64, u64>> galaxy_coordinates;

    for (u64 row = 0; row < map.size(); ++row)
        for (u64 column = 0; column < map[row].size(); ++column)
            if (map[row][column] == '#')
                galaxy_coordinates.push_back({row, column});

    auto count_elements_less_than = [](const std::vector<u64> &expansions, const u64 &target)
    {
        /* use lower_bound to find the first element not less than target */
        auto it = std::lower_bound(expansions.begin(), expansions.end(), target);

        /* calculate the number of elements less than target */
        return std::distance(expansions.begin(), it);
    };

    for (auto &[x, y]: galaxy_coordinates) {
        x += count_elements_less_than(empty_rows, x) * expansion_width;
        y += count_elements_less_than(empty_columns, y) * expansion_width;
    }

    for (u64 i = 0; i < galaxy_coordinates.size() - 1; ++i)
        for (u64 j = i + 1; j < galaxy_coordinates.size(); ++j) {
            result += std::abs((long long)galaxy_coordinates[i].first - (long long)galaxy_coordinates[j].first);
            result += std::abs((long long)galaxy_coordinates[i].second - (long long)galaxy_coordinates[j].second);
        }

    return result;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <Puzzle Input File>" << std::endl;
        return 1;
    }

    std::cout << solve(std::string(argv[1]), 1) << std::endl << solve(std::string(argv[1]), 1000000 - 1) << std::endl;

    return 0;
}