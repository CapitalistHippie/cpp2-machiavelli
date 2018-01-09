cmake_minimum_required(VERSION 3.2)

set(MACHIAVELLI_VIEW_COMMON_TARGET_NAME "machiavelli-view-common")

project("Machiavelli View Common library")

add_library(${MACHIAVELLI_VIEW_COMMON_TARGET_NAME}
    STATIC
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/view/console.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/view/helpers.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/console.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/helpers.cpp")

target_include_directories(${MACHIAVELLI_VIEW_COMMON_TARGET_NAME}
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

target_link_libraries(${MACHIAVELLI_VIEW_COMMON_TARGET_NAME}
    "machiavelli-dal-common")
    