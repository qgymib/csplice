project(cJSON)

add_library(${PROJECT_NAME}
    "cJSON.c"
    "cJSON_Utils.c"
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
