#pragma once
#include <vector>

#include "i_recognizer.hpp"

class regex_version : public i_recognizer {
    private:

    public:
    std::pair<bool, std::vector<std::string>> terminal_parser(const std::string&) override;
    std::pair<bool, std::vector<std::string>> file_parser(const std::string&) override;
};