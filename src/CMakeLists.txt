add_library(MPDFramework SHARED
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

set_target_properties(MPDFramework PROPERTIES
    VERSION     ${MPDFRAMEWORK_VERSION}
    SOVERSION   ${MPDFRAMEWORK_SOVERSION}
    EXPORT_NAME MPDFramework
)

target_link_libraries(
    MPDFramework
    Qt${QT_VERSION_MAJOR}::Core
     KF${QT_MAJOR_VERSION}::ThreadWeaver
    spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
    libpqxx::pqxx
    PahoMqttCpp::paho-mqttpp3
)

target_include_directories(
    MPDFramework
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/3rdparty)

if(SPDLOG_BUILD_EXAMPLE_HO)
    add_executable(example_header_only example.cpp)
    target_link_libraries(example_header_only PRIVATE spdlog::spdlog_header_only)
endif()


include(GNUInstallDirs)
install(TARGETS MPDFramework
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
target_include_directories(MPDFramework PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
                                                "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
                                                )
target_include_directories(MPDFramework PUBLIC ${PostgreSQL_INCLUDE_DIRS})
