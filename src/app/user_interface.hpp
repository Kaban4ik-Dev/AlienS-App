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

// Предобъявления
class UIManager;

/*==================================================*/
/*		Классы страниц графического интерфейса		*/
/*==================================================*/

// Абстрактный базовый класс страницы
class ABCTab
{
protected:
	UIManager& ui_manager;	// Ссылка на менеджер UI
	tgui::Gui& tab;			// Поверхность UI
	uint8_t page_id;		// Номер страницы
public:
	// Конструктор базового класса принимает ссылку от наследников
	explicit ABCTab(UIManager& manager, tgui::Gui& tab, uint8_t id) : ui_manager(manager), tab(tab), page_id(id) {}
	virtual ~ABCTab() = default;
	// Функция создания страницы
	virtual void CreateTab(sf::RenderWindow& window) = 0;
	// Функция обновления при изменении размера
	virtual void UpdateOnResize(const sf::Vector2u& window_size) = 0;
	// Функция обновления в кадре
	virtual void UpdateOnFrame() = 0;

	// Отобразить элементы страницы
	virtual void Show() = 0;
	// Спрятать элементы текущей страницы
	virtual void Hide() = 0;
};

// Главное меню
class TabMainMenu :public ABCTab
{
public:
	// Конструктор TabMainMenu передаёт ссылку базовому классу
	explicit TabMainMenu(UIManager& manager, tgui::Gui& tab, uint8_t id) : ABCTab(manager, tab, id) {}
	// Функция создания страницы
	void CreateTab(sf::RenderWindow& window) override;
	// Функция обновления при изменении размера
	void UpdateOnResize(const sf::Vector2u& window_size) override;
	// Функция обновления в кадре
	void UpdateOnFrame() override;

	// Отобразить элементы страницы
	void Show();
	// Спрятать элементы текущей страницы
	void Hide();
};

// Экран просмотра курсов
class TabReadCourse :public ABCTab
{
public:
	// Конструктор TabReadCourse
	explicit TabReadCourse(UIManager& manager, tgui::Gui& tab, uint8_t id) : ABCTab(manager, tab, id) {}
	// Функция создания страницы
	void CreateTab(sf::RenderWindow& window) override;
	// Функция обновления при изменении размера
	void UpdateOnResize(const sf::Vector2u& window_size) override;
	// Функция обновления в кадре
	void UpdateOnFrame() override;

	// Отобразить элементы страницы
	void Show();
	// Спрятать элементы текущей страницы
	void Hide();
};

// Экран создания курсов
class TabCreateCourse :public ABCTab
{
public:
	// Конструктор TabCreateCourse
	explicit TabCreateCourse(UIManager& manager, tgui::Gui& tab, uint8_t id) : ABCTab(manager, tab, id) {}
	// Функция создания страницы
	void CreateTab(sf::RenderWindow& window) override;
	// Функция обновления при изменении размера
	void UpdateOnResize(const sf::Vector2u& window_size) override;
	// Функция обновления в кадре
	void UpdateOnFrame() override;

	// Отобразить элементы страницы
	void Show();
	// Спрятать элементы текущей страницы
	void Hide();
};

// Экран аккаунта
class TabAccount :public ABCTab
{
public:
	// Конструктор TabAccount
	explicit TabAccount(UIManager& manager, tgui::Gui& tab, uint8_t id) : ABCTab(manager, tab, id) {}
	// Функция создания страницы
	void CreateTab(sf::RenderWindow& window) override;
	// Функция обновления при изменении размера
	void UpdateOnResize(const sf::Vector2u& window_size) override;
	// Функция обновления в кадре
	void UpdateOnFrame() override;

	// Отобразить элементы страницы
	void Show();
	// Спрятать элементы текущей страницы
	void Hide();
};

// Экран настроек
class TabSettings :public ABCTab
{
public:
	// Конструктор TabSettings
	explicit TabSettings(UIManager& manager, tgui::Gui& tab, uint8_t id) : ABCTab(manager, tab, id) {}
	// Функция создания страницы
	void CreateTab(sf::RenderWindow& window) override;
	// Функция обновления при изменении размера
	void UpdateOnResize(const sf::Vector2u& window_size) override;
	// Функция обновления в кадре
	void UpdateOnFrame() override;

	// Отобразить элементы страницы
	void Show();
	// Спрятать элементы текущей страницы
	void Hide();
};

// Экран редактора кода
class TabCodeEditor :public ABCTab
{
private:
	static constexpr int POOL_SIZE = 16; // Размер пула файлов
	std::vector<bool> is_used = std::vector<bool>(POOL_SIZE, false); // Список занятости пула файлов

public:
	// Конструктор TabCodeEditor
	explicit TabCodeEditor(UIManager& manager, tgui::Gui& tab, uint8_t id) : ABCTab(manager, tab, id) {}
	// Функция создания страницы
	void CreateTab(sf::RenderWindow& window) override;
	// Функция обновления при изменении размера
	void UpdateOnResize(const sf::Vector2u& window_size) override;
	// Функция обновления в кадре
	void UpdateOnFrame() override;

	// Отобразить элементы страницы
	void Show();
	// Спрятать элементы текущей страницы
	void Hide();
};


/*==================================================================*/
/*		Класс для управления объектами графического интерфейса		*/
/*==================================================================*/
class UIManager
{
private:
	sf::ContextSettings settings;	// Настройки окна приложения
	sf::RenderWindow window;		// Окно приложения
	std::unique_ptr<tgui::Gui> tab;	// Текущая страница
	uint8_t curr_tab_id = 0;		// Номер текущей страницы
	uint8_t prev_tab_id = 0;		// Номер предыдущей страницы
	std::vector<std::unique_ptr<ABCTab>> pages;	// Вектор страниц
	std::shared_ptr<tgui::Font> regular_font;	// Обычный шрифт
	std::shared_ptr<tgui::Font> mono_font;		// Моноширинный шрифт
	
public:

	// Конструктор класса
	UIManager();
	// Деструктор класса
	~UIManager();

	// Геттер ID текущей страницы
	uint8_t GetCurrTabID() const noexcept { return curr_tab_id; }
	// Сеттер ID текущей страницы
	void SetCurrTabID(const uint8_t new_id) noexcept
	{
		prev_tab_id = curr_tab_id;
		curr_tab_id = new_id;
	}
	// Геттер ID предыдущей страницы
	uint8_t GetPrevTabID() const noexcept { return prev_tab_id; }

	// Геттер обычного шрифта
	std::shared_ptr<tgui::Font> GetRegularFont() { return regular_font; }

	// Геттер моноширинного шрифта
	std::shared_ptr<tgui::Font> GetMonoFont() { return mono_font; }

	// Функция для создания всех элементов страниц
	void CreateUI();
	// Основной цикл работы графического интерфейса
	void Run();

	void SwitchToPage(uint8_t new_tab_id);
	void UpdateOnResize(uint8_t tab_id, const sf::Vector2u& window_size);
};

/*======================================================*/
/*			Вспомогательные функции элементов			*/
/*======================================================*/

// Функция создания кнопки
tgui::Button::Ptr CreateButton(const std::string& name, uint16_t x, uint16_t y, uint16_t width, uint16_t height);