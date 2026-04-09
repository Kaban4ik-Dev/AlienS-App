#pragma execution_character_set("utf-8")
#include "app.hpp"

App::App() : user_interface()
{

}

void App::Run()
{
	user_interface.CreateUI();
	user_interface.Run();
}