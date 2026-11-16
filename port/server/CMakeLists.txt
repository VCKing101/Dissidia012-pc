cmake_minimum_required(VERSION 3.20)
# dissidia012-server, the public online server (d012_server.cpp,
# docs/port/online-matchmaking.md). It needs nothing of the game, no graphics
# and no sound: PortableKit's ad hoc code and the port's battle relay. Build it
# on its own, on the machine that runs it:
#
#   cmake -S port/server -B build-server -DD012_PORTABLEKIT_DIR=<PortableKit with the port's patches>
#   cmake --build build-server
#
# or within the port's build with -DD012_BUILD_SERVER=ON (port/CMakeLists.txt).
project(D012Server LANGUAGES CXX)

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release)
endif()

get_filename_component(d012_server_repo "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
set(D012_PORTABLEKIT_DIR "${d012_server_repo}/tools/local/port/PortableKit" CACHE PATH
    "PortableKit checkout to build against")
set(d012_kit "${D012_PORTABLEKIT_DIR}")
if(NOT EXISTS "${d012_kit}/host/adhoc/server.cpp")
    message(FATAL_ERROR "No PortableKit at ${d012_kit}: set D012_PORTABLEKIT_DIR")
endif()

add_executable(dissidia012-server
    "${CMAKE_CURRENT_SOURCE_DIR}/d012_server.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../host/d012_battle_relay.cpp"
    "${d012_kit}/host/profile.cpp"
    "${d012_kit}/host/adhoc/client.cpp"
    "${d012_kit}/host/adhoc/discovery.cpp"
    "${d012_kit}/host/adhoc/matching.cpp"
    "${d012_kit}/host/adhoc/port_mapping.cpp"
    "${d012_kit}/host/adhoc/server.cpp")
target_include_directories(dissidia012-server PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../host" "${d012_kit}/host" "${d012_kit}/include")
if(MSVC)
    target_compile_options(dissidia012-server PRIVATE /utf-8)
endif()
find_package(Threads REQUIRED)
target_link_libraries(dissidia012-server PRIVATE Threads::Threads)
if(WIN32)
    target_link_libraries(dissidia012-server PRIVATE ws2_32 iphlpapi)
endif()
