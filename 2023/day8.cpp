#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

typedef unsigned long int u64;

using Map_T = std::map<std::string, std::pair<std::string, std::string>>;

std::pair<Map_T, std::string> parse_map(std::ifstream &file)
{
    std::string         line;
    Map_T               desert_map;
    std::string         instructions;
    std::string         node, left, right;

    std::getline(file, instructions);
    std::getline(file, line);

    while (std::getline(file, line)) {
        line.erase(std::remove_if(line.begin(), line.end(), [](char c) { return c == '='; }));
        line.erase(std::remove_if(line.begin(), line.end(), [](char c) { return c == '('; }));
        line.erase(std::remove_if(line.begin(), line.end(), [](char c) { return c == ','; }));
        line.erase(std::remove_if(line.begin(), line.end(), [](char c) { return c == ')'; }));

        std::istringstream  sstr(line);

        sstr >> node >> left >> right;

        desert_map[node] = {left, right};
    }

    return {desert_map, instructions};
}

u64 gcd(u64 a, u64 b)
{
    if (b == 0)
        return a;

    return gcd(b, a % b);
}

u64 lcm(const std::vector<u64> &numbers)
{
    u64 result  = numbers[0];

    for (size_t i = 1; i < numbers.size(); ++i)
        result = (result * numbers[i]) / gcd(result, numbers[i]);

    return result;
}

u64 solve_1(const std::string &filename)
{
    std::ifstream   file(filename);
    auto            input           = parse_map(file);
    Map_T           desert_map      = input.first;
    std::string     instructions    = input.second;

    const std::string   path_end        = desert_map.rbegin()->first;
    std::string         current_node    = desert_map.begin()->first;
    u64                 idx             = 0;
    u64                 steps           = 0;

    do {
        if (idx == instructions.size())
            idx = 0;

        if (instructions[idx] == 'L')
            current_node = desert_map[current_node].first;
        else
            current_node = desert_map[current_node].second;
    } while ((++idx) && (++steps) && (current_node != path_end));

    return steps;
}

u64 solve_2(const std::string &filename)
{
    std::ifstream   file(filename);
    auto            input           = parse_map(file);
    Map_T           desert_map      = input.first;
    std::string     instructions    = input.second;

    std::vector<std::pair<std::string, uint>>   min_steps_to_z_for_node;
    u64                                         idx     = 0;
    u64                                         steps   = 0;

    for (const auto &[node, paths]: desert_map)
        if (node[2] == 'A')
            min_steps_to_z_for_node.push_back({node, 0});

    do {
        if (idx == instructions.size())
            idx = 0;

        for (auto &[node, min_steps_to_z]: min_steps_to_z_for_node) {
            if ((node[2] == 'Z') && (!min_steps_to_z))
                min_steps_to_z = steps;

            if (instructions[idx] == 'L')
                node = desert_map[node].first;
            else
                node = desert_map[node].second;
        }

        int count = 0;

        for (auto &[node, min_steps_to_z]: min_steps_to_z_for_node)
            if (min_steps_to_z)
                count++;

        if (count == min_steps_to_z_for_node.size())
            break;
    } while (++idx && ++steps);

    std::vector<u64> numbers;

    for (auto &[node, min_steps_to_z]: min_steps_to_z_for_node)
        numbers.push_back(min_steps_to_z);

    return lcm(numbers);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <Puzzle Input File>" << std::endl;
        return 1;
    }

    std::cout << solve_1(std::string(argv[1])) << std::endl;
    std::cout << solve_2(std::string(argv[1])) << std::endl;

    return 0;
}