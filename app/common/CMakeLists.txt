cmake_minimum_required(VERSION 3.2)

set(MACHIAVELLI_APP_COMMON_TARGET_NAME "machiavelli-app-common")

project("Machiavelli App Common library")

add_library(${MACHIAVELLI_APP_COMMON_TARGET_NAME}
    STATIC
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/app/events/clientconnectedevent.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/app/models/clientinfo.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/placeholder.cpp")

target_include_directories(${MACHIAVELLI_APP_COMMON_TARGET_NAME}
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

target_link_libraries(${MACHIAVELLI_APP_COMMON_TARGET_NAME}
    "machiavelli-infra-common")