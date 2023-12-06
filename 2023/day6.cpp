#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

typedef unsigned long int u64;

std::pair<u64, u64> solve(const std::string &filename)
{
    std::ifstream       file(filename);
    std::string         line;
    std::pair<u64, u64> result  = {0, 0};
    std::vector<u64>    times, distances, possibilities;
    u64                 number;

    std::getline(file, line);
    std::istringstream  line_1(line);
    std::getline(line_1, line, ':');

    while (line_1 >> number)
        times.push_back(number);

    std::getline(file, line);
    std::istringstream  line_2(line);
    std::getline(line_2, line, ':');

    while (line_2 >> number)
        distances.push_back(number);

    possibilities.resize(times.size(), 0);

    for (u64 race = 0; race < times.size(); ++race)
        for (u64 speed = 1; speed < times[race]; ++speed)
            if ((speed * (times[race] - speed)) > distances[race])
                ++possibilities[race];
            else if (possibilities[race])
                break;

    for (const auto &p: possibilities)
        if (!p) {
            continue;
        } else {
            if (!result.first)
                result.first = p;
            else
                result.first *= p;
        }

    std::string         time_str, distance_str;
    u64                 time, distance;

    for (const auto &t: times)
        time_str        += std::to_string(t);

    for (const auto &d: distances)
        distance_str    += std::to_string(d);

    time        = std::strtoull(time_str.c_str(), nullptr, 10);
    distance    = std::strtoull(distance_str.c_str(), nullptr, 10);

    for (u64 speed = 1; speed < time; speed++)
        if ((speed * (time - speed)) > distance)
            ++(result.second);
        else if (result.second)
            break;

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