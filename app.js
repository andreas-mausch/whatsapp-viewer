var loadDatabase = document.getElementById("load-database");
var sqlite3 = require('sqlite3').verbose();
// this is discouraged however there doesn't seem to be a solution for npm without webpack/browserify
// https://vuejs.org/guide/installation.html
var Vue = require('vue/dist/vue.js');

var database = null;

var app = new Vue({
  el: '#container',
  data: {
    chats: [],
    messages: []
  },
  methods: {
    loadDatabase: function() {
      var chats = this.chats;
      chats.length = 0;
      database = new sqlite3.Database('./database/msgstore.db', sqlite3.OPEN_READONLY);

      database.each('SELECT * FROM chat_list', function(error, row) {
        chats.push({
          key: row.key_remote_jid
        });
      });
    },
    loadChat: function(key) {
      var messages = this.messages;
      messages.length = 0;
      database.each(`SELECT *
            FROM messages
            WHERE key_remote_jid = ?
            ORDER BY timestamp ASC`,
        key,
        function(error, row) {
          messages.push(row.data);
        });
    }
  }
});
