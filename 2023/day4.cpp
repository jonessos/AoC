#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <cmath>

typedef unsigned int uint;

std::pair<uint, uint> solve(const std::string &filename)
{
    std::ifstream           file(filename);
    std::string             line;
    std::pair<uint, uint>   result      = {0, 0};
    uint                    line_num    = 0;
    std::vector<uint>       scratchcards;

    scratchcards.resize(
        std::count(
            std::istreambuf_iterator<char>(file),
            std::istreambuf_iterator<char>(), '\n') + 1,
        1);

    file.seekg(0, std::ios::beg);

    while (std::getline(file, line)) {
        std::istringstream  sstr(line);
        std::set<uint>      win_nums, our_nums, com_nums;
        uint                num;

        std::getline(sstr, line, ':');
        std::getline(sstr, line, '|');

        std::istringstream  win(line);

        std::getline(sstr, line, '|');

        std::istringstream  our(line);

        while (win >> num)
            win_nums.insert(num);

        while (our >> num)
            our_nums.insert(num);

        std::set_intersection(
            win_nums.begin(), win_nums.end(), our_nums.begin(), our_nums.end(),
            std::inserter(com_nums, com_nums.begin()));

        result.first += (uint) std::pow(2, com_nums.size() - 1);

        for (uint i = line_num + 1; (i <= line_num + com_nums.size()) && (i < scratchcards.size()); i++)
            scratchcards[i] += scratchcards[line_num] * 1;

        line_num++;
    }

    for (const auto &count: scratchcards)
        result.second += count;

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