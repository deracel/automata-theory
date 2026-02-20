#include <algorithm>
#include <iostream>
#include <memory>
#include <print>

#include "regex_version.hpp"
#include "i_recognizer.hpp"

int main() {
    auto rel = std::make_unique<regex_version>();
    auto* interface = dynamic_cast<i_recognizer*>(rel.get());
    while (1) {
        std::string input;
        std::getline(std::cin, input);
        interface->terminal_parser(input);
        const auto& expressions= interface->get_parser_info();
        for (auto iter = expressions.begin(); iter != expressions.end(); ++iter) {
            std::cout << "\t" << iter->first << ": ";
            std::ranges::for_each(iter->second, [](const auto& attr) {
                std::cout << attr << " ";
            });
            std::cout << std::endl;
        }

    }
    return 0;
}
