#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <sstream>
#include <iomanip>

#include "webui.hpp"

#include "settings_manager.h"
#include "account_manager.h"
#include "course_manger.h"


/************************************************/
/*			Основной класс приложения			*/
/************************************************/

class AlienSApp 
{
private:
	// Окно приложения
	webui::window mainWindow;

	// Инициализация систем
	SettingsManager settings_manager;
	AccountManager account_manager;
	CourseManager course_manager;

public:
	// Конструктор класса
	AlienSApp();
	// Деструктор класса
	~AlienSApp() = default;
	// Основной цикл работы программы
	void Run();

private:

	// ========== Вспомогательные методы ==========

	//// Список курсов в JSON
	//std::string ConvertCoursesToJson() const;
	//// Тема в JSON
	//std::string ConvertTopicDataToJson(const TopicData& topic) const;
	//// Данные аккаунта в JSON
	//std::string ConvertAccountDataToJson() const;
	//// Результат выполнения кода в JSON
	//std::string ConvertCodeResultToJson() const;
	//// Код из JSON в исполняемый вид
	//std::string ConvertJsonCodeToRunnable() const;

	// ========== Навигация ==========

	void ShowMainMenu(webui::window::event* e);
	void ShowCources(webui::window::event* e);
	void ShowAccount(webui::window::event* e);
	void ShowSettings(webui::window::event* e);
	void ExitApp(webui::window::event* e);

	// ========== Обработчики событий окон ==========

	// Отладочный вывод при нажатии на любую кнопку
	void DebugOutputWindowEvent(webui::window::event* e);

	// Передать список курсов и тем
	void SendCoursesList(webui::window::event* e);
	// Передать содержимое темы
	void SendTopicContent(webui::window::event* e);
	// Получить код
	void ReceiveCode(webui::window::event* e);
	// Получить ответ на тест
	void ReceiveTestAnswer(webui::window::event* e);
	
	// Передать список настроек
	void SendSettingsList(webui::window::event* e);
	// Передать изменённую натройку
	void SendChangedSetting(webui::window::event* e);
	// Получить новые настройки
	void ReceiveSavedSettings(webui::window::event* e);
	// Передать настройки по-умолчанию
	void SendDefaultSettings(webui::window::event* e);

	// Передать данные аккаунта
	void SendAccountData(webui::window::event* e);
	// Получить данные изменённого поля
	void SendChangedField(webui::window::event* e);
	// Получить новые данные аккаунта
	void RecieveAccountData(webui::window::event* e);
};