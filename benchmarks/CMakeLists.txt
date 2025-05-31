add_executable(benchmarks main.cpp)
set_target_properties(benchmarks PROPERTIES MACOSX_BUNDLE FALSE)
target_link_libraries(benchmarks benchmark::benchmark)
