#include <string>
#include <vector>
#include <windows.h>

#include "CLI.h"
#include "WhatsApp/Crypt5.h"
#include "WhatsApp/Crypt7.h"
#include "WhatsApp/Crypt8.h"

void displayUsage()
{
	const WCHAR *usage = L"Supported commands:\n\n"
		L"-decrypt5 [input database] [account name] [output filename]\n"
		L"example: -decrypt5 msgstore.db.crypt5 andreas.mausch@gmail.com msgstore.decrypted.db\n\n"
		L"-decrypt7 [input database] [key filename] [output filename]\n"
		L"example: -decrypt7 msgstore.db.crypt7 key msgstore.decrypted.db\n\n"
		L"-decrypt8 [input database] [key filename] [output filename]\n"
		L"example: -decrypt8 msgstore.db.crypt8 key msgstore.decrypted.db";

	MessageBox(NULL, usage, L"WhatsApp Viewer", MB_OK | MB_ICONINFORMATION);
}

void decrypt5(const std::vector<std::string *> arguments)
{
	if (arguments.size() != 5)
	{
		displayUsage();
		return;
	}
	
	std::string &databaseFilename = *arguments[2];
	std::string &accountName = *arguments[3];
	std::string &outputFilename = *arguments[4];

	decryptWhatsappDatabase5(databaseFilename, outputFilename, accountName);
}

void decrypt7(const std::vector<std::string *> arguments)
{
	if (arguments.size() != 5)
	{
		displayUsage();
		return;
	}

	std::string &databaseFilename = *arguments[2];
	std::string &keyFilename = *arguments[3];
	std::string &outputFilename = *arguments[4];
	decryptWhatsappDatabase7(databaseFilename, outputFilename, keyFilename);
}

void decrypt8(const std::vector<std::string *> arguments)
{
	if (arguments.size() != 5)
	{
		displayUsage();
		return;
	}

	std::string &databaseFilename = *arguments[2];
	std::string &keyFilename = *arguments[3];
	std::string &outputFilename = *arguments[4];
	decryptWhatsappDatabase8(databaseFilename, outputFilename, keyFilename);
}

bool handleCommandLineArguments(const std::vector<std::string *> arguments)
{
	if (arguments.size() > 1)
	{
		std::string &command = *arguments[1];
		if (command == "-decrypt5")
		{
			decrypt5(arguments);
			return true;
		}
		else if (command == "-decrypt7")
		{
			decrypt7(arguments);
			return true;
		}
		else if (command == "-decrypt8")
		{
			decrypt8(arguments);
			return true;
		}
	}

	return false;
}
