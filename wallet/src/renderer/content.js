/*
 * This file is part of The UNIGRID Wallet
 * Copyright (C) 2019 The Swipp developers <info@swippcoin.com>
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
import "./content.css";

export default class Content extends React.Component {
	constructor(props) {
		super(props);
		var element = this.props.children[0] == undefined ? this.props.children : this.props.children[0];

		this.state = {
			active: element._owner.pendingProps.active
		};

		ipcRenderer.on("navigate", (event, source) => {
			this.setState({ active: source.toLowerCase().replace(" ", "") == this.props.id });
		});
	}

	render() {
		return(
			<div id={this.props.id} className={this.state.active ? "active " : "inactive " + this.props.className}>
				{this.props.children}
			</div>
		);
	}
}

