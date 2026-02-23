add_library(dialogue STATIC)

target_sources(dialogue
        PRIVATE
        "source/dialogue.cpp"

        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "."
        FILES
        "include/dialogue.hpp"
)

target_include_directories(dialogue PUBLIC
        "include"
)

target_link_libraries(dialogue PRIVATE
        interfaces
        parser
)
