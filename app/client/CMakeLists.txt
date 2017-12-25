cmake_minimum_required(VERSION 3.2)

set(MACHIAVELLI_CLIENT_TARGET_NAME "machiavelli-client")

project("Machiavelli Client")

add_library(${MACHIAVELLI_CLIENT_TARGET_NAME}
    STATIC
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/app/client.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/app/client.cpp")

target_include_directories(${MACHIAVELLI_CLIENT_TARGET_NAME}
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

target_link_libraries(${MACHIAVELLI_CLIENT_TARGET_NAME}
    "machiavelli-infra")