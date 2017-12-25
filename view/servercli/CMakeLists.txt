cmake_minimum_required(VERSION 3.2)

set(MACHIAVELLI_SERVER_CLI_TARGET_NAME "machiavelli-server-cli")

project("Machiavelli Server CLI")

add_executable(${MACHIAVELLI_SERVER_CLI_TARGET_NAME}
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/statehandlers/configureserver.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/statehandlers/configureserver.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/statehandlers/statehandler.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/statehandlers/statehandlerbase.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/init.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/servercli.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/servercli.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/serverclistate.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/technicalerror.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/technicalerrorcategory.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/technicalerrorcategory.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/main.cpp")

target_include_directories(${MACHIAVELLI_SERVER_CLI_TARGET_NAME}
    PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src")

target_link_libraries(${MACHIAVELLI_SERVER_CLI_TARGET_NAME}
    "machiavelli-infra"
    "machiavelli-server")
