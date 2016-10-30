var loadDatabase = document.getElementById("load-database");
var sqlite3 = require('sqlite3').verbose();
loadDatabase.addEventListener('click', function() {
    var database = new sqlite3.Database('./database/msgstore.db', sqlite3.OPEN_READONLY);

    database.each("SELECT * FROM chat_list", function(error, row) {
        console.log('row.key_remote_jid: ' + row.key_remote_jid);
        console.log('row.subject: ' + row.subject);
        console.log('row.creation: ' + row.creation);
    });

    database.close();
});
