add_library(MPDCore SHARED
    mqtt/asyncsubscribe.h mqtt/asyncsubscribe.cpp
    logger/logger.cpp
    logger/logger.h
    http/client/httpclientinterface.h
    http/client/httpclientimpl.h
    http/client/httpclientimpl.cpp
    dbacess/entitymanager.cpp
    dbacess/entitymanager.h
    mqtt/asyncpublish.h mqtt/asyncpublish.cpp
    mqtt/callbackimpl.h mqtt/callbackimpl.cpp
    mqtt/actionlistener.h mqtt/actionlistener.cpp
    controller/gatewaycontroller.h controller/gatewaycontroller.cpp
    configmanager.h configmanager.cpp
    service/registerservice.h service/registerservice.cpp


)

set_target_properties(MPDCore PROPERTIES
    VERSION     ${MPDCore_VERSION}
    SOVERSION   ${MPDCore_SOVERSION}
    EXPORT_NAME MPDCore
)
find_package(ODB REQUIRED)
target_link_libraries(
    MPDCore
    Qt${QT_VERSION_MAJOR}::Core
     KF${QT_MAJOR_VERSION}::ThreadWeaver
    spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
    libpqxx::pqxx
    PahoMqttCpp::paho-mqttpp3
    ODB::odb
    ODB::odb-pgsql
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
