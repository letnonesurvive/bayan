cmake_minimum_required(VERSION 3.5)
project(bayan)

set(BOOST_ROOT "C:\\Work\\boost\\boost_1_82_0\\boost_1_82_0")
set(BOOST_LIBRARYDIR "C:\\Work\\boost\\boost_1_82_0\\boost_1_82_0\\lib")
set(Boost_INCLUDE_DIR "C:\\Work\\boost\\boost_1_82_0\\boost_1_82_0")

FIND_PACKAGE(Boost REQUIRED COMPONENTS program_options filesystem)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

set(CMAKE_CXX_STANDARD 17)

add_executable(bayan main.cpp)

target_link_libraries(bayan ${Boost_LIBRARIES})