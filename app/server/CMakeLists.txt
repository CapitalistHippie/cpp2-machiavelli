cmake_minimum_required(VERSION 3.2)

set(MACHIAVELLI_SERVER_TARGET_NAME "machiavelli-server")

project("Machiavelli Server")

add_library(${MACHIAVELLI_SERVER_TARGET_NAME}
    STATIC
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/app/server.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/app/serverclient.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/app/server.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/app/serverclient.cpp")

target_include_directories(${MACHIAVELLI_SERVER_TARGET_NAME}
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

target_link_libraries(${MACHIAVELLI_SERVER_TARGET_NAME}
    "machiavelli-infra")