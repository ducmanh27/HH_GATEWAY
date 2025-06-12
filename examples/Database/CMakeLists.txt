project(Database)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(Database
  main.cpp
  userdao.cpp
  userdao.h
)
target_link_libraries(
    Database
    MPDCore
)

