add_library(interfaces INTERFACE)

target_sources(interfaces INTERFACE
        FILE_SET HEADERS
        BASE_DIRS "include"
        FILES

        "include/i_recognizer.hpp"
)

target_include_directories(interfaces INTERFACE
        "include"
)