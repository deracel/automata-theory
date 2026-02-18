#include "regex_version.hpp"
#include <fstream>
#include <regex>


std::pair<bool, std::string> regex_version::terminal_parser(const std::string& str) {
    std::cmatch result;
    std::string name_atr_pattern = "[a-zA-Z_.,][a-zA-Z0-9_.,]+";
    std::regex name_atr_regex(name_atr_pattern);
    std::string atribute_part = "(" + name_atr_pattern + R"((,\s*|\s\)))" + ")+";
    std::regex atribute_list_regex(R"(create\s+)" +
                                        name_atr_pattern +
                                        R"(\s+\(\s)" +
                                        atribute_part);

}



