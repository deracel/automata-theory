add_library(service STATIC)

target_sources(service
        PRIVATE
        "source/service.cpp"

        PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "."
        FILES
        "include/service.hpp"
)

target_include_directories(service PUBLIC
        "include/"
)



target_link_libraries(service PRIVATE
        interfaces
)
