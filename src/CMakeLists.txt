set(ODB_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/entity/node.hxx
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
    --std c++17
    --generate-schema
    -d pgsql
    --generate-query
    --generate-session
    --default-pointer std::shared_ptr
    --hxx-prologue "#include \"traits.hxx\""
    ${ODB_SOURCES}
  DEPENDS ${ODB_SOURCES}
  COMMENT "Running ODB compiler for generating schema"
  VERBATIM
)
add_library(Gateway SHARED
    controller/gatewaycontroller.h controller/gatewaycontroller.cpp
    configmanager.h configmanager.cpp
    service/registerservice.h service/registerservice.cpp
    entity/node.hxx
    entity/traits-pgsql.hxx
    entity/traits.hxx
    entity/datetime.hxx
    entity/bool.hxx
    entity/database.hxx
    dao/nodedaoimpl.h
    dao/nodedaoimpl.cpp
    dao/nodedao.h
    ${ODB_GENERATED_SOURCES}
)
set_target_properties(Gateway PROPERTIES
    VERSION     ${GATEWAY_VERSION}
    SOVERSION   ${GATEWAY_SOVERSION}
    EXPORT_NAME Gateway
)
find_package(ODB REQUIRED)
find_package(Boost REQUIRED COMPONENTS date_time serialization)
target_link_libraries(
    Gateway
    ODB::odb
    ODB::odb-pgsql
    Boost::date_time
    Boost::serialization
    Core
)

target_include_directories(
    Gateway
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/3rdparty)

if(SPDLOG_BUILD_EXAMPLE_HO)
    add_executable(example_header_only example.cpp)
    target_link_libraries(example_header_only PRIVATE spdlog::spdlog_header_only)
endif()


include(GNUInstallDirs)
install(TARGETS Gateway
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
target_include_directories(Gateway PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
                                                "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
                                                )
target_include_directories(Gateway PUBLIC ${PostgreSQL_INCLUDE_DIRS})
add_definitions(-DDATABASE_PGSQL)
# For generated code can include file .hxx
target_include_directories(Gateway PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/entity)
