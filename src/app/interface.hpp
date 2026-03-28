#include "webui.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <sstream>
#include <iomanip>


// Структура блока в теме
struct TopicBlock 
{
	// Перечисление типов блоков в теме
	enum TopicBlockType :uint8_t
	{
		Text = 0,			// Текст
		CodeNoRun,			// Неисполняемый код
		CodeRunnable,		// Исполняемый код
		CodeResponce,		// Результат исполнения кода
		CodeFakeResponce,	// Готовый результат исполнения кода
		Image,				// Изображение
		Test				// Тест
	};
	
	TopicBlockType type;				// Тип блока
	std::string text_content;			// Текстовое содержимое блока (текст, код или вопрос)
	std::string relative_incource_path;	// Путь внутри курса (для изображений)
	std::vector<std::string> options;	// Варианты ответов (для тестов)
	uint8_t correct_option_index;		// Индекс правильного ответа (для тестов)
};

struct ABCTopic
{
	// Перечисление типов тем
	enum TopicType :bool { Lesson, Test };

	uint8_t id;						// Числовой ID темы внутри курса
	std::string title;				// Название темы
	TopicType type;					// Тип темы
};

// Структура данных темы в курсе
struct TopicData : ABCTopic
{
	std::vector<TopicBlock> blocks;	// Содержимое темы
};

// Структура метаданных темы в курсе
struct TopicMeta : ABCTopic
{
	bool unlocked = false;	// Флаг доступности темы к прохождению пользователем
	std::string file_path;	// Путь внутри курса до файла с темой
};

// Структура метаданных курса
struct CourseMeta
{
	std::string name;				// Название курса
	std::vector<TopicMeta> content;	// Список метаданных тем
};

// Структура метаданных о всех курсах
struct CoursesMeta
{
	std::vector<std::string> cources_paths;	// Пути до курсов (названия папок)
	std::vector<CourseMeta> cources_data;	// Массив метаданных обо всех курсах
};

struct UserData {
	std::string username;
	std::string email;
	std::string fullName;
	std::string bio;

	// Прогресс по курсам
	std::map<std::string, std::vector<bool>> completedTopics;  // courseId -> vector<bool>
	std::map<std::string, std::map<int, bool>> testResults;    // courseId -> (topicIndex -> passed)
};

struct AppSettings {
	// Общие
	bool darkMode = false;
	bool autoSave = true;
	bool showLineNumbers = true;

	// Редактор
	int fontSize = 14;
	std::string fontFamily = "Consolas";
	int tabSize = 4;

	// Уведомления
	bool soundEnabled = true;
	bool notificationsEnabled = true;

	// Язык
	std::string language = "ru";

	// Метод для получения всех настроек в виде JSON
	std::string toJson() const {
		std::stringstream ss;
		ss << "[";

		// Общие
		ss << "{"
			<< "\"id\":\"darkMode\","
			<< "\"type\":\"toggle\","
			<< "\"category\":\"Общие\","
			<< "\"label\":\"Тёмная тема\","
			<< "\"description\":\"Включить тёмную тему оформления\","
			<< "\"value\":" << (darkMode ? "true" : "false")
			<< "},";

		ss << "{"
			<< "\"id\":\"autoSave\","
			<< "\"type\":\"toggle\","
			<< "\"category\":\"Общие\","
			<< "\"label\":\"Автосохранение\","
			<< "\"description\":\"Автоматически сохранять изменения\","
			<< "\"value\":" << (autoSave ? "true" : "false")
			<< "},";

		ss << "{"
			<< "\"id\":\"showLineNumbers\","
			<< "\"type\":\"toggle\","
			<< "\"category\":\"Общие\","
			<< "\"label\":\"Номера строк\","
			<< "\"description\":\"Показывать номера строк в редакторе кода\","
			<< "\"value\":" << (showLineNumbers ? "true" : "false")
			<< "},";

		// Редактор
		ss << "{"
			<< "\"id\":\"fontSize\","
			<< "\"type\":\"range\","
			<< "\"category\":\"Редактор кода\","
			<< "\"label\":\"Размер шрифта\","
			<< "\"description\":\"Размер шрифта в редакторе кода\","
			<< "\"min\":8,"
			<< "\"max\":24,"
			<< "\"step\":1,"
			<< "\"unit\":\"px\","
			<< "\"value\":" << fontSize
			<< "},";

		ss << "{"
			<< "\"id\":\"fontFamily\","
			<< "\"type\":\"select\","
			<< "\"category\":\"Редактор кода\","
			<< "\"label\":\"Шрифт\","
			<< "\"description\":\"Моноширинный шрифт для редактора\","
			<< "\"options\":["
			<< "{\"value\":\"Consolas\",\"label\":\"Consolas\"},"
			<< "{\"value\":\"Courier New\",\"label\":\"Courier New\"},"
			<< "{\"value\":\"Monaco\",\"label\":\"Monaco\"},"
			<< "{\"value\":\"Fira Code\",\"label\":\"Fira Code\"}"
			<< "],"
			<< "\"value\":\"" << fontFamily << "\""
			<< "},";

		ss << "{"
			<< "\"id\":\"tabSize\","
			<< "\"type\":\"range\","
			<< "\"category\":\"Редактор кода\","
			<< "\"label\":\"Размер табуляции\","
			<< "\"description\":\"Количество пробелов в табуляции\","
			<< "\"min\":2,"
			<< "\"max\":8,"
			<< "\"step\":2,"
			<< "\"unit\":\"пробелов\","
			<< "\"value\":" << tabSize
			<< "},";

		// Уведомления
		ss << "{"
			<< "\"id\":\"soundEnabled\","
			<< "\"type\":\"toggle\","
			<< "\"category\":\"Уведомления\","
			<< "\"label\":\"Звуки\","
			<< "\"description\":\"Включить звуковые уведомления\","
			<< "\"value\":" << (soundEnabled ? "true" : "false")
			<< "},";

		ss << "{"
			<< "\"id\":\"notificationsEnabled\","
			<< "\"type\":\"toggle\","
			<< "\"category\":\"Уведомления\","
			<< "\"label\":\"Уведомления\","
			<< "\"description\":\"Показывать уведомления о событиях\","
			<< "\"value\":" << (notificationsEnabled ? "true" : "false")
			<< "},";

		// Язык
		ss << "{"
			<< "\"id\":\"language\","
			<< "\"type\":\"select\","
			<< "\"category\":\"Язык\","
			<< "\"label\":\"Язык интерфейса\","
			<< "\"description\":\"Выберите язык\","
			<< "\"options\":["
			<< "{\"value\":\"ru\",\"label\":\"Русский\"},"
			<< "{\"value\":\"en\",\"label\":\"English\"}"
			<< "],"
			<< "\"value\":\"" << language << "\""
			<< "}";

		ss << "]";
		return ss.str();
	}

	// Обновление настройки по ID
	void updateFromJson(const std::string& id, const std::string& value) {
		if (id == "darkMode") darkMode = (value == "true");
		else if (id == "autoSave") autoSave = (value == "true");
		else if (id == "showLineNumbers") showLineNumbers = (value == "true");
		else if (id == "fontSize") fontSize = std::stoi(value);
		else if (id == "fontFamily") fontFamily = value;
		else if (id == "tabSize") tabSize = std::stoi(value);
		else if (id == "soundEnabled") soundEnabled = (value == "true");
		else if (id == "notificationsEnabled") notificationsEnabled = (value == "true");
		else if (id == "language") language = value;
	}
};

// ========== Основной класс приложения ==========

class AlienSApp {
public:
	AlienSApp() {
		// Привязка методов главного меню
		mainWindow.bind("open_course", this, &AlienSApp::ShowCources);
		mainWindow.bind("show_account", this, &AlienSApp::ShowAccount);
		mainWindow.bind("show_settings", this, &AlienSApp::ShowSettings);
		mainWindow.bind("exit_app", this, &AlienSApp::ExitApp);

		// Привязка методов окна курсов
		mainWindow.bind("show_main_menu", this, &AlienSApp::ShowMainMenu);
		mainWindow.bind("execute_code", this, &AlienSApp::SubmitAndExecuteCode);
		mainWindow.bind("request_courses_list", this, &AlienSApp::GetCoursesList);
		mainWindow.bind("load_topic_content", this, &AlienSApp::onLoadTopicContent);
		mainWindow.bind("submit_test_answer", this, &AlienSApp::onSubmitTestAnswer);

		// Привязка методов окна настроек
		mainWindow.bind("go_to_main_menu", this, &AlienSApp::ShowMainMenu);
		mainWindow.bind("load_settings", this, &AlienSApp::GetSettings);
		mainWindow.bind("setting_changed", this, &AlienSApp::onSettingChanged);
		mainWindow.bind("save_settings", this, &AlienSApp::onSaveSettings);
		mainWindow.bind("settings_reset", this, &AlienSApp::onSettingsReset);

		// Привязка методов окна аккаунта
		mainWindow.bind("go_to_main_menu", this, &AlienSApp::ShowMainMenu);
		mainWindow.bind("load_account_data", this, &AlienSApp::onLoadAccountData);
		mainWindow.bind("validate_field", this, &AlienSApp::onValidateField);
		mainWindow.bind("save_account", this, &AlienSApp::onSaveAccount);
		mainWindow.bind("cancel_changes", this, &AlienSApp::onAccountCancel);

		// Привязка событий окон
		mainWindow.bind("", this, &AlienSApp::onWindowEvent);

		// Инициализация данных
		initializeDemoData();
	}

	void run() {
		std::cout << "AlienS Application Starting..." << std::endl;

		mainWindow.set_event_blocking(true);

		mainWindow.set_root_folder("ui_html");

		mainWindow.set_kiosk(true);

		mainWindow.show_browser("main-menu.html", AnyBrowser);

		// Ждем закрытия всех окон
		webui::wait();

		std::cout << "AlienS Application Finished." << std::endl;
	}

private:
	// Окна приложения
	webui::window mainWindow;

	// Данные приложения
	std::vector<CourseMeta> courses;
	UserData userData;
	AppSettings settings;

	// Временные данные для текущей сессии
	std::map<std::string, std::map<int, int>> tempTestAnswers;  // topicId -> (testIndex -> selectedOption)

	// ========== Инициализация демо-данных ==========

	void initializeDemoData() {
		// Данные пользователя
		userData.username = "student123";
		userData.email = "student@example.com";
		userData.fullName = "Иван Петров";
		userData.bio = "Начинающий разработчик, увлекаюсь C++ и Python";

		// Курс 1: Основы C++
		CourseMeta cppCourse;
		cppCourse.id = "cpp_basics";
		cppCourse.title = "Основы C++";

		// Тема 1.1: Введение в C++
		TopicData topic1;
		topic1.id = "cpp_intro";
		topic1.title = "Введение в C++";
		topic1.type = "lesson";

		TopicBlock block1_1;
		block1_1.type = "text";
		block1_1.content = "C++ — это компилируемый, статически типизированный язык программирования общего назначения. Создан Бьёрном Страуструпом в 1985 году как расширение языка C.";

		TopicBlock block1_2;
		block1_2.type = "code";
		block1_2.content = "#include <iostream>\n\nint main() {\n    std::cout << \"Hello, World!\" << std::endl;\n    return 0;\n}";

		TopicBlock block1_3;
		block1_3.type = "image";
		block1_3.content = "https://upload.wikimedia.org/wikipedia/commons/1/18/ISO_C%2B%2B_Logo.svg";
		block1_3.caption = "Логотип C++";

		topic1.blocks.push_back(block1_1);
		topic1.blocks.push_back(block1_2);
		topic1.blocks.push_back(block1_3);

		// Тема 1.2: Переменные и типы данных
		TopicData topic2;
		topic2.id = "cpp_variables";
		topic2.title = "Переменные и типы данных";
		topic2.type = "lesson";

		TopicBlock block2_1;
		block2_1.type = "text";
		block2_1.content = "В C++ есть следующие основные типы данных:\n- int (целые числа)\n- float/double (числа с плавающей точкой)\n- char (символы)\n- bool (логический тип)\n- string (строки)";

		TopicBlock block2_2;
		block2_2.type = "code";
		block2_2.content = "int age = 25;\ndouble price = 99.99;\nchar grade = 'A';\nbool isStudent = true;\nstd::string name = \"John\";";

		topic2.blocks.push_back(block2_1);
		topic2.blocks.push_back(block2_2);

		// Тема 1.3: Тест по основам C++
		TopicData topic3;
		topic3.id = "cpp_test";
		topic3.title = "Тест: Основы C++";
		topic3.type = "test";

		TopicBlock testBlock;
		testBlock.type = "test";
		testBlock.question = "Какой заголовочный файл нужно включить для использования std::cout?";
		testBlock.options = { "<stdio.h>", "<iostream>", "<conio.h>", "<stdlib.h>" };
		testBlock.correctOption = 1;  // <iostream>

		topic3.blocks.push_back(testBlock);

		cppCourse.topics.push_back(topic1);
		cppCourse.topics.push_back(topic2);
		cppCourse.topics.push_back(topic3);

		// Курс 2: WebUI библиотека
		CourseMeta webuiCourse;
		webuiCourse.id = "webui";
		webuiCourse.title = "WebUI для C++";

		TopicData wTopic1;
		wTopic1.id = "webui_intro";
		wTopic1.title = "Введение в WebUI";
		wTopic1.type = "lesson";

		TopicBlock wBlock1;
		wBlock1.type = "text";
		wBlock1.content = "WebUI — это библиотека для создания графических интерфейсов на C++ с использованием веб-технологий.";

		wTopic1.blocks.push_back(wBlock1);
		webuiCourse.topics.push_back(wTopic1);

		courses.push_back(cppCourse);
		courses.push_back(webuiCourse);

		// Инициализация прогресса пользователя
		for (const auto& course : courses) {
			userData.completedTopics[course.id] = std::vector<bool>(course.topics.size(), false);
		}
	}

	// ========== Вспомогательные методы ==========

	std::string coursesToJson() const {
		std::stringstream ss;
		ss << "[";

		for (size_t i = 0; i < courses.size(); ++i) {
			if (i > 0) ss << ",";

			ss << "{";
			ss << "\"title\":\"" << courses[i].title << "\",";
			ss << "\"topics\":[";

			for (size_t j = 0; j < courses[i].topics.size(); ++j) {
				if (j > 0) ss << ",";
				ss << "{";
				ss << "\"title\":\"" << courses[i].topics[j].title << "\",";
				ss << "\"type\":\"" << courses[i].topics[j].type << "\"";
				ss << "}";
			}

			ss << "]}";
		}

		ss << "]";
		return ss.str();
	}

	std::string topicToJson(const TopicData& topic) const {
		std::stringstream ss;
		ss << "{";
		ss << "\"title\":\"" << topic.title << "\",";
		ss << "\"blocks\":[";

		for (size_t i = 0; i < topic.blocks.size(); ++i) {
			if (i > 0) ss << ",";

			const auto& block = topic.blocks[i];
			ss << "{";
			ss << "\"type\":\"" << block.type << "\",";

			if (block.type == "text") {
				ss << "\"content\":\"" << escapeJson(block.content) << "\"";
			}
			else if (block.type == "code") {
				ss << "\"content\":\"" << escapeJson(block.content) << "\"";
			}
			else if (block.type == "image") {
				ss << "\"content\":\"" << block.content << "\",";
				ss << "\"caption\":\"" << block.caption << "\"";
			}
			else if (block.type == "test") {
				ss << "\"question\":\"" << block.question << "\",";
				ss << "\"options\":[";

				for (size_t j = 0; j < block.options.size(); ++j) {
					if (j > 0) ss << ",";
					ss << "\"" << block.options[j] << "\"";
				}

				ss << "]";

				// Добавляем feedback если уже отвечали
				auto courseIt = std::find_if(courses.begin(), courses.end(),
					[&](const CourseMeta& c) {
						return std::find_if(c.topics.begin(), c.topics.end(),
							[&](const TopicData& t) { return t.id == topic.id; }) != c.topics.end();
					});

				if (courseIt != courses.end()) {
					const std::string& courseId = courseIt->id;
					auto resultsIt = userData.testResults.find(courseId);

					if (resultsIt != userData.testResults.end()) {
						const std::map<int, bool>& results = resultsIt->second;
						if (results.find(i) != results.end()) {
							auto resultIt = results.find(i);
							bool isCorrect = resultIt->second;

							ss << ",\"feedback\":{";
							ss << "\"isCorrect\":" << (isCorrect ? "true" : "false") << ",";
							ss << "\"message\":\"" << (isCorrect ? "Правильно!" : "Неправильно. Попробуйте еще раз.") << "\"";
							ss << "}";
						}
					}
				}
			}

			ss << "}";
		}

		ss << "]}";
		return ss.str();
	}

	std::string userDataToJson() const {
		std::stringstream ss;
		ss << "[";

		ss << "{\"name\":\"username\",\"label\":\"Имя пользователя\",\"type\":\"text\",\"value\":\"" << userData.username << "\",\"hint\":\"От 3 до 20 символов\"},";
		ss << "{\"name\":\"email\",\"label\":\"Email\",\"type\":\"email\",\"value\":\"" << userData.email << "\",\"hint\":\"Введите корректный email\"},";
		ss << "{\"name\":\"fullName\",\"label\":\"Полное имя\",\"type\":\"text\",\"value\":\"" << userData.fullName << "\",\"hint\":\"Ваше имя и фамилия\"},";
		ss << "{\"name\":\"bio\",\"label\":\"О себе\",\"type\":\"text\",\"value\":\"" << escapeJson(userData.bio) << "\",\"hint\":\"Краткая информация о себе\"}";

		ss << "]";
		return ss.str();
	}

	std::string escapeJson(const std::string& str) const {
		std::string result;
		for (char c : str) {
			switch (c) {
			case '"': result += "\\\""; break;
			case '\\': result += "\\\\"; break;
			case '\b': result += "\\b"; break;
			case '\f': result += "\\f"; break;
			case '\n': result += "\\n"; break;
			case '\r': result += "\\r"; break;
			case '\t': result += "\\t"; break;
			default: result += c;
			}
		}
		return result;
	}

	// ========== Обработчики событий окон ==========

	void onWindowEvent(webui::window::event* e) {
		std::cout << "[Event] Window: " << e->window
			<< " Type: " << e->get_type()
			<< " Element: '" << e->get_element() << "'" << std::endl;
	}

	// ========== Навигация ==========

	void ShowMainMenu(webui::window::event* e) {
		std::cout << "Navigating to Main Menu" << std::endl;

		//// Закрываем все окна кроме главного
		//if (coursesWindow.is_shown()) coursesWindow.close();
		//if (settingsWindow.is_shown()) settingsWindow.close();
		//if (accountWindow.is_shown()) accountWindow.close();

		//// Показываем главное меню (если оно не показано)
		//if (!mainWindow.is_shown()) {
		//    mainWindow.show("main-menu.html");
		//}
		mainWindow.show("main-menu.html");
	}

	void ShowCources(webui::window::event* e) {
		std::cout << "Opening Courses Window" << std::endl;
		/*coursesWindow.show("open-course.html");*/
		mainWindow.show("open-course.html");
	}

	void ShowAccount(webui::window::event* e) {
		std::cout << "Opening Account Window" << std::endl;
		/*accountWindow.show("account.html");*/
		mainWindow.show("account.html");
	}

	void ShowSettings(webui::window::event* e) {
		std::cout << "Opening Settings Window" << std::endl;
		/*settingsWindow.show("settings.html");*/
		mainWindow.show("settings.html");
	}

	void ExitApp(webui::window::event* e) {
		std::cout << "Exiting Application" << std::endl;
		webui::exit();
	}

	// ========== Обработчики окна курсов ==========

	void GetCoursesList(webui::window::event* e) {
		std::string coursesJson = coursesToJson();
		e->get_window().run("updateCoursesList('" + coursesJson + "');");
	}

	void onLoadTopicContent(webui::window::event* e) {
		std::string data = e->get_string();

		try {
			// Парсим JSON (упрощенно)
			size_t coursePos = data.find("\"courseIndex\":");
			size_t topicPos = data.find("\"topicIndex\":");

			if (coursePos != std::string::npos && topicPos != std::string::npos) {
				int courseIndex = std::stoi(data.substr(coursePos + 14));
				int topicIndex = std::stoi(data.substr(topicPos + 13));

				if (courseIndex >= 0 && courseIndex < (int)courses.size() &&
					topicIndex >= 0 && topicIndex < (int)courses[courseIndex].topics.size()) {

					const auto& topic = courses[courseIndex].topics[topicIndex];
					std::string topicJson = topicToJson(topic);
					e->get_window().run("updateTopicContent('" + topicJson + "');");
				}
			}
		}
		catch (const std::exception& ex) {
			std::cerr << "Error parsing topic load request: " << ex.what() << std::endl;
		}
	}

	void SubmitAndExecuteCode(webui::window::event* e) {
		std::string code = e->get_string();
		std::cout << "Executing code: " << code << std::endl;

		// Имитация выполнения кода
		std::string result = "> " + code + "\n\nHello, World!\n\nProcess finished with exit code 0";

		e->get_window().run("showResult('" + escapeJson(result) + "');");
	}

	void onSubmitTestAnswer(webui::window::event* e) {
		std::string data = e->get_string();

		try {
			// Парсим JSON (упрощенно)
			size_t testPos = data.find("\"testIndex\":");
			size_t optionPos = data.find("\"selectedOption\":");
			size_t topicPos = data.find("\"topicId\":");

			if (testPos != std::string::npos && optionPos != std::string::npos) {
				int testIndex = std::stoi(data.substr(testPos + 12));
				int selectedOption = std::stoi(data.substr(optionPos + 17));

				// Находим курс и тему
				for (auto& course : courses) {
					for (size_t t = 0; t < course.topics.size(); ++t) {
						if (course.topics[t].type == "test" &&
							t == (size_t)testIndex) {  // Упрощенно

							// Проверяем ответ
							bool isCorrect = false;
							if (testIndex < (int)course.topics[t].blocks.size() &&
								course.topics[t].blocks[testIndex].correctOption == selectedOption) {
								isCorrect = true;
								userData.testResults[course.id][testIndex] = true;
							}
							else {
								userData.testResults[course.id][testIndex] = false;
							}

							// Формируем результат
							std::stringstream result;
							result << "{";
							result << "\"testIndex\":" << testIndex << ",";
							result << "\"isCorrect\":" << (isCorrect ? "true" : "false") << ",";
							result << "\"message\":\"" << (isCorrect ? "Правильно! Отличная работа!" : "Неправильно. Попробуйте еще раз.") << "\"";
							result << "}";

							e->get_window().run("showTestResult('" + result.str() + "');");
							break;
						}
					}
				}
			}
		}
		catch (const std::exception& ex) {
			std::cerr << "Error parsing test answer: " << ex.what() << std::endl;
		}
	}

	// ========== Обработчики окна настроек ==========

	void GetSettings(webui::window::event* e) {
		std::string settingsJson = settings.toJson();
		e->get_window().run("initializeSettings('" + settingsJson + "');");
	}

	void onSettingChanged(webui::window::event* e) {
		std::string data = e->get_string();

		try {
			// Парсим JSON (упрощенно)
			size_t idPos = data.find("\"id\":\"");
			size_t valuePos = data.find("\"value\":");

			if (idPos != std::string::npos && valuePos != std::string::npos) {
				size_t idStart = idPos + 6;
				size_t idEnd = data.find("\"", idStart);
				std::string id = data.substr(idStart, idEnd - idStart);

				size_t valueStart = valuePos + 8;
				size_t valueEnd = data.find(valueStart == '"' ? "\"" : ",", valueStart);
				std::string value = data.substr(valueStart, valueEnd - valueStart);

				// Обновляем настройку временно (без сохранения)
				settings.updateFromJson(id, value);
			}
		}
		catch (const std::exception& ex) {
			std::cerr << "Error parsing setting change: " << ex.what() << std::endl;
		}
	}

	void onSaveSettings(webui::window::event* e) {
		std::string data = e->get_string();

		try {
			// Парсим JSON (упрощенно)
			size_t valuesPos = data.find("\"values\":{");

			if (valuesPos != std::string::npos) {
				// Здесь должно быть полноценное сохранение настроек
				// В демо-версии просто считаем что сохранили

				std::stringstream result;
				result << "{";
				result << "\"success\":true,";
				result << "\"message\":\"Настройки успешно сохранены\"";
				result << "}";

				e->get_window().run("onSaveComplete('" + result.str() + "');");
			}
		}
		catch (const std::exception& ex) {
			std::cerr << "Error saving settings: " << ex.what() << std::endl;

			std::stringstream result;
			result << "{";
			result << "\"success\":false,";
			result << "\"message\":\"Ошибка при сохранении настроек\"";
			result << "}";

			e->get_window().run("onSaveComplete('" + result.str() + "');");
		}
	}

	void onSettingsReset(webui::window::event* e) {
		std::cout << "Settings reset requested" << std::endl;
		// Здесь можно добавить логику при сбросе
	}

	// ========== Обработчики окна аккаунта ==========

	void onLoadAccountData(webui::window::event* e) {
		std::string userJson = userDataToJson();
		e->get_window().run("initializeAccount('" + userJson + "');");
	}

	void onValidateField(webui::window::event* e) {
		std::string data = e->get_string();

		try {
			// Парсим JSON (упрощенно)
			size_t namePos = data.find("\"name\":\"");
			size_t valuePos = data.find("\"value\":\"");

			if (namePos != std::string::npos && valuePos != std::string::npos) {
				size_t nameStart = namePos + 8;
				size_t nameEnd = data.find("\"", nameStart);
				std::string name = data.substr(nameStart, nameEnd - nameStart);

				size_t valueStart = valuePos + 8;
				size_t valueEnd = data.find("\"", valueStart);
				std::string value = data.substr(valueStart, valueEnd - valueStart);

				bool isValid = true;
				std::string message;

				// Валидация полей
				if (name == "username") {
					isValid = (value.length() >= 3 && value.length() <= 20);
					message = isValid ? "Имя пользователя корректно" : "Имя должно быть от 3 до 20 символов";
				}
				else if (name == "email") {
					isValid = (value.find('@') != std::string::npos && value.find('.') != std::string::npos);
					message = isValid ? "Email корректен" : "Введите корректный email";
				}
				else if (name == "fullName") {
					isValid = !value.empty();
					message = isValid ? "" : "Поле не может быть пустым";
				}

				std::stringstream result;
				result << "{";
				result << "\"name\":\"" << name << "\",";
				result << "\"isValid\":" << (isValid ? "true" : "false") << ",";
				result << "\"message\":\"" << message << "\"";
				result << "}";

				e->get_window().run("updateValidationStatus('" + result.str() + "');");
			}
		}
		catch (const std::exception& ex) {
			std::cerr << "Error validating field: " << ex.what() << std::endl;
		}
	}

	void onSaveAccount(webui::window::event* e) {
		std::string data = e->get_string();

		try {
			// Парсим JSON (упрощенно)
			size_t valuesPos = data.find("\"values\":{");

			if (valuesPos != std::string::npos) {
				// Здесь должно быть полноценное сохранение данных
				// В демо-версии просто обновляем данные пользователя из currentValues

				// Обновляем данные пользователя (упрощенно)
				userData.username = "student123";  // В реальном приложении брали бы из JSON
				userData.email = "student@example.com";
				userData.fullName = "Иван Петров";
				userData.bio = "Начинающий разработчик, увлекаюсь C++ и Python";

				std::stringstream result;
				result << "{";
				result << "\"success\":true,";
				result << "\"message\":\"Данные успешно сохранены\"";
				result << "}";

				e->get_window().run("onSaveComplete('" + result.str() + "');");
			}
		}
		catch (const std::exception& ex) {
			std::cerr << "Error saving account: " << ex.what() << std::endl;

			std::stringstream result;
			result << "{";
			result << "\"success\":false,";
			result << "\"message\":\"Ошибка при сохранении данных\"";
			result << "}";

			e->get_window().run("onSaveComplete('" + result.str() + "');");
		}
	}

	void onAccountCancel(webui::window::event* e) {
		std::cout << "Account changes cancelled" << std::endl;
		// Здесь можно добавить логику при отмене
	}
};