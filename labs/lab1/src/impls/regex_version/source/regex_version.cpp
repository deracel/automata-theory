#include "regex_version.hpp"
#include <fstream>
#include <regex>


std::pair<bool, std::vector<std::string>> regex_version::terminal_parser(const std::string& str) {
    std::smatch matches;
    std::string name_atr_pattern = "[a-zA-Z_.,][a-zA-Z0-9_.,]*";
    std::regex name_atr_regex(name_atr_pattern);
    std::regex atribute_list_regex("(create\\s+)"
                                        "(" + name_atr_pattern + ")"
                                        "(\\s+)"
                                        "(\\(\\s*" + "(" + name_atr_pattern + "\\s*)+" + "\\))"
                                        );


    std::pair<bool, std::vector<std::string>> result = std::make_pair(false, std::vector<std::string>{});
    if (std::regex_search(str, matches, atribute_list_regex)) {
        std::regex single_atribute_regex(name_atr_pattern);
        std::string atribs = matches[4];

        auto w_begin = std::sregex_iterator(atribs.begin(), atribs.end(), single_atribute_regex);
        auto w_end = std::sregex_iterator(); // по умолчанию в конце

        for (std::sregex_iterator i = w_begin; i != w_end; ++i) {
            std::string token = i->str();

            if (i != w_end && !token.empty() && token.back() == ',')
                token.pop_back();

            result.second.push_back(token);
        }
        result.first = true;
    }
    return result;
}

std::pair<bool, std::vector<std::string>> regex_version::file_parser(const std::string&) {
    return std::make_pair(false, std::vector<std::string>{});
}



