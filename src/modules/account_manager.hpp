#pragma once

#include <string>
#include <map>
#include <iostream>
#include <system_error>
#include <vector>


// Структура данных о пользователе
struct UserData
{
	std::string username;		// Имя аккаунта
	std::string full_name;		// ФИО
	std::string phone_number;	// Номер телефона
	std::string email;			// Почта
	std::string password;		// Пароль

	// Прогресс по курсам
	std::map<std::string, std::vector<bool>> completedTopics;  // ID курса - массив пройденных тем
	std::map<std::string, std::map<int, bool>> testResults;    // ID курса - массив ответов и корректности
};

// Класс-синглтон для управления данными пользователя
class AccountManager
{
private:
	UserData data; // Данные пользователя

public:
	AccountManager();
	~AccountManager() = default;

	// Получить данные пользователя
	std::error_code& GetUserData(UserData* return_data);
	// Проверить возможность установки данных пользователя
	std::error_code& TrySetUserData(const UserData& new_data);
	// Подтвердить установку данных пользователя
	std::error_code& SubmitUserData(const UserData& new_data);
};