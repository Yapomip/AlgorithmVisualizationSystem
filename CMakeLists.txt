cmake_minimum_required(VERSION 3.15)
project(Experiment LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++-18")

file(GLOB_RECURSE FILES_IN_SRC "src/*")
set(SRC_FILES "")

foreach(FILE ${FILES_IN_SRC})
    if(FILE MATCHES "\\.cpp$" OR FILE MATCHES "\\.c$")
        list(APPEND SRC_FILES ${FILE})
        message(STATUS "Source file: ${FILE}")
    endif()
endforeach()

file(GLOB_RECURSE INCLUDE_DIRS_AND_FILES LIST_DIRECTORIES true "include/*")
set(INCLUDE_DIRS "")
set(INCLUDE_HEADERS "")

foreach(FILE ${INCLUDE_DIRS_AND_FILES})
    if(IS_DIRECTORY ${FILE})
        list(APPEND INCLUDE_DIRS ${FILE})
        message(STATUS "Include directory: ${FILE}")
    else()
        if(FILE MATCHES "\\.hpp$" OR FILE MATCHES "\\.h$")
            list(APPEND INCLUDE_HEADERS ${FILE})
        endif()
    endif()
endforeach()


# find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Quick)
# find_package(yaml-cpp REQUIRED)

add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    ${INCLUDE_HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${INCLUDE_DIRS}
)

add_custom_target(build
    DEPENDS ${PROJECT_NAME}
    COMMENT "Building project."
)

add_custom_target(clear
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
    COMMENT "Cleaning up generated files."
)

add_custom_target(build_and_run
    DEPENDS ${PROJECT_NAME}
    COMMAND $<TARGET_FILE:${PROJECT_NAME}>
    COMMENT "Building and running ${PROJECT_NAME}"
)