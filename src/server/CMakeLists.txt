# Set minimum CMake version
cmake_minimum_required(VERSION 3.15)

# Define the executable target
add_executable(server
    main.cpp
    dispatcher.cpp
    session_manager.cpp
    pty_session.cpp
)

# Include directories for server source files
target_include_directories(server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link the common library
target_link_libraries(server PRIVATE common)

# Add debug flags for development
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(server PRIVATE DEBUG_BUILD=1)
endif()
