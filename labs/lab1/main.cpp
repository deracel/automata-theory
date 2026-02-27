
#include <memory>


#include "dialogue.hpp"
#include "regex_version.hpp"

#include "service.hpp"

int main() {
    auto rel = std::make_unique<regex_version>();
    auto serv = std::make_unique<service>(*rel);
    auto dia = std::make_unique<dialogue>(*serv);
    dia->main_dialogue();
    return 0;
}
