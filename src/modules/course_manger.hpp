#pragma once

#include <string>
#include <vector>


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

	uint8_t id;			// Числовой ID темы внутри курса
	std::string title;	// Название темы
	TopicType type;		// Тип темы
};

// Структура данных темы в курсе
struct TopicData : ABCTopic
{
	std::vector<TopicBlock> blocks;	// Содержимое темы
};

// Структура метаданных темы в курсе
struct TopicMeta : ABCTopic
{
	bool unlocked = false;	// Флаг доступности к прохождению темы пользователем
	std::string file_path;	// Путь внутри курса до файла с темой
};

// Структура метаданных курса
struct CourseMeta
{
	std::string name;				// Название курса
	std::string id;					// Уникальный идентификатор курса
	std::vector<TopicMeta> content;	// Список метаданных тем
};

// Структура метаданных обо всех курсах
struct CoursesDatabase
{
	std::vector<std::string> courses_paths;	// Пути до курсов (названия папок)
	std::vector<CourseMeta> courses_data;	// Массив метаданных обо всех курсах
};

class CourseManager
{
private:

	CoursesDatabase course_list; // Список всех курсов

public:
	// Конструктор класса
	CourseManager();
	// Деструктор класса
	~CourseManager() = default;

	// Геттер списка всех курсов
	inline const CoursesDatabase& GetCourseList() const noexcept { return course_list; }
	// Чтение содержимого темы из курса
	const TopicData& GetTopicData(const std::string& course_path, const std::string& topic_path);
};