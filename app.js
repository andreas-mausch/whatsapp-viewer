var loadDatabase = document.getElementById("load-database");
var sqlite3 = require('sqlite3').verbose();

var clearChats = function() {
    var chats = document.getElementById("chats");
    while (chats.firstChild) {
        chats.removeChild(chats.firstChild);
    }
}

loadDatabase.addEventListener('click', function() {
    clearChats();
    var chats = document.getElementById("chats");
    var database = new sqlite3.Database('./database/msgstore.db', sqlite3.OPEN_READONLY);

    database.each("SELECT * FROM chat_list", function(error, row) {
        var div = document.createElement('div');
        div.className = 'chat';

        var caption = document.createElement('span');
        caption.className = 'chat-caption';
        caption.innerText = row.key_remote_jid;

        var lastMessage = document.createElement('span');
        lastMessage.className = 'chat-last-message';
        lastMessage.innerText = '';

        div.appendChild(caption);
        div.appendChild(lastMessage);

        chats.appendChild(div);
    });

    database.close();
});
