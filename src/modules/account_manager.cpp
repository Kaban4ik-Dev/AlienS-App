#include "account_manager.hpp"

AccountManager::AccountManager()
{
	data.username = "Examle name";
	data.full_name = "Examle full name";
	data.phone_number = "Phone number";
	data.email = "Email";
	data.password = "Password";

	std::vector<bool> completed_topics;
	completed_topics.push_back(true);
	data.completedTopics.insert({ "Example Topic", completed_topics });

	std::map<int, bool> test_results;
	test_results.insert({ 1, true });
	data.testResults.insert({ "Examle Test", test_results});
}

std::error_code& AccountManager::GetUserData(UserData* return_data)
{
	return_data = &data;
	return std::error_code(0, std::generic_category());
}

std::error_code& AccountManager::TrySetUserData(const UserData& new_data)
{
    return std::error_code(0, std::generic_category());
}

std::error_code& AccountManager::SubmitUserData(const UserData& new_data)
{
    return std::error_code(0, std::generic_category());
}