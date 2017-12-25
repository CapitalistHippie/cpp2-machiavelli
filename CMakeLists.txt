cmake_minimum_required(VERSION 3.2)

project("Machiavelli")

add_subdirectory("infra/infra")

add_subdirectory("app/server")
add_subdirectory("app/client")

add_subdirectory("view/servercli")
