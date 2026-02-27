#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>

#include "i_lexer.hpp"

class i_lexer;

class service {
    private:
        i_lexer& lexer_;
        std::map<std::string, std::vector<std::string>> attribute_to_expression_;
        std::map<std::string, std::vector<std::string>> expression_to_attribute_;
        bool state_ = false;


        [[nodiscard]] std::vector<std::string> get_attribute(const std::string& expression_name) const noexcept;
        [[nodiscard]] std::vector<std::string> get_expression(const std::string& attribute_name) const noexcept;
        [[nodiscard]] bool add_expression(const std::string& expression_name, std::vector<std::string>& attributes);
        [[nodiscard]] bool get_condition() const noexcept {return state_;}
        void set_condition(bool new_condition) {state_ = new_condition;}
        [[nodiscard]] bool combine_expressions(const std::string& expression_1, const std::string& expression_2, const std::string& new_expression);
    public:
        explicit service(i_lexer& lexer) : lexer_(lexer) {}
        void get_line(const std::string& line);
        [[nodiscard]] const std::map<std::string, std::vector<std::string>>& get_lexer_info() const noexcept {return expression_to_attribute_;}
        [[nodiscard]] bool get_lexer_state() const noexcept {return state_;}
};
