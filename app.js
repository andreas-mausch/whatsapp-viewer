var loadDatabase = document.getElementById("load-database");
var sqlite3 = require('sqlite3').verbose();
var Ractive = require('ractive');

var database = null;
var model = {
  chats: [],
  messages: []
};

var ractive = new Ractive({
  el: '#container',
  template: '#template',
  magic: true,
  modifyArrays: true,
  data: model
});

ractive.on('load-database', function(event) {
  model.chats.length = 0;
  database = new sqlite3.Database('./database/msgstore.db', sqlite3.OPEN_READONLY);

  database.each('SELECT * FROM chat_list', function(error, row) {
    model.chats.push({
      key: row.key_remote_jid
    });
  });
});

ractive.on('load-chat', function(event, key) {
  model.messages.length = 0;
  database.each(`SELECT *
        FROM messages
        WHERE key_remote_jid = ?
        ORDER BY timestamp ASC`,
    key,
    function(error, row) {
      model.messages.push(row.data);
    });
});
