add_library(common
    logging.cpp
    config.cpp
)

# If you installed the library using a package manager
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} /usr/include)

# If you manually downloaded the library to vendor/nlohmann/
# Uncomment this line:
# target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ../../vendor/nlohmann)
