#include <filesystem>

#include "settings_manager.hpp"
#include <pugixml.hpp>

SettingsManager::SettingsManager()
    : first_launch(true)
    , skip_settings_load_errors(true)
    , skip_module_load_errors(true)
    , skip_course_load_errors(true)
    , enable_debug_output(false)
{
    std::string config_path = "../config/setup_config.xml";

    // Проверка существования файла настроек запуска
    if (!std::filesystem::exists(config_path)) 
    {
        std::cerr << "Warning: Config file not found: " << config_path << std::endl;
        return;
    }

    // Открытие файла настроек запуска
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(config_path.c_str());
    if (!result) 
    {
        std::cerr << "Error parsing config file: " << result.description() << std::endl;
        return;
    }

    // Получение корневого элемента
    pugi::xml_node settings = doc.child("settings");
    if (!settings) 
    {
        std::cerr << "Error: No <settings> root element found" << std::endl;
        return;
    }

    // Получение узла с параметрами приложения
    pugi::xml_node app_params = settings.child("app_params");
    if (!app_params) 
    {
        std::cerr << "Error: No <app_params> element found" << std::endl;
        return;
    }

    // Считываем все настройки
    if (pugi::xml_node node = app_params.child("first_launch")) { first_launch = node.text().as_bool(); }
    if (pugi::xml_node node = app_params.child("skip_settings_load_errors")) { skip_settings_load_errors = node.text().as_bool(); }
    if (pugi::xml_node node = app_params.child("skip_module_load_errors")) { skip_module_load_errors = node.text().as_bool(); }
    if (pugi::xml_node node = app_params.child("skip_course_load_errors")) { skip_course_load_errors = node.text().as_bool(); }
    if (pugi::xml_node node = app_params.child("enable_debug_output")) { enable_debug_output = node.text().as_bool(); }
}

std::error_code& SettingsManager::LoadSettings(std::string* filename)
{
    return std::error_code(0, std::generic_category());
}

std::error_code& SettingsManager::ValidateSettingValue(std::string setting_name, SettingValueType new_value)
{
    return std::error_code(0, std::generic_category());
}

std::error_code& SettingsManager::SaveSettingsToFile(std::string* filename, Setting setting)
{
    return std::error_code(0, std::generic_category());
}