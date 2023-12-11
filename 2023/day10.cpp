#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <deque>

typedef unsigned int uint;

std::pair<uint, uint> solve(const std::string &filename)
{
    std::ifstream               file(filename);
    std::string                 line;
    std::pair<uint, uint>       result = {0, 0};
    std::vector<std::string>    plot;
    std::pair<uint, uint>       s_pos;

    while (std::getline(file, line))
        plot.push_back(line);

    for (uint r = 0; r < plot.size(); ++r)
        for (uint c = 0; c < plot[r].size(); ++c)
            if (plot[r][c] == 'S') {
                s_pos = {r, c};
                break;
            }

    std::vector<std::pair<uint, uint>>  seen_points = {s_pos};
    std::deque<std::pair<uint, uint>>   deq         = {s_pos};

    auto is_pipe_not_seen = [&seen_points](const std::pair<uint, uint> &pos)
    {
        return (std::find(seen_points.begin(), seen_points.end(), pos) == seen_points.end()) ? true : false;
    };

    auto is_ch_in_symbols = [](const std::string &symbols, const char &ch)
    {
        return (symbols.find(ch) != std::string::npos) ? true : false;
    };

    do {
        uint r  = deq.at(0).first;
        uint c  = deq.at(0).second;

        deq.pop_front();

        /* go up */
        if (r > 0)
            if (is_ch_in_symbols("S|JL", plot[r][c]))
                if (is_ch_in_symbols("|7F", plot[r - 1][c]))
                    if (is_pipe_not_seen({r - 1, c})) {
                        seen_points.push_back({r - 1, c});
                        deq.push_back({r - 1, c});
                    }

        /* go down */
        if (r < (plot.size() - 1))
            if (is_ch_in_symbols("S|7F", plot[r][c]))
                if (is_ch_in_symbols("|JL", plot[r + 1][c]))
                    if (is_pipe_not_seen({r + 1, c})) {
                        seen_points.push_back({r + 1, c});
                        deq.push_back({r + 1, c});
                    }

        /* go left */
        if (c > 0)
            if (is_ch_in_symbols("S-J7", plot[r][c]))
                if (is_ch_in_symbols("-LF", plot[r][c - 1]))
                    if (is_pipe_not_seen({r, c - 1})) {
                        seen_points.push_back({r, c - 1});
                        deq.push_back({r, c - 1});
                    }

        /* go right */
        if (c < (plot[0].size() - 1))
            if (is_ch_in_symbols("S-LF", plot[r][c]))
                if (is_ch_in_symbols("-J7", plot[r][c + 1]))
                    if (is_pipe_not_seen({r, c + 1})) {
                        seen_points.push_back({r, c + 1});
                        deq.push_back({r, c + 1});
                    }
    } while (deq.size());

    result.first = seen_points.size() / 2;

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