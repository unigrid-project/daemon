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
import WarningController from "./warning-controller";

autoUpdater.autoDownload = true;
autoUpdater.allowPrerelease = true;
let testing = false;
const { crashReporter } = require('electron');
const packageJSON = require('../../package.json');
const log = require('electron-log');
const deps = packageJSON.dependencies;
var mainWindow;
var isWarningOpen = false;
var trackRejectUpdates = 0;
var skipTimesAllocated = 20;
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

ipcMain.on('update-the-wallet', () => {
	console.log("restart and update")
	log.info('Restarting wallet to install the update!');
	if (global.rpcPort != undefined) {
		new RPCClient().stop();
	}
	autoUpdater.quitAndInstall();
});

ipcMain.on("open-asteroids", () => {
	var asteroidsController = new AsteroidsController();
});

const defaultRPCPort = 51992;

app.on("ready", () => {
	var splashController = new SplashController();
	log.info("app ready");
	// for notifications on windows
	app.setAppUserModelId("unigrid-electron");

	splashController.window.webContents.on("did-finish-load", () => {
		log.info("did-finish-load");
		splashController.window.webContents.send("progress", "indeterminate", "Initializing UNIGRID daemon...");
		log.info("Initializing UNIGRID daemon...");
		Daemon.start(splashController.window).then(() => {
			log.info("daemon start...");
			var rpcClient = new RPCClient();
			splashController.version_control(rpcClient).then(() => {
				log.info("version_control");
				splashController.daemon_loading(rpcClient).then(() => {
					log.info("daemon_loading");
					splashController.synchronize_wallet(rpcClient).then(() => {
						log.info("synchronize_wallet");
						splashController.check_errors(rpcClient).then(() => {
							log.info("Load MainController");
							/* If sync was a success, we close the splash and move on to the main wallet window */
							var mainController = new MainController();
							mainWindow = mainController.window;
							splashController.window.close();
							manuallyCheckForUpdates(mainWindow);
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

autoUpdater.on('checking-for-update', function () {
	log.info("Checking for a new wallet release.");
	if (testing) {
		var vCurr = '2.0.14';
		var nVer = '2.0.15';
		var currentVersion = vCurr.split('.');
		var newVersion = nVer.split('.');

		log.info("currentVersion ", currentVersion[1]);
		log.info("newVersion ", newVersion[1]);
		// testing === for prod <
		if (currentVersion[1] === newVersion[1]) {
			let message = {
				title: "WARNING!",
				version: '2.0.15',
				message: `There is a new wallet release with a protocol change. This update is manadatory! 
                Please update now to get on the latest version 2.0.15 of ${packageJSON.name}.`
			}
			log.info("SEND WARNING UPDATE MESSAGE!");
			openwarningWindow(message);
		}
	}
});

autoUpdater.on('update-available', function (info) {
	log.info('Update available.');
});

autoUpdater.on('update-not-available', function (info) {
	log.info('Update not available.');
});

autoUpdater.on('error', function (err) {
	log.warn("Auto Updater: ", err);
});

autoUpdater.on('download-progress', function (progressObj) {
	let log_message = "Download speed: " + progressObj.bytesPerSecond;
	log_message = log_message + ' - Downloaded ' + parseInt(progressObj.percent) + '%';
	log_message = log_message + ' (' + progressObj.transferred + "/" + progressObj.total + ')';
	log.info(log_message);
});

autoUpdater.on('update-downloaded', function (info) {
	log.info('Download complete: ', info);
	var currentVersion = packageJSON.version.split('.');
	var newVersion = info.version.split('.');
	log.info("currentVersion ", currentVersion[1]);
	log.info("newVersion ", newVersion[1]);
	// testing === for prod <
	if (currentVersion[1] < newVersion[1]) {
		log.info("Your wallet daemon needs updating!");
		let message = {
			title: "WARNING!",
			version: info.version,
			message: `There is a new wallet release with a protocol change. This update is manadatory! 
            Please update now to get on the latest version ${info.version} of ${packageJSON.name}.`
		}
		openwarningWindow(message);
	}
	mainWindow.webContents.send("wallet-update-available", info);
});

const openwarningWindow = (data) => {
	if (!isWarningOpen) {
		var warningController = new WarningController();
		warningController.window.webContents.on("did-finish-load", () => {
			warningController.window.webContents.send("warning-data", data);
		});
		isWarningOpen = true;
		warningController.window.on('close', function () { isWarningOpen = false });
		ipcMain.on("close-warning-window", () => {
			trackRejectUpdates++;
			if(trackRejectUpdates > skipTimesAllocated) dancingPickles();
			warningController.window.close();
		});
	}
}

function dancingPickles(){

}

function manuallyCheckForUpdates(mainWindow) {
	window = mainWindow;
	autoUpdater.checkForUpdates();
	autoCheckForUpdates;
}

const autoCheckForUpdates = setInterval(() => {
	autoUpdater.checkForUpdates();
}, 60000);


