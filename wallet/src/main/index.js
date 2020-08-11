/*
 * This file is part of The UNIGRID Wallet
 * Copyright (C) 2019 The Swipp developers <info@swippcoin.com>
 * Copyright (C) 2020 The UNIGRID Organization
 *
 * The UNIGRID Wallet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * The UNIGRID Wallet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with The UNIGRID Wallet. If not, see <https://www.gnu.org/licenses/>.
 */

import { app, ipcMain, remote } from "electron";
import * as path from "path";
import { format as formatUrl } from "url";
import Daemon from "../common/daemon";
import RPCClient from "../common/rpc-client.js"
import AsteroidsController from "./asteroids-controller";
import MainController from "./main-controller";
import SplashController from "./splash-controller";
import { Notification } from "electron";
import request from 'request';
import { autoUpdater } from "electron-updater";

const { crashReporter } = require('electron');
const packageJSON = require('../../package.json');
const deps = packageJSON.dependencies;

autoUpdater.autoDownload = true;

process.on('uncaughtException', (err) => {
	console.log("uncaughtException: ", err)
	request.post('http://crashreports.unigrid.org/POST', {
		form: {
			ver: deps['electron-prebuilt'],
			platform: process.platform,
			process_type: remote ? 'renderer' : 'main',
			version: packageJSON.version,
			productName: packageJSON.productName,
			prod: 'Electron',
			companyName: 'UNIGRID Organization',
			upload_file_minidump: err.stack,
		}
	});
});

crashReporter.start({
	productName: 'UNIGRID Wallet',
	companyName: 'UNIGRID Organization',
	submitURL: 'http://crashreports.unigrid.org/POST',
	uploadToServer: true
});

global.isDevelopment = process.env.NODE_ENV !== "production";

if (app.getGPUFeatureStatus().gpu_compositing.includes("disabled")) {
	app.disableHardwareAcceleration();
}

app.on("window-all-closed", () => {
	if (global.rpcPort != undefined) {
		new RPCClient().stop();
	}

	app.quit();
});
ipcMain.on("wallet-restart", () => {
	console.log('calling relaunch app')
	if (global.rpcPort != undefined) {
		new RPCClient().stop();
	}
	app.relaunch();
	app.quit();
});

app.on("activate", () => {
	/*	if (mainWindow === null) {
			mainWindow = createMainWindow();
		}*/
});

ipcMain.on("open-asteroids", () => {
	var asteroidsController = new AsteroidsController();
});

const defaultRPCPort = 35075;

app.on("ready", () => {
	var splashController = new SplashController();

	// for notifications on windows
	app.setAppUserModelId("unigrid-electron");

	splashController.window.webContents.on("did-finish-load", () => {
		splashController.window.webContents.send("progress", "indeterminate", "Initializing UNIGRID daemon...");

		Daemon.start(splashController.window).then(() => {
			var rpcClient = new RPCClient();
			splashController.version_control(rpcClient).then(() => {
				splashController.daemon_loading(rpcClient).then(() => {
					splashController.synchronize_wallet(rpcClient).then(() => {
						splashController.check_errors(rpcClient).then(() => {
							/* If sync was a success, we close the splash and move on to the main wallet window */
							var mainController = new MainController();
							splashController.window.close();
						}, (stderr) => {
							console.error(stderr);
						});
					}, (stderr) => {
						console.error(stderr);
					});
				}, (stderr) => {
					console.error(stderr);
				});
			}, (stderr) => {
				console.error(stderr);
			});
		}, (stderr) => {
			console.error(stderr);
		});
	});
});

// auto updater status signals
autoUpdater.on('checking-for-update', function () {
    sendStatusToWindow('Checking for update...');
});

autoUpdater.on('update-available', function (info) {
    sendStatusToWindow('Update available.');
});

autoUpdater.on('update-not-available', function (info) {
    sendStatusToWindow('Update not available.');
});

autoUpdater.on('error', function (err) {
    sendStatusToWindow('Error in auto-updater.');
});

autoUpdater.on('download-progress', function (progressObj) {
    let log_message = "Download speed: " + progressObj.bytesPerSecond;
    log_message = log_message + ' - Downloaded ' + parseInt(progressObj.percent) + '%';
    log_message = log_message + ' (' + progressObj.transferred + "/" + progressObj.total + ')';
    sendStatusToWindow(log_message);
});

autoUpdater.on('update-downloaded', function (info) {
    sendStatusToWindow('send signal to show update icon in controlbar');
});

autoUpdater.checkForUpdates();

function sendStatusToWindow(message) {
    console.log(message);
}

