project(epoxy)

option(epoxy_VERBOSE "Verbose epoxy package reporting." ON)

find_package(PkgConfig QUIET)
pkg_check_modules(PKG_epoxy QUIET epoxy)

set(epoxy_VERSION ${PKG_epoxy_VERSION})
set(epoxy_DEFINITIONS ${PKG_epoxy_CFLAGS})

find_path(
    epoxy_INCLUDE_DIRS

    NAMES "epoxy/gl.h"
    HINTS ${PKG_epoxy_INCLUDEDIR} ${PKG_epoxy_INCLUDE_DIRS}
)

find_library(
    epoxy_LIBRARIES

    NAMES epoxy
    HINTS ${PKG_epoxy_LIBDIR} ${PKG_epoxy_LIBRARY_DIRS}
)

find_file(
    epoxy_GLX_HEADER

    NAMES "epoxy/glx.h"
    HINTS ${PKG_epoxy_LIBRARY_DIRS}
)

if (epoxy_GLX_HEADER STREQUAL "epoxy_GLX_HEADER-NOTFOUND")
    set(epoxy_HAS_GLX FALS CACHE BOOL "whether glx is available")
else ()
    set(epoxy_HAS_GLX TRUE CACHE BOOL "whether glx is available")
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    epoxy
    
    FOUND_VAR epoxy_FOUND
    REQUIRED_VARS epoxy_LIBRARIES
    VERSION_VAR epoxy_VERSION
)

if (epoxy_FOUND AND NOT TARGET epoxy::epoxy)
    add_library(epoxy::epoxy MODULE IMPORTED)
    set_target_properties(
        epoxy::epoxy PROPERTIES

        IMPORTED_LOCATION "${epoxy_LIBRARIES}"
        INTERFACE_COMPILE_OPTIONS "${epoxy_DEFINITIONS}"
        INTERFACE_INCLUDE_DIRECTORIES "${epoxy_INCLUDE_DIRS}"
    )
endif ()

if (epoxy_VERBOSE)
    message(STATUS "epoxy library found:")
    message(STATUS "    verion: ${epoxy_VERSION}")
    message(STATUS "    ${epoxy_LIBRARIES}")
endif ()

mark_as_advanced(
    epoxy_DEFINITIONS
    epoxy_HAS_GLX
    epoxy_INCLUDE_DIRS
    epoxy_LIBRARIES
    epoxy_VERSION
)
