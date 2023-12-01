#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>

typedef uint8_t         u8;
typedef unsigned int    uint;

uint solve_1(const std::string &filename)
{
    u8      first   = -1;
    u8      last    = -1;
    uint    result  = 0;

    std::ifstream   file(filename);
    std::string     line;

    while (std::getline(file, line)) {
        for (auto iter = line.begin(); iter != line.end(); iter++)
            if (isdigit(*iter)) {
                first = *iter - 0x30;
                break;
            }

        for (auto iter = line.end(); iter >= line.begin(); iter--)
            if (isdigit(*iter)) {
                last = *iter - 0x30;
                break;
            }

        result += (first * 10) + last;
    }

    return result;
}

uint solve_2(const std::string &filename)
{
    const std::vector<std::pair<u8, std::string>> LUT = {
        {0, "zero"},
        {1, "one"},
        {2, "two"},
        {3, "three"},
        {4, "four"},
        {5, "five"},
        {6, "six"},
        {7, "seven"},
        {8, "eight"},
        {9, "nine"},
    };

    uint            result  = 0;
    std::ifstream   file(filename);
    std::string     line;

    while (std::getline(file, line)) {
        std::pair<size_t, u8>  first   = {-1, 0};
        std::pair<size_t, u8>  last    = {-1, 0};

        for (const auto &[num, word]: LUT) {
            size_t found = std::min(line.find(std::to_string(num)), line.find(word));

            if (found != std::string::npos)
                if ((first.first == -1) || (first.first > found))
                    first = {found, num};
        }

        for (const auto &[num, word]: LUT) {
            size_t num_pos  = line.rfind(std::to_string(num));
            size_t word_pos = line.rfind(word);
            size_t found;

            if ((num_pos == std::string::npos) || (word_pos == std::string::npos))
                found = std::min(line.rfind(std::to_string(num)), line.rfind(word));
            else
                found = std::max(line.rfind(std::to_string(num)), line.rfind(word));

            if (found != std::string::npos)
                if ((last.first == -1) || (last.first < found))
                    last = {found, num};
        }

        result += (first.second * 10) + last.second;
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