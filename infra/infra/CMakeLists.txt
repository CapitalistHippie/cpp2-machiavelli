cmake_minimum_required(VERSION 3.2)

set(MACHIAVELLI_INFRA_TARGET_NAME "machiavelli-infra")

project("Machiavelli Infra")

add_library(${MACHIAVELLI_INFRA_TARGET_NAME}
    STATIC
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/abstractfactory.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/clicommand.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/clicommandparser.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/command.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/commandmediator.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/functionalerror.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/functionalerrorcategory.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/noncopyable.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/sockets.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/tcpclient.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/infra/tcpserver.h"
	"${CMAKE_CURRENT_LIST_DIR}/src/mach/infra/clicommandparser.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/infra/command.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/infra/commandmediator.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/infra/functionalerrorcategory.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/infra/sockets.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/infra/tcpclient.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/infra/tcpserver.cpp")

set_target_properties(${MACHIAVELLI_INFRA_TARGET_NAME} PROPERTIES
    CXX_STANDARD_REQUIRED 14)

target_include_directories(${MACHIAVELLI_INFRA_TARGET_NAME}
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

if (WIN32)
    target_link_libraries(${MACHIAVELLI_INFRA_TARGET_NAME}
        wsock32
        ws2_32)
endif()