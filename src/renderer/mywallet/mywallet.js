/*
 * This file is part of The Swipp Wallet
 * Copyright (C) 2019 The Swipp developers <info@swippcoin.com>
 *
 * The Swipp Wallet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * The Swipp Wallet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with The Swipp Wallet. If not, see <https://www.gnu.org/licenses/>.
 */

import React from "react";
import { library } from "@fortawesome/fontawesome-svg-core";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { faWallet } from "@fortawesome/free-solid-svg-icons";
import NavEntry from "../naventry";

library.add(faWallet);

export default class MyWallet extends React.Component {
	getContent() {
		return(
			<div>
				<script src="https://widgets.coingecko.com/coingecko-coin-ticker-widget.js"></script>
				<coingecko-coin-ticker-widget  coin-id="swipp" currency="usd" locale="en"></coingecko-coin-ticker-widget>
			</div>
		);
	}

	render() {
		return(<NavEntry><FontAwesomeIcon size="lg" icon="wallet"/>My Wallet</NavEntry>);
	}
}

