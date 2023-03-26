#include <iostream>
#include <chat_tools/v1/utils/split.hpp>

#include <cmath>

int main() {
    std::cout << "test split" << std::endl;

    auto var_1 = chat_tools::v1::utils::split("test1 test2\ttest3\ntest4!!!test5   test6;test7:test8");
    auto var_2 = chat_tools::v1::utils::split("test1 test2\ttest3\ntest4!!!test5   test6;test7:test8","\n\t\r ");

    std::cout << "var_1:" << std::endl << std::endl;
    for (auto &item : var_1) {
        std::cout << item << std::endl;
    }

    std::cout << std::endl;

    std::cout << "var_2:" << std::endl << std::endl;
    for (auto &item : var_2) {
        std::cout << item << std::endl;
    }

    std::cout << std::endl;

    std::vector<std::u32string> repeateds = {
        U"pampampampampampampam",
        U"pampampampampampam",
        U"pampampampampam",
        U"pampampampampamp",
        U"pampampampampampa",
        U"pampamp",
        U"pampam",
        U"pam",
        U"pppp",
        U"pp",
        U"p",
        U"",
    };

    for (size_t i = 0; i < repeateds.size(); ++i) {
        auto res = chat_tools::v1::utils::find_repeated_substring(repeateds[i]);
        std::cout << utf8::utf32to8(repeateds[i]) << " res: " << utf8::utf32to8(res.first) << " " << res.second << std::endl;
    }

    return 0;
}
