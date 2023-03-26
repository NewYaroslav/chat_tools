#include <iostream>
#include <chat_tools/v1/tools/token.hpp>
#include "utf8.h"

int main() {
    std::cout << "test token" << std::endl;

    chat_tools::v1::tools::Token token_banan;
    token_banan.set_name(U"banan");
    token_banan.add_lexeme(U"banan");
    token_banan.add_lexeme(U"banana");
    token_banan.add_lexeme(U"bananas");

    chat_tools::v1::tools::Token token_ban;
    token_ban.set_name(U"ban");
    token_ban.add_lexeme(U"ban");

    chat_tools::v1::tools::Token token_pam;
    token_pam.set_name(U"pam");
    token_pam.add_lexeme(U"pam");
    token_pam.add_lexeme(U"pampam");

    chat_tools::v1::tools::Token token_pull;
    token_pull.set_name(U"pull");
    token_pull.add_lexeme(U"pull");

    chat_tools::v1::tools::Token token_bull;
    token_bull.set_name(U"bull");
    token_bull.add_lexeme(U"bull");

    // "The quick brown fox jumps over the lazy dog"

    chat_tools::v1::tools::Token token_the;
    token_the.set_name(U"the");
    token_the.add_lexeme(U"the");

    chat_tools::v1::tools::Token token_quick;
    token_quick.set_name(U"quick");
    token_quick.add_lexeme(U"quick");

    chat_tools::v1::tools::Token token_brown;
    token_brown.set_name(U"brown");
    token_brown.add_lexeme(U"brown");

    chat_tools::v1::tools::Token token_fox;
    token_fox.set_name(U"fox");
    token_fox.add_lexeme(U"fox");

    chat_tools::v1::tools::Token token_jumps;
    token_jumps.set_name(U"jumps");
    token_jumps.add_lexeme(U"jumps");

    chat_tools::v1::tools::Token token_over;
    token_over.set_name(U"over");
    token_over.add_lexeme(U"over");

    chat_tools::v1::tools::Token token_lazy;
    token_lazy.set_name(U"lazy");
    token_lazy.add_lexeme(U"lazy");

    chat_tools::v1::tools::Token token_dog;
    token_dog.set_name(U"dog");
    token_dog.add_lexeme(U"dog");

    std::vector<chat_tools::v1::tools::Token*> tokens;
    tokens.push_back(&token_banan);
    tokens.push_back(&token_ban);
    tokens.push_back(&token_pam);
    tokens.push_back(&token_pull);
    tokens.push_back(&token_bull);
    tokens.push_back(&token_the);
    tokens.push_back(&token_quick);
    tokens.push_back(&token_brown);
    tokens.push_back(&token_fox);
    tokens.push_back(&token_jumps);
    tokens.push_back(&token_over);
    tokens.push_back(&token_lazy);
    tokens.push_back(&token_dog);

    chat_tools::v1::tools::LexemeComparatorConfig config;

    std::vector<std::u32string> lexemes = {
        U"banan",
        U"pampam",
        U"pampampam",
        U"pampampampampam",
        U"bull",
        U"pull",
        U"pulll",
        U"ball",
        U"bam",
        U"spam",
        U"pam",
        U"am",
        U"a",
    };

    std::cout << "start test" << std::endl;

    for (size_t i = 0; i < lexemes.size(); ++i) {
        std::vector<chat_tools::v1::tools::Token::Result> result =
            chat_tools::v1::tools::lexeme_to_tokens(
                lexemes[i],
                tokens,
                config);

        std::cout << "lexeme: " << utf8::utf32to8(lexemes[i]) << std::endl;
        for (auto &item : result) {
            std::cout << "token: "
                << utf8::utf32to8(item.token->get_name())
                << "; dist: " << item.distance
                << "; lex: " << utf8::utf32to8(item.lexeme)
                << std::endl;
        }
    }

    std::vector<std::u32string> strings = {
        //U"bananpampambullspam",
        U"ban anp amp amb ull spam",
        U"The quick brown fox jumps over the lazy dog",
        U"he quic brawn fax jump aver te laz dag",
        U"Thequickbrownfoxjumpsoverthelazydog",
        U"hequicbrawnfaxjumpavertelazdag",
        //U"pampampam",
        //U"pampampampampam",
        //U"bull",
        //U"pull",
        //U"pulll",
        //U"ball",
        //U"bam",
        //U"spam",
    };

    std::cout << std::endl;
    std::cout << "start test" << std::endl;

    for (size_t i = 0; i < strings.size(); ++i) {
        std::vector<std::vector<chat_tools::v1::tools::Token::Result>> result =
            chat_tools::v1::tools::tokenize_string(
                strings[i],
                10,
                tokens,
                config);

        std::cout << "string: " << utf8::utf32to8(strings[i]) << std::endl;
        std::cout << "tokens: " << result.size() << std::endl;
        size_t pos = 0;
        for (auto &tokens : result) {
            std::cout << "pos: " << pos << std::endl;
            for (auto &token : tokens) {
                std::cout << "token: "
                    << utf8::utf32to8(token.token->get_name())
                    << "; dist: " << token.distance
                    << "; lex: " << utf8::utf32to8(token.lexeme)
                    << std::endl;
            }
            ++pos;
        }
    }
    return 0;
}
