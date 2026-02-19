#include <algorithm>
#include <iostream>
#include <memory>
#include <print>

#include "regex_version.hpp"
#include "i_recognizer.hpp"

int main() {
    auto rel = std::make_unique<regex_version>();
    auto* interface = dynamic_cast<i_recognizer*>(rel.get());
    std::string input;
    std::getline(std::cin, input);
    auto res = interface->terminal_parser(input);
    std::cout << res.first << std::endl;
    std::ranges::for_each(res.second, [&](auto&& result) {
        std::cout << result << std::endl;
    });
    return 0;
}
