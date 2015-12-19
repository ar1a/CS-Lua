#include "Console.h"
#include <iostream>
#include <sstream>

ConsoleManager::ConsoleManager()
{

}

void ConsoleManager::Think()
{

	std::string cmd;
	std::getline(std::cin, cmd);
	if (cmd.empty())
		return;
	auto args = split(cmd, ' ');
	for (auto it : m_commands) {
		if (it.first == args[0])
		{
			it.second(args);
		}
	}

}

void ConsoleManager::AddItem(std::string command, std::function<void(std::vector<std::string>)> callback)
{
	m_commands.insert(std::make_pair(command, callback));
}

std::vector<std::string> ConsoleManager::split(std::string& str, char delimiter)
{
	std::vector<std::string> internal;
	std::stringstream ss(str);
	std::string tok;

	while (getline(ss, tok, delimiter))
		internal.push_back(tok);

	return internal;
}