project(HttpClient)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(HttpClient
  main.cpp
)
target_link_libraries(
    HttpClient
    Gateway
)
