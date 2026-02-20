#pragma once
#include <vector>

#include "i_recognizer.hpp"
#include "parser.hpp"

class regex_version : public i_recognizer {
    private:
        parser parser_;
    public:
    void terminal_parser(const std::string&) override;
    void file_parser(const std::string&) override;
    [[nodiscard]] const std::map<std::string, std::vector<std::string>>& get_parser_info() const noexcept override;
};
