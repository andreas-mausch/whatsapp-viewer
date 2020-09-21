# WhatsApp Viewer

Small tool to display chats from the Android msgstore.db database.

## How to use

1. **You need root access to your phone.** If you don't know what it is: [Wikipedia](https://en.wikipedia.org/wiki/Rooting_%28Android%29)
2. Download your WhatsApp database and key files:
  - /data/data/com.whatsapp/files/key
  - /data/data/com.whatsapp/databases/msgstore.db
  - /data/data/com.whatsapp/databases/wa.db
3. Open WhatsApp Viewer
4. File -> Open -> Select file
5. Select msgstore.db in the folder "extracted"
6. Leave account name empty, is was used for older versions of WhatsApp (crypt5)
7. Optional: If you want, you can import contact names from the wa.db file

## Command line support

```bash
"WhatsApp Viewer.exe" -decrypt12 msgstore-2016-10-22.1.db.crypt12 whatsapp.cryptkey12 decrypted.db
```

You cannot see bigger images because only thumbnails are stored in the database.

## Build

See *build-release.cmd*.

## Credits

* TripCode for crypt12 support https://github.com/EliteAndroidApps/WhatsApp-Crypt12-Decrypter
* Whatsapp Xtract
* pwncrypt5.py https://github.com/aramosf/pwncrypt5/blob/master/pwncrypt5.py
* SQLite
* MD5 http://bobobobo.wordpress.com/2010/10/17/md5-c-implementation/
* UTF8 http://utfcpp.sourceforge.net/
* AES https://polarssl.org/aes-source-code
* Base64 http://www.adp-gmbh.ch/cpp/common/base64.html
* Zlib http://www.zlib.net/
* RapidJSON http://rapidjson.org/
* mbedtls https://github.com/ARMmbed/mbedtls
