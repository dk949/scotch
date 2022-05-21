# Use the toolchain file
set(CMAKE_TOOLCHAIN_FILE
    "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING ""
)

# Read manifest
file(READ "${CMAKE_SOURCE_DIR}/vcpkg.json" _manifest)


# Get project name from manifest
string(JSON THIS_PROJECT_NAME ERROR_VARIABLE _err GET ${_manifest} "name")
if(_err)
    message(FATAL_ERROR "Could not get project name from manifest: ${_err}")
endif()


# get versino from manifest. try all formats until one works
set(VERSION_TYPES "version-string" "version" "version-semver" "version-date")
foreach(_version ${VERSION_TYPES})
    string(JSON THIS_PROJECT_VERSION ERROR_VARIABLE _err GET ${_manifest} "${_version}")
    if(_err STREQUAL "NOTFOUND")
        break()
    endif()
endforeach()
if(_err)
    message(FATAL_ERROR "Could not get project version from manifest: ${_err}")
endif()


# get description from manifest.
string(JSON THIS_PROJECT_DESCRIPTION ERROR_VARIABLE _err GET ${_manifest} "description")
if(_err)
    message(FATAL_ERROR "Could not get project description from manifest: ${_err}")
endif()
