find_package(Qt6Widgets ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE)
find_package(Qt6Test ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE)
add_subdirectory(Logger)
add_subdirectory(HttpClient)
add_subdirectory(Database)
add_subdirectory(Gateway)

