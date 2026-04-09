#pragma once
#pragma execution_character_set("utf-8")

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

// Класс для управления объектами графического интерфейса
class UIManager
{
private:

	sf::ContextSettings settings;	// Настройки окна приложения
	sf::RenderWindow window;		// Окно приложения
	uint8_t curr_tab_id;			// Номер текущей страницы
	uint8_t prev_tab_id;			// Номер предыдущей страницы

	std::vector<std::unique_ptr<tgui::Gui>> screens_list;	// Массив всех окон программы
	class ABCTab;// Предобъявление класса
	std::vector<std::unique_ptr<ABCTab>> tabs_list;			// Массив всех страниц программы

public:
	// Конструктор класса
	UIManager();
	// Деструктор класса
	~UIManager() = default;

	// Функция для создания всех элементов страниц
	void CreateUI();
	// Основной цикл работы графического интерфейса
	void Run();

private:
	/*==================================================*/
	/*		Классы страниц графического интерфейса		*/
	/*==================================================*/
	
	// Абстрактный базовый класс страницы
	class ABCTab
	{
	public:
		std::unique_ptr<tgui::Gui> tab;		// Указатель на страницу
		virtual ~ABCTab() = default;
		// Функция создания страницы
		virtual void CreateTab(sf::RenderWindow& window) = 0;
		// Функция обновления при изменении размера
		virtual void UpdateOnResize(const sf::Vector2u& window_size) = 0;
		// Функция обновления в кадре
		virtual void UpdateOnFrame() = 0;
	};

	// Главное меню
	class TabMainMenu :public ABCTab
	{
		void CreateTab(sf::RenderWindow& window) override;
		void UpdateOnResize(const sf::Vector2u& window_size) override;
		void UpdateOnFrame() override;
	};

	// Экран просмотра курсов
	class TabReadCourse :public ABCTab
	{
		void CreateTab(sf::RenderWindow& window) override;
		void UpdateOnResize(const sf::Vector2u& window_size) override;
		void UpdateOnFrame() override;
	};

	// Экран создания курсов
	class TabCreateCourse :public ABCTab
	{
		void CreateTab(sf::RenderWindow& window) override;
		void UpdateOnResize(const sf::Vector2u& window_size) override;
		void UpdateOnFrame() override;
	};

	// Экран аккаунта
	class TabAccount :public ABCTab
	{
		void CreateTab(sf::RenderWindow& window) override;
		void UpdateOnResize(const sf::Vector2u& window_size) override;
		void UpdateOnFrame() override;
	};

	// Экран настроек
	class TabSettings :public ABCTab
	{
		void CreateTab(sf::RenderWindow& window) override;
		void UpdateOnResize(const sf::Vector2u& window_size) override;
		void UpdateOnFrame() override;
	};

	// Экран редактора кода
	class TabCodeEditor :public ABCTab
	{
		void CreateTab(sf::RenderWindow& window) override;
		void UpdateOnResize(const sf::Vector2u& window_size) override;
		void UpdateOnFrame() override;
	};


	/*======================================================*/
	/*			Вспомогательные классы элементов			*/
	/*======================================================*/
	
	// Функция создания кнопки
	static tgui::Button::Ptr CreateButton(const std::string& name, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

	// Функция для обновления параметров кнопки
	void updateButtonSizes(tgui::Gui* gui_ptr, const sf::Vector2u& windowSize);

	
};