#include "regex_version.hpp"
#include <fstream>
#include <iostream>
#include <regex>

std::pair<STATE, std::pair<std::string, std::vector<std::string>>>
regex_version::lexline(const std::string& line) {

    std::smatch matches;
    std::string name_attr_pattern = "[a-zA-Z_.][a-zA-Z0-9_.]*";

    std::regex combined_regex(
        "^\\s*create\\s+"
        "(" + name_attr_pattern + ")"
        "(?:"
            "\\s*\\(\\s*"
            "(" + name_attr_pattern + ")"
            "((?:\\s*,\\s*" + name_attr_pattern + ")*)"
            "\\s*\\)"
            "|"
            "\\s+as\\s+"
            "(" + name_attr_pattern + ")"
            "\\s+join\\s+"
            "(" + name_attr_pattern + ")"
        ")"
        "\\s*$"
    );

    if (std::regex_search(line, matches, combined_regex)) {
        std::string name = matches[1];
        if (matches[2].matched) {
            std::vector<std::string> attributes;
            attributes.push_back(matches[2]);

            if (matches[3].matched) {
                std::regex single_attribute_regex(name_attr_pattern);
                std::string attribs = matches[3];

                auto w_begin = std::sregex_iterator(attribs.begin(), attribs.end(),
                                                    single_attribute_regex);
                auto w_end = std::sregex_iterator();

                for (std::sregex_iterator i = w_begin; i != w_end; ++i) {
                    std::string token = i->str();

                    token.erase(0, token.find_first_not_of(" "));
                    token.erase(token.find_last_not_of(" ") + 1);

                    if (token.empty()) {
                        return {STATE::NO, {"", {}}};
                    }
                    attributes.push_back(token);
                }
            }
            return {STATE::NEW_EXP, {name, attributes}};
        }
        else if (matches[4].matched) {
            std::vector<std::string> expressions;
            expressions.push_back(matches[4]);  // первое выражение
            expressions.push_back(matches[5]);  // второе выражение


            return {STATE::COMBINE_EXP, {name, expressions}};
        }
    }
    return {STATE::NO, {"", {}}};
}
