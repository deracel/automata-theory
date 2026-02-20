#include "parser.hpp"

#include <algorithm>
#include <set>
#include <unordered_map>

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
    if (expression_to_attribute_.contains(expression_name)) {
        condition_ = false;
        return;
    }
    if (attributes.empty()) {
        condition_ = false;
        return;
    }
    std::ranges::for_each(attributes, [&](const std::string& attr){
        auto vec = get_expression(attr);

        if (!vec.empty()) {
            auto& expression_vector = attribute_to_expression_.find(attr)->second;
            expression_vector.push_back(expression_name);
        }
        else {
            std::vector new_vec = {expression_name};
            attribute_to_expression_.emplace(attr, new_vec);
        }
    });
    expression_to_attribute_.insert({expression_name, attributes});
}


void parser::combine_expressions(const std::string& expression_1, const std::string& expression_2, const std::string& new_expression) {
    if (!expression_to_attribute_.contains(expression_1) ||
        !expression_to_attribute_.contains(expression_2) || expression_1 == expression_2) {
        condition_ = false;
        return;
    }
    auto attributes_1 = get_attribute(expression_1);
    auto attributes_2 = get_attribute(expression_2);
    std::vector<std::string> summary_vector;
    summary_vector.reserve(attributes_1.size() + attributes_2.size());

    std::unordered_map<std::string, std::vector<size_t>> map_2;
    for (size_t i = 0; i < attributes_2.size(); ++i)
        map_2[attributes_2[i]].push_back(i);

    for (size_t i = 0; i < attributes_1.size(); ++i) {
        auto it = map_2.find(attributes_1[i]);
        if (it != map_2.end()) {
            attributes_1[i] = attributes_1[i] + "." + expression_1;
            if (!it->second.empty()) {
                for (size_t index : it->second)
                    attributes_2[index] = attributes_2[index] + "." + expression_2;
                it->second.clear();
            }
        }
    }


    summary_vector.insert(summary_vector.end(), attributes_1.begin(), attributes_1.end());
    summary_vector.insert(summary_vector.end(), attributes_2.begin(), attributes_2.end());
    condition_ = true;
    add_expression(new_expression, summary_vector);
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

