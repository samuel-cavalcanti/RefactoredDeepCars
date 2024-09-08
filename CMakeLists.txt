cmake_minimum_required(VERSION 3.16)

include(ExternalProject)

project(DeepCars)
project(TestDeepCars)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FindPkgConfig)

file(GLOB_RECURSE PIG "PIG/*.cpp" "PIG/*.h" )

file(GLOB_RECURSE UNITARY_TEST PIG/*.teste.cpp )

list(REMOVE_ITEM PIG ${UNITARY_TEST} )


include_directories("PIG/*")
include_directories("codigoFonte/*")


set(EXTERNAL_INSTALL_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/external)

ExternalProject_Add(sdl2_pkg
    GIT_REPOSITORY git@github.com:libsdl-org/SDL.git
    GIT_TAG release-2.26.0 
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)

ExternalProject_Add(sdl2_image_pkg
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG "SDL2" 
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)


ExternalProject_Add(sdl2_ttf_pkg
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git 
    GIT_TAG "SDL2"
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)


ExternalProject_Add(gtest_pkg
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.x
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)


ExternalProject_Add(gsl_pkg
    GIT_REPOSITORY git@github.com:ampl/gsl.git
    GIT_TAG v2.7.0
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION}
)

# # Define the installation directory
# set(SDL2_INSTALL_DIR ${EXTERNAL_INSTALL_LOCATION})
#
# # Set the include and library directories based on the installation
# set(SDL2_INCLUDE_DIR ${SDL2_INSTALL_DIR}/include)
# set(SDL2_LIBRARY_DIR ${SDL2_INSTALL_DIR}/lib)



include_directories(${EXTERNAL_INSTALL_LOCATION}/include)


add_executable(DeepCars
        ${PIG}
 #       ${SOURCE_CODE}
        main.cpp)


# add_dependencies(DeepCars sdl2_image sdl2_ttf sdl2 gsl)

add_executable(TestDeepCars
        ${UNITARY_TEST}
        ${PIG}
  #      ${SOURCE_CODE}
        main.teste.cpp)


add_dependencies(DeepCars  sdl2_pkg sdl2_image_pkg sdl2_ttf_pkg gsl_pkg)
add_dependencies(TestDeepCars  sdl2_pkg sdl2_image_pkg sdl2_ttf_pkg gsl_pkg gtest_pkg)



find_package(SDL2 REQUIRED)
target_link_libraries(DeepCars SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(TestDeepCars SDL2::SDL2 SDL2::SDL2main)



find_package(SDL2_image REQUIRED PATHS ${EXTERNAL_INSTALL_LOCATION})
target_link_libraries(DeepCars SDL2_image::SDL2_image)
target_link_libraries(TestDeepCars SDL2_image::SDL2_image)

find_package(SDL2_ttf REQUIRED PATHS ${EXTERNAL_INSTALL_LOCATION})
target_link_libraries(DeepCars SDL2_ttf::SDL2_ttf)
target_link_libraries(TestDeepCars SDL2_ttf::SDL2_ttf)

find_package(GSL REQUIRED PATHS ${EXTERNAL_INSTALL_LOCATION})
target_link_libraries(DeepCars GSL::gsl GSL::gslcblas)
target_link_libraries(TestDeepCars GSL::gsl GSL::gslcblas)

find_package(GTest 1.15.2 EXACT REQUIRED PATHS ${EXTERNAL_INSTALL_LOCATION})
target_link_libraries(TestDeepCars GTest::gtest GTest::gtest_main)

