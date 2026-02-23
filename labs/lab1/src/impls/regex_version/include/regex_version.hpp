#pragma once
#include <vector>

#include "i_recognizer.hpp"
#include "parser.hpp"

class regex_version : public i_recognizer {
    private:
        parser parser_;
        bool create_new_expression(const std::string& str);
        bool combine_expressions(const std::string& str);
    public:
    void terminal_parser(const std::string&) override;
    void file_parser(const std::string&) override;
    [[nodiscard]] const std::map<std::string, std::vector<std::string>>& get_parser_info() const noexcept override;
    [[nodiscard]] bool get_parser_state() const noexcept override {return parser_.get_condition();}
};
