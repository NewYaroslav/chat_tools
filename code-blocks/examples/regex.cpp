#include <iostream>
#include <chat_tools/v1/utils/regex.hpp>

int main() {
    std::cout << "test regex" << std::endl;

    chat_tools::v1::utils::RegexMatcher RegexMatcher;
    std::cout << "init: " << RegexMatcher.is_init() << std::endl;
    RegexMatcher.set_pattern("^[A-Za-z]+$");
    std::cout << "init: " << RegexMatcher.is_init() << std::endl;

    std::cout << "123: " << RegexMatcher.is_match("123") << std::endl;
    std::cout << "Hello!: " << RegexMatcher.is_match("Hello!") << std::endl;
    std::cout << "Hello: " << RegexMatcher.is_match("Hello") << std::endl;

    chat_tools::v1::utils::RegexReplacer RegexReplacer;
    std::cout << "init: " << RegexReplacer.is_init() << std::endl;
    RegexReplacer.add_rule("brown", "red");
    RegexReplacer.add_rule("lazy", "active");
    std::cout << "init: " << RegexReplacer.is_init() << std::endl;
    std::string text = "The quick brown fox jumps over the lazy dog";
    std::cout << "src: " << text << std::endl;
    std::cout << "dst: " << RegexReplacer.replace_all(text) << std::endl;

    std::u32string text32 = U"The quick brown fox jumps over the lazy dog";
    std::cout << "src: " << utf8::utf32to8(text32) << std::endl;
    std::cout << "dst: " << utf8::utf32to8(RegexReplacer.replace_all(text32)) << std::endl;

    return 0;
}
