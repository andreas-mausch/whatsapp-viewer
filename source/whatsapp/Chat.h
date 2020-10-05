#pragma once

#include <string>

namespace WhatsApp
{

class Chat
{
private:
    std::string id;

public:
    Chat(const std::string &id);

	std::string getId();
};

}
