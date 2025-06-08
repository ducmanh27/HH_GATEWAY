project(Logger)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(Logger
  main.cpp
)
target_link_libraries(
    Logger
    MPDFramework
)
