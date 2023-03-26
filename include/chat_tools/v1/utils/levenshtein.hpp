#pragma once
#ifndef CHAT_TOOLS_V1_UTILS_LEVENSHTEIN_HPP_INCLUDED
#define CHAT_TOOLS_V1_UTILS_LEVENSHTEIN_HPP_INCLUDED

#include <vector>
#include <string>
#include "utf8.h"
#include "utf8caseconv.hpp"

namespace chat_tools {
    namespace v1 {
        namespace utils {

            /* Алгоритм Расстояния Левенштейна
             * Расстояние Левенштейна между двумя словами-это минимальное количество односимвольных правок
             * (т. е. вставок, удалений или замен),
             * необходимых для изменения одного слова в другое
             * Оригинал кода: https://www.nookery.ru/hamming-distance-and-lowenstein-distance-example-implementation-in-c/
             */
            inline const size_t calc_levenshtein_distance(
                    const std::u32string &s,
                    const std::u32string &t) noexcept {
                const size_t n = s.size();
                const size_t m = t.size();
                // step 1
                if (n == 0) return m;
                if (m == 0) return n;
                // step 2
                std::vector<std::vector<size_t>> d(n + 1, std::vector<size_t>(m + 1, 0));
                for (size_t i = 0; i <= n; ++i) {
                    d[i][0] = i;
                }
                for (size_t j = 0; j <= m; ++j) {
                    d[0][j] = j;
                }
                // Step 3
                for (size_t i = 1; i <= n; ++i) {
                    //Step 4
                    for (size_t j = 1; j <= m; j++) {
                        // Step 5
                        const size_t cost = (t[j - 1] == s[i - 1]) ? 0 : 1;
                        // Step 6
                        d[i][j] = std::min(
                            std::min(d[i - 1][j] + 1, d[i][j - 1] + 1),
                            d[i - 1][j - 1] + cost);
                    }
                }
                // Step 7
                return d[n][m];
            }

            inline const size_t calc_levenshtein_distance(const std::string &s, const std::string &t) {
                return calc_levenshtein_distance(utf8::utf8to32(s), utf8::utf8to32(t));
            }

        }; // utils
    }; // v1
}; // chat_tools


#endif //CHAT_TOOLS_V1_UTILS_LEVENSHTEIN_HPP_INCLUDED
