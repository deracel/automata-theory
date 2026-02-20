#include "parser.hpp"

#include <algorithm>

std::vector<std::string> parser::get_attribute(const std::string& expression_name) const noexcept {
    auto iter = expression_to_attribute_.find(expression_name);
    if (iter != expression_to_attribute_.end())
        return iter->second;
    return {};
}

std::vector<std::string> parser::get_expression(const std::string& attribute_name) const noexcept {
    auto iter = attribute_to_expression_.find(attribute_name);
    if (iter != attribute_to_expression_.end())
        return iter->second;
    return {};
}

void parser::add_expression(const std::string& expression_name, std::vector<std::string>& attributes) {
    if (expression_to_attribute_.find(expression_name) != expression_to_attribute_.end()) {
        condition_ = false;
        return;
    }
    if (attributes.empty()) {
        condition_ = false;
        return;
    }
    std::ranges::for_each(attributes, [&](std::string& attr){
        auto vec = get_expression(attr);
        std::string new_attr = attr + "." + expression_name;
        std::string old_attr = attr;


        if (!vec.empty()) {
            if (vec.size() == 1){
                const std::string& expr = vec.front();
                change_attribute_name(expr, attr);
            }
            attr = new_attr;
            auto& expression_vector = attribute_to_expression_.find(old_attr)->second;
            expression_vector.push_back(expression_name);
        }
        else {
            std::vector<std::string> new_vec = {expression_name};
            attribute_to_expression_.emplace(attr, new_vec);
        }
    });
    expression_to_attribute_.insert({expression_name, attributes});
}

void parser::change_attribute_name(const std::string& expression_name, const std::string& old_atr_name) {
    auto it = expression_to_attribute_.find(expression_name);
    if (it == expression_to_attribute_.end())
        return;
    auto& attributes = it->second;
    for (auto& attr : attributes) {
        if (attr == old_atr_name)
            attr =  old_atr_name + "." + expression_name;
    }
}

const std::map<std::string, std::vector<std::string>>& parser::get_map() const noexcept {
    return expression_to_attribute_;
}

