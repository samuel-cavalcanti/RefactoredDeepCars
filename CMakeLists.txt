cmake_minimum_required(VERSION 3.7)

project(DeepCars)

project(TestDeepCars)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmakes/")

include(FindPkgConfig)


file(GLOB_RECURSE PIG "PIG/*.cpp" "PIG/*.h" )


file(GLOB_RECURSE UNITARY_TEST PIG/*.teste.cpp )

#MESSAGE(STATUS  unitary ${UNITARY_TEST})


#list(REMOVE_ITEM SOURCE_CODE ${UNITARY_TEST} )

list(REMOVE_ITEM PIG ${UNITARY_TEST} )


include_directories("PIG/*")
include_directories("codigoFonte/*")

add_executable(DeepCars
        ${PIG}
 #       ${SOURCE_CODE}
        main.cpp)


add_executable(TestDeepCars
        ${UNITARY_TEST}
        ${PIG}
  #      ${SOURCE_CODE}
        main.teste.cpp)


PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)

#find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(DeepCars ${SDL2_LIBRARIES})
target_link_libraries(TestDeepCars ${SDL2_LIBRARIES})

find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(DeepCars ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(TestDeepCars ${SDL2_IMAGE_LIBRARIES})

find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIRS})
target_link_libraries(DeepCars ${SDL2_TTF_LIBRARIES})
target_link_libraries(TestDeepCars ${SDL2_TTF_LIBRARIES})

find_package(GSL REQUIRED)
target_link_libraries(DeepCars GSL::gsl GSL::gslcblas)
target_link_libraries(TestDeepCars GSL::gsl GSL::gslcblas)

find_package(GTest REQUIRED)
target_link_libraries(TestDeepCars GTest::GTest GTest::Main)
### printing things

#macro(print_all_variables)
#    message(STATUS "print_all_variables------------------------------------------{")
#    foreach (file ${PIG})
#        message(STATUS "${file}\n")
#    endforeach()
#    message(STATUS "print_all_variables------------------------------------------}")
#endmacro()


#print_all_variables()
