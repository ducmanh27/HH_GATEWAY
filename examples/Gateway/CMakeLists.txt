project(HHGateway)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(HHGateway
  main.cpp
)
target_link_libraries(
    HHGateway
    Gateway
)
target_compile_definitions(HHGateway PRIVATE DATABASE_PGSQL)
