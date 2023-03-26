#pragma once
#ifndef CHAT_TOOLS_V1_UTILS_SPLIT_HPP_INCLUDED
#define CHAT_TOOLS_V1_UTILS_SPLIT_HPP_INCLUDED

#include <vector>
#include <string>
#include "utf8.h"

namespace chat_tools {
    namespace v1 {
        namespace utils {

            /** \brief Разобрать строку списка
             * \param text      Исходный текст
             * \param delimiter Список разделителей
             * \return Массив слов
             */
            std::vector<std::string> split(const std::string &text, const std::string &delimiter = "\t\n\r\"/!?:;',.-{}<>() ") noexcept {
                std::vector<std::string> result;

                size_t found = text.find_first_of(delimiter);
                size_t start_index = 0;

                while(found != std::string::npos) {
                    std::size_t len = found - start_index;
                    if(len > 0) result.push_back(text.substr(start_index, len));
                    start_index = found + 1;
                    found = text.find_first_of(delimiter, start_index);
                }
                if(start_index != text.size()) {
                    result.push_back(std::string(text.begin() + start_index, text.end()));
                }
                return result;
            }

            std::vector<std::string> utf8_to_str_vec(const std::string &text) noexcept {
                std::vector<std::string> temp;
                if (text.empty()) return std::move(temp);

                const std::string utf8_text = utf8::replace_invalid(text);

                char* str = (char*)utf8_text.c_str();       // utf-8 string
                char* str_i = str;                      // string iterator
                char* str_end = str + utf8_text.size() + 1;  // end iterator

                while (str_i < str_end) {
                    char32_t code = 0;
                    try {
                        code = utf8::next(str_i, str_end); // get 32 bit code of a utf-8 symbol
                    } catch(...) {
                        return std::move(temp);
                    }
                    if (code == 0) continue;
                    std::string symbol;
                    utf8::append(code, symbol);
                    temp.push_back(symbol);
                }
                return std::move(temp);
            }

            template<class T = std::string>
            std::pair<T, int> find_repeated_substring(const T& str) {
                if (str.empty()) return std::make_pair(str, 1);
                T result;
                int count = 0;
                int length = str.length();
                for (int i = 0; i < length - 1; i++) {
                    for (int j = i + 1; j < length; j++) {
                        int k = i;
                        int l = j;
                        while (k < j && str[k] == str[l]) {
                            k++;
                            l++;
                        }
                        if (k == j) {
                            int sub_len = j - i;
                            int sub_count = 1;
                            while (l < length - sub_len + 1 && str.substr(l, sub_len) == str.substr(i, sub_len)) {
                                sub_count++;
                                l += sub_len;
                            }
                            if (sub_count > count) {
                                result = str.substr(i, sub_len);
                                count = sub_count;
                            }
                        }
                    }
                }
                if (count == 0) {
                    return std::make_pair(str, 1);
                }
                return std::make_pair(result, count + 1);
            }

        };
    };
};

#endif // CHAT_TOOLS_V1_UTILS_SPLIT_HPP_INCLUDED
