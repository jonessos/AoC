#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>

typedef uint8_t         u8;
typedef unsigned int    uint;

uint solve_1(const std::string &filename)
{
    std::ifstream   file(filename);
    std::string     line;
    uint            result = 0;

    while (std::getline(file, line)) {
        std::istringstream          ss_line(line);
        std::vector<std::string>    winning_nums;
        std::vector<std::string>    hand_nums;
        std::string                 num;

        std::getline(ss_line, line, ':');
        std::getline(ss_line, line, '|');

        std::istringstream          win(line);

        while (win >> num)
            winning_nums.push_back(num);

        std::getline(ss_line, line, '|');

        std::istringstream          hand(line);

        while (hand >> num)
            hand_nums.push_back(num);

        uint        point = 0;
        for (const auto &winning_num : winning_nums) {
            auto it = std::find(hand_nums.begin(), hand_nums.end(), winning_num);

            if (it != hand_nums.end())
                point = (!point) ? 1 : point * 2;
        }

        result += point;
    }
    return result;
}

uint solve_2(const std::string &filename)
{
    std::ifstream       file(filename);
    std::string         line;
    uint                result      = 0;
    uint                line_count  = 0;
    std::vector<uint>   scratchcards;
    const size_t        MAX_CARDS   = 200;  /* >= Number of lines of input */

    scratchcards.resize(MAX_CARDS, 0);

    while (std::getline(file, line)) {
        std::istringstream          ss_line(line);
        std::vector<std::string>    winning_nums;
        std::vector<std::string>    hand_nums;
        std::string                 num;

        std::getline(ss_line, line, ':');
        std::getline(ss_line, line, '|');

        std::istringstream          win(line);

        while (win >> num)
            winning_nums.push_back(num);

        std::getline(ss_line, line, '|');

        std::istringstream          hand(line);

        while (hand >> num)
            hand_nums.push_back(num);

        scratchcards[line_count] += 1;

        uint                        matches = 0;

        for (const auto &winning_num : winning_nums) {
            auto it = std::find(hand_nums.begin(), hand_nums.end(), winning_num);

            if (it != hand_nums.end())
                matches++;
        }

        for (uint i = line_count + 1; (i <= line_count + matches) && (i < MAX_CARDS); i++)
            scratchcards[i] += scratchcards[line_count] * 1;

        line_count++;
    }

    for (int i = 0; i < line_count; i++)
        result += scratchcards[i];

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