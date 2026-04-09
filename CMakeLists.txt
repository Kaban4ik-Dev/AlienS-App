cmake_minimum_required(VERSION 3.15)
project(alien-s VERSION 0.0.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================
# Настройка выходных директорий
# ============================================
# Определяем корневую директорию для сборки
set(APP_ROOT_DIR "${CMAKE_BINARY_DIR}/AlienS App")

# Настройка выходных директорий для исполняемых файлов и библиотек
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${APP_ROOT_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${APP_ROOT_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${APP_ROOT_DIR}/lib")

# Для мульти-конфигурационных генераторов (Visual Studio)
foreach(OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUT_CONFIG} OUTPUT_CONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG_UPPER} "${APP_ROOT_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUT_CONFIG_UPPER} "${APP_ROOT_DIR}/bin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUT_CONFIG_UPPER} "${APP_ROOT_DIR}/lib")
endforeach()

# ============================================
# Поиск зависимостей
# ============================================
find_package(pugixml CONFIG REQUIRED)
find_package(SFML COMPONENTS Graphics Window System CONFIG REQUIRED)
find_package(TGUI CONFIG REQUIRED)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ============================================
# Линковка
# ============================================
target_link_libraries(${PROJECT_NAME} PRIVATE 
    pugixml::pugixml
    SFML::Graphics 
    SFML::Window 
    SFML::System
    TGUI::TGUI
)

# Дополнительные библиотеки для Windows
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ws2_32 user32 shell32 ole32
    )
endif()

# ============================================
# Копирование DLL в папку bin
# ============================================
# Функция для копирования DLL
function(copy_dll_to_bin dll_name)
    # Пытаемся найти DLL в стандартных местах
    find_file(DLL_PATH
        NAMES ${dll_name}
        PATHS
            ${CMAKE_PREFIX_PATH}/bin
            ${CMAKE_PREFIX_PATH}/lib
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}/..
        PATH_SUFFIXES bin lib
        NO_DEFAULT_PATH
    )
    
    if(DLL_PATH)
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${DLL_PATH}"
                "${APP_ROOT_DIR}/bin/"
            COMMENT "Copying ${dll_name} to ${APP_ROOT_DIR}/bin/"
        )
    else()
        message(STATUS "Warning: ${dll_name} not found, will not be copied")
    endif()
endfunction()

# Список DLL для копирования (если они есть в системе)
if(WIN32)
    # Копируем DLL из текущей директории (если они там есть)
    file(GLOB DLL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.dll")
    foreach(DLL_FILE ${DLL_FILES})
        get_filename_component(DLL_NAME ${DLL_FILE} NAME)
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${DLL_FILE}"
                "${APP_ROOT_DIR}/bin/"
            COMMENT "Copying ${DLL_NAME} to bin/"
        )
    endforeach()
endif()

# ============================================
# Создание структуры папок
# ============================================

# Функция для копирования папки
function(copy_folder src_folder dest_folder)
    if(EXISTS "${CMAKE_SOURCE_DIR}/${src_folder}")
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${APP_ROOT_DIR}/${dest_folder}"
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_SOURCE_DIR}/${src_folder}"
                "${APP_ROOT_DIR}/${dest_folder}"
            COMMENT "Copying ${src_folder} to ${APP_ROOT_DIR}/${dest_folder}"
        )
    endif()
endfunction()

# Функция для создания пустой папки
function(ensure_directory dir_path)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${APP_ROOT_DIR}/${dir_path}"
        COMMENT "Creating directory ${APP_ROOT_DIR}/${dir_path}"
    )
endfunction()

# Копирование ассетов
copy_folder(assets assets)
# Копирование курсов
copy_folder(courses/example courses/example)
# Копирование настроек
copy_folder(data/config data/config)
copy_folder(data/user data/user)

# Создаём остальные папки
ensure_directory(courses/download)
ensure_directory(courses/local)
ensure_directory(data/code/temp_files)
ensure_directory(data/code/c_cpp)
ensure_directory(data/code/python)
ensure_directory(data/code/verilog)

# ============================================
# Копирование файлов из корня (если есть)
# ============================================
# Копируем .dll и .pdb файлы из корня проекта в bin/
file(GLOB ROOT_DLL_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.dll")
file(GLOB ROOT_PDB_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.pdb")

foreach(DLL_FILE ${ROOT_DLL_FILES})
    get_filename_component(DLL_NAME ${DLL_FILE} NAME)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DLL_FILE}"
            "${APP_ROOT_DIR}/bin/"
        COMMENT "Copying ${DLL_NAME} to bin/"
    )
endforeach()

foreach(PDB_FILE ${ROOT_PDB_FILES})
    get_filename_component(PDB_NAME ${PDB_FILE} NAME)
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${PDB_FILE}"
            "${APP_ROOT_DIR}/bin/"
        COMMENT "Copying ${PDB_NAME} to bin/"
    )
endforeach()

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
message(STATUS "App root directory: ${APP_ROOT_DIR}")
message(STATUS "========================================")