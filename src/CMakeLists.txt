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
add_library(MPDCore SHARED
    mqtt/asyncsubscribe.h mqtt/asyncsubscribe.cpp
    logger/logger.cpp
    logger/logger.h
    http/client/httpclientinterface.h
    http/client/httpclientimpl.h
    http/client/httpclientimpl.cpp
    mqtt/asyncpublish.h mqtt/asyncpublish.cpp
    mqtt/callbackimpl.h mqtt/callbackimpl.cpp
    mqtt/actionlistener.h mqtt/actionlistener.cpp
    controller/gatewaycontroller.h controller/gatewaycontroller.cpp
    configmanager.h configmanager.cpp
    service/registerservice.h service/registerservice.cpp
    entity/node.hxx
    entity/traits-pgsql.hxx
    entity/traits.hxx
    ${ODB_GENERATED_SOURCES}

    entity/datetime.hxx
    entity/bool.hxx
    entity/database.hxx
    dao/nodedaoimpl.h dao/nodedaoimpl.cpp
    dao/nodedao.h
)
set_target_properties(MPDCore PROPERTIES
    VERSION     ${MPDCore_VERSION}
    SOVERSION   ${MPDCore_SOVERSION}
    EXPORT_NAME MPDCore
)
find_package(ODB REQUIRED)
find_package(Boost REQUIRED COMPONENTS date_time serialization)
target_link_libraries(
    MPDCore
    Qt${QT_VERSION_MAJOR}::Core
    KF${QT_MAJOR_VERSION}::ThreadWeaver
    spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
    libpqxx::pqxx
    PahoMqttCpp::paho-mqttpp3
    ODB::odb
    ODB::odb-pgsql
    Boost::date_time
    Boost::serialization
)

target_include_directories(
    MPDCore
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/3rdparty)

if(SPDLOG_BUILD_EXAMPLE_HO)
    add_executable(example_header_only example.cpp)
    target_link_libraries(example_header_only PRIVATE spdlog::spdlog_header_only)
endif()


include(GNUInstallDirs)
install(TARGETS MPDCore
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
target_include_directories(MPDCore PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
                                                "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
                                                )
target_include_directories(MPDCore PUBLIC ${PostgreSQL_INCLUDE_DIRS})
add_definitions(-DDATABASE_PGSQL)
# For generated code can include file .hxx
target_include_directories(MPDCore PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/entity)
