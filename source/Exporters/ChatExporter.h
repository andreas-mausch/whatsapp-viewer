#pragma once

#include <string>

class ChatExporter
{
public:
	ChatExporter() {}
	virtual ~ChatExporter() {}

	virtual void exportChat(const std::string &filename) = 0;
};
