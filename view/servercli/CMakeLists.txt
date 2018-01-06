cmake_minimum_required(VERSION 3.2)

set(MACHIAVELLI_VIEW_SERVER_CLI_TARGET_NAME "machiavelli-view-servercli")

project("Machiavelli View Server CLI")

add_executable(${MACHIAVELLI_VIEW_SERVER_CLI_TARGET_NAME}
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/statehandlers/configureserver.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/statehandlers/configureserver.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/statehandlers/serverrunning.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/view/statehandlers/serverrunning.h"
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

target_include_directories(${MACHIAVELLI_VIEW_SERVER_CLI_TARGET_NAME}
    PRIVATE "${CMAKE_CURRENT_LIST_DIR}/src")

target_link_libraries(${MACHIAVELLI_VIEW_SERVER_CLI_TARGET_NAME}
    "machiavelli-view-common"
    "machiavelli-infra-common"
    "machiavelli-app-server")
