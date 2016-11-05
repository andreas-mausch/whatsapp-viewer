const {
  app,
  BrowserWindow,
  Menu
} = require('electron');

let mainWindow;

app.on('ready', () => {
  const template = [{
    label: 'File',
    submenu: [{
      label: 'Open',
      click() {}
    }]
  }];
  const menu = Menu.buildFromTemplate(template);
  Menu.setApplicationMenu(menu);

  mainWindow = new BrowserWindow({
    height: 720,
    width: 1024
  });

  mainWindow.loadURL('file://' + __dirname + '/index.html');
});
