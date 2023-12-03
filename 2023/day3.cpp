#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

typedef uint8_t         u8;
typedef unsigned int    uint;

uint solve_1(const std::string &filename)
{
    std::vector<std::string>    schematic;
    std::ifstream               file(filename);
    std::string                 line;
    uint                        result = 0;

    while (std::getline(file, line))
        schematic.emplace_back(line.begin(), line.end());

    std::vector<std::string> part_numbers;

    auto extract_surrounders = [schematic](const int &line, const int &start,
        const int &size) -> std::string
    {
        std::string surrounders;

        for (int row = line; row < line + 3; row++) {
            if (row == -1)
                continue;
            if (row == schematic.size())
                break;

            for (int column = start; column < size; column++) {
                if (column == -1)
                    continue;

                surrounders += schematic[row][column];
            }
        }

        return surrounders;
    };

    auto is_part_number = [](const std::string &surrounders) -> bool
    {
        for (const auto &c: surrounders)
            if (c == '.')
                continue;
            else if ((c > 0x20) && (c < 0x30))
                return true;
            else if ((c > 0x39) && (c < 0x41))
                return true;
            else if ((c > 0x5A) && (c < 0x61))
                return true;
            else if ((c > 0x7A) && (c < 0x7F))
                return true;
    };

    for (int row = 0; row < schematic.size(); row++) {
        std::string number;

        for (int column = 0; column < schematic[row].size(); column++) {
            if (isdigit(schematic[row][column])) {
                number += schematic[row][column];
            } else {
                if (number.size()) {
                    if (is_part_number(extract_surrounders(row - 1,
                        column - number.size() - 1, column + 1)))
                        part_numbers.push_back(number);

                    number.clear();
                }
            }
        }

        if (number.size())
            if (is_part_number(extract_surrounders(row - 1,
                schematic[row].size() - number.size() - 1,
                schematic[row].size() + 1 + 1)))
                part_numbers.push_back(number);
    }

    for (const auto &num: part_numbers)
        result += std::atoi(num.c_str());

    return result;
}

uint solve_2(const std::string &filename)
{
    std::unordered_multimap<std::string, std::string>   gear_parts;
    std::vector<std::string>                            schematic;
    std::ifstream                                       file(filename);
    std::string                                         line;
    uint                                                result = 0;

    while (std::getline(file, line))
        schematic.emplace_back(line.begin(), line.end());

    auto update_gear_parts = [schematic, &gear_parts](const int &line,
        const int &start, const int &size, const std::string &number) -> bool
    {
        for (int row = line; row < line + 3; row++) {
            if (row == -1)
                continue;
            if (row == schematic.size())
                break;

            for (int column = start; column < size; column++) {
                if (column == -1)
                    continue;

                if (schematic[row][column] == '*') {
                    std::string gear_pos;

                    gear_pos = std::to_string(row) + ":" + std::to_string(column);
                    gear_parts.insert({gear_pos, number});
                }
            }
        }
    };

    for (int row = 0; row < schematic.size(); row++) {
        std::string number;

        for (int column = 0; column < schematic[row].size(); column++) {
            if (isdigit(schematic[row][column])) {
                number += schematic[row][column];
            } else {
                if (number.size()) {
                    update_gear_parts(row - 1, column - number.size() - 1,
                        column + 1, number);
                    number.clear();
                }
            }
        }

        if (number.size())
            update_gear_parts(row - 1, schematic[row].size() - number.size() - 1,
                schematic[row].size() + 1 + 1, number);
    }

    std::string prev_key;
    std::string prev_value;
    int         idx = 0;

    for (const auto &[key, value]: gear_parts) {
        if (!idx) {
            prev_key    = key;
            prev_value  = value;
            idx++;
            continue;
        }

        if ((prev_key == key) && (prev_value != value)) {
            result += (std::atoi(prev_value.c_str()) * std::atoi(value.c_str()));
        }

        prev_key    = key;
        prev_value  = value;
    }

    return result;
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