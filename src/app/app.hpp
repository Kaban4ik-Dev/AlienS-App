#pragma once
#pragma execution_character_set("utf-8")

#include "user_interface.hpp"

class App
{
private:
	UIManager user_interface;

public:
	App();
	~App() = default;

	void Run();
};

