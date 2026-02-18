#pragma once
#include <string>

class i_recognizer {
    private:

    public:
        virtual ~i_recognizer() = default;
        virtual std::pair<bool, std::string> terminal_parser(const std::string&) = 0;
        virtual std::pair<bool, std::string> file_parser(const std::string&) = 0;
};
