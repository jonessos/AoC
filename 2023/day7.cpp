#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <unordered_map>
#include <cmath>

typedef uint8_t     u8;
typedef uint64_t    u64;

enum class CardType
{
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OAK,
    FULL_HOUSE,
    FOUR_OAK,
    FIVE_OAK,
    DEFAULT
};

CardType get_card_type(const std::string &hand_str, bool jokers_allowed = false)
{
    std::set<char> unique_chars(hand_str.begin(), hand_str.end());

    if (jokers_allowed)
        unique_chars.erase('J');

    switch (unique_chars.size()) {
        case 1:
            return CardType::FIVE_OAK;
        case 2: {
            std::string                     wildcards           = hand_str;
            char                            first_char          = wildcards[0];
            char                            most_occuring_char  = 0;
            std::pair<u8, u8>               count               = {0, 0};
            std::unordered_map<char, u8>    char_map;

            if (jokers_allowed) {
                u8 max_char_count = 0;

                /* store the first character that is not 'J' */
                for (const char &c: wildcards)
                    if (c != 'J') {
                        first_char = c;
                        break;
                    }

                /* count occurrences of each character */
                for (const char &c: wildcards)
                    char_map[c]++;

                /* find the maximum occurrence count */
                for (const auto &[c, c_count] : char_map)
                    max_char_count = std::max(max_char_count, c_count);

                /* find the character with maximum occurrence */
                for (const auto &[c, c_count] : char_map)
                    if ((c_count == max_char_count) && (c != 'J')) {
                        most_occuring_char = c;
                        break;
                    }

                /* replace jokers for processing */
                for (auto iter = wildcards.begin(); iter < wildcards.end(); ++iter)
                    if (*iter == 'J')
                        *iter = most_occuring_char;
            }

            for (u8 i = 0; i < wildcards.size(); ++i)
                if (wildcards[i] == first_char)
                    ++(count.first);
                else
                    ++(count.second);

            if ((u8) std::abs(count.first - count.second) == 3)
                return CardType::FOUR_OAK;
            else
                return CardType::FULL_HOUSE;
        }
        case 3: {
            std::string                     wildcards           = hand_str;
            char                            most_occuring_char  = 0;
            std::unordered_map<char, u8>    char_map;
            u8                              num_of_jokers;

            num_of_jokers = jokers_allowed ? std::count(hand_str.begin(), hand_str.end(), 'J') : 0;

            if (jokers_allowed) {
                u8 max_char_count = 0;

                /* count occurrences of each character */
                for (const char &c: wildcards)
                    char_map[c]++;

                /* find the maximum occurrence count */
                for (const auto &[c, c_count] : char_map)
                    max_char_count = std::max(max_char_count, c_count);

                /* find the character with maximum occurrence */
                for (const auto &[c, c_count] : char_map)
                    if ((c_count == max_char_count) && (c != 'J')) {
                        most_occuring_char = c;
                        break;
                    }

                if (num_of_jokers == 2)
                    most_occuring_char = wildcards[0];   /* corner case (234JJ for example) */

                /* replace jokers for processing */
                for (auto iter = wildcards.begin(); iter < wildcards.end(); ++iter)
                    if (*iter == 'J')
                        *iter = most_occuring_char;

                char_map.clear();
            }

            for (u8 i = 0; i < wildcards.size(); ++i)
                char_map[wildcards[i]] += 1;

            for (const auto &[key, value]: char_map)
                if (value == 3)
                    return CardType::THREE_OAK;

            return CardType::TWO_PAIR;
        }
        case 4:
            return CardType::ONE_PAIR;
        case 5:
            return CardType::HIGH_CARD;
    }
}

std::pair<u64, u64> solve(const std::string &filename)
{
    using Input_T = std::tuple<std::string, u64, CardType, uint>;

    std::ifstream           file(filename);
    std::string             line;
    std::pair<u64, u64>     result  = {0, 0};
    std::vector<Input_T>    tuples;
    std::string             hand, bid;
    u64                     current_rank;

    while (std::getline(file, line)) {
        std::istringstream  sstr(line);

        sstr >> hand >> bid;

        tuples.push_back({hand, std::strtoull(bid.c_str(), nullptr, 10),
                CardType::DEFAULT, 0});
    }

    auto sort_by_type = [&tuples](void)
    {
        std::sort(tuples.begin(), tuples.end(),
            [](const Input_T &a, const Input_T &b)
            {
                return std::get<2>(a) < std::get<2>(b);
            }
        );
    };

    auto sort_by_hand_for_each_type = [&tuples](bool jokers_allowed = false)
    {
        for (int i = static_cast<int>(CardType::HIGH_CARD); i < static_cast<int>(CardType::DEFAULT); ++i) {
            auto type_check = [i](const Input_T &t)
            {
                return std::get<2>(t) == (CardType)i;
            };

            auto first          = std::find_if(tuples.begin(), tuples.end(), type_check);
            auto last           = first;
            bool type_exists    = std::any_of(tuples.begin(), tuples.end(), type_check);

            if (!type_exists)
                continue;

            do {
                if (!type_check(*last) || !type_check(*(last + 1)))
                    break;
            } while (++last != tuples.end());

            std::sort(first, last + 1,
                [jokers_allowed](const Input_T &a, const Input_T &b)
                {
                    const std::string ORDER_OF_CARDS = jokers_allowed ? "AKQT98765432J" : "AKQJT98765432";

                    for (u8 i = 0; i < std::get<0>(a).size(); ++i)
                        if (ORDER_OF_CARDS.find(std::get<0>(a)[i]) < ORDER_OF_CARDS.find(std::get<0>(b)[i]))
                            return false;
                        else if (ORDER_OF_CARDS.find(std::get<0>(a)[i]) > ORDER_OF_CARDS.find(std::get<0>(b)[i]))
                            return true;
                }
            );
        }
    };

    /***** Part 1 *****/
    for (auto &[hand, bid, type, rank]: tuples)
        type = get_card_type(hand);

    sort_by_type();
    sort_by_hand_for_each_type();

    current_rank = 0;

    for (const auto &[hand, bid, type, rank]: tuples)
        result.first += ++current_rank * bid;
    /***** End of Part 1 *****/

    /***** Part 2 *****/
    for (auto &[hand, bid, type, rank]: tuples)
        type = get_card_type(hand, true);

    sort_by_type();
    sort_by_hand_for_each_type(true);

    current_rank = 0;

    for (const auto &[hand, bid, type, rank]: tuples)
        result.second += ++current_rank * bid;
    /***** End of Part 2 *****/

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