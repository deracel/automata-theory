#include "regex_version.hpp"
#include <fstream>
#include <iostream>
#include <regex>

bool create_new_expression(const std::string& str, parser& parser_) {
    std::smatch matches;
    std::string name_attr_pattern = "[a-zA-Z_.,][a-zA-Z0-9_.,]*";
    std::regex attribute_list_regex("(create\\s+)"
                                        "(" + name_attr_pattern + ")"
                                        "(\\s+)"
                                        "(\\(\\s*" + "(" + name_attr_pattern + "\\s*)+" + "\\))"
                                        );


    std::vector<std::string> final_attributes;
    if (std::regex_search(str, matches, attribute_list_regex)) {
        std::regex single_attribute_regex(name_attr_pattern);
        std::string name = matches[2];
        std::string attribs = matches[4];

        auto w_begin = std::sregex_iterator(attribs.begin(), attribs.end(), single_attribute_regex);
        auto w_end = std::sregex_iterator(); // по умолчанию в конце

        for (std::sregex_iterator i = w_begin; i != w_end; ++i) {
            std::string token = i->str();

            if (i != w_end && !token.empty() && token.back() == ',')
                token.pop_back();

            final_attributes.push_back(token);
        }
        parser_.add_expression(name, final_attributes);
        return true;
    }
    return false;
}

bool combine_expressions(const std::string& str, parser& parser_) {
    std::smatch matches;
    std::string name_pattern = "[a-zA-Z_.,][a-zA-Z0-9_.,]*";
    std::regex combine_regex("(create\\s+)"
                                "(" + name_pattern + ")"
                                "(\\s+)"
                                "(as\\s+)"
                                "(" + name_pattern + ")"
                                "(\\s+join\\s+)"
                                "(" + name_pattern + ")"
    );
    if (std::regex_search(str, matches, combine_regex)) {
        std::string new_expression = matches[2];
        std::string expression_1 = matches[5];
        std::string expression_2 = matches[7];
        parser_.combine_expressions(expression_1, expression_2, new_expression);
        return true;
    }
    return false;
}

void regex_version::terminal_parser(const std::string& str) {
    if ((create_new_expression(str, parser_) && !combine_expressions(str, parser_)) ||
        (!create_new_expression(str, parser_) && combine_expressions(str, parser_)))
        parser_.set_condition(true);
    else
        parser_.set_condition(false);
}

const std::map<std::string, std::vector<std::string>>& regex_version::get_parser_info() const noexcept {
    return parser_.get_map();
}

void regex_version::file_parser(const std::string&) {
}



