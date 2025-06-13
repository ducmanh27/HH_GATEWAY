# FindODB.cmake
find_path(ODB_INCLUDE_DIR odb/database.hxx PATHS /usr/include)

find_library(ODB_LIBRARY NAMES odb PATHS
    /usr/lib
    /usr/lib/x86_64-linux-gnu
    /usr/local/lib)

find_library(ODB_PGSQL_LIBRARY NAMES odb-pgsql PATHS
    /usr/lib
    /usr/lib/x86_64-linux-gnu
    /usr/local/lib)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ODB DEFAULT_MSG ODB_LIBRARY ODB_INCLUDE_DIR)


if(ODB_FOUND)
    set(ODB_LIBRARIES ${ODB_LIBRARY} ${ODB_PGSQL_LIBRARY})
    set(ODB_INCLUDE_DIRS ${ODB_INCLUDE_DIR})

    mark_as_advanced(ODB_LIBRARY ODB_PGSQL_LIBRARY ODB_INCLUDE_DIR)

    add_library(ODB::odb UNKNOWN IMPORTED)
    set_target_properties(ODB::odb PROPERTIES
        IMPORTED_LOCATION "${ODB_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${ODB_INCLUDE_DIR}"
    )

    add_library(ODB::odb-pgsql UNKNOWN IMPORTED)
    set_target_properties(ODB::odb-pgsql PROPERTIES
        IMPORTED_LOCATION "${ODB_PGSQL_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${ODB_INCLUDE_DIR}"
    )
endif()

