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

import http from "http";

export default class Explorer {
	constructor() {
		this.send_command = (command, args = []) => {
			return new Promise((resolve, reject) => {
				http.get("http://explorer.unigrid.org/api/" + command, (resp) => {
					let data = "";

					resp.on("data", (chunk) => {
						data += chunk;
					});

					resp.on("end", () => {
						try {
							resolve(JSON.parse(data));
						} catch(exception) {
							reject(exception);
						}
					});
				}).on("error", (err) => {
					reject(err.message);
				});
			});
		}
	}

	async getblockcount() {
		return await this.send_command("getblockcount");
	}
}

