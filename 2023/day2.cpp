#include <iostream>
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
        std::stringstream   ss_line(line);
        std::string         id;
        std::string         hand;
        bool                impossible = false;

        std::getline(ss_line, id, ':');
        id.erase(id.begin(), id.begin() + 5);

        while (std::getline(ss_line, hand, ';')) {
            std::istringstream ss_hand(hand);

            while (!ss_hand.eof()) {
                uint        number;
                std::string colour;

                ss_hand >> number >> colour;

                if (ss_hand.fail())
                    break;

                if ((colour.find("red") != std::string::npos) && number > 12) {
                    impossible = true;
                    break;
                } else if ((colour.find("green") != std::string::npos) && number > 13) {
                    impossible = true;
                    break;
                } else if ((colour.find("blue") != std::string::npos) && number > 14) {
                    impossible = true;
                    break;
                }
            }
        }

        if (!impossible)
            result += std::atoi(id.c_str());
    }

    return result;
}

uint solve_2(const std::string &filename)
{
    std::ifstream   file(filename);
    std::string     line;
    uint            result = 0;

    while (std::getline(file, line)) {
        std::stringstream   ss_line(line);
        std::string         id;
        std::string         hand;

        std::vector<std::pair<std::string, uint>> minimums = {
            {"red", 1},
            {"green", 1},
            {"blue", 1}
        };

        std::getline(ss_line, id, ':');
        id.erase(id.begin(), id.begin() + 5);

        while (std::getline(ss_line, hand, ';')) {
            std::istringstream ss_hand(hand);

            while (!ss_hand.eof()) {
                uint        number;
                std::string colour;

                ss_hand >> number >> colour;

                if (ss_hand.fail())
                    break;

                if (colour.find("red") != std::string::npos) {
                    if (number > minimums[0].second)
                        minimums[0].second = number;
                } else if (colour.find("green") != std::string::npos) {
                    if (number > minimums[1].second)
                        minimums[1].second = number;
                } else if (colour.find("blue") != std::string::npos) {
                    if (number > minimums[2].second)
                        minimums[2].second = number;
                }
            }
        }

        result += minimums[0].second * minimums[1].second * minimums[2].second;
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