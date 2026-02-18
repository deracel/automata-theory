add_library(impls STATIC)

target_sources(impls
    PRIVATE
        "regex_version/source/regex_version.cpp"

    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS "."
    FILES
        "regex_version/include/regex_version.hpp"
)

target_include_directories(impls PUBLIC
        "regex_version/include"
)

target_link_libraries(impls PRIVATE
        interfaces
)
