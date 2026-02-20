#include "regex_version.hpp"
#include <fstream>
#include <regex>


void regex_version::terminal_parser(const std::string& str) {
    std::smatch matches;
    std::string name_attr_pattern = "[a-zA-Z_.,][a-zA-Z0-9_.,]*";
    std::regex attribute_list_regex("(create\\s+)"
                                        "(" + name_attr_pattern + ")"
                                        "(\\s+)"
                                        "(\\(\\s*" + "(" + name_attr_pattern + "\\s*)+" + "\\))"
                                        );


    std::vector<std::string> final_attributes;
    if (std::regex_search(str, matches, attribute_list_regex)) {
        std::regex single_atribute_regex(name_attr_pattern);
        std::string name = matches[2];
        std::string attribs = matches[4];

        auto w_begin = std::sregex_iterator(attribs.begin(), attribs.end(), single_atribute_regex);
        auto w_end = std::sregex_iterator(); // по умолчанию в конце

        for (std::sregex_iterator i = w_begin; i != w_end; ++i) {
            std::string token = i->str();

            if (i != w_end && !token.empty() && token.back() == ',')
                token.pop_back();

            final_attributes.push_back(token);
        }
        parser_.set_condition(true);
        parser_.add_expression(name, final_attributes);
    }
    parser_.set_condition(false);
}

const std::map<std::string, std::vector<std::string>>& regex_version::get_parser_info() const noexcept {
    return parser_.get_map();
}

void regex_version::file_parser(const std::string&) {
}



