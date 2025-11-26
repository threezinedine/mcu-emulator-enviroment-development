import { app, BrowserWindow } from "electron";

function createWindow() {
    const window = new BrowserWindow({
        width: 800,
        height: 600,
        webPreferences: {
            nodeIntegration: false,
            contextIsolation: true,
            webSecurity: true,
            enableWebSQL: false,
        },
    });

    window.maximize();
    window.loadFile("../assets/index.html");
    window.once("ready-to-show", () => {
        window.show();
    });

    window.webContents.openDevTools();
}

app.whenReady().then(() => {
    createWindow();
});

app.on("window-all-closed", () => {
    if (process.platform !== "darwin") {
        app.quit();
    }
});
