cmake_minimum_required(VERSION 3.15)
project(alien-s)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Поиск пакетов
find_package(Vulkan REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(glm REQUIRED)
#find_package(Stb REQUIRED)
find_package(pugixml CONFIG REQUIRED)

# [WEBUI] Указываем путь к собранной библиотеке WebUI
set(WEBUI_PATH "C:/Users/admin/webui" CACHE PATH "Path to WebUI library")

# [WEBUI] Добавляем пути для поиска заголовочных файлов и библиотек
set(WEBUI_INCLUDE_DIR "${WEBUI_PATH}/include")
set(WEBUI_LIB_DIR "${WEBUI_PATH}/dist" CACHE PATH "Path to WebUI libraries")

# [WEBUI] Проверяем существование папок
if(NOT EXISTS ${WEBUI_INCLUDE_DIR})
    message(FATAL_ERROR "WebUI include directory not found: ${WEBUI_INCLUDE_DIR}")
endif()

if(NOT EXISTS ${WEBUI_LIB_DIR})
    message(FATAL_ERROR "WebUI library directory not found: ${WEBUI_LIB_DIR}")
endif()

# [WEBUI] Определяем тип сборки и имя библиотеки
if(WIN32)
    # Для Windows определяем, какая версия библиотеки доступна
    if(EXISTS "${WEBUI_LIB_DIR}/webui-2-static.lib")
        set(WEBUI_LIB_NAME "webui-2-static")
        set(WEBUI_LIB_TYPE "static")
    elseif(EXISTS "${WEBUI_LIB_DIR}/webui-2.lib")
        set(WEBUI_LIB_NAME "webui-2")
        set(WEBUI_LIB_TYPE "dynamic")
    elseif(EXISTS "${WEBUI_LIB_DIR}/webui-2-secure-static.lib")
        set(WEBUI_LIB_NAME "webui-2-secure-static")
        set(WEBUI_LIB_TYPE "static")
    elseif(EXISTS "${WEBUI_LIB_DIR}/webui-2-secure.lib")
        set(WEBUI_LIB_NAME "webui-2-secure")
        set(WEBUI_LIB_TYPE "dynamic")
    else()
        message(FATAL_ERROR "WebUI library not found in ${WEBUI_LIB_DIR}")
    endif()
else()
    # Для Linux/macOS
    if(EXISTS "${WEBUI_LIB_DIR}/libwebui-2-static.a")
        set(WEBUI_LIB_NAME "webui-2-static")
        set(WEBUI_LIB_TYPE "static")
    elseif(EXISTS "${WEBUI_LIB_DIR}/libwebui-2.so" OR EXISTS "${WEBUI_LIB_DIR}/libwebui-2.dylib")
        set(WEBUI_LIB_NAME "webui-2")
        set(WEBUI_LIB_TYPE "dynamic")
    else()
        message(FATAL_ERROR "WebUI library not found in ${WEBUI_LIB_DIR}")
    endif()
endif()

message(STATUS "[WEBUI] Found library: ${WEBUI_LIB_NAME} (${WEBUI_LIB_TYPE})")

# Функция для компиляции шейдеров Vulkan
function(compile_vulkan_shaders)
    # Установка путей к папкам с шейдерами
    set(SHADER_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders/not_compiled")
    set(SHADER_DST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders/compiled")
    
    # Проверка существования исходной директории
    if(NOT EXISTS ${SHADER_SRC_DIR})
        message(WARNING "Source shader directory not found: ${SHADER_SRC_DIR}")
        return()
    endif()
    
    # Создание целевой директории для скомпилированных шейдеров
    file(MAKE_DIRECTORY ${SHADER_DST_DIR})
    
    # Поиск всех файлов шейдеров в исходной директории
    file(GLOB SHADER_FILES 
        "${SHADER_SRC_DIR}/*.vert"
        "${SHADER_SRC_DIR}/*.frag" 
        "${SHADER_SRC_DIR}/*.comp"
        "${SHADER_SRC_DIR}/*.geom"
        "${SHADER_SRC_DIR}/*.tesc"
        "${SHADER_SRC_DIR}/*.tese"
        "${SHADER_SRC_DIR}/*.rgen"
        "${SHADER_SRC_DIR}/*.rint"
        "${SHADER_SRC_DIR}/*.rahit"
        "${SHADER_SRC_DIR}/*.rchit"
        "${SHADER_SRC_DIR}/*.rmiss"
        "${SHADER_SRC_DIR}/*.rcall"
        "${SHADER_SRC_DIR}/*.mesh"
        "${SHADER_SRC_DIR}/*.task"
        "${SHADER_SRC_DIR}/*.glsl"
    )
    
    # Проверка наличия компилятора
    find_program(GLSLC glslc)
    if(NOT GLSLC)
        # Пробуем найти glslangValidator как запасной вариант
        find_program(GLSLC glslangValidator)
        if(NOT GLSLC)
            message(WARNING "glslc/glslangValidator not found! Vulkan shaders will not be compiled.")
            return()
        endif()
        message(STATUS "Using glslangValidator for shader compilation")
        set(GLSLC_ARGS "-V")  # Аргументы для glslangValidator
    else()
        message(STATUS "Using glslc for shader compilation")
        set(GLSLC_ARGS "")    # Аргументы для glslc
    endif()
    
    # Если шейдеры не найдены
    if(NOT SHADER_FILES)
        message(STATUS "No shaders found in ${SHADER_SRC_DIR}")
        return()
    endif()
    
    # Список для хранения скомпилированных шейдеров
    set(COMPILED_SHADERS "")
    
    # Для каждого найденного файла шейдера
    foreach(SHADER_FILE ${SHADER_FILES})
        # Получаем имя файла без пути
        get_filename_component(SHADER_NAME_EXT ${SHADER_FILE} NAME)
        
        # Разделяем имя файла и расширение
        get_filename_component(SHADER_NAME ${SHADER_FILE} NAME_WE)
        get_filename_component(SHADER_EXT ${SHADER_FILE} EXT)
        
        # Определяем выходной файл с сохранением исходного имени и расширения
        set(OUTPUT_FILE "${SHADER_DST_DIR}/${SHADER_NAME}${SHADER_EXT}.spv")
        
        # Создаем команду компиляции
        if(GLSLC_ARGS STREQUAL "-V")
            # Используем glslangValidator
            add_custom_command(
                OUTPUT ${OUTPUT_FILE}
                COMMAND ${GLSLC} ${GLSLC_ARGS} ${SHADER_FILE} -o ${OUTPUT_FILE}
                DEPENDS ${SHADER_FILE}
                COMMENT "Compiling ${SHADER_NAME_EXT} to ${SHADER_NAME}${SHADER_EXT}.spv"
                VERBATIM
            )
        else()
            # Используем glslc с автоматическим определением стадии
            add_custom_command(
                OUTPUT ${OUTPUT_FILE}
                COMMAND ${GLSLC} ${SHADER_FILE} -o ${OUTPUT_FILE}
                DEPENDS ${SHADER_FILE}
                COMMENT "Compiling ${SHADER_NAME_EXT} to ${SHADER_NAME}${SHADER_EXT}.spv"
                VERBATIM
            )
        endif()
        
        # Добавляем выходной файл в список
        list(APPEND COMPILED_SHADERS ${OUTPUT_FILE})
        
    endforeach()
    
    # Создаем цель для компиляции всех шейдеров
    add_custom_target(compile_shaders ALL DEPENDS ${COMPILED_SHADERS})
    
    # Добавляем зависимость для основного таргета
    add_dependencies(${PROJECT_NAME} compile_shaders)
    
    # Возвращаем список скомпилированных шейдеров в родительскую область видимости
    set(COMPILED_SHADERS ${COMPILED_SHADERS} PARENT_SCOPE)
    
    # Сообщаем о количестве найденных шейдеров
    list(LENGTH SHADER_FILES SHADER_COUNT)
    message(STATUS "Found ${SHADER_COUNT} shaders to compile from ${SHADER_SRC_DIR}")
    
endfunction()

# Рекурсивно найти все .cpp и .h файлы в src и подпапках
file(GLOB_RECURSE SOURCE_FILES
    "src/*.cpp"
    "src/*.hpp"
    "src/*.h"
)

# Создание исполняемого файла
add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
)

# Включаемые директории
target_include_directories(${PROJECT_NAME} PRIVATE
    ${Vulkan_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/include  # Добавляем папку include
    ${CMAKE_CURRENT_SOURCE_DIR}/src      # Добавляем папку src
    ${Stb_INCLUDE_DIR}
    # [WEBUI] Добавляем путь к заголовочным файлам WebUI
    ${WEBUI_INCLUDE_DIR}
)

# Вызов функции компиляции шейдеров
compile_vulkan_shaders()

# Получаем список скомпилированных шейдеров (если есть)
if(COMPILED_SHADERS)
    set(SPV_SHADERS ${COMPILED_SHADERS})
endif()

# Копирование скомпилированных шейдеров в выходную директорию
if(SPV_SHADERS)
    # Создаем папку для шейдеров в выходной директории
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
    )
    
    # Копируем каждый скомпилированный шейдер
    foreach(SPV_SHADER ${SPV_SHADERS})
        get_filename_component(SHADER_FILENAME ${SPV_SHADER} NAME)
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${SPV_SHADER}
                $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders/${SHADER_FILENAME}
            COMMENT "Copying ${SHADER_FILENAME} to output directory"
        )
    endforeach()
endif()

# Настройки компилятора для разных конфигураций
if(MSVC)
    # Настройки для MSVC
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/O2 /fp:fast>
        $<$<CONFIG:Debug>:/Od /Zi /RTC1>
    )
    
    # Настройка runtime библиотек
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
    )
    
    # Отключение конкретных предупреждений C++
    target_compile_options(${PROJECT_NAME} PRIVATE
        /wd26495  # Отключение предупреждения о неинициализированных переменных
    )
else()
    # Настройки для других компиляторов (GCC, Clang)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:-O3 -march=native>
        $<$<CONFIG:Debug>:-O0 -g3>
    )
endif()

# Выходные директории
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Отладочная информация
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG _DEBUG)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE NDEBUG _NDEBUG)
endif()

# [WEBUI] Добавляем путь к библиотекам для линковщика
target_link_directories(${PROJECT_NAME} PRIVATE ${WEBUI_LIB_DIR})

# Линковка
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Vulkan::Vulkan
    SDL2::SDL2
    glm::glm
    pugixml::pugixml
    # [WEBUI] Линковка с WebUI
    ${WEBUI_LIB_NAME}
)

# [WEBUI] Добавляем дополнительные системные библиотеки для Windows
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ws2_32
        user32
        shell32
        ole32
    )
endif()

# [WEBUI] Копирование DLL файлов WebUI в выходную директорию (только для Windows динамическая сборка)
if(WIN32 AND WEBUI_LIB_TYPE STREQUAL "dynamic")
    # Ищем DLL файл
    set(WEBUI_DLL_NAME "${WEBUI_LIB_NAME}.dll")
    if(EXISTS "${WEBUI_LIB_DIR}/${WEBUI_DLL_NAME}")
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${WEBUI_LIB_DIR}/${WEBUI_DLL_NAME}"
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying ${WEBUI_DLL_NAME} to output directory"
        )
    else()
        # Пробуем найти без суффикса static
        string(REPLACE "-static" "" WEBUI_DLL_BASE_NAME ${WEBUI_LIB_NAME})
        set(WEBUI_DLL_NAME "${WEBUI_DLL_BASE_NAME}.dll")
        if(EXISTS "${WEBUI_LIB_DIR}/${WEBUI_DLL_NAME}")
            add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${WEBUI_LIB_DIR}/${WEBUI_DLL_NAME}"
                    $<TARGET_FILE_DIR:${PROJECT_NAME}>
                COMMENT "Copying ${WEBUI_DLL_NAME} to output directory"
            )
        else()
            message(WARNING "WebUI DLL not found. Make sure to copy it manually to the output directory.")
        endif()
    endif()
endif()

# Просто используем директорию исполняемого файла
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -rf "$<TARGET_FILE_DIR:${PROJECT_NAME}>/ui_html"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/src/ui_html" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/ui_html"
    COMMENT "Copying ui_html to build folder"
)

# Отладочный вывод информации о настройках
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Vulkan include dirs: ${Vulkan_INCLUDE_DIRS}")
message(STATUS "Vulkan libraries found: ${Vulkan_FOUND}")
message(STATUS "GLM libraries found: ${glm_FOUND}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Generator: ${CMAKE_GENERATOR}")

# [WEBUI] Отладочная информация о WebUI
message(STATUS "[WEBUI] Include directory: ${WEBUI_INCLUDE_DIR}")
message(STATUS "[WEBUI] Library directory: ${WEBUI_LIB_DIR}")
message(STATUS "[WEBUI] Library name: ${WEBUI_LIB_NAME}")
message(STATUS "[WEBUI] Library type: ${WEBUI_LIB_TYPE}")

# Предупреждение, если используется неправильная конфигурация
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(WARNING "No build type specified, defaulting to Release")
endif()