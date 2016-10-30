const {
    app,
    BrowserWindow
} = require('electron');

let mainWindow;

app.on('ready', () => {
    mainWindow = new BrowserWindow({
        height: 720,
        width: 1024
    });

    mainWindow.loadURL('file://' + __dirname + '/index.html');
});
