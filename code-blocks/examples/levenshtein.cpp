#include <iostream>
#include <chat_tools/v1/utils/levenshtein.hpp>

int main() {
    std::cout << "test levenshtein distance" << std::endl;

    std::cout
        << "test/tast: "
        << chat_tools::v1::utils::calc_levenshtein_distance(u8"test",u8"tast")
        << std::endl;

    std::cout
        << "test/test: "
        << chat_tools::v1::utils::calc_levenshtein_distance(u8"test",u8"test")
        << std::endl;

    std::cout
        << "tes/test: "
        << chat_tools::v1::utils::calc_levenshtein_distance(u8"tes",u8"test")
        << std::endl;

    std::cout
        << "tes/est: "
        << chat_tools::v1::utils::calc_levenshtein_distance(u8"tes",u8"est")
        << std::endl;

    std::cout
        << "te/test: "
        << chat_tools::v1::utils::calc_levenshtein_distance(u8"te",u8"test")
        << std::endl;

    return 0;
}
