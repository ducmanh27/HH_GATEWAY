project(Database)
set(ODB_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/employee.hxx
)
set(ODB_GENERATED_SOURCES "")
foreach(header ${ODB_SOURCES})
  get_filename_component(fname ${header} NAME_WE)
  list(APPEND ODB_GENERATED_SOURCES
    ${CMAKE_CURRENT_BINARY_DIR}/${fname}-odb.cxx
    ${CMAKE_CURRENT_BINARY_DIR}/${fname}-odb.hxx
  )
endforeach()
add_custom_command(
  OUTPUT ${ODB_GENERATED_SOURCES}
  COMMAND odb
    --std c++11
    --generate-schema
    -d pgsql
    --generate-query
    --generate-session
    --default-pointer std::shared_ptr
    ${ODB_SOURCES}
  DEPENDS ${ODB_SOURCES}
  COMMENT "Running ODB compiler for employee.hxx"
  VERBATIM
)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(Database
  driver.cxx
  database.hxx
  employee.hxx
  ${ODB_GENERATED_SOURCES}
)
target_link_libraries(
    Database
    MPDCore
)

add_definitions(-DDATABASE_PGSQL)
