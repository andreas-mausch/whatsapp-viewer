var loadDatabase = document.getElementById("load-database");
var sqlite3 = require('sqlite3').verbose();

var database = null;

var clearChats = function() {
    var chats = document.getElementById("chats");
    while (chats.firstChild) {
        chats.removeChild(chats.firstChild);
    }
};

var clearMessages = function() {
    var chats = document.getElementById("messages");
    while (chats.firstChild) {
        chats.removeChild(chats.firstChild);
    }
};

var loadChat = function(key) {
    clearMessages();
    var messages = document.getElementById("messages");

    database.each(`SELECT *
        FROM messages
        WHERE key_remote_jid = ?
        ORDER BY timestamp ASC`,
        key,
        function(error, row) {
            var div = document.createElement('div');
            div.className = 'whatsapp-message';
            var text = document.createTextNode(row.data);
            div.appendChild(text);
            messages.appendChild(div);
        });
};

loadDatabase.addEventListener('click', function() {
    clearChats();
    var chats = document.getElementById("chats");
    database = new sqlite3.Database('./database/msgstore.db', sqlite3.OPEN_READONLY);

    database.each(`SELECT *
    FROM chat_list`, function(error, row) {
        var div = document.createElement('div');
        div.className = 'whatsapp-chat';

        var caption = document.createElement('span');
        caption.className = 'whatsapp-chat-caption';

        var a = document.createElement('a');
        var linkText = document.createTextNode(row.key_remote_jid);
        a.appendChild(linkText);
        a.title = "row.key_remote_jid";
        a.addEventListener('click', function() {
            loadChat(row.key_remote_jid);
        });
        caption.appendChild(a);

        var lastMessage = document.createElement('span');
        lastMessage.className = 'whatsapp-chat-last-message';
        lastMessage.innerText = '';

        div.appendChild(caption);
        div.appendChild(lastMessage);

        chats.appendChild(div);
    });
});
