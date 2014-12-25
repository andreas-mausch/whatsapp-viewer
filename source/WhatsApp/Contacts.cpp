#include <string>

#include "Contacts.h"
#include "../Settings.h"
#include "../Exceptions/SQLiteException.h"
#include "../Libraries/SQLite/SQLiteDatabase.h"
#include "../Libraries/SQLite/sqlite3.h"

void importContacts(Settings &settings, const std::string &wadbFilename)
{
	SQLiteDatabase wadb(wadbFilename);

	const char *query = "SELECT jid, display_name, status " \
						"FROM wa_contacts";

	sqlite3_stmt *res;
	if (sqlite3_prepare_v2(wadb.getHandle(), query, -1, &res, NULL) != SQLITE_OK)
	{
		throw SQLiteException("Could not load contacts list", wadb);
	}

	while (sqlite3_step(res) == SQLITE_ROW)
	{
		std::string jid = wadb.readString(res, 0);
		std::string displayName = wadb.readString(res, 1);
		std::string status = wadb.readString(res, 2);

		if (displayName.length() > 0)
		{
			settings.write("Contacts/" + jid, displayName);
		}
	}

	sqlite3_finalize(res);
}
