#include <algorithm>
#include <iostream>
#include <memory>
#include <print>

#include "dialogue.hpp"
#include "regex_version.hpp"
#include "i_recognizer.hpp"

int main() {
    auto rel = std::make_unique<regex_version>();
    auto dia = std::make_unique<dialogue>(std::move(rel));
    dia->main_dialogue();
    return 0;
}
