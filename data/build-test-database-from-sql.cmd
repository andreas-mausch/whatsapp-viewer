@echo off

del test-database.db
del test-database-large.db

echo Building test-database.db...
sqlite3 test-database.db < test-database.sql
echo Building test-database-large.db...
sqlite3 test-database-large.db < test-database-large.sql
