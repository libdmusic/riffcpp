cmake_minimum_required(VERSION 3.1)
project(riffcpp VERSION 2.0.0
                DESCRIPTION "Simple library for reading RIFF files"
                LANGUAGES CXX)

add_library(riffcpp "")
add_library(RiffCpp::riffcpp ALIAS riffcpp)

set(RIFFCPP_EXAMPLE_INSTALL_DIR "bin" CACHE "PATH" "Where to install the example executable")
option(RIFFCPP_INSTALL_EXAMPLE "Install example executable" ON)

include(GenerateExportHeader)
generate_export_header(riffcpp)

target_sources(riffcpp
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/riffcpp.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/membuffer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/Error.cpp
)

target_compile_features(riffcpp PUBLIC cxx_std_14)
target_include_directories(riffcpp
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>
)

add_executable(riff_dump ${CMAKE_CURRENT_LIST_DIR}/src/test.cpp)
target_link_libraries(riff_dump PRIVATE riffcpp)

include(GNUInstallDirs)

# install library files needed for linking
install(
    TARGETS riffcpp
    EXPORT riffcpp-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# install the public header files
install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    FILES ${PROJECT_BINARY_DIR}/riffcpp_export.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# installs the *cmake files in share directory
install(
    EXPORT riffcpp-targets
    FILE riffcpp-targets.cmake
    NAMESPACE riffcpp::
    DESTINATION share/riffcpp
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/cmake/riffcpp-config-version.cmake
    VERSION ${RIFFCPP_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_LIST_DIR}/cmake/riffcpp-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/riffcpp-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/riffcpp
)

install(
    FILES
        ${CMAKE_BINARY_DIR}/cmake/riffcpp-config.cmake
        ${CMAKE_BINARY_DIR}/cmake/riffcpp-config-version.cmake
    DESTINATION share/riffcpp
)

if(RIFFCPP_INSTALL_EXAMPLE)
    install(
        TARGETS riff_dump
        RUNTIME DESTINATION ${RIFFCPP_EXAMPLE_INSTALL_DIR}
    )
endif()

#Register package in user's package registry
export(PACKAGE riffcpp)

# Packaging support
set(CPACK_PACKAGE_VENDOR "Francesco Bertolaccini")
set(CPACK_PACKAGE_CONTACT "bertolaccinifrancesco@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Simple library for reading RIFF files")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

include(CPack)

include(CTest)
add_subdirectory(tests)