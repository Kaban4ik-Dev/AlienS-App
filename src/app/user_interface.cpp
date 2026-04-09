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
	// Создать место под 6 окон:
	// Главное меню, Открыть курс, Создать курс, Аккаунт, Настройки, Исполнитель кода
	screens_list.reserve(6);
	// Установить текущую страницу на главный экран
	curr_tab_id = 0;

	// Определение главного меню (элемент 0)
	tabs_list.emplace_back(std::make_unique<TabMainMenu>());
	
}

tgui::Button::Ptr UIManager::CreateButton(const std::string& name, uint16_t x, uint16_t y, uint16_t width, uint16_t height)
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

// Функция для обновления размеров и позиций кнопок
void UIManager::updateButtonSizes(tgui::Gui* gui_ptr, const sf::Vector2u& windowSize)
{
	float screenWidth = static_cast<float>(windowSize.x);
	float screenHeight = static_cast<float>(windowSize.y);

	// Рассчитываем размеры кнопок относительно размера экрана
	float buttonWidth = screenWidth * 0.2f;		// 20% от ширины экрана
	float buttonHeight = screenHeight * 0.12f;	// 12% от высоты экрана
	float buttonSpacing = screenHeight * 0.04f;	// Отступ между кнопками
	float startX = screenWidth * 0.4f;			// 40% от левого края
	float startY = screenHeight * 0.12f;		// 12% от верхнего края

	// Обновляем кнопки по их именам
	if (auto button = gui_ptr->get<tgui::Button>("Открыть курсы"))
	{
		button->setSize({ buttonWidth, buttonHeight });
		button->setPosition({ startX, startY });
	}

	if (auto button = gui_ptr->get<tgui::Button>("Создать курс"))
	{
		button->setSize({ buttonWidth, buttonHeight });
		button->setPosition({ startX, startY + buttonHeight + buttonSpacing });
	}

	if (auto button = gui_ptr->get<tgui::Button>("Аккаунт"))
	{
		button->setSize({ buttonWidth, buttonHeight });
		button->setPosition({ startX, startY + (buttonHeight + buttonSpacing) * 2 });
	}

	if (auto button = gui_ptr->get<tgui::Button>("Настройки"))
	{
		button->setSize({ buttonWidth, buttonHeight });
		button->setPosition({ startX, startY + (buttonHeight + buttonSpacing) * 3 });
	}

	if (auto button = gui_ptr->get<tgui::Button>("Выход"))
	{
		button->setSize({ buttonWidth, buttonHeight });
		button->setPosition({ startX, startY + (buttonHeight + buttonSpacing) * 4 });
	}
}

void UIManager::CreateUI()
{
	tabs_list[0]->CreateTab(window);
	
	//for (int i = 0; i < 6; ++i)
	//{
	//	screens_list.emplace_back(std::make_unique<tgui::Gui>(window));
	//}
	//
	//// Создаем canvas для фона
	//auto backgroundCanvas = tgui::CanvasSFML::create({ 800, 600 });
	//backgroundCanvas->setSize("100%", "100%"); // Растягиваем на всё окно
	//backgroundCanvas->setPosition(0, 0);

	//// Рисуем на canvas
	//backgroundCanvas->clear(sf::Color(50, 50, 80)); // Очищаем цветом фона

	//// Рисуем геометрию на canvas
	//backgroundCanvas->draw(sf::RectangleShape(sf::Vector2f(800, 300))); // Ошибка - нужен другой подход

	//backgroundCanvas->display(); // Обязательно вызвать после рисования
	//
	//// Объект UI главного меню
	//tgui::Gui main_menu(window);

	//// Добавляем canvas в GUI
	//screens_list[0]->add(backgroundCanvas);

	//// Создаём кнопки с именами
	//auto open_courses_tab = CreateButton("Открыть курсы", 20, 20, 200, 80);
	//auto create_course_tab = CreateButton("Создать курс", 20, 120, 200, 80);
	//auto account_tab = CreateButton("Аккаунт", 20, 220, 200, 80);
	//auto settings_tab = CreateButton("Настройки", 20, 320, 200, 80);
	//auto exit = CreateButton("Выход", 20, 420, 200, 80);

	//screens_list[0]->add(open_courses_tab, "Открыть курсы");
	//screens_list[0]->add(create_course_tab, "Создать курс");
	//screens_list[0]->add(account_tab, "Аккаунт");
	//screens_list[0]->add(settings_tab, "Настройки");
	//screens_list[0]->add(exit, "Выход");
}

void UIManager::Run()
{
	//// Изначально устанавливаем размеры
	//updateButtonSizes(screens_list[curr_tab_id].get(), window.getSize());

	//// Основной цикл работы программы
	//while (window.isOpen())
	//{
	//	// Цикл обработки событий экрана
	//	while (const std::optional event = window.pollEvent())
	//	{
	//		// Обработка изменения размера окна
	//		if (const auto* resized = event->getIf<sf::Event::Resized>())
	//		{
	//			// Обновляем размеры кнопок
	//			updateButtonSizes(screens_list[curr_tab_id].get(), resized->size);
	//		}

	//		screens_list[curr_tab_id]->handleEvent(*event);

	//		if (event->is<sf::Event::Closed>())
	//			window.close();
	//	}

	//	window.clear();
	//	screens_list[curr_tab_id]->draw();
	//	window.display();
	//}

	// Изначально устанавливаем размеры
	tabs_list[0]->UpdateOnResize(window.getSize());

	// Основной цикл работы программы
	while (window.isOpen())
	{
		// Цикл обработки событий экрана
		while (const std::optional event = window.pollEvent())
		{
			// Обработка изменения размера окна
			if (const auto* resized = event->getIf<sf::Event::Resized>())
			{
				// Обновляем размеры кнопок
				tabs_list[0]->UpdateOnResize(resized->size);
			}

			tabs_list[0]->tab->handleEvent(*event);

			if (event->is<sf::Event::Closed>())
				window.close();
		}

		window.clear();
		tabs_list[0]->tab->draw();
		window.display();
	}
}



/*==================================================*/
/*		Классы страниц графического интерфейса		*/
/*==================================================*/

void UIManager::TabMainMenu::CreateTab(sf::RenderWindow& window)
{
	tab = std::make_unique<tgui::Gui>(window);

	// Создаем canvas для фона
	auto backgroundCanvas = tgui::CanvasSFML::create({ 800, 600 });
	backgroundCanvas->setSize("100%", "100%"); // Растягиваем на всё окно
	backgroundCanvas->setPosition(0, 0);

	// Рисуем на canvas
	backgroundCanvas->clear(sf::Color(50, 50, 80)); // Очищаем цветом фона

	// Рисуем геометрию на canvas
	backgroundCanvas->draw(sf::RectangleShape(sf::Vector2f(800, 300))); // Ошибка - нужен другой подход

	backgroundCanvas->display(); // Обязательно вызвать после рисования

	// Добавляем canvas в GUI
	tab->add(backgroundCanvas);

	// Создаём кнопки с именами
	auto open_courses_tab = CreateButton("Открыть курсы", 20, 20, 200, 80);
	auto create_course_tab = CreateButton("Создать курс", 20, 120, 200, 80);
	auto account_tab = CreateButton("Аккаунт", 20, 220, 200, 80);
	auto settings_tab = CreateButton("Настройки", 20, 320, 200, 80);
	auto exit = CreateButton("Выход", 20, 420, 200, 80);

	tab->add(open_courses_tab, "Открыть курсы");
	tab->add(create_course_tab, "Создать курс");
	tab->add(account_tab, "Аккаунт");
	tab->add(settings_tab, "Настройки");
	tab->add(exit, "Выход");
}

void::UIManager::TabMainMenu::UpdateOnResize(const sf::Vector2u& window_size)
{
	float screenWidth = static_cast<float>(window_size.x);
	float screenHeight = static_cast<float>(window_size.y);

	// Рассчитываем размеры кнопок относительно размера экрана
	float buttonWidth = screenWidth * 0.2f;		// 20% от ширины экрана
	float buttonHeight = screenHeight * 0.12f;	// 12% от высоты экрана
	float buttonSpacing = screenHeight * 0.04f;	// Отступ между кнопками
	float startX = screenWidth * 0.4f;			// 40% от левого края
	float startY = screenHeight * 0.12f;		// 12% от верхнего края

	// Обновляем кнопки по их именам
	if (auto button = tab->get<tgui::Button>("Открыть курсы"))
	{
		button->setSize({ buttonWidth, buttonHeight });
		button->setPosition({ startX, startY });
	}

	if (auto button = tab->get<tgui::Button>("Создать курс"))
	{
		button->setSize({ buttonWidth, buttonHeight });
		button->setPosition({ startX, startY + buttonHeight + buttonSpacing });
	}

	if (auto button = tab->get<tgui::Button>("Аккаунт"))
	{
		button->setSize({ buttonWidth, buttonHeight });
		button->setPosition({ startX, startY + (buttonHeight + buttonSpacing) * 2 });
	}

	if (auto button = tab->get<tgui::Button>("Настройки"))
	{
		button->setSize({ buttonWidth, buttonHeight });
		button->setPosition({ startX, startY + (buttonHeight + buttonSpacing) * 3 });
	}

	if (auto button = tab->get<tgui::Button>("Выход"))
	{
		button->setSize({ buttonWidth, buttonHeight });
		button->setPosition({ startX, startY + (buttonHeight + buttonSpacing) * 4 });
	}
}

void UIManager::TabMainMenu::UpdateOnFrame()
{

}