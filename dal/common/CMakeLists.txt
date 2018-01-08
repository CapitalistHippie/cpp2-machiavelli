cmake_minimum_required(VERSION 3.2)

set(MACHIAVELLI_DAL_COMMON_TARGET_NAME "machiavelli-dal-common")

project("Machiavelli DAL Common library")

add_library(${MACHIAVELLI_DAL_COMMON_TARGET_NAME}
    STATIC
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/dal/models/buildingcard.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/dal/models/buildingcolor.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/dal/models/charactercard.h"
    "${CMAKE_CURRENT_LIST_DIR}/include/mach/dal/buildingcardrepository.h"
	"${CMAKE_CURRENT_LIST_DIR}/include/mach/dal/charactercardrepository.h"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/dal/buildingcardrepository.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/src/mach/dal/charactercardrepository.cpp")

set_target_properties(${MACHIAVELLI_DAL_COMMON_TARGET_NAME} PROPERTIES
    CXX_STANDARD_REQUIRED 14)

target_include_directories(${MACHIAVELLI_DAL_COMMON_TARGET_NAME}
    PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")

target_link_libraries(${MACHIAVELLI_DAL_COMMON_TARGET_NAME}
	"machiavelli-infra-common")
	