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

import React from "react";
import ReactDOM from "react-dom";
import { ipcRenderer } from "electron";
import { Router } from "common/router";
import ControlBar from "./controlbar";
import NavBar from "./navbar";
import Splash from "./splash";
import MyWalletContent from "./mywallet/mywallet-content.js";
import TransactionsContent from "./transactions/transactions-content.js";
import AddressesContent from "./addresses/addresses-content.js";
import SettingsContent from "./settings/settings-content.js";
import CLIContent from "./cli/cli-content.js";
import Reacteroids from "./asteroids/reacteroids.js";
import File from "common/file";
import { library } from "@fortawesome/fontawesome-svg-core";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { faRocket } from "@fortawesome/free-solid-svg-icons";
import "./index.css";
import ExplorerContent from "./explorer/explorer-content";
import MasternodesContent from "./masternodes/masternodes-content";

library.add(faRocket);

let onStartGame = () => {
	ipcRenderer.send("open-asteroids");
};

const routes = {
	asteroids: [
		<ControlBar key={1} headerText="UNIGRID Asteroids" fullControls={true} />,
		<Reacteroids key={2} />
	],
	main: [
		<ControlBar key={1} headerText="UNIGRID" fullControls={true} />,
		<div className="nav" key={2}>
			<NavBar />
			<div>
				<MyWalletContent key="mywallet-content" active={true} />
				<AddressesContent key="addressbook-content" />
				<TransactionsContent key="transactions-content" />
				<MasternodesContent key="masternodes-content" />
				<ExplorerContent key ="explorer-content" />
				<SettingsContent key="settings-content" />
				<CLIContent key="cli-content" />
			</div>
		</div>
	],
	splash: [
		<ControlBar key={1} className="nobg" fullControls={false} extraButton="rocket"
		            extraButtonOnClick={onStartGame}/>, <Splash key={2} />
	]
};

ReactDOM.render (<Router routes={routes} />, document.getElementById ("app"));

