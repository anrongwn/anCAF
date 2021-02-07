if(EXISTS CMakeLists.txt)
	message(FATAL_ERROR
		"Looks like you are trying to run cmake from the base sysdig source directory.\n"
		"** RUNNING CMAKE FROM THE BASE SYSDIG DIRECTORY WILL NOT WORK **\n"
		"To Fix:\n"
		" 1. Remove the CMakeCache.txt file in this directory. ex: rm CMakeCache.txt\n"
		" 2. Create a build directory from here. ex: mkdir build\n"
		" 3. cd into that directory. ex: cd build\n"
		" 4. Run cmake from the build directory. ex: cmake ..\n"
		" 5. Run make from the build directory. ex: make\n"
		"Full paste-able example:\n"
		"( rm -f CMakeCache.txt; mkdir build; cd build; cmake ..; make )\n")
endif()

cmake_minimum_required(VERSION 3.4)

project(anCAF VERSION 1.0.0.0)
message("CMAKE_PROJECT_NAME = ${CMAKE_PROJECT_NAME}")
message("PROJECT_VERSION = ${PROJECT_VERSION}")
message("PROJECT_VERSION_MAJOR = ${PROJECT_VERSION_MAJOR}")
message("PROJECT_VERSION_MINOR = ${PROJECT_VERSION_MINOR}")
message("PROJECT_VERSION_PATCH = ${PROJECT_VERSION_PATCH}")
message("PROJECT_VERSION_TWEAK = ${PROJECT_VERSION_TWEAK}")


if(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
    #SET(CMAKE_BUILD_TYPE Debug)
endif()
message("${CMAKE_PROJECT_NAME} BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
message("cmake_module_path: " ${CMAKE_MODULE_PATH})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#set(PACKAGE_NAME "antt_1")
#add_definitions(-DPLATFORM_NAME="${CMAKE_SYSTEM_NAME}")

#Boost lib
find_package(Boost 1.75.0 COMPONENTS coroutine container thread context  REQUIRED)
if (Boost_FOUND)
    message("BOOST_LIB found. ${Boost_INCLUDE_DIRS} ${Boost_LIBRARY_DIRS} ${Boost_LIBRARIES}")
    set(BOOST_INCLUDE_DIC "${Boost_INCLUDE_DIRS}")
    set(BOOST_LIB_DIC "${Boost_LIBRARY_DIRS}")
else()
    MESSAGE(FATAL_ERROR  "BOOST_LIB not found.")
endif()

#GTest lib
set (GTEST_ROOT "/usr/local/gtest")
find_package(GTest REQUIRED)
if (GTEST_FOUND)
    message("GTest found.  ${GTEST_INCLUDE_DIRS} ${GTEST_LIBRARY_DIRS} ${GTEST_LIBRARIES}")
    set(GTEST_INCLUDE_DIC "${GTEST_INCLUDE_DIRS}")
    set(GTEST_LIB_DIC "${GTEST_ROOT}/lib")
else()
    message(FATAL_ERROR "GTest not found.")
endif()

#CAF lib
set(CAF_ROOT "/usr/local/CAF")
find_package(CAF REQUIRED)
if (CAF_FOUND)
	message("CAF found. ${CAF_INCLUDE_DIR} ${CAF_LIBRARY_DIR} ${CAF_LIBRARIES}")
    set(CAF_INCLUDE_DIC "${CAF_ROOT}/inlcude")
    set(CAF_LIB_DIC "${CAF_ROOT}/lib")
else()
	message(FATAL_ERROR "CAF not found.")
endif()


set(DEPS_INCLUDE_DIC "${PROJECT_SOURCE_DIR}/deps/include")
#set(ASIO_INCLUDE_DIC "${PROJECT_SOURCE_DIR}/deps/asio-1.18.0/include")
#set(BOOST_INCLUDE_DIC "${PROJECT_SOURCE_DIR}/deps/boost_1_75_0/include")
#set(BOOST_LIB_DIC "${PROJECT_SOURCE_DIR}/deps/boost_1_75_0/lib/")
#set(GTEST_INCLUDE_DIC "${PROJECT_SOURCE_DIR}/deps/gtest/include")
#set(GTEST_LIB_DIC "${PROJECT_SOURCE_DIR}/deps/gtest/lib")

#set(UV_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/deps/libuv-v1.37.0/include")
#set(UV_LIB "${PROJECT_SOURCE_DIR}/deps/libuv-v1.37.0/lib/libuv_a.a")
#set(HIREDIS_INCLUDE "${PROJECT_SOURCE_DIR}/deps/hiredis/include")
#set(HIREDIS_LIB "${PROJECT_SOURCE_DIR}/deps/hiredis/lib/libhiredis_static.a")

#set(NANOMSG_INCLUDE "${PROJECT_SOURCE_DIR}/deps/libnanomsg/include")
#set(NANOMSG_LIB "${PROJECT_SOURCE_DIR}/deps/libnanomsg/lib/libnanomsg.a")

#set(NNG_INCLUDE "${PROJECT_SOURCE_DIR}/deps/libnng/include")
#set(NNG_LIB "${PROJECT_SOURCE_DIR}/deps/libnng/lib/libnng.a")

#set(CJsonObject_INCLUDE "${PROJECT_SOURCE_DIR}/deps/CJsonObject")
#set(SQLITE3_INCLUDE "${PROJECT_SOURCE_DIR}/deps/sqlite3.33")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	#set(TCMALLOC_LIB "${PROJECT_SOURCE_DIR}/deps/libtcmalloc/lib/libtcmalloc.a")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	#set(TCMALLOC_LIB "${PROJECT_SOURCE_DIR}/deps/libtcmalloc/lib/libtcmalloc_minimal.a")
endif()

#set(TCMALLOC_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/deps/libtcmalloc/include")

#set(SSL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/deps/openssl_1_1_1g/include")
#set(SSL_LIB "${PROJECT_SOURCE_DIR}/deps/openssl_1_1_1g/lib/libssl.a")
#set(CRYPTO_LIB "${PROJECT_SOURCE_DIR}/deps/openssl_1_1_1g/lib/libcrypto.a")


#message("ASIO_INCLUDE_DIC = ${ASIO_INCLUDE_DIC}")
message("CAF_INCLUDE_DIC = ${CAF_INCLUDE_DIC}")
message("CAF_LIB_DIC = ${CAF_LIB_DIC}")
message("BOOST_INCLUDE_DIC = ${BOOST_INCLUDE_DIC}")
message("BOOST_LIB_DIC = ${BOOST_LIB_DIC}")
message("GTEST_INCLUDE_DIC = ${GTEST_INCLUDE_DIC}")
message("GTEST_LIB_DIC = ${GTEST_LIB_DIC}")


if(NOT WIN32)
	set(anAsio_DEBUG_FLAGS "-DDEBUG -fno-omit-frame-pointer")
	set(anAsio_RELEASE_FLAGS "-DNDEBUG")

	#c17 / c++17 std
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c17 -ffunction-sections -fdata-sections")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -ffunction-sections -fdata-sections")

	#code coverage
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")

	#debug
	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0 -Wall -g -ggdb ${anAsio_DEBUG_FLAGS}")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall -g -ggdb ${anAsio_DEBUG_FLAGS}")

	#release
	#set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -Wall -g -fno-strict-aliasing ${anAsio_RELEASE_FLAGS}")
	#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Wall -g -fno-strict-aliasing ${anAsio_RELEASE_FLAGS}")
	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -Wall -fno-strict-aliasing ${anAsio_RELEASE_FLAGS}")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -Wall -fno-strict-aliasing ${anAsio_RELEASE_FLAGS}")

	set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")
	set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -Wl,--gc-sections")
	#kernal
	#if(CMAKE_SYSTEM_NAME MATCHES "Linux")
		#if(CMAKE_BUILD_TYPE STREQUAL "Debug")
			#set(KBUILD_FLAGS "${SYSDIG_DEBUG_FLAGS} ${SYSDIG_FEATURE_FLAGS}")
		#else()
			#set(KBUILD_FLAGS "${SYSDIG_FEATURE_FLAGS}")
		#endif()
	#endif()

else()

	set(anAsio_FLAGS_WIN "-D_CRT_SECURE_NO_WARNINGS -DWIN32 /EHsc /W3 /Zi")
	set(anAsio_FLAGS_WIN_DEBUG "/MTd /Od")
	set(anAsio_FLAGS_WIN_RELEASE "/MT")

	set(CMAKE_C_FLAGS "${anAsio_FLAGS_WIN}")
	set(CMAKE_CXX_FLAGS "${anAsio_FLAGS_WIN}")

	set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${anAsio_FLAGS_WIN_DEBUG}")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${anAsio_FLAGS_WIN_DEBUG}")

	set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${anAsio_FLAGS_WIN_RELEASE}")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${anAsio_FLAGS_WIN_RELEASE}")

endif()

add_subdirectory(an_tp1)
