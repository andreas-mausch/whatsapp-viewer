// npm install node-aes-gcm zlib

var fs = require('fs');
var gcm = require('node-aes-gcm');
var zlib = require('zlib');

if (process.argv.length != 5) {
  console.log('Usage: node ./decrypt-crypt12.js ./msgstore.db.crypt12 ./key ./msgstore.db');
  process.exit(-1);
}

var databaseFilename = process.argv[2];
var keyFilename = process.argv[3];
var outputFilename = process.argv[4];

var crypt12 = fs.readFileSync(databaseFilename);
var keyfile = fs.readFileSync(keyFilename);

var iv = crypt12.slice(51, 67);
var key = keyfile.slice(126, 158);
var crypted = crypt12.slice(67, crypt12.length - 20);

console.log('Key: ', key);
console.log('Initialization vector: ', iv);
console.log('crypted: ', crypted);

var decryptedGzip = gcm.decrypt(key, iv, crypted, new Buffer([]), new Buffer(16)).plaintext;
console.log('decrypted (gzipped): ', decryptedGzip);

var decrypted = zlib.inflateSync(decryptedGzip);
console.log('decrypted: ', decrypted);

fs.writeFile(outputFilename, decrypted, { flag: 'wx' }, function (err) {
    if (err) throw err;
    console.log();
    console.log('written to ', outputFilename);
});

