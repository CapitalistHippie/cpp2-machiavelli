cmake_minimum_required(VERSION 3.2)

project("Machiavelli")

add_subdirectory("infra/common")

add_subdirectory("app/common")
add_subdirectory("app/server")
add_subdirectory("app/client")

add_subdirectory("view/common")
add_subdirectory("view/servercli")
add_subdirectory("view/clientcli")
