#include <iostream>
#include <chat_tools/v1/utils/utf8caseconv.hpp>

int main() {
    std::cout << "test utf8caseconv" << std::endl;

    auto upr = chat_tools::v1::utils::utf8_to_upr_utf8("TesT");
    auto lwr = chat_tools::v1::utils::utf8_to_lwr_utf8("TesT");

    std::cout << "upr: " << upr << std::endl;
    std::cout << "lwr: " << lwr << std::endl;

    return 0;
}
