# Windows version details on a program: its name, publisher and version, as
# Explorer's Properties > Details shows them. Antivirus checks are warier of a
# program that carries none. The version is the release's
# (tools/port/release-version.txt, read into d012_version by CMakeLists.txt).
#
#   d012_version_info(<target> "<description>")
#
# D012_VERSION_INFO=OFF builds without them, for comparing how antivirus checks
# treat the same program with and without.

option(D012_VERSION_INFO "Windows version details on the programs" ON)
# This file's folder, where the template is: inside the function,
# CMAKE_CURRENT_LIST_DIR is the caller's.
set(d012_version_info_dir "${CMAKE_CURRENT_LIST_DIR}")

string(REGEX MATCH "^([0-9]+)\\.([0-9]+)\\.([0-9]+)" d012_rc_numbers "${d012_version}")
set(d012_rc_major "${CMAKE_MATCH_1}")
set(d012_rc_minor "${CMAKE_MATCH_2}")
set(d012_rc_patch "${CMAKE_MATCH_3}")

function(d012_version_info target description)
    if(NOT D012_VERSION_INFO)
        return()
    endif()
    set(D012_RC_MAJOR "${d012_rc_major}")
    set(D012_RC_MINOR "${d012_rc_minor}")
    set(D012_RC_PATCH "${d012_rc_patch}")
    set(D012_RC_VERSION "${d012_version}")
    set(D012_RC_DESCRIPTION "${description}")
    get_target_property(name ${target} OUTPUT_NAME)
    if(NOT name)
        set(name "${target}")
    endif()
    set(D012_RC_NAME "${name}")
    set(D012_RC_FILE "${name}.exe")
    set(rc "${CMAKE_CURRENT_BINARY_DIR}/generated/${target}_version.rc")
    configure_file("${d012_version_info_dir}/version_info.rc.in" "${rc}" @ONLY)
    target_sources(${target} PRIVATE "${rc}")
endfunction()
