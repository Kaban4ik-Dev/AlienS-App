#pragma execution_character_set("utf-8")
#include "user_interface.hpp"

UIManager::UIManager()
{
	// Уровень сглаживания - 16
	settings.antiAliasingLevel = 16;
	// Создать окно
	window = sf::RenderWindow(sf::VideoMode({ 800, 600 }), "AlienS - Code Learning App", sf::State::Windowed, settings);
	// Установить максимальную частоту кадров
	window.setFramerateLimit(60);
	// Установить текущую страницу на главный экран
	curr_tab_id = 0;
	prev_tab_id = 0;
	// Создать поверхность UI
	tab = std::make_unique<tgui::Gui>(window);

	// Загрузка шрифта
	regular_font = std::make_shared<tgui::Font>("C:/Windows/Fonts/arial.ttf");
	mono_font = std::make_shared<tgui::Font>("C:/Windows/Fonts/consola.ttf");
	tab->setFont(*regular_font);
}

UIManager::~UIManager()
{
	std::cout << "UIManager destructor called" << std::endl;

	// Очистить классы страниц
	pages.clear();

	// Удалить все виджеты
	tab->removeAllWidgets();

	// Удалить шрифты
	regular_font.reset();
	mono_font.reset();

	// Сбросить GUI
	tab.reset();

	std::cout << "UIManager destructor finished" << std::endl;
}

tgui::Button::Ptr CreateButton(const std::string& name, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
	// Создание кнопки
	tgui::Button::Ptr button = tgui::Button::create();
	button->setWidgetName(name);
	button->setText(name);
	button->setPosition({ x, y });
	button->setSize({ width, height });

	// Установка стиля
	auto renderer = button->getRenderer();
	renderer->setRoundedBorderRadius(std::min(width, height) * 0.5);
	renderer->setBorders(3);
	renderer->setBackgroundColor(sf::Color(255, 255, 255));      
	renderer->setBackgroundColorHover(sf::Color(102, 126, 234)); 
	renderer->setBackgroundColorDown(sf::Color(118, 75, 162));
	renderer->setTextColor(sf::Color(102, 126, 234));
	renderer->setTextColorHover(sf::Color::White);
	renderer->setTextColorDown(sf::Color::White);
	renderer->setTextColorDisabled(sf::Color(150, 150, 150));
	renderer->setBorderColor(sf::Color(102, 126, 234));
	renderer->setBorderColorHover(sf::Color(102, 126, 234));
	renderer->setBorderColorDown(sf::Color(118, 75, 162));

	// Дополнительные настройки стиля
	renderer->setTextStyle(tgui::TextStyle::Bold);	// Жирный шрифт
	renderer->setOpacity(1.0f);                     // Полная непрозрачность

	return button;
}

void UIManager::CreateUI()
{
	// Создание страниц с их ID
	pages.push_back(std::make_unique<TabMainMenu>(*this, *this->tab, 0));
	pages.push_back(std::make_unique<TabReadCourse>(*this, *this->tab, 1));
	pages.push_back(std::make_unique<TabCreateCourse>(*this, *this->tab, 2));
	pages.push_back(std::make_unique<TabAccount>(*this, *this->tab, 3));
	pages.push_back(std::make_unique<TabSettings>(*this, *this->tab, 4));
	pages.push_back(std::make_unique<TabCodeEditor>(*this, *this->tab, 5));

	// Создание виджетов для каждой страницы
	for (auto& page : pages) { page->CreateTab(window); }

	// Сокрытие виджетов страниц, кроме главного меню
	for (auto& page : pages) { page->Hide(); }
	pages[0]->Show();
}

void UIManager::SwitchToPage(uint8_t new_tab_id)
{
	if (new_tab_id >= pages.size()) return; // Неверный ID

	// Перейти к новой странице
	SetCurrTabID(new_tab_id);

	// Спрятать элементы предыдущей страницы
	pages[prev_tab_id]->Hide();

	// Показать элементы новой страницы
	pages[curr_tab_id]->Show();

	// Вызвать обновление размера для новой страницы
	pages[curr_tab_id]->UpdateOnResize(window.getSize());
}

void UIManager::UpdateOnResize(uint8_t tab_id, const sf::Vector2u& window_size)
{
	if (tab_id < pages.size()) {
		// Вызов обновления размера у соответствующей страницы
		pages[tab_id]->UpdateOnResize(window_size);
	}
}

void UIManager::Run()
{
	// Вызвать начальное обновление размера
	pages[curr_tab_id]->UpdateOnResize(window.getSize());

	// Основной цикл работы программы
	while (window.isOpen())
	{
		// Цикл обработки событий экрана
		while (const std::optional event = window.pollEvent())
		{
			// Обработка закрытия окна
			if (event->is<sf::Event::Closed>())
			{
				window.close();
				break;
			}

			// Обработка изменения размера окна
			if (const auto* resized = event->getIf<sf::Event::Resized>())
			{
				UpdateOnResize(curr_tab_id, resized->size);
			}

			// Обработка событий TGUI
			tab->handleEvent(*event);
		}

		// Обновление текущей страницы каждый кадр
		if (curr_tab_id < pages.size()) {
			pages[curr_tab_id]->UpdateOnFrame();
		}

		// Отрисовка
		window.clear();
		tab->draw();
		window.display();
	}
}



/*==================================================*/
/*		Классы страниц графического интерфейса		*/
/*==================================================*/



/*==================================================*/
/*				Класс главного экрана				*/
/*==================================================*/

void TabMainMenu::CreateTab(sf::RenderWindow& window)
{
	// Получение размеров окна
	uint16_t width = window.getSize().x;
	uint16_t height = window.getSize().y;

	// Фон окна
	auto back_panel = tgui::Panel::create();
	auto rndr_back = back_panel->getRenderer();
	rndr_back->setBackgroundColor(sf::Color({ 105, 82, 255 }));
	tab.add(back_panel, "Backpanel1");

	// Прямоугольник с элементами программы
	auto element_panel = tgui::Panel::create({ "96%", "96%" });
	element_panel->setPosition({ "2%", "2%" });
	auto rndr_elem = element_panel->getRenderer();
	rndr_elem->setBackgroundColor(sf::Color({ 255,255,255 }));
	rndr_elem->setRoundedBorderRadius(std::min(width, height) * 0.05);
	tab.add(element_panel, "Backpanel2");

	// Создание кнопок
	auto open_courses_tab = CreateButton("Открыть курс", 20, 20, 200, 80);
	open_courses_tab->setSize({ "20%", "12%" });
	open_courses_tab->setPosition({ "40%", "12%" });
	tab.add(open_courses_tab, "Main_OpenCR");
	auto create_course_tab = CreateButton("Создать курс", 20, 120, 200, 80);
	create_course_tab->setSize({ "20%", "12%" });
	create_course_tab->setPosition({ "40%", "28%" });
	tab.add(create_course_tab, "Main_OpenCE");
	auto account_tab = CreateButton("Аккаунт", 20, 220, 200, 80);
	account_tab->setSize({ "20%", "12%" });
	account_tab->setPosition({ "40%", "44%" });
	tab.add(account_tab, "Main_OpenAcc");
	auto settings_tab = CreateButton("Настройки", 20, 320, 200, 80);
	settings_tab->setSize({ "20%", "12%" });
	settings_tab->setPosition({ "40%", "60%" });
	tab.add(settings_tab, "Main_OpenSet");
	auto exit = CreateButton("Выход", 20, 420, 200, 80);
	exit->setSize({ "20%", "12%" });
	exit->setPosition({ "40%", "76%" });
	tab.add(exit, "Main_Exit");

	// Настройка событий при нажатии кнопок
	// Перейти на экран "Открыть курс"
	open_courses_tab->onClick([this]() {
		ui_manager.SwitchToPage(1);
		});
}

void TabMainMenu::UpdateOnResize(const sf::Vector2u& window_size)
{
	// Получение размеров окна
	unsigned int width = window_size.x;
	unsigned int height = window_size.y;

	// Обновление размеров фона
	tgui::PanelRenderer* back_renderer = tab.get<tgui::Panel>("Backpanel2")->getRenderer();
	if (back_renderer) back_renderer->setRoundedBorderRadius(std::min(width, height) * 0.05);

	// Задание новых размеров кнопок относительно размера экрана
	float buttonWidth = width * 0.2f;		// 20% - ширина кнопки
	float buttonHeight = height * 0.12f;	// 12% - высота кнопки
	float buttonSpacing = height * 0.04f;	// 4% - отступ между кнопками
	float startX = width * 0.4f;			// 40% - начало по x
	float startY = height * 0.12f;			// 12% - начало по y

	// Обновление размеров кнопок
	tgui::ButtonRenderer* button_renderer = nullptr;

	// Кнопка "Открыть курс"
	button_renderer = tab.get<tgui::Button>("Main_OpenCR")->getRenderer();
	if (button_renderer) button_renderer->setTextSize(std::min(buttonWidth, buttonHeight) * 0.2);

	// Кнопка "Создать курс"
	button_renderer = tab.get<tgui::Button>("Main_OpenCE")->getRenderer();
	if (button_renderer) button_renderer->setTextSize(std::min(buttonWidth, buttonHeight) * 0.2);

	// Кнопка "Аккаунт"
	button_renderer = tab.get<tgui::Button>("Main_OpenAcc")->getRenderer();
	if (button_renderer) button_renderer->setTextSize(std::min(buttonWidth, buttonHeight) * 0.2);

	// Кнопка "Настройки"
	button_renderer = tab.get<tgui::Button>("Main_OpenSet")->getRenderer();
	if (button_renderer) button_renderer->setTextSize(std::min(buttonWidth, buttonHeight) * 0.2);

	// Кнопка "Выход"
	button_renderer = tab.get<tgui::Button>("Main_Exit")->getRenderer();
	if (button_renderer) button_renderer->setTextSize(std::min(buttonWidth, buttonHeight) * 0.2);
}

void TabMainMenu::UpdateOnFrame()
{
	// Пустая реализация
}

void TabMainMenu::Show()
{
	tab.get<tgui::Button>("Main_OpenCR")->setVisible(true);
	tab.get<tgui::Button>("Main_OpenCE")->setVisible(true);
	tab.get<tgui::Button>("Main_OpenAcc")->setVisible(true);
	tab.get<tgui::Button>("Main_OpenSet")->setVisible(true);
	tab.get<tgui::Button>("Main_Exit")->setVisible(true);
}

void TabMainMenu::Hide()
{
	tab.get<tgui::Button>("Main_OpenCR")->setVisible(false);
	tab.get<tgui::Button>("Main_OpenCE")->setVisible(false);
	tab.get<tgui::Button>("Main_OpenAcc")->setVisible(false);
	tab.get<tgui::Button>("Main_OpenSet")->setVisible(false);
	tab.get<tgui::Button>("Main_Exit")->setVisible(false);
}



/*==================================================*/
/*			Класс страницы чтения курсов			*/
/*==================================================*/

void TabReadCourse::CreateTab(sf::RenderWindow& window)
{
	// Получение размеров окна
	uint16_t width = window.getSize().x;
	uint16_t height = window.getSize().y;

	// Создание кнопок
	auto open_main_menu = CreateButton("Главное меню", 60, 20, 200, 80);
	open_main_menu->setSize({ "20%", "8%" });
	open_main_menu->setPosition({ "3%", "3%" });
	tab.add(open_main_menu, "OpenCR_OpenMain");
	auto open_code_editor = CreateButton("Открыть редактор кода", 360, 20, 200, 80);
	open_code_editor->setSize({ "28%", "8%" });
	open_code_editor->setPosition({ "69%", "3%" });
	tab.add(open_code_editor, "OpenCR_OpenCode");

	// Прямоугольник с закруглёнными углами для списка тем
	auto topic_bg_panel = tgui::Panel::create({ "24%", "84%" });
	topic_bg_panel->setPosition({ "3%", "13%" });
	auto topic_bg_renderer = topic_bg_panel->getRenderer();
	topic_bg_renderer->setBackgroundColor(sf::Color({ 242,242,242 }));
	topic_bg_renderer->setRoundedBorderRadius(std::min(width, height) * 0.04);
	tab.add(topic_bg_panel, "OpenCR_TopicPanel_BG");

	// Прямоугольник с закруглёнными углами для содержимого курса
	auto content_bg_panel = tgui::Panel::create({ "69%", "84%" });
	content_bg_panel->setPosition({ "28%", "13%" });
	auto content_bg_renderer = content_bg_panel->getRenderer();
	content_bg_renderer->setBackgroundColor(sf::Color({ 242,242,242 }));
	content_bg_renderer->setRoundedBorderRadius(std::min(width, height) * 0.04);
	tab.add(content_bg_panel, "OpenCR_ContentPanel_BG");

	// Колонка тем
	auto topic_panel = tgui::ScrollablePanel::create({ "22%", "82%" });
	topic_panel->setPosition({ "4%", "14%" });
	auto topic_renderer = topic_panel->getRenderer();
	topic_renderer->setBackgroundColor({ 242,242,242 });

	// Цикл заполнения колонки тем
	unsigned int buttons_num = 0;
	for (int i = 0; i < 3; i++)
	{
		// Создать кнопку курса
		auto course_button = tgui::Button::create();
		course_button->setSize({ "98%", "10%" });
		course_button->setPosition({ "1%", (std::to_string(11 * buttons_num) + "%").c_str()});
		buttons_num++;
		course_button->setText("Course " + std::to_string(i));
		auto course_renderer = course_button->getRenderer();
		course_renderer->setBackgroundColor({ 127, 255, 212 });
		topic_panel->add(course_button, "Course_" + std::to_string(i));

		// Цикл создания кнопки темы (в курсе)
		for (int j = 0; j < 5; j++)
		{
			// Создать кнопку темы
			auto topic_button = tgui::Button::create();
			topic_button->setSize({ "98%", "10%" });
			topic_button->setPosition({ "1%", (std::to_string(11 * buttons_num) + "%").c_str() });
			buttons_num++;
			topic_button->setText("Topic " + std::to_string(j));
			auto topic_renderer = topic_button->getRenderer();
			topic_renderer->setBackgroundColor({ 250, 231, 181 });
			topic_panel->add(topic_button, "Topic_" + std::to_string(i) + "_" + std::to_string(j));
		}
	}
	tab.add(topic_panel, "OpenCR_TopicPanel");

	// Колонка содержимого
	auto content_panel = tgui::ScrollablePanel::create({ "67%", "82%" });
	content_panel->setPosition({ "29%", "14%" });
	auto content_renderer = content_panel->getRenderer();
	content_renderer->setBackgroundColor({ 242,242,242 });
	// Цикл заполнения колонки содержимого
	for (int i = 0; i < 5 ; i++)
	{
		// Создать текст
		auto text = tgui::TextArea::create();
		text->setSize({ "98%", "10%" });
		text->setPosition({ "1%", (std::to_string(11 * i) + "%").c_str() });
		text->setText("Text " + std::to_string(i) + "\nText n");
		auto text_renderer = text->getRenderer();
		text_renderer->setFont(*ui_manager.GetMonoFont());
		//text_renderer->setBackgroundColor({ 242,242,242 });
		content_panel->add(text, "Text_" + std::to_string(i));
	}
	tab.add(content_panel, "OpenCR_ContentPanel");

	// Настройка событий при нажатии кнопок
	// Перейти на экран "Главное меню"
	open_main_menu->onClick([this]() {
		ui_manager.SwitchToPage(0);
		});

	// Перейти на экран "Редактор кода"
	open_code_editor->onClick([this]() {
		ui_manager.SwitchToPage(5);
		});
}

void TabReadCourse::UpdateOnResize(const sf::Vector2u& window_size)
{
	// Получение размеров окна
	unsigned int width = window_size.x;
	unsigned int height = window_size.y;

	// Обновление размеров фона
	tgui::PanelRenderer* back_renderer = tab.get<tgui::Panel>("Backpanel2")->getRenderer();
	if (back_renderer) back_renderer->setRoundedBorderRadius(std::min(width, height) * 0.05);

	// Задание новых размеров кнопок относительно размера экрана
	float buttonWidth = width * 0.3f;		// 30% - ширина кнопки
	float buttonHeight = height * 0.12f;	// 12% - высота кнопки
	float startX = width * 0.4f;			// 60% - начало по x
	float startY = height * 0.12f;			// 12% - начало по y

	// Обновление размеров кнопок
	tgui::ButtonRenderer* button_renderer = nullptr;

	// Кнопка "Главное меню"
	button_renderer = tab.get<tgui::Button>("OpenCR_OpenMain")->getRenderer();
	if (button_renderer) button_renderer->setTextSize(std::min(buttonWidth, buttonHeight) * 0.2);

	// Кнопка "Открыть редактор кода"
	button_renderer = tab.get<tgui::Button>("OpenCR_OpenCode")->getRenderer();
	if (button_renderer) button_renderer->setTextSize(std::min(buttonWidth, buttonHeight) * 0.2);

	// Кнопки курсов и тем
	auto topic_panel = tab.get<tgui::ScrollablePanel>("OpenCR_TopicPanel");
	
	// Обновление размера шрифта для всех кнопок курсов
	int i = 0;
	while (true) 
	{
		auto course_button = topic_panel->get<tgui::Button>("Course_" + std::to_string(i));
		if (!course_button) break;
		course_button->getRenderer()->setTextSize(std::min(buttonWidth, buttonHeight) * 0.2);
		i++;
	}

	// Обновление размера шрифта для всех кнопок тем
	i = 0;
	while (true) 
	{
		int j = 0;
		bool foundAnyInCourse = false;
		while (true) 
		{
			auto topic_button = topic_panel->get<tgui::Button>("Topic_" + std::to_string(i) + "_" + std::to_string(j));
			if (!topic_button) break; // Выход, если темы в этом курсе закончились
			topic_button->getRenderer()->setTextSize(std::min(buttonWidth, buttonHeight) * 0.2);
			j++;
			foundAnyInCourse = true;
		}
		if (!foundAnyInCourse) break; // Выход, если закончились курсы
		i++;
	}

	// Обновление размера шрифта для всех элементов содержимого
	i = 0;
	auto content_panel = tab.get<tgui::ScrollablePanel>("OpenCR_ContentPanel");
	while (true)
	{
		auto text = content_panel->get<tgui::Label>("Text_" + std::to_string(i));
		if (!text) break;
		text->getRenderer()->setTextSize(std::min(buttonWidth, buttonHeight) * 0.2);
		i++;
	}
}

void TabReadCourse::UpdateOnFrame()
{
	// Пустая реализация
}

void TabReadCourse::Show()
{
	tab.get<tgui::Button>("OpenCR_OpenMain")->setVisible(true);
	tab.get<tgui::Button>("OpenCR_OpenCode")->setVisible(true);
	tab.get<tgui::ScrollablePanel>("OpenCR_TopicPanel")->setVisible(true);
	tab.get<tgui::Panel>("OpenCR_TopicPanel_BG")->setVisible(true);
	tab.get<tgui::ScrollablePanel>("OpenCR_ContentPanel")->setVisible(true);
	tab.get<tgui::Panel>("OpenCR_ContentPanel_BG")->setVisible(true);
}

void TabReadCourse::Hide()
{
	tab.get<tgui::Button>("OpenCR_OpenMain")->setVisible(false);
	tab.get<tgui::Button>("OpenCR_OpenCode")->setVisible(false);
	tab.get<tgui::ScrollablePanel>("OpenCR_TopicPanel")->setVisible(false);
	tab.get<tgui::Panel>("OpenCR_TopicPanel_BG")->setVisible(false);
	tab.get<tgui::ScrollablePanel>("OpenCR_ContentPanel")->setVisible(false);
	tab.get<tgui::Panel>("OpenCR_ContentPanel_BG")->setVisible(false);
}



/*==================================================*/
/*			Класс страницы создания курсов			*/
/*==================================================*/

void TabCreateCourse::CreateTab(sf::RenderWindow& window)
{

}

void TabCreateCourse::UpdateOnResize(const sf::Vector2u& window_size)
{

	
}

void TabCreateCourse::UpdateOnFrame()
{
	// Пустая реализация
}

void TabCreateCourse::Show()
{

}

void TabCreateCourse::Hide()
{

}


/*==================================================*/
/*				Класс страницы аккаунта				*/
/*==================================================*/

void TabAccount::CreateTab(sf::RenderWindow& window)
{



}

void TabAccount::UpdateOnResize(const sf::Vector2u& window_size)
{
	
}

void TabAccount::UpdateOnFrame()
{
	// Пустая реализация
}

void TabAccount::Show()
{

}

void TabAccount::Hide()
{

}


/*==================================================*/
/*				Класс страницы настроек				*/
/*==================================================*/

void TabSettings::CreateTab(sf::RenderWindow& window)
{



}

void TabSettings::UpdateOnResize(const sf::Vector2u& window_size)
{



}

void TabSettings::UpdateOnFrame()
{
	// Пустая реализация
}

void TabSettings::Show()
{

}

void TabSettings::Hide()
{

}



/*==================================================*/
/*			Класс страницы редактора кода			*/
/*==================================================*/

void TabCodeEditor::CreateTab(sf::RenderWindow& window)
{
	// Получение размеров окна
	uint16_t width = window.getSize().x;
	uint16_t height = window.getSize().y;

	// Создание кнопок
	auto open_course_reader = CreateButton("Вернуться к курсам", 60, 20, 200, 80);
	open_course_reader->setSize({ "22%", "8%" });
	open_course_reader->setPosition({ "3%", "3%" });
	tab.add(open_course_reader, "OpenCode_OpenCR");
	auto run_code = CreateButton("Запустить код", 360, 20, 200, 80);
	run_code->setSize({ "20%", "8%" });
	run_code->setPosition({ "77%", "3%" });
	tab.add(run_code, "OpenCode_RunCode");

	// Кнопка создать файл
	auto create_file = tgui::Button::create();
	create_file->setText("+ Добавить файл");
	create_file->setSize({ "15%", "6%" });
	create_file->setPosition({ "3%", "12%" });
	create_file->getRenderer()->setFont(*ui_manager.GetRegularFont());
	tab.add(create_file, "OpenCode_AddFile");
	
	// Левая панель - дерево файлов
	auto files_tree = tgui::ScrollablePanel::create({ "15%", "77%" });
	files_tree->setPosition("3%", "18%");
	auto file_tree_renderer = files_tree->getRenderer();
	file_tree_renderer->setBackgroundColor(sf::Color(240, 240, 240));
	tab.add(files_tree, "OpenCode_FileTree");

	// Правая панель — вкладки файлов
	auto files_code_container = tgui::TabContainer::create();
	files_code_container->setSize("79%", "83%");
	files_code_container->setPosition("18%", "12%");
	tab.add(files_code_container, "OpenCode_FileContainer");

	// Создание заданного числа контейнеров и вкладок файлов
	for (int i = 0; i < POOL_SIZE; i++)
	{
		// Создать контейнер для файла
		auto file_layout = tgui::HorizontalLayout::create();
		file_layout->setSize({ "98%", "8%" });
		file_layout->setPosition({ "1%", (std::to_string(9 * i) + "%").c_str() });
		file_layout->setVisible(false);

		// Создать имя файла с уникальным именем по умолчанию
		auto file_name = tgui::EditBox::create();
		file_name->setDefaultText("unnamed_" + std::to_string(i) + ".sv");
		file_name->setSize({ "80%", "100%" });
		file_layout->add(file_name, "0");

		// Обновление при вводе нового имени
		file_name->onTextChange([this, i]() {
			// Получить актуальные указатели на виджеты
			auto files_tree = tab.get<tgui::ScrollablePanel>("OpenCode_FileTree");
			auto files_code_container = tab.get<tgui::TabContainer>("OpenCode_FileContainer");
			if (!files_tree || !files_code_container) return;

			// Вычислить реальный индекс вкладки (количество используемых файлов до текущего)
			unsigned int tab_index = 0;
			for (int j = 0; j < i; j++) {
				if (is_used[j]) {
					tab_index++;
				}
			}

			// Только если текущий файл используется
			if (is_used[i]) {
				auto text = files_tree->get<tgui::HorizontalLayout>(std::to_string(i))->get<tgui::EditBox>("0")->getText();
				auto def_text = files_tree->get<tgui::HorizontalLayout>(std::to_string(i))->get<tgui::EditBox>("0")->getDefaultText();
				files_code_container->changeTabText(tab_index, !text.empty() ? text : def_text);
			}
		});

		// Создать кнопку удаления
		auto file_delete = tgui::Button::create();
		file_delete->setText("Del");
		file_delete->setSize({ "20%", "100%" });
		file_layout->add(file_delete, "1");

		file_delete->onClick([this, i]() {
			// Проверка, не удалён ли уже файл
			if (i >= POOL_SIZE || !is_used[i]) {
				return; // Файл уже не используется
			}

			// Скрыть и освободить файл
			is_used[i] = false;

			// Получить актуальные указатели на виджеты
			auto files_tree = tab.get<tgui::ScrollablePanel>("OpenCode_FileTree");
			auto files_code_container = tab.get<tgui::TabContainer>("OpenCode_FileContainer");
			if (!files_tree || !files_code_container) return;

			// Скрыть файл
			auto layout = files_tree->get<tgui::HorizontalLayout>(std::to_string(i));
			if (layout) 
			{
				layout->get<tgui::EditBox>("0")->setText("");
				layout->setVisible(false);
			}

			// Очистить вкладки с кодом
			while (files_code_container->getPanelCount() > 0)
			{
				files_code_container->removeTab(0);
			}

			// Показать все используемые файлы
			unsigned int num_files = 0;
			for (int j = 0; j < POOL_SIZE; j++)
			{
				if (is_used[j])
				{
					// Обновить позицию файла и показать
					auto curr_layout = files_tree->get<tgui::HorizontalLayout>(std::to_string(j));
					curr_layout->setPosition({ "1%", (std::to_string(9 * num_files) + "%").c_str() });
					curr_layout->setVisible(true);

					// Показать содержимое файла
					files_code_container->addTab(std::to_string(num_files));
					auto text = files_tree->get<tgui::HorizontalLayout>(std::to_string(j))->get<tgui::EditBox>("0")->getText();
					auto def_text = files_tree->get<tgui::HorizontalLayout>(std::to_string(j))->get<tgui::EditBox>("0")->getDefaultText();
					files_code_container->changeTabText(num_files, !text.empty() ? text : def_text);
					files_code_container->getPanel(num_files)->setSize({ "100%", "100%" });
					auto code = tgui::TextArea::create();
					code->setSize({ "100%", "100%" });
					code->setDefaultText("No code");
					files_code_container->getPanel(num_files)->add(code, "code");

					num_files++;
				}
			}
		});

		// Добавить контейнер файла в дерево файлов
		file_layout->setRatio(0, 4);
		file_layout->setRatio(1, 1);
		files_tree->add(file_layout, std::to_string(i));
	}

	// Функционал кнопки "Создать файл"
	create_file->onClick([this, files_tree, files_code_container]() {
		
		// Найти индекс первого свободного элемента
		int free_index = -1;
		for (int i = 0; i < POOL_SIZE; i++)
		{
			if (!is_used[i])
			{
				free_index = i;
				break;
			}
		}
		
		// Выделить файл из пула
		if (free_index != -1)
		{
			// Показать доступный файл
			is_used[free_index] = true;

			// Очистить вкладки с кодом
			while (files_code_container->getPanelCount() > 0)
			{
				files_code_container->removeTab(0);
			}

			// Показать все используемые файлы
			unsigned int num_files = 0;
			for (int i = 0; i < POOL_SIZE; i++)
			{
				// Если файл используется
				if (is_used[i])
				{
					// Обновить его позицию и показать
					files_tree->get<tgui::HorizontalLayout>(std::to_string(i))->setPosition({ "1%", (std::to_string(9 * num_files) + "%").c_str() });
					files_tree->get<tgui::HorizontalLayout>(std::to_string(i))->setVisible(true);

					// Показать содержимое файла
					files_code_container->addTab(std::to_string(num_files));
					auto text = files_tree->get<tgui::HorizontalLayout>(std::to_string(i))->get<tgui::EditBox>("0")->getText();
					auto def_text = files_tree->get<tgui::HorizontalLayout>(std::to_string(i))->get<tgui::EditBox>("0")->getDefaultText();
					files_code_container->changeTabText(num_files, !text.empty() ? text : def_text);
					files_code_container->getPanel(num_files)->setSize({ "100%", "100%" });
					auto code = tgui::TextArea::create();
					code->setSize({ "100%", "100%" });
					code->setDefaultText("No code");
					files_code_container->getPanel(num_files)->add(code, "code");

					num_files++;
				}
			}
		}
	});

	// Настройка событий при нажатии кнопок
	open_course_reader->onClick([this]() {
		ui_manager.SwitchToPage(1);
		});
}

void TabCodeEditor::UpdateOnResize(const sf::Vector2u& window_size)
{
	// Получение размеров окна
	unsigned int width = window_size.x;
	unsigned int height = window_size.y;

	// Обновление размеров фона
	tgui::PanelRenderer* back_renderer = tab.get<tgui::Panel>("Backpanel2")->getRenderer();
	if (back_renderer) back_renderer->setRoundedBorderRadius(std::min(width, height) * 0.05);

	// Задание новых размеров кнопок относительно размера экрана
	float buttonWidth = width * 0.3f;		// 30% - ширина кнопки
	float buttonHeight = height * 0.12f;	// 12% - высота кнопки
	float startX = width * 0.4f;			// 60% - начало по x
	float startY = height * 0.12f;			// 12% - начало по y

	// Обновление размеров кнопок
	tgui::ButtonRenderer* button_renderer = nullptr;

	// Кнопка "Главное меню"
	button_renderer = tab.get<tgui::Button>("OpenCode_OpenCR")->getRenderer();
	if (button_renderer) button_renderer->setTextSize(std::min(buttonWidth, buttonHeight) * 0.2);

	// Кнопка "Открыть редактор кода"
	button_renderer = tab.get<tgui::Button>("OpenCode_RunCode")->getRenderer();
	if (button_renderer) button_renderer->setTextSize(std::min(buttonWidth, buttonHeight) * 0.2);

	// Кнопка "Добавить файл"
	button_renderer = tab.get<tgui::Button>("OpenCode_AddFile")->getRenderer();
	if (button_renderer) button_renderer->setTextSize(std::min(buttonWidth, buttonHeight) * 0.2);

	// Названия файлов
	tgui::EditBoxRenderer* file_name_renderer = nullptr;
	for (int i = 0; i < POOL_SIZE; i++)
	{
		file_name_renderer = tab.get<tgui::ScrollablePanel>("OpenCode_FileTree")->get<tgui::HorizontalLayout>(std::to_string(i))->get<tgui::EditBox>("0")->getRenderer();
		if (file_name_renderer) file_name_renderer->setTextSize(std::min(buttonWidth, buttonHeight) * 0.2);
	}
}

void TabCodeEditor::UpdateOnFrame()
{
	// Пустая реализация
}

void TabCodeEditor::Show()
{
	tab.get<tgui::Button>("OpenCode_OpenCR")->setVisible(true);
	tab.get<tgui::Button>("OpenCode_RunCode")->setVisible(true);
	tab.get<tgui::Button>("OpenCode_AddFile")->setVisible(true);
	tab.get<tgui::ScrollablePanel>("OpenCode_FileTree")->setVisible(true);
	tab.get<tgui::TabContainer>("OpenCode_FileContainer")->setVisible(true);
}

void TabCodeEditor::Hide()
{
	tab.get<tgui::Button>("OpenCode_OpenCR")->setVisible(false);
	tab.get<tgui::Button>("OpenCode_RunCode")->setVisible(false);
	tab.get<tgui::Button>("OpenCode_AddFile")->setVisible(false);
	tab.get<tgui::ScrollablePanel>("OpenCode_FileTree")->setVisible(false);
	tab.get<tgui::TabContainer>("OpenCode_FileContainer")->setVisible(false);
}