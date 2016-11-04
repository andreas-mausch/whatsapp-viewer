var loadDatabase = document.getElementById("load-database");
var sqlite3 = require('sqlite3').verbose();
// this is discouraged however there doesn't seem to be a solution for npm without webpack/browserify
// https://vuejs.org/guide/installation.html
var Vue = require('vue/dist/vue.js');
var moment = require('moment');
var base64 = require('base64-js');

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
          var message = {
            data: row.data,
            fromMe: row.key_from_me,
            status: row.status,
            timestamp: moment(row.timestamp),
            remoteResource: row.remote_resource,
            rawData: row.raw_data
          };

          if (row.latitude != 0 || row.longitude != 0) {
            message.location = {
              latitude: row.latitude,
              longitude: row.longitude
            };
          }

          if (row.media_url || row.media_mime_type || row.media_wa_type != 0 ||
            row.media_size != 0 || row.media_name || row.media_duration != 0 || row.thumb_image) {
            message.media = {
              url: row.media_url,
              mimeType: row.media_mime_type,
              whatsappType: row.media_wa_type,
              size: row.media_size,
              name: row.media_name,
              duration: row.media_duration,
              thumbImage: row.thumb_image
            };
          }


          messages.push(message);
        });
    },
    toBase64: function(image) {
      return 'data:image/jpeg;base64,' + base64.fromByteArray(image);
    }
  }
});
