/*
 * This file is part of The UNIGRID Wallet
 * Copyright (C) 2019 The UNIGRID Organization
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

const ObjectsToCsv = require('objects-to-csv');

export default class ExportCSV {
    async convert(data) {
        const csv = new ObjectsToCsv(data);

        // Save to file:
        await csv.toDisk('./test.csv');

        // Return the CSV file as string:
        console.log(await csv.toString());
    }
}

