#include "interface.hpp";

AlienSApp::AlienSApp()
{
	// Привязка методов главного меню
	mainWindow.bind("open_course", this, &AlienSApp::ShowCources);
	mainWindow.bind("show_account", this, &AlienSApp::ShowAccount);
	mainWindow.bind("show_settings", this, &AlienSApp::ShowSettings);
	mainWindow.bind("exit_app", this, &AlienSApp::ExitApp);

	// Привязка методов окна курсов
	mainWindow.bind("show_main_menu", this, &AlienSApp::ShowMainMenu);
	mainWindow.bind("execute_code", this, &AlienSApp::ReceiveCode);
	mainWindow.bind("request_courses_list", this, &AlienSApp::SendCoursesList);
	mainWindow.bind("load_topic_content", this, &AlienSApp::SendTopicContent);
	mainWindow.bind("submit_test_answer", this, &AlienSApp::ReceiveTestAnswer);

	// Привязка методов окна настроек
	mainWindow.bind("go_to_main_menu", this, &AlienSApp::ShowMainMenu);
	mainWindow.bind("load_settings", this, &AlienSApp::SendSettingsList);
	mainWindow.bind("setting_changed", this, &AlienSApp::SendChangedSetting);
	mainWindow.bind("save_settings", this, &AlienSApp::ReceiveSavedSettings);
	mainWindow.bind("settings_reset", this, &AlienSApp::SendDefaultSettings);

	// Привязка методов окна аккаунта
	mainWindow.bind("go_to_main_menu", this, &AlienSApp::ShowMainMenu);
	mainWindow.bind("load_account_data", this, &AlienSApp::SendAccountData);
	mainWindow.bind("validate_field", this, &AlienSApp::SendChangedField);
	mainWindow.bind("save_account", this, &AlienSApp::RecieveAccountData);
	mainWindow.bind("cancel_changes", this, &AlienSApp::ShowMainMenu);

	// Привязка отладочного вывода
	mainWindow.bind("", this, &AlienSApp::DebugOutputWindowEvent);
}

void AlienSApp::Run() 
{
	std::cout << "AlienS Application Starting..." << std::endl;

	mainWindow.set_event_blocking(true);

	mainWindow.set_root_folder("ui_html");

	mainWindow.set_kiosk(true);

	mainWindow.show_browser("main-menu.html", AnyBrowser);

	// Ждем закрытия всех окон
	webui::wait();

	std::cout << "AlienS Application Finished." << std::endl;
}


// ========== Вспомогательные методы ==========

//std::string AlienSApp::ConvertCoursesToJson() const {
//	std::stringstream ss;
//	ss << "[";
//
//	for (size_t i = 0; i < courses.size(); ++i) {
//		if (i > 0) ss << ",";
//
//		ss << "{";
//		ss << "\"title\":\"" << courses[i].title << "\",";
//		ss << "\"topics\":[";
//
//		for (size_t j = 0; j < courses[i].topics.size(); ++j) {
//			if (j > 0) ss << ",";
//			ss << "{";
//			ss << "\"title\":\"" << courses[i].topics[j].title << "\",";
//			ss << "\"type\":\"" << courses[i].topics[j].type << "\"";
//			ss << "}";
//		}
//
//		ss << "]}";
//	}
//
//	ss << "]";
//	return ss.str();
//}
//
//std::string AlienSApp::ConvertTopicDataToJson(const TopicData& topic) const {
//	std::stringstream ss;
//	ss << "{";
//	ss << "\"title\":\"" << topic.title << "\",";
//	ss << "\"blocks\":[";
//
//	for (size_t i = 0; i < topic.blocks.size(); ++i) {
//		if (i > 0) ss << ",";
//
//		const auto& block = topic.blocks[i];
//		ss << "{";
//		ss << "\"type\":\"" << block.type << "\",";
//
//		if (block.type == "text") {
//			ss << "\"content\":\"" << escapeJson(block.content) << "\"";
//		}
//		else if (block.type == "code") {
//			ss << "\"content\":\"" << escapeJson(block.content) << "\"";
//		}
//		else if (block.type == "image") {
//			ss << "\"content\":\"" << block.content << "\",";
//			ss << "\"caption\":\"" << block.caption << "\"";
//		}
//		else if (block.type == "test") {
//			ss << "\"question\":\"" << block.question << "\",";
//			ss << "\"options\":[";
//
//			for (size_t j = 0; j < block.options.size(); ++j) {
//				if (j > 0) ss << ",";
//				ss << "\"" << block.options[j] << "\"";
//			}
//
//			ss << "]";
//
//			// Добавляем feedback если уже отвечали
//			auto courseIt = std::find_if(courses.begin(), courses.end(),
//				[&](const CourseMeta& c) {
//					return std::find_if(c.topics.begin(), c.topics.end(),
//						[&](const TopicData& t) { return t.id == topic.id; }) != c.topics.end();
//				});
//
//			if (courseIt != courses.end()) {
//				const std::string& courseId = courseIt->id;
//				auto resultsIt = userData.testResults.find(courseId);
//
//				if (resultsIt != userData.testResults.end()) {
//					const std::map<int, bool>& results = resultsIt->second;
//					if (results.find(i) != results.end()) {
//						auto resultIt = results.find(i);
//						bool isCorrect = resultIt->second;
//
//						ss << ",\"feedback\":{";
//						ss << "\"isCorrect\":" << (isCorrect ? "true" : "false") << ",";
//						ss << "\"message\":\"" << (isCorrect ? "Правильно!" : "Неправильно. Попробуйте еще раз.") << "\"";
//						ss << "}";
//					}
//				}
//			}
//		}
//
//		ss << "}";
//	}
//
//	ss << "]}";
//	return ss.str();
//}
//
//std::string AlienSApp::ConvertAccountDataToJson() const {
//	std::stringstream ss;
//	ss << "[";
//
//	ss << "{\"name\":\"username\",\"label\":\"Имя пользователя\",\"type\":\"text\",\"value\":\"" << userData.username << "\",\"hint\":\"От 3 до 20 символов\"},";
//	ss << "{\"name\":\"email\",\"label\":\"Email\",\"type\":\"email\",\"value\":\"" << userData.email << "\",\"hint\":\"Введите корректный email\"},";
//	ss << "{\"name\":\"fullName\",\"label\":\"Полное имя\",\"type\":\"text\",\"value\":\"" << userData.fullName << "\",\"hint\":\"Ваше имя и фамилия\"},";
//	ss << "{\"name\":\"bio\",\"label\":\"О себе\",\"type\":\"text\",\"value\":\"" << escapeJson(userData.bio) << "\",\"hint\":\"Краткая информация о себе\"}";
//
//	ss << "]";
//	return ss.str();
//}
//
//std::string AlienSApp::escapeJson(const std::string& str) const {
//	std::string result;
//	for (char c : str) {
//		switch (c) {
//		case '"': result += "\\\""; break;
//		case '\\': result += "\\\\"; break;
//		case '\b': result += "\\b"; break;
//		case '\f': result += "\\f"; break;
//		case '\n': result += "\\n"; break;
//		case '\r': result += "\\r"; break;
//		case '\t': result += "\\t"; break;
//		default: result += c;
//		}
//	}
//	return result;
//}

// ========== Навигация ==========

void AlienSApp::ShowMainMenu(webui::window::event* e) 
{
	std::cout << "Navigating to Main Menu" << std::endl;
	mainWindow.show("main-menu.html");
}

void AlienSApp::ShowCources(webui::window::event* e) 
{
	std::cout << "Opening Courses Window" << std::endl;
	mainWindow.show("open-course.html");
}

void AlienSApp::ShowAccount(webui::window::event* e) 
{
	std::cout << "Opening Account Window" << std::endl;
	mainWindow.show("account.html");
}

void AlienSApp::ShowSettings(webui::window::event* e) 
{
	std::cout << "Opening Settings Window" << std::endl;
	mainWindow.show("settings.html");
}

void AlienSApp::ExitApp(webui::window::event* e) 
{
	std::cout << "Exiting Application" << std::endl;
	webui::exit();
}

// ========== Обработчики событий окон ==========

void AlienSApp::DebugOutputWindowEvent(webui::window::event* e)
{
	std::cout << "[Event] Window: " << e->window
		<< " Type: " << e->get_type()
		<< " Element: '" << e->get_element() << "'" << std::endl;
}

void AlienSApp::SendCoursesList(webui::window::event* e) 
{
	// Получение списка метаданных всех курсов
	CoursesDatabase courses_list = course_manager.GetCourseList();
	// Строка для отправки списка курсов и тем
	std::string courses_json = R"([)";

	// Цикл по всем курсам
	for (uint8_t i = 0; i < courses_list.courses_data.size(); i++)
	{
		std::cout << courses_list.courses_data[i].name << std::endl;
		courses_json += (R"({ "title:" ")" + courses_list.courses_data[i].name + R"(", "topics": [)"); // Ввод заголовка курса
		
		// Цикл по всем темам в курсе
		for (uint8_t j = 0; j < courses_list.courses_data[i].content.size(); j++)
		{
			courses_json += (R"({ "title:" ")" + courses_list.courses_data[i].content[j].title + R"(",)"); // Ввод названия темы
			courses_json += (R"({ "type:" ")" + std::to_string(courses_list.courses_data[i].content[j].type) + R"(" })"); // Ввод номера типа темы
		}

		// Закрывающие скобки списка тем и списка курсов
		courses_json += R"(] })";
		// Добавление запятой, если курс не последний
		if (i < courses_list.courses_data.size() - 1) { courses_json += R"(, )"; }
		// Закрывающая скобка JSON строки
		courses_json += R"(])";
	}
	e->get_window().run("updateCoursesList('" + courses_json + "');");
}

void AlienSApp::SendTopicContent(webui::window::event* e) 
{
	std::string data = e->get_string();

	e->get_window().run("updateTopicContent('" + data + "');");
}

void AlienSApp::ReceiveCode(webui::window::event* e) {
	std::string code = e->get_string();
	std::cout << "Executing code: " << code << std::endl;

	// Имитация выполнения кода
	std::string result = "> " + code + "\n\nHello, World!\n\nProcess finished with exit code 0";

	e->get_window().run("showResult('" + result + "');");
}

void AlienSApp::ReceiveTestAnswer(webui::window::event* e) {
	std::string data = e->get_string();
	e->get_window().run("showTestResult('" + data + "');");
}

// ========== Обработчики окна настроек ==========

void AlienSApp::SendSettingsList(webui::window::event* e) {
	std::string settingsJson = "Example settings";
	e->get_window().run("initializeSettings('" + settingsJson + "');");
}

void AlienSApp::SendChangedSetting(webui::window::event* e) {
	std::string data = e->get_string();
	e->get_window().run("initializeSettings('" + data + "');");
}

void AlienSApp::ReceiveSavedSettings(webui::window::event* e) {
	std::string data = e->get_string();
	e->get_window().run("onSaveComplete('" + data + "');");
}

void AlienSApp::SendDefaultSettings(webui::window::event* e) {
	std::cout << "Settings reset requested" << std::endl;
	// Здесь можно добавить логику при сбросе
}

// ========== Обработчики окна аккаунта ==========

void AlienSApp::SendAccountData(webui::window::event* e) {
	std::string userJson = "Example Account";
	e->get_window().run("initializeAccount('" + userJson + "');");
}

void AlienSApp::SendChangedField(webui::window::event* e) {
	std::string data = e->get_string();
	e->get_window().run("updateValidationStatus('" + data + "');");
}

void AlienSApp::RecieveAccountData(webui::window::event* e) {
	std::string data = e->get_string();
	e->get_window().run("onSaveComplete('" + data + "');");
}

//void AlienSApp::onAccountCancel(webui::window::event* e) {
//	std::cout << "Account changes cancelled" << std::endl;
//	// Здесь можно добавить логику при отмене
//}