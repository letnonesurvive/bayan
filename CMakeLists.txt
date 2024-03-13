cmake_minimum_required(VERSION 3.12)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project(bayan VERSION ${PROJECT_VESRION})

if (MSVC)
    set(BOOST_ROOT "C:\\Work\\boost\\boost_1_82_0\\boost_1_82_0")
    set(BOOST_LIBRARYDIR "C:\\Work\\boost\\boost_1_82_0\\boost_1_82_0\\lib")
    set(Boost_INCLUDE_DIR "C:\\Work\\boost\\boost_1_82_0\\boost_1_82_0")
endif()

FIND_PACKAGE(Boost REQUIRED COMPONENTS program_options filesystem regex)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

set(CMAKE_CXX_STANDARD 17)

add_executable(bayan main.cpp FileCommander.hpp FileCommander.cpp FilesReader.cpp FilesReader.hpp HashGenerator.hpp HashGenerator.cpp)

if (MSVC)
    target_compile_options(bayan PRIVATE
        /W4
    )
    target_compile_options(bayan PRIVATE
        /W4
    )
else ()
    target_compile_options(bayan PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(bayan PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS bayan RUNTIME DESTINATION bin)

target_link_libraries(bayan ${Boost_LIBRARIES})

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT hippiman1221@gmail.com)
include(CPack)