cmake_minimum_required(VERSION 3.26.4)


project(tetrisECS)


message(STATUS "fetching external lib SFML...")
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)


file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB HEADERS src/*.h)
add_executable(tetrisECS ${SRC_FILES} ${HEADERS})
target_include_directories(tetrisECS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(tetrisECS PUBLIC sfml-graphics sfml-window sfml-system sfml-network sfml-audio)


if(WIN32)
    message(STATUS "copying dlls to target dir...")
    add_custom_command(
        TARGET tetrisECS
        COMMENT "Copy DLLs"
        POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:tetrisECS>
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:tetrisECS>/assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/bin $<TARGET_FILE_DIR:tetrisECS>
        VERBATIM
    )
endif()