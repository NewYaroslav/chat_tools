#pragma once
#ifndef CHAT_TOOLS_V1_TOKEN_HPP_INCLUDED
#define CHAT_TOOLS_V1_TOKEN_HPP_INCLUDED

#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include "../utils/levenshtein.hpp"
#include "../utils/utf8caseconv.hpp"
#include "../utils/regex.hpp"

namespace chat_tools {
    namespace v1 {
        namespace tools {

            enum class TokenFlag {
                VALUE = 0,
            };

            class Token {
            private:
                std::vector<std::u32string> m_lexemes;
                std::set<TokenFlag>         m_flags;
                int64_t                     m_id = 0;
                std::u32string              m_name;
                size_t m_max_len = 0;

                utils::RegexMatcher m_regex_matcher;

            public:

                Token() {};

                inline void add_lexeme(const std::u32string &lexeme) noexcept {
                    m_lexemes.push_back(lexeme);
                    m_max_len = std::max(m_max_len, lexeme.size());
                }

                inline const bool set_pattern(const std::u32string &pattern) noexcept {
                    m_regex_matcher.set_pattern(pattern);
                }

                inline const bool set_pattern(const std::string &pattern) noexcept {
                    m_regex_matcher.set_pattern(pattern);
                }

                inline void set_flag(const TokenFlag flag) noexcept {
                    m_flags.insert(flag);
                }

                inline bool check_flag(const TokenFlag flag) noexcept {
                    return (m_flags.find(flag) != m_flags.end());
                }

                inline std::set<TokenFlag> get_flags() noexcept {
                    return m_flags;
                }

                inline void set_name(const std::u32string &name) noexcept {
                    m_name = name;
                }

                inline const std::u32string get_name() noexcept {
                    return m_name;
                }

                inline void set_id(const int64_t id) noexcept {
                    m_id = id;
                }

                inline int64_t get_id() noexcept {
                    return m_id;
                }

                class Result {
                public:
                    Token* token    = nullptr;
                    size_t distance = 0;
                    std::u32string lexeme;
                    std::u32string src_lexeme;
                    std::u32string dst_lexeme;
                    Result() {};
                }; // Result

                inline const Result distance(const std::u32string &lexeme) noexcept {
                    size_t min_distance = std::max(m_max_len, lexeme.size());
                    Result result;
                    result.token = this;
                    result.src_lexeme = lexeme;
                    result.distance = min_distance;
                    if (m_regex_matcher.is_init()) {
                        if (m_regex_matcher.is_match(lexeme)) {
                            result.distance = 0;
                            return std::move(result);
                        }
                        return std::move(result);
                    }
                    for (size_t i = 0; i < m_lexemes.size(); ++i) {
                        const size_t d = utils::calc_levenshtein_distance(
                            m_lexemes[i], lexeme);
                        if (d == 0) {
                            result.distance = 0;
                            result.dst_lexeme = m_lexemes[i];
                            return std::move(result);
                        }
                        if (min_distance > d) {
                            min_distance = d;
                            result.distance = min_distance;
                            result.dst_lexeme = m_lexemes[i];
                        }
                    }
                    return std::move(result);
                }
            }; // Token

            class LexemeComparatorConfig {
            private:
                std::vector<size_t> m_max_dist;

            public:

                LexemeComparatorConfig() {
                    m_max_dist = {
                        0,0,1,1,    //0-3
                        2,2,2,3,    //4-7
                        3,3,4,4,    //8-11
                        4,4,5,6,    //12-15
                        6,7,7,7,    //16-19
                        8,8,9,9,    //20-23
                        10,10,11,11,//24-27
                        11,11,12,12,//28-31
                    };
                }

                inline void set_max_dist(const size_t n, const size_t d) {
                    if (m_max_dist.size() <= n) m_max_dist.resize(n + 1);
                    m_max_dist[n] = d;
                }

                inline const size_t get_max_dist(const size_t n) const noexcept {
                    if (m_max_dist.size() <= n) return 0;
                    return m_max_dist[n];
                }
            }; // LexemeComparatorConfig

            inline std::vector<Token::Result> lexeme_to_tokens(
                    const std::u32string            &lexeme,
                    const std::vector<Token*>       &tokens,
                    const LexemeComparatorConfig    &config) noexcept {

                std::vector<std::u32string> lexemes;
                lexemes.push_back(lexeme);
                lexemes.push_back(utils::utf32_to_lwr_utf32(lexeme));

                std::vector<Token::Result> results;
                size_t token_distance = 0;
                for (size_t i = 0; i < tokens.size(); ++i) {
                    Token::Result result;
                    bool success = false;

                    for (size_t j = 0; j < lexemes.size(); ++j) {
                        Token::Result lexeme_result(tokens[i]->distance(lexemes[j]));

                        const size_t max_distance = config.get_max_dist(lexeme_result.dst_lexeme.size());
                        if (lexeme_result.distance > max_distance) continue;

                        const size_t min_len = lexeme_result.dst_lexeme.size() - max_distance;
                        if (lexemes[j].size() < min_len) continue;

                        if (!success) {
                            result = std::move(lexeme_result);
                            success = true;
                            continue;
                        }

                        if (result.distance > lexeme_result.distance) {
                            result = std::move(lexeme_result);
                        }
                    } // for j

                    if (!success) continue;

                    if (results.empty()) {
                        result.lexeme = lexeme;
                        results.push_back(result);
                        token_distance = result.distance;
                        continue;
                    }

                    if (result.distance < token_distance) {
                        results.clear();
                        result.lexeme = lexeme;
                        results.push_back(result);
                        token_distance = result.distance;
                        continue;
                    }

                    if (result.distance == token_distance) {
                        result.lexeme = lexeme;
                        results.push_back(result);
                        continue;
                    }
                } // for i
                return std::move(results);
            }

            std::vector<std::vector<Token::Result>> tokenize_string(
                    const std::u32string& str,
                    const size_t max_token_len,
                    const std::vector<Token*>& tokens,
                    const LexemeComparatorConfig& config) {
                const size_t n = str.size();
                std::vector<std::vector<Token::Result>> temp(n);
                const size_t max_len = std::min(str.size(), max_token_len);
                for (size_t len = 1; len <= max_len; ++len) {
                    for (size_t i = 0; i <= n - len; ++i) {
                        std::u32string sub_str = str.substr(i, len);
                        std::vector<Token::Result> sub_tokens = lexeme_to_tokens(sub_str, tokens, config);
                        if (sub_tokens.empty()) continue;

                        if (temp[i].empty()) {
                            temp[i] = std::move(sub_tokens);
                            std::sort(temp[i].begin(), temp[i].end(), [](Token::Result &a, Token::Result &b) { return a.token < b.token; });
                            continue;
                        }

                        for (auto& sub_token : sub_tokens) {
                            // ищем токен
                            auto it = std::lower_bound(temp[i].begin(), temp[i].end(), sub_token, [](const Token::Result &a, const Token::Result &b) { return a.token < b.token; });
                            if (it != temp[i].end() && it->token == sub_token.token) {
                                if (it->distance > sub_token.distance) {
                                    temp[i] = std::move(sub_tokens);
                                    break;
                                }
                                if (it->distance == sub_token.distance) {
                                    *it = sub_token;
                                }
                                continue;
                            }
                            // если токен не найден, проверяем, является ли данный токен самым маленьким
                            // так как у токенов одинаковая дистанция, то можно проверить первый элемен
                            if (temp[i].back().distance > sub_token.distance) {
                                temp[i] = std::move(sub_tokens);
                                break;
                            }
                            if (temp[i].back().distance == sub_token.distance) {
                                temp[i].insert(temp[i].end(), sub_tokens.begin(), sub_tokens.end());
                                break;
                            }
                        }
                    }
                }

                // выбираем наиболее длинный токен
                for (size_t i = 0; i < temp.size(); ++i) {
                    if (temp[i].size() <= 1) continue;
                    std::vector<Token::Result> sub_tokens = {temp[i][0]};
                    for (size_t j = 1; j < temp[i].size(); ++j) {
                        if (temp[i][j].src_lexeme.size() > sub_tokens.back().src_lexeme.size()) {
                            sub_tokens.back() = temp[i][j];
                            continue;
                        }
                        if (temp[i][j].src_lexeme.size() == sub_tokens.back().src_lexeme.size()) {
                            sub_tokens.push_back(temp[i][j]);
                            continue;
                        }
                    }
                    temp[i] = sub_tokens;
                }

                // удаляем лишние токены внутри однозначного токена
                for (size_t i = 0; i < temp.size(); ++i) {
                    if (temp[i].empty()) continue;
                    if (temp[i].back().distance != 0) continue;
                    size_t max_len = 0;
                    for (auto &item : temp[i]) {
                        max_len = std::max(max_len, item.src_lexeme.size());
                    }
                    max_len = std::min(i + max_len, temp.size());
                    for (size_t j = i + 1; j < max_len; ++j) {
                        temp[j].clear();
                    }
                }
                // удаляем токены, которые "наползают" на однозначеные
                for (int i = temp.size()-1; i > 0; --i) {
                    if (temp[i].empty()) continue;
                    if (temp[i].back().distance != 0) continue;
                    for (int j = i - 1; j >= 0; --j) {
                        if (temp[j].empty()) continue;
                        size_t max_len = 0;
                        for (auto &item : temp[j]) {
                            max_len = std::max(max_len, item.src_lexeme.size());
                        }
                        const int last_pos = j + max_len - 1;
                        if (last_pos >= i) {
                            temp[j].clear();
                        }
                    }
                }
                // находим центры токенов
                for (size_t i = 0; i < temp.size(); ++i) {
                    if (temp[i].empty()) continue;
                    for (auto &sub_token : temp[i]) {
                        // берем конкретный токен и смотрим его окрестность
                        const size_t max_pos = std::min(i + sub_token.src_lexeme.size(), temp.size());
                        size_t sub_token_distance = sub_token.distance;
                        size_t sub_token_pos = i;
                        for (size_t j = i + 1; j < max_pos; ++j) {
                            // ищем токен
                            auto it = std::lower_bound(temp[j].begin(), temp[j].end(), sub_token, [](const Token::Result &a, const Token::Result &b) { return a.token < b.token; });
                            if (it != temp[j].end() && it->token == sub_token.token) {
                                // токен найден
                                if (it->distance < sub_token_distance) {
                                    // новый центр токена
                                    sub_token_pos = j;
                                    sub_token_distance = it->distance;
                                } else
                                if (it->distance == sub_token_distance &&
                                    it->src_lexeme.size() < sub_token.src_lexeme.size()) {
                                    // новый центр токена
                                    sub_token_pos = j;
                                }
                                continue;
                            }
                        } // for j
                        // удаляем токен из всех мест, кроме центра
                        for (size_t j = i; j < max_pos; ++j) {
                            if (j == sub_token_pos) continue;
                            // ищем токен
                            auto it = std::lower_bound(temp[j].begin(), temp[j].end(), sub_token, [](const Token::Result &a, const Token::Result &b) { return a.token < b.token; });
                            if (it != temp[j].end() && it->token == sub_token.token) {
                                temp[j].erase(it);
                            }
                        }
                    } // for token
                } // for i

                // объедиянем пересекающиеся токены
                for (size_t i = 0; i < temp.size(); ++i) {
                    auto copy_tokens = temp[i];
                    for (auto sub_token : copy_tokens) {
                        // берем конкретный токен и смотрим его окрестность
                        const size_t max_pos = std::min(i + sub_token.src_lexeme.size(), temp.size());
                        for (size_t j = i + 1; j < max_pos; ++j) {
                            if (temp[j].empty()) continue;
                            // определяем сепень пересечения
                            size_t max_len = 0;
                            for (auto &item : temp[j]) {
                                max_len = std::max(max_len, item.src_lexeme.size());
                            }

                            const size_t intersection = i + sub_token.src_lexeme.size() - j;
                            if (intersection <= config.get_max_dist(std::min(max_len, sub_token.src_lexeme.size()))) {
                                continue;
                            }

                            if (temp[j].back().distance > sub_token.distance) {
                                temp[j].clear();
                                continue;
                            }
                            if (temp[j].back().distance < sub_token.distance) {
                                temp[i].clear();
                                continue;
                            }
                            temp[i].insert(temp[i].end(), temp[j].begin(), temp[j].end());
                            temp[j].clear();
                        }
                    }
                }

                // оставляем токены с самыми большими лексемами
                for (size_t i = 0; i < temp.size(); ++i) {
                    if (temp[i].size() <= 1) continue;
                    size_t max_len = temp[i][0].src_lexeme.size();
                    std::vector<Token::Result> tokens = {temp[i][0]};
                    for (size_t j = 1; j < temp[i].size(); ++j) {
                        const size_t len = temp[i][j].src_lexeme.size();
                        if (len > max_len) {
                            max_len = len;
                            tokens.clear();
                            tokens.push_back(temp[i][j]);
                            continue;
                        }
                        if (len == max_len) {
                            tokens.push_back(temp[i][j]);
                        }
                    }
                    temp[i] = tokens;
                }

                std::vector<std::vector<Token::Result>> result;
                for (auto& tokens : temp) {
                    if (tokens.empty()) continue;
                    result.push_back(tokens);
                }
                return std::move(result);
            }

        }; // tools
    }; // v1
}; // chat_tools

#endif // CHAT_TOOLS_V1_TOKEN_HPP_INCLUDED
