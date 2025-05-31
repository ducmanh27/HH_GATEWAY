add_library(VTXFramework SHARED)

set_target_properties(VTXFramework PROPERTIES
    VERSION     ${VTXFRAMEWORK_VERSION}
    SOVERSION   ${VTXFRAMEWORK_SOVERSION}
    EXPORT_NAME VTXFramework
)
target_sources(VTXFramework PRIVATE
    logger/logger.cpp
    logger/logger.h
    http/client/httpclientinterface.h
    http/client/httpclientimpl.h
    http/client/httpclientimpl.cpp
    dbacess/entitymanager.cpp
    dbacess/entitymanager.h
    )

target_link_libraries(
    VTXFramework
    Qt${QT_VERSION_MAJOR}::Core
     KF${QT_MAJOR_VERSION}::ThreadWeaver
    spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
    libpqxx::pqxx
    PahoMqttCpp::paho-mqttpp3
)

target_include_directories(
    VTXFramework
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/3rdparty)

if(SPDLOG_BUILD_EXAMPLE_HO)
    add_executable(example_header_only example.cpp)
    target_link_libraries(example_header_only PRIVATE spdlog::spdlog_header_only)
endif()


include(GNUInstallDirs)
install(TARGETS VTXFramework
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
target_include_directories(VTXFramework PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
                                                "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
                                                )
target_include_directories(VTXFramework PUBLIC ${PostgreSQL_INCLUDE_DIRS})
