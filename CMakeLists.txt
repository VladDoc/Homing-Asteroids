cmake_minimum_required(VERSION 3.16)
project(HomingAsteroids)

set(CMAKE_CXX_STANDARD 17)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

file(GLOB SOURCE_EXE_TOP
        ${PROJECT_SOURCE_DIR}/*.cpp
        ${PROJECT_SOURCE_DIR}/*.c)

file(GLOB_RECURSE SOURCE_EXE_NESTED
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/src/*.c)

if (WIN32)
    if (MSVC)
        set(SDL2_PATH "D:/SDL2-2.0.12MSVC/")
        set(SDL2_IMAGE_PATH "D:/SDL2Image2.0.5MSVC/")
    endif ()
    if (MINGW)
        if(CMAKE_SIZEOF_VOID_P EQUAL 4)
            set(SDL2_PATH "D:/SDL2-2.0.10/i686-w64-mingw32/")
            set(SDL2_IMAGE_PATH "D:/SDL2_image-2.0.5/i686-w64-mingw32/")
        endif()
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(SDL2_PATH "D:/SDL2-2.0.10/x86_64-w64-mingw32/")
            set(SDL2_IMAGE_PATH "D:/SDL2_image-2.0.5/x86_64-w64-mingw32/")
        endif()
    endif ()
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
#find_package(SDL2_gfx REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

set(SOURCE_EXE ${SOURCE_EXE_TOP} ${SOURCE_EXE_NESTED})

add_executable(HomingAsteroids ${SOURCE_EXE})

target_link_libraries(HomingAsteroids ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
