#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>

typedef unsigned int uint;

auto is_all_zeroes = [](const std::vector<int> &diffs)
{
    return std::all_of(diffs.begin(), diffs.end(), [](int v) { return v == 0; });
};

int future_value_of(const std::vector<int> &values)
{
    std::vector<int> diffs;

    for (uint i = 0; i < values.size() - 1; i++)
        diffs.push_back(values[i + 1] - values[i]);

    if (is_all_zeroes(diffs))
        return values[values.size() - 1];
    else
        return values[values.size() - 1] + future_value_of(diffs);
}

int past_value_of(const std::vector<int> &values)
{
    std::vector<int> diffs;

    for (uint i = 0; i < values.size() - 1; i++)
        diffs.push_back(values[i + 1] - values[i]);

    if (is_all_zeroes(diffs))
        return values[0];
    else
        return values[0] - past_value_of(diffs);
}

std::pair<uint, uint> solve(const std::string &filename)
{
    std::ifstream           file(filename);
    std::string             line;
    std::pair<uint, uint>   result  = {0, 0};

    while (std::getline(file, line)) {
        std::vector<int>    values;
        std::istringstream  sstr(line);
        int                 num;

        while (sstr >> num)
            values.push_back(num);

        result.first    += future_value_of(values);
        result.second   += past_value_of(values);
    }

    return result;
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <Puzzle Input File>" << std::endl;
        return 1;
    }

    auto result = solve(std::string(argv[1]));

    std::cout << result.first << std::endl << result.second << std::endl;

    return 0;
}