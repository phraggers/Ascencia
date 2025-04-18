cmake_minimum_required(VERSION 3.14)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Ship>:Release>")

# === DEPS ===

# zlib
# SDL3
# SDL3_image
# SDL3_ttf
# SDL3_net
# PhysFS
# GLM
# OpenAL TODO
# imgui TODO

function(FetchDependency name url tag)
    if(NOT EXISTS "${CMAKE_SOURCE_DIR}/deps/${name}")
        execute_process(
            COMMAND git clone --branch ${tag} --single-branch --recurse-submodules -j8 ${url} deps/${name}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            RESULT_VARIABLE DepsResult_${name}
        )
        if(NOT DepsResult_${name} EQUAL 0)
            message(FATAL_ERROR "Failed to get ${name}")
        endif()
    else()
        execute_process(
            COMMAND git pull
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/deps/${name}
        )
endif()
endfunction()

# zlib
FetchDependency(zlib https://github.com/madler/zlib.git v1.3.1)
set(ZLIB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/deps/zlib CACHE PATH "zlib include dir")
set(ZLIB_LIBRARY zlibstatic CACHE FILEPATH "zlib library")
set(-DZLIB_LIBRARY zlibstatic)
add_subdirectory(deps/zlib)

# SDL3
FetchDependency(SDL https://github.com/libsdl-org/sdl release-3.2.10)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries")
set(SDL_SHARED OFF CACHE BOOL "Disable SDL shared library")
set(SDL_STATIC ON CACHE BOOL "Enable SDL static library")
add_subdirectory(deps/SDL)

# SDL3_image
FetchDependency(SDL_image https://github.com/libsdl-org/SDL_image release-3.2.4)
set(SDL3IMAGE_SHARED OFF CACHE BOOL "Disable SDL3_image shared library")
set(SDL3IMAGE_STATIC ON CACHE BOOL "Enable SDL3_image static library")
add_subdirectory(deps/SDL_image)

# SDL3_ttf
FetchDependency(SDL_ttf https://github.com/libsdl-org/SDL_ttf release-3.2.2)
set(SDL3TTF_SHARED OFF CACHE BOOL "Disable SDL3_ttf shared library")
set(SDL3TTF_STATIC ON CACHE BOOL "Enable SDL3_ttf static library")
add_subdirectory(deps/SDL_ttf)

# SDL3_net
FetchDependency(SDL_net https://github.com/libsdl-org/SDL_net main) # check for when tag v3+ is released
set(SDL3NET_SHARED OFF CACHE BOOL "Disable SDL3_net shared library")
set(SDL3NET_STATIC ON CACHE BOOL "Enable SDL3_net static library")
add_subdirectory(deps/SDL_net)

# PhysFS
FetchDependency(PhysFS https://github.com/icculus/physfs release-3.2.0)
add_subdirectory(deps/PhysFS)

# GLM
FetchDependency(GLM https://github.com/g-truc/glm 1.0.1)
set(-DGLM_BUILD_TESTS=OFF)
set(-DBUILD_SHARED_LIBS=OFF)
add_subdirectory(deps/GLM)

# === ASCENCIA ===

project(Ascencia)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE ASCENCIA_SRC CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/ascencia/*.cpp")

if(WIN32)

    if(MSVC)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    endif()

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_executable(${PROJECT_NAME} "${ASCENCIA_SRC}")
    else()
        add_executable(${PROJECT_NAME} WIN32 "${ASCENCIA_SRC}")
    endif()

else()
    add_executable(${PROJECT_NAME} "${ASCENCIA_SRC}")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data/")
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data/")
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Ship")
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DATA_PATH="./data/")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    zlibstatic
    SDL3-static
    SDL3_image-static
    SDL3_ttf-static 
    SDL3_net-static 
    physfs-static
	glm::glm-header-only
)

# === POST BUILD ===

if(WIN32)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/util/rh/rh.exe" -open "$<TARGET_FILE:${PROJECT_NAME}>" -resource "${CMAKE_CURRENT_SOURCE_DIR}/util/rh/Ascencia.ico" -mask ICONGROUP,MAINICON, -action addskip -save "$<TARGET_FILE:${PROJECT_NAME}>"
        COMMAND "${CMAKE_COMMAND}" -E echo "Post-build steps completed."
        VERBATIM
        )

    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/util/rh/rh.exe" -open "$<TARGET_FILE:${PROJECT_NAME}>" -resource "${CMAKE_CURRENT_SOURCE_DIR}/util/rh/Ascencia.ico" -mask ICONGROUP,MAINICON, -action addskip -save "$<TARGET_FILE:${PROJECT_NAME}>"
        COMMAND "${CMAKE_COMMAND}" -E echo "Post-build steps completed."
        VERBATIM
        )

    elseif(CMAKE_BUILD_TYPE STREQUAL "Ship")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/data" $<TARGET_FILE_DIR:Ascencia/data>
        COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/util/rh/rh.exe" -open "$<TARGET_FILE:${PROJECT_NAME}>" -resource "${CMAKE_CURRENT_SOURCE_DIR}/util/rh/Ascencia.ico" -mask ICONGROUP,MAINICON, -action addskip -save "$<TARGET_FILE:${PROJECT_NAME}>"
        COMMAND "${CMAKE_COMMAND}" -E echo "Post-build steps completed."
        VERBATIM
        )

    endif()
elseif(APPLE)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E echo "Post-build steps completed."
        VERBATIM
        )

    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E echo "Post-build steps completed."
        VERBATIM
        )

    elseif(CMAKE_BUILD_TYPE STREQUAL "Ship")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/data" $<TARGET_FILE_DIR:Ascencia/data>
        COMMAND "${CMAKE_COMMAND}" -E echo "Post-build steps completed."
        VERBATIM
        )

    endif()
elseif(UNIX)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E echo "Post-build steps completed."
        VERBATIM
        )

    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E echo "Post-build steps completed."
        VERBATIM
        )

    elseif(CMAKE_BUILD_TYPE STREQUAL "Ship")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/data" $<TARGET_FILE_DIR:Ascencia/data>
        COMMAND "${CMAKE_COMMAND}" -E echo "Post-build steps completed."
        VERBATIM
        )

    endif()
endif()