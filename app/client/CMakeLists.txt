cmake_minimum_required(VERSION 3.2)

set(MACHIAVELLI_APP_CLIENT_TARGET_NAME "machiavelli-app-client")

project("Machiavelli App Client library")

add_library(${MACHIAVELLI_APP_CLIENT_TARGET_NAME}
    STATIC
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/app/client.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/app/onlineclient.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/app/onlineclientconfiguration.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/app/onlineclient.cpp")

target_include_directories(${MACHIAVELLI_APP_CLIENT_TARGET_NAME}
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

target_link_libraries(${MACHIAVELLI_APP_CLIENT_TARGET_NAME}
    "machiavelli-infra-common"
    "machiavelli-app-common")
