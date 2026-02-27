#pragma once
#include <vector>

#include "i_lexer.hpp"


class regex_version : public i_lexer {
    public:
        std::pair<STATE, std::pair<std::string, std::vector<std::string>>> lexline(const std::string& line) override;

};
