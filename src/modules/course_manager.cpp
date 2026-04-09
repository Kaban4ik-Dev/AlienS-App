#include "course_manger.hpp"

CourseManager::CourseManager()
{
    CourseMeta course;
    course.name = "Introduction to C++ Programming";
    course.id = "CPP-101";

    // Тема 1: Введение в C++ (урок)
    TopicMeta topic1;
    topic1.id = 1;
    topic1.title = "Introduction to C++";
    topic1.type = ABCTopic::TopicType::Lesson;
    topic1.unlocked = true;  // Первая тема доступна сразу    topic1.file_path = "lessons/lesson1.bin";

    // Тема 2: Переменные и типы данных (урок)
    TopicMeta topic2;
    topic2.id = 2;
    topic2.title = "Variables and Data Types";
    topic2.type = ABCTopic::TopicType::Lesson;
    topic2.unlocked = false;
    topic2.file_path = "lessons/lesson2.bin";

    // Тема 3: Тест по переменным (тест)
    TopicMeta topic3;
    topic3.id = 3;
    topic3.title = "Quiz: Variables and Data Types";
    topic3.type = ABCTopic::TopicType::Test;
    topic3.unlocked = false;
    topic3.file_path = "tests/test1.bin";

    // Тема 4: Условные операторы (урок)
    TopicMeta topic4;
    topic4.id = 4;
    topic4.title = "Conditional Statements";
    topic4.type = ABCTopic::TopicType::Lesson;
    topic4.unlocked = false;
    topic4.file_path = "lessons/lesson3.bin";

    // Тема 5: Итоговый тест (тест)
    TopicMeta topic5;
    topic5.id = 5;
    topic5.title = "Final Exam: C++ Basics";
    topic5.type = ABCTopic::TopicType::Test;
    topic5.unlocked = false;
    topic5.file_path = "tests/final_test.bin";

    course.content.push_back(topic1);
    course.content.push_back(topic2);
    course.content.push_back(topic3);
    course.content.push_back(topic4);
    course.content.push_back(topic5);

    course_list.courses_data.push_back(course);
    course_list.courses_paths.push_back("no path");
}