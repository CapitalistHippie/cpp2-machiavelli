cmake_minimum_required(VERSION 3.2)

set(MACHIAVELLI_CLIENT_CLI_TARGET_NAME "machiavelli-client-cli")

project("Machiavelli Client CLI")

add_executable(${MACHIAVELLI_CLIENT_CLI_TARGET_NAME}
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/init.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/clientcli.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/clientcli.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/clientclistate.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/technicalerror.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/technicalerrorcategory.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/technicalerrorcategory.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/main.cpp")

target_include_directories(${MACHIAVELLI_CLIENT_CLI_TARGET_NAME}
    PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src")

target_link_libraries(${MACHIAVELLI_CLIENT_CLI_TARGET_NAME}
    "machiavelli-infra"
    "machiavelli-client")
