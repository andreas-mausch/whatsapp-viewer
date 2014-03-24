WhatsApp Viewer

Small tool to display chats from the Android msgstore.db.crypt5 database.

How to use:
1. Download msgstore.db.crypt5 from your Android phone (located at /sdcard/WhatsApp/Databases)
2. Open WhatsApp Viewer
3. File -> Open -> Select file
4. Enter account name. This must be the same Google account name used on your phone. If you have more than one account, try the first one. Make sure you enter the whole email address as account name. If you have trouble, sometimes leaving the account name empty is known to work.
5. Click on a chat to show the messages.

Currently only text and image messages are supported (no audio, gps).
You cannot see bigger images because only thumbnails are stored in the database.
You see cryptic phone numbers because account names or details are not stored in the database (no support for wa.db yet).

This release is not stable and known to crash sometimes. Sorry.

Credits:
	Whatsapp Xtract
	pwncrypt5.py https://github.com/aramosf/pwncrypt5/blob/master/pwncrypt5.py
	SQLite
	MD5 http://bobobobo.wordpress.com/2010/10/17/md5-c-implementation/
	UTF8 http://utfcpp.sourceforge.net/
	AES https://polarssl.org/aes-source-code
