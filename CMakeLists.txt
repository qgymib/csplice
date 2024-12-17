cmake_minimum_required(VERSION 3.5)
project(csplice)

###############################################################################
# Functions
###############################################################################

# Enable all warning for target
function(setup_target_wall name)
    if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${name} PRIVATE /W4 /WX)
    else ()
        target_compile_options(${name} PRIVATE -Wall -Wextra -Werror)
    endif ()
endfunction()

###############################################################################
# Setup executable
###############################################################################

add_executable(${PROJECT_NAME}
    "src/function/__init__.c"
    "src/function/dirname.c"
    "src/function/openfile.c"
    "src/function/readfile.c"
    "src/function/splitlines.c"
    "src/function/splitwhitespaces.c"
    "src/function/writefile.c"
    "src/pass/__init__.c"
    "src/utils/log.c"
    "src/utils/luaext.c"
    "src/utils/openfile.c"
    "src/utils/readfile.c"
    "src/utils/str.c"
    "src/main.c"
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

setup_target_wall(${PROJECT_NAME})

###############################################################################
# Dependency
###############################################################################

add_subdirectory(third_party/lua-cmake)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        liblua
)

add_subdirectory(third_party/cJSON)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        cJSON
)
