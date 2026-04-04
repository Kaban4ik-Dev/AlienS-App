cmake_minimum_required(VERSION 3.15)
project(alien-s VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================
# Настройка выходных директорий
# ============================================
# Устанавливаем базовую директорию для артефактов
if(NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endif()
if(NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
endif()
if(NOT DEFINED CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
endif()

# Для мульти-конфигурационных генераторов (Visual Studio)
foreach(OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUT_CONFIG} OUTPUT_CONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG_UPPER} "${CMAKE_BINARY_DIR}/bin/${OUTPUT_CONFIG}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUT_CONFIG_UPPER} "${CMAKE_BINARY_DIR}/lib/${OUTPUT_CONFIG}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUT_CONFIG_UPPER} "${CMAKE_BINARY_DIR}/lib/${OUTPUT_CONFIG}")
endforeach()

# ============================================
# Поиск зависимостей
# ============================================
find_package(Vulkan REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(glm REQUIRED)
find_package(pugixml CONFIG REQUIRED)

# [WEBUI] Настройка WebUI
set(WEBUI_PATH "C:/Users/admin/webui" CACHE PATH "Path to WebUI library")
set(WEBUI_INCLUDE_DIR "${WEBUI_PATH}/include")
set(WEBUI_LIB_DIR "${WEBUI_PATH}/dist")

if(NOT EXISTS ${WEBUI_INCLUDE_DIR})
    message(FATAL_ERROR "WebUI include directory not found: ${WEBUI_INCLUDE_DIR}")
endif()
if(NOT EXISTS ${WEBUI_LIB_DIR})
    message(FATAL_ERROR "WebUI library directory not found: ${WEBUI_LIB_DIR}")
endif()

# Определяем тип библиотеки WebUI
if(WIN32)
    if(EXISTS "${WEBUI_LIB_DIR}/webui-2-static.lib")
        set(WEBUI_LIB_NAME "webui-2-static")
        set(WEBUI_LIB_TYPE "static")
    elseif(EXISTS "${WEBUI_LIB_DIR}/webui-2.lib")
        set(WEBUI_LIB_NAME "webui-2")
        set(WEBUI_LIB_TYPE "dynamic")
    else()
        message(FATAL_ERROR "WebUI library not found")
    endif()
else()
    if(EXISTS "${WEBUI_LIB_DIR}/libwebui-2-static.a")
        set(WEBUI_LIB_NAME "webui-2-static")
        set(WEBUI_LIB_TYPE "static")
    elseif(EXISTS "${WEBUI_LIB_DIR}/libwebui-2.so")
        set(WEBUI_LIB_NAME "webui-2")
        set(WEBUI_LIB_TYPE "dynamic")
    else()
        message(FATAL_ERROR "WebUI library not found")
    endif()
endif()

message(STATUS "[WEBUI] Using ${WEBUI_LIB_NAME} (${WEBUI_LIB_TYPE})")

# ============================================
# Компиляция шейдеров
# ============================================
function(compile_vulkan_shaders)
    set(SHADER_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders/not_compiled")
    set(SHADER_DST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders/compiled")
    
    if(NOT EXISTS ${SHADER_SRC_DIR})
        message(WARNING "Shader source directory not found: ${SHADER_SRC_DIR}")
        return()
    endif()
    
    file(MAKE_DIRECTORY ${SHADER_DST_DIR})
    
    file(GLOB SHADER_FILES 
        "${SHADER_SRC_DIR}/*.vert" "${SHADER_SRC_DIR}/*.frag"
        "${SHADER_SRC_DIR}/*.comp" "${SHADER_SRC_DIR}/*.geom"
        "${SHADER_SRC_DIR}/*.glsl"
    )
    
    if(NOT SHADER_FILES)
        message(STATUS "No shaders found")
        return()
    endif()
    
    find_program(GLSLC glslc)
    if(NOT GLSLC)
        message(WARNING "glslc not found! Shaders will not be compiled.")
        return()
    endif()
    
    set(COMPILED_SHADERS "")
    foreach(SHADER_FILE ${SHADER_FILES})
        get_filename_component(SHADER_NAME ${SHADER_FILE} NAME_WE)
        get_filename_component(SHADER_EXT ${SHADER_FILE} EXT)
        set(OUTPUT_FILE "${SHADER_DST_DIR}/${SHADER_NAME}${SHADER_EXT}.spv")
        
        add_custom_command(
            OUTPUT ${OUTPUT_FILE}
            COMMAND ${GLSLC} ${SHADER_FILE} -o ${OUTPUT_FILE}
            DEPENDS ${SHADER_FILE}
            COMMENT "Compiling shader: ${SHADER_NAME}${SHADER_EXT}"
        )
        list(APPEND COMPILED_SHADERS ${OUTPUT_FILE})
    endforeach()
    
    add_custom_target(compile_shaders ALL DEPENDS ${COMPILED_SHADERS})
    set(COMPILED_SHADERS ${COMPILED_SHADERS} PARENT_SCOPE)
    list(LENGTH SHADER_FILES SHADER_COUNT)
    message(STATUS "Found ${SHADER_COUNT} shaders to compile")
endfunction()

# ============================================
# Сборка исходного кода
# ============================================
file(GLOB_RECURSE SOURCE_FILES
    "src/*.cpp"
    "src/*.hpp"
    "src/*.h"
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Включаемые директории
target_include_directories(${PROJECT_NAME} PRIVATE
    ${Vulkan_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${WEBUI_INCLUDE_DIR}
)

# Компиляция шейдеров
compile_vulkan_shaders()
if(COMPILED_SHADERS)
    add_dependencies(${PROJECT_NAME} compile_shaders)
    
    # Копирование шейдеров в выходную директорию
    foreach(SPV_SHADER ${COMPILED_SHADERS})
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SPV_SHADER}
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/
        )
    endforeach()
endif()

# ============================================
# Линковка
# ============================================
target_link_directories(${PROJECT_NAME} PRIVATE ${WEBUI_LIB_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE 
    Vulkan::Vulkan
    SDL2::SDL2
    glm::glm
    pugixml::pugixml
    ${WEBUI_LIB_NAME}
)

# Дополнительные библиотеки для Windows
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ws2_32 user32 shell32 ole32
    )
endif()

# ============================================
# Копирование ресурсов
# ============================================
# Копирование UI HTML файлов
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove_directory "$<TARGET_FILE_DIR:${PROJECT_NAME}>/ui_html"
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ui_html" 
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/ui_html"
    COMMENT "Copying UI resources"
)

# Копирование DLL для динамической линковки
if(WIN32 AND WEBUI_LIB_TYPE STREQUAL "dynamic")
    set(WEBUI_DLL_NAME "${WEBUI_LIB_NAME}.dll")
    if(EXISTS "${WEBUI_LIB_DIR}/${WEBUI_DLL_NAME}")
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${WEBUI_LIB_DIR}/${WEBUI_DLL_NAME}"
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying WebUI DLL"
        )
    endif()
endif()

# ============================================
# Настройки компилятора
# ============================================
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/O2 /fp:fast /MT>
        $<$<CONFIG:Debug>:/Od /Zi /RTC1 /MTd>
        /wd26495
    )
    
    # Отключаем предупреждения для внешних библиотек
    target_compile_options(${PROJECT_NAME} PRIVATE
        /wd4244 /wd4305 /wd4996
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:-O3 -march=native -DNDEBUG>
        $<$<CONFIG:Debug>:-O0 -g3 -DDEBUG>
    )
endif()

# ============================================
# Информация о сборке
# ============================================
message(STATUS "========================================")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "========================================")