add_library(Core SHARED
    mqtt/asyncsubscribe.h mqtt/asyncsubscribe.cpp
    logger/logger.cpp
    logger/logger.h
    http/client/httpclientinterface.h
    http/client/httpclientimpl.h
    http/client/httpclientimpl.cpp
    mqtt/asyncpublish.h mqtt/asyncpublish.cpp
    mqtt/callbackimpl.h mqtt/callbackimpl.cpp
    mqtt/actionlistener.h mqtt/actionlistener.cpp
)
set_target_properties(Core PROPERTIES
    VERSION     ${CORE_VERSION}
    SOVERSION   ${CORE_SOVERSION}
    EXPORT_NAME Core
)
find_package(ODB REQUIRED)
find_package(Boost REQUIRED COMPONENTS date_time serialization)
target_link_libraries(
    Core
    Qt${QT_VERSION_MAJOR}::Core
    KF${QT_MAJOR_VERSION}::ThreadWeaver
    spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
    libpqxx::pqxx
    PahoMqttCpp::paho-mqttpp3
)

target_include_directories(
    Core
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/3rdparty)

if(SPDLOG_BUILD_EXAMPLE_HO)
    add_executable(example_header_only example.cpp)
    target_link_libraries(example_header_only PRIVATE spdlog::spdlog_header_only)
endif()


include(GNUInstallDirs)
install(TARGETS Core
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
target_include_directories(Core PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
                                        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
                                        ${CMAKE_CURRENT_SOURCE_DIR}
                                            )
target_include_directories(Core PUBLIC ${PostgreSQL_INCLUDE_DIRS})
add_definitions(-DDATABASE_PGSQL)
# For generated code can include file .hxx
