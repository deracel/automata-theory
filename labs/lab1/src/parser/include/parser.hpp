#pragma once
#include <map>
#include <string>
#include <vector>

class parser {
    private:
        std::map<std::string, std::vector<std::string>> attribute_to_expression_;
        std::map<std::string, std::vector<std::string>> expression_to_attribute_;
        bool condition_ = false;
    public:
        [[nodiscard]] std::vector<std::string> get_attribute(const std::string& expression_name) const noexcept;
        [[nodiscard]] std::vector<std::string> get_expression(const std::string& attribute_name) const noexcept;
        void add_expression(const std::string& expression_name, std::vector<std::string>& attributes);
        [[nodiscard]] bool get_condition() const noexcept {return condition_;}
        void change_attribute_name(const std::string& expression_name, const std::string& old_atr_name);
        void set_condition(bool new_condition) {condition_ = new_condition;}
        [[nodiscard]] const std::map<std::string, std::vector<std::string>>& get_map() const noexcept;
};
