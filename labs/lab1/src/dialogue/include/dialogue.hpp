#pragma once
#include <iostream>

#include "i_lexer.hpp"
#include "service.hpp"

class dialogue {
    private:
        service& service_;
    public:
        explicit dialogue(service& service_) : service_(service_) {};
        void terminal_input();
        void file_input();
        void main_dialogue();
        void show_info();
        static std::string input_selection();
};
