add_library(parser STATIC)

target_sources(parser
        PRIVATE
        "source/parser.cpp"

        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "."
        FILES
        "include/parser.hpp"
)

target_include_directories(parser PUBLIC
        "include/"
)


