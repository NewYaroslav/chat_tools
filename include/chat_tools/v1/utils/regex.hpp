#pragma once
#ifndef CHAT_TOOLS_V1_REGEX_HPP_INCLUDED
#define CHAT_TOOLS_V1_REGEX_HPP_INCLUDED

#include <regex>
#include <string>
#include <utility>
#include <type_traits>
#include <unordered_map>
#include "utf8.h"

namespace chat_tools {
    namespace v1 {
        namespace utils {

            inline const bool is_match(const std::string &str, const std::string &pattern) noexcept {
                try {
                    const std::regex re(pattern);
                    return std::regex_match(str, re);
                } catch (...) {}
                return false;
            }

            class RegexMatcher {
            public:

                RegexMatcher() {};

                RegexMatcher(const std::string &pattern) {
                    set_pattern(pattern);
                }

                inline bool set_pattern(const std::string &pattern) noexcept {
                    try {
                        m_re.assign(pattern);
                        m_init = true;
                    } catch (...) {
                        m_init = false;
                    }
                    return m_init;
                }

                inline bool set_pattern(const std::u32string &pattern) noexcept {
                    try {
                        m_re.assign(utf8::utf32to8(pattern));
                        m_init = true;
                    } catch (...) {
                        m_init = false;
                    }
                    return m_init;
                }

                inline const bool is_match(const std::string &str) noexcept {
                    if (!is_init()) return false;
                    return std::regex_match(str, m_re);
                }

                inline const bool is_match(const std::u32string &str) noexcept {
                    if (!is_init()) return false;
                    return std::regex_match(utf8::utf32to8(str), m_re);
                }

                inline const bool is_init() noexcept {
                    return m_init;
                }

            private:
                std::regex m_re;
                bool m_init = false;
            };

            class RegexReplacer {
            public:

                RegexReplacer() {};

                inline const bool add_rule(const std::string &pattern, const std::string &replacement) noexcept {
                    try {
                        m_rules[pattern] = std::make_pair(std::regex(pattern), replacement);
                        return true;
                    } catch (...) {}
                    return false;
                }

                inline const bool add_rule(const std::u32string &pattern, const std::u32string &replacement) noexcept {
                    return add_rule(utf8::utf32to8(pattern), utf8::utf32to8(replacement));
                }

                inline const std::string replace_all(const std::string &str) noexcept {
                    if (!is_init()) return str;
                    std::string temp(str);
                    for (const auto& rule : m_rules) {
                        temp = std::regex_replace(temp, rule.second.first, rule.second.second);
                    }
                    return std::move(temp);
                }

                inline const std::u32string replace_all(const std::u32string &str) noexcept {
                    if (!is_init()) return str;
                    std::string temp(utf8::utf32to8(str));
                    for (const auto& rule : m_rules) {
                        temp = std::regex_replace(temp, rule.second.first, rule.second.second);
                    }
                    return utf8::utf8to32(temp);
                }

                inline const bool is_init() noexcept {
                    return !m_rules.empty();
                }

            private:
                std::unordered_map<std::string, std::pair<std::regex, std::string>> m_rules;
            };

        }; // utils
    }; // v1
}; // chat_tools

#endif // CHAT_TOOLS_V1_REGEX_HPP_INCLUDED
