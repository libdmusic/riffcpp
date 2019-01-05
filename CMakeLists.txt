cmake_minimum_required(VERSION 3.1)
project(libriffcpp VERSION 1.0.0
                DESCRIPTION "Simple library for reading RIFF files"
                LANGUAGES CXX)

add_library(riffcpp
  src/riffcpp.cpp
)

add_library(RiffCpp::riffcpp ALIAS riffcpp)

target_compile_features(riffcpp PUBLIC cxx_std_14)
target_include_directories(riffcpp
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/RiffCpp)

install(TARGETS riffcpp
        EXPORT riffcpp-targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
      )
set_target_properties(riffcpp PROPERTIES EXPORT_NAME RiffCpp)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#Export the targets to a script
install(EXPORT riffcpp-targets
  FILE
    RiffCppTargets.cmake
  NAMESPACE
    RiffCpp::
  DESTINATION
    ${INSTALL_CONFIGDIR}
)

#Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/RiffCppConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/RiffCppConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/RiffCppConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

#Install the config, configversion and custom find modules
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/RiffCppConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/RiffCppConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

export(EXPORT riffcpp-targets FILE ${CMAKE_CURRENT_BINARY_DIR}/RiffCppTargets.cmake NAMESPACE RiffCpp::)

#Register package in user's package registry
export(PACKAGE RiffCpp)

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