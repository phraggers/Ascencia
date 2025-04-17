cmake_minimum_required(VERSION 3.16)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Ship>:Release>")

# === DEPS ===

# SDL3
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/deps/SDL")
    message(STATUS "Getting dependecy SDL...")
    execute_process(
        COMMAND git clone --recurse-submodules -j8 https://github.com/libsdl-org/sdl deps/SDL
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE DepsSDLResult
    )
    if(NOT DepsSDLResult EQUAL 0)
        message(FATAL_ERROR "Failed to get dependency SDL")
    endif()
else()
    message(STATUS "Updating SDL...")
    execute_process(
        COMMAND git pull
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/deps/SDL
    )
endif()

# SDL3_image
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/deps/SDL_image")
    message(STATUS "Getting dependency SDL_image...")
    execute_process(
        COMMAND git clone --recurse-submodules -j8 https://github.com/libsdl-org/SDL_image deps/SDL_image
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE DepsSDLimageResult
    )
    if(NOT DepsSDLimageResult EQUAL 0)
        message(FATAL_ERROR "Failed to get dependency SDL_image")
    endif()
else()
    message(STATUS "Updating SDL_image...")
    execute_process(
        COMMAND git pull
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/deps/SDL_image
    )
endif()

# SDL3_ttf
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/deps/SDL_ttf")
    message(STATUS "Getting dependency SDL_ttf...")
    execute_process(
        COMMAND git clone --recurse-submodules -j8 https://github.com/libsdl-org/SDL_ttf deps/SDL_ttf
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE DepsSDLttfResult
    )
    if(NOT DepsSDLttfResult EQUAL 0)
        message(FATAL_ERROR "Failed to get dependency SDL_ttf")
    endif()
else()
    message(STATUS "Updating SDL_ttf...")
    execute_process(
        COMMAND git pull
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/deps/SDL_ttf
    )
endif()

# SDL3_net
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/deps/SDL_net")
    message(STATUS "Getting dependency SDL_net...")
    execute_process(
        COMMAND git clone --recurse-submodules -j8 https://github.com/libsdl-org/SDL_net deps/SDL_net
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE DepsSDLnetResult
    )
    if(NOT DepsSDLnetResult EQUAL 0)
        message(FATAL_ERROR "Failed to get dependency SDL_net")
    endif()
else()
    message(STATUS "Updating SDL_net...")
    execute_process(
        COMMAND git pull
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/deps/SDL_net
    )
endif()

# GLM
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/deps/GLM")
    message(STATUS "Getting dependency GLM...")
    execute_process(
        COMMAND git clone https://github.com/g-truc/glm deps/GLM
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE DepsGLMResult
    )
    if(NOT DepsGLMResult EQUAL 0)
        message(FATAL_ERROR "Failed to get dependency GLM")
    endif()
else()
    message(STATUS "Updating GLM...")
    execute_process(
        COMMAND git pull
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/deps/GLM
    )
endif()

# PhysFS
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/deps/PhysFS")
    message(STATUS "Getting dependency PhysFS...")
    execute_process(
        COMMAND git clone https://github.com/icculus/physfs deps/PhysFS
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE DepsPhysFSResult
    )
    if(NOT DepsPhysFSResult EQUAL 0)
        message(FATAL_ERROR "Failed to get dependency PhysFS")
    endif()
else()
    message(STATUS "Updating PhysFS...")
    execute_process(
        COMMAND git pull
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/deps/PhysFS
    )
endif()

add_subdirectory(deps/SDL)
add_subdirectory(deps/SDL_image)
add_subdirectory(deps/SDL_ttf)
add_subdirectory(deps/SDL_net)
add_subdirectory(deps/GLM)
add_subdirectory(deps/PhysFS)

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

target_include_directories(Ascencia PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

target_link_libraries(Ascencia PRIVATE 
    SDL3::SDL3 
    SDL3_image::SDL3_image 
    SDL3_ttf::SDL3_ttf 
    SDL3_net::SDL3_net 
    glm 
    physfs-static
)

# === POST BUILD ===

if(WIN32)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:Ascencia>
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3_image::SDL3_image-shared> $<TARGET_FILE_DIR:Ascencia>
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3_ttf::SDL3_ttf-shared> $<TARGET_FILE_DIR:Ascencia>
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3_net::SDL3_net-shared> $<TARGET_FILE_DIR:Ascencia>
        COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/scripts/rh.bat" "$<TARGET_FILE:${PROJECT_NAME}>"
        COMMAND "${CMAKE_COMMAND}" -E echo "Post-build steps completed."
        VERBATIM
        )

    elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:Ascencia>
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3_image::SDL3_image-shared> $<TARGET_FILE_DIR:Ascencia>
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3_ttf::SDL3_ttf-shared> $<TARGET_FILE_DIR:Ascencia>
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3_net::SDL3_net-shared> $<TARGET_FILE_DIR:Ascencia>
        COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/scripts/rh.bat" "$<TARGET_FILE:${PROJECT_NAME}>"
        COMMAND "${CMAKE_COMMAND}" -E echo "Post-build steps completed."
        VERBATIM
        )

    elseif(CMAKE_BUILD_TYPE STREQUAL "Ship")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3::SDL3-shared> $<TARGET_FILE_DIR:Ascencia>
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3_image::SDL3_image-shared> $<TARGET_FILE_DIR:Ascencia>
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3_ttf::SDL3_ttf-shared> $<TARGET_FILE_DIR:Ascencia>
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_FILE:SDL3_net::SDL3_net-shared> $<TARGET_FILE_DIR:Ascencia>
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/data" $<TARGET_FILE_DIR:Ascencia/data>
        COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/scripts/rh.bat" "$<TARGET_FILE:${PROJECT_NAME}>"
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