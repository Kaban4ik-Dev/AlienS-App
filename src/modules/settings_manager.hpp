#pragma once

#include <string>
#include <variant>
#include <vector>
#include <unordered_map>
#include <optional>
#include <iostream>
#include <system_error>

#include <pugixml.hpp>

// Класс-синглтон для управления настройками
class SettingsManager
{
private:

	// Тип поля настройки
	using SettingValueType = std::variant<bool, int, float, std::string>;

	// Перечисление способов валидации
	enum ValidationType :uint8_t
	{
		NoValidation,		// Без валидации - bool
		IsRanged,			// Ограничено диапазоном - int, float
		HasAcceptableValues	// Есть список значений - int, float, string
	};

	// Структура настройки
	struct Setting
	{
		std::string name;									// Имя настройки
		SettingValueType value;								// Значение
		SettingValueType min_value;							// Минимальное значение
		SettingValueType max_value;							// Максимальное значение
		std::vector<SettingValueType> acceptable_values;	// Список приемлемых значений
		ValidationType type;								// Тип ограничения для валидации
		bool has_changed;									// Отметка об изменении
	};

	std::unordered_map<std::string, std::string> changes_list; // Список изменённых настроек
	std::vector<Setting> settings; // Список настроек

	// Поля для хранения настроек запуска
	bool first_launch;
	bool skip_settings_load_errors;
	bool skip_module_load_errors;
	bool skip_course_load_errors;
	bool enable_debug_output;

	// Проверить допустимость значения настройки
	std::error_code& ValidateSettingValue(std::string setting_name, SettingValueType new_value);

public:
	SettingsManager();
	~SettingsManager() = default;

	// Получить значение настройки по имени
	SettingValueType GetSettingValue(std::string setting_name);
	// Установить значение настройки по имени
	std::error_code& SetSettingValue(std::string setting_name, SettingValueType new_value);

	// Загрузить настройки из файла
	std::error_code& LoadSettings(std::string* filename);

	// Сохранить значение настройки
	std::error_code& SaveSettingsToFile(std::string* filename, Setting setting);
};