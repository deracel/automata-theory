#pragma once
#include <string>

#include "parser.hpp"

class i_recognizer {
    private:

    public:
        virtual ~i_recognizer() = default;
        virtual void terminal_parser(const std::string&) = 0;
        virtual void file_parser(const std::string&) = 0;
        [[nodiscard]] virtual const std::map<std::string, std::vector<std::string>>& get_parser_info() const noexcept = 0;
};
