#pragma once
#pragma execution_character_set("utf-8")

#include "user_interface.hpp"
#include "../modules/course_manger.hpp"
#include "../modules/code_exec_manager.hpp"

class App
{
private:
	CourseManager course_manager;
	CodeExecManager code_exec_manager;
	UIManager user_interface;

public:
	App();
	~App() = default;

	void Run();
};

