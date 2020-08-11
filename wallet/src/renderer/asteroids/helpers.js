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

export function asteroidVertices(count, rad) {
	let p = [];

	for (let i = 0; i < count; i++) {
		p[i] = {
			x: (-Math.sin((360 / count) * i * Math.PI / 180) +
			    Math.round(Math.random() * 2 - 1) * Math.random() / 3) * rad,
			y: (-Math.cos((360 / count) * i * Math.PI / 180) +
			    Math.round(Math.random() * 2 - 1) * Math.random() / 3) * rad
		};
	}

	return p;
}

export function rotatePoint(p, center, angle) {
	return {
		x: ((p.x - center.x) * Math.cos(angle) - (p.y - center.y) * Math.sin(angle)) + center.x,
		y: ((p.x - center.x) * Math.sin(angle) + (p.y - center.y) * Math.cos(angle)) + center.y
	};
}

export function randomNumBetween(min, max) {
	return Math.random() * (max - min + 1) + min;
}

export function randomNumBetweenExcluding(min, max, exMin, exMax) {
	let random = randomNumBetween(min, max);

	while (random > exMin && random < exMax) {
		random = Math.random() * (max - min + 1) + min;
	}

	return random;
}

