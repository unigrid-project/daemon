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
import { ipcRenderer, remote } from "electron";
import { library } from "@fortawesome/fontawesome-svg-core";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { faSpinner, faWindowMinimize, faWindowMaximize, faWindowClose } from "@fortawesome/free-solid-svg-icons";
import File from "common/file";
import "./controlbar.css"
import { homePage } from "../common/consts";

library.add(faSpinner, faWindowMinimize, faWindowMaximize, faWindowClose);

export default class ControlBar extends React.Component {
	constructor(props) {
		super(props);
		this.state = { showSpinner: false };

		ipcRenderer.on("state", (event, message) => {
			this.setState({ showSpinner: (message == "working" ? true : false) });
		});
	}

	render() {
		var onMinimize = () => {
			remote.getCurrentWindow().minimize();
		}

		var onMaximize = () => {
			var w = remote.getCurrentWindow();

			if (w.isMaximized()) {
				w.unmaximize();
			} else {
				w.maximize();
			}
		}

		var onClose = () => {
			remote.getCurrentWindow().close();
		}

		return(
			<div className={"controlbar " + this.props.className}>
				<div>
						{this.props.fullControls == true &&
						<a href={homePage} target="_blank">
							<img src={File.get("logo.png")} className="piclet" />
							</a>
						}
						<div>{this.props.headerText}</div>			
				</div>
				<div>
					{this.state.showSpinner == true &&
						<FontAwesomeIcon className="spinner" icon="spinner" spin />
					}
					{this.props.fullControls == true &&
						<div>
							<FontAwesomeIcon onClick={onMinimize} className="minimize"
							                 icon="window-minimize" />
							<FontAwesomeIcon onClick={onMaximize} className="maximize"
							                 icon="window-maximize" />
						</div>
					}
					{this.props.extraButton &&
						<FontAwesomeIcon className="extra" onClick={this.props.extraButtonOnClick}
						                 icon={this.props.extraButton} />
					}
					<FontAwesomeIcon onClick={onClose} className="close" icon="window-close" />
				</div>
			</div>
		);
	}
}

