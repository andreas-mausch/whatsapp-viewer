var loadDatabase = document.getElementById("load-database");
var sqlite3 = require('sqlite3').verbose();
// this is discouraged however there doesn't seem to be a solution for npm without webpack/browserify
// https://vuejs.org/guide/installation.html
var Vue = require('vue/dist/vue.js');
var moment = require('moment');
var base64 = require('base64-js');

var database = null;

var messageTypes = {
  TEXT: 0,
  IMAGE: 1,
  AUDIO: 2,
  VIDEO: 3,
  CONTACT: 4,
  LOCATION: 5
};

var thumbnailMixin = {
  methods: {
    containsImage: function(message) {
      return message.rawData != null && message.rawData.length > 0;
    },
    toBase64: function(image) {
      return 'data:image/jpeg;base64,' + base64.fromByteArray(image);
    }
  }
};

Vue.component('message', {
  props: ['value'],
  template: `<div>
               <message-text v-if="value.type == messageTypes.TEXT" :value="value"/>
               <message-image v-if="value.type == messageTypes.IMAGE" :value="value"/>
               <message-audio v-if="value.type == messageTypes.AUDIO" :value="value"/>
               <message-video v-if="value.type == messageTypes.VIDEO" :value="value"/>
               <message-contact v-if="value.type == messageTypes.CONTACT" :value="value"/>
               <message-location v-if="value.type == messageTypes.LOCATION" :value="value"/>
             </div>`,
  data: function() {
    return {
      messageTypes: messageTypes
    }
  }
});

Vue.component('message-text', {
  props: ['value'],
  template: `<span>{{value.data}}</span>`
});

Vue.component('message-image', {
  mixins: [thumbnailMixin],
  props: ['value'],
  template: `<div><img v-if="containsImage(value)" :src="toBase64(value.rawData)"></div>`
});

Vue.component('message-audio', {
  props: ['value'],
  template: `<span>
               Audio {{value.media.duration}}s
               <span v-if="value.media.name">: {{value.media.name}}</span>
             </span>`
});

Vue.component('message-video', {
  mixins: [thumbnailMixin],
  props: ['value'],
  template: `<div><img v-if="containsImage(value)" :src="toBase64(value.rawData)"></div>`
});

Vue.component('message-contact', {
  props: ['value'],
  template: `<span>Contact</span>`
});

Vue.component('message-location', {
  mixins: [thumbnailMixin],
  props: ['value'],
  template: `<div>
               <div><img v-if="containsImage(value)" :src="toBase64(value.rawData)"></div>
               <div>{{value.location.latitude}},{{value.location.longitude}}</div>
             </div>`
});

var app = new Vue({
  el: '#container',
  data: {
    chats: [],
    messages: []
  },
  methods: {
    loadDatabase: function() {
      this.chats = [];
      var chats = this.chats;
      database = new sqlite3.Database('./database/msgstore.db', sqlite3.OPEN_READONLY);

      database.each('SELECT * FROM chat_list', function(error, row) {
        chats.push({
          key: row.key_remote_jid
        });
      });
    },
    loadChat: function(key) {
      this.messages = [];
      var messages = this.messages;
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
            type: row.media_wa_type,
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
              size: row.media_size,
              name: row.media_name,
              duration: row.media_duration,
              thumbImage: row.thumb_image
            };
          }


          messages.push(message);
        });
    }
  }
});
