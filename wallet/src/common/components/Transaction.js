/*
 * This file is part of The UNIGRID Wallet
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

import React, { useState, useEffect } from "react";
import "./Transaction.css";
import { faSignInAlt, faSignOutAlt, faCoins, faClock, faCompass, faCubes } from "@fortawesome/free-solid-svg-icons";
import { library } from "@fortawesome/fontawesome-svg-core";
import Tooltip from "react-simple-tooltip";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { css } from "styled-components";
library.add(faSignInAlt, faSignOutAlt, faCoins, faClock, faCubes);

function Transaction({ data, index, style }) {
    const [amount] = useState(data.amount);
    const [numWidth, setNumberWidth] = useState();
    const [largeTrans, setLargeTrans] = useState(false);
    useEffect(() => {
        if (style === "trans--long") {
            setNumberWidth("long--div");
            setLargeTrans(true);
        } else {
            setNumberWidth("short--div");
            setLargeTrans(false);
        }
        //console.log("transaction style: ", style);
    }, []);
    return (
        <div className={"transaction--main " + style}>
            <div className="trans--item">
                <div className="circle">
                    {getArrayIndex(index)}
                </div></div>
            <div className="trans--item">
                <Tooltip
                    arrow={10}
                    zIndex={200}
                    fadeDuration={150}
                    radius={10}
                    fontFamily='Roboto'
                    fontSize='5'
                    fadeEasing="linear"
                    background={css`
                    var(--tooltip--background)
                  `}
                    content={calculateDateTimeFromEpoch(data.timereceived)}
                    customCss={css`
                    white-space: nowrap;
                  `}
                >
                    {getTimeObject(data.timereceived)}

                </Tooltip>
            </div >
            <div className="trans--item">
                <Tooltip
                    arrow={10}
                    zIndex={200}
                    fadeDuration={150}
                    radius={10}
                    fontFamily='Roboto'
                    fontSize='5'
                    fadeEasing="linear"
                    background={css`
                    var(--tooltip--background)
                  `}
                    content={getBlockCount(data.confirmations)}
                    customCss={css`
                    white-space: nowrap;
                  `}
                >
                    {getBlockObject(data.confirmations)}

                </Tooltip>
            </div >
            <div className="trans--item">
                <Tooltip
                    arrow={10}
                    zIndex={200}
                    fadeDuration={150}
                    radius={10}
                    fontFamily='Roboto'
                    fontSize='5'
                    fadeEasing="linear"
                    background={css`
                    var(--tooltip--background)
                  `}
                    content={getCategory(data.category)}
                >{getCategroyIcon(data.category)}
                </Tooltip>

            </div>
            <div className="trans--item">
                <Tooltip
                    arrow={10}
                    zIndex={200}
                    fadeDuration={150}
                    radius={10}
                    fontFamily='Roboto'
                    fontSize='5'
                    fadeEasing="linear"
                    background={css`
                    var(--tooltip--background)
                  `}
                    content={data.amount.toFixed(8)}
                    customCss={css`
                    white-space: nowrap;
                  `}
                >
                    {setAmountColor()}
                </Tooltip>
            </div>
            <div className="trans--item">
                <Tooltip
                    arrow={10}
                    zIndex={200}
                    fadeDuration={150}
                    radius={10}
                    fontFamily='Roboto'
                    fontSize='5'
                    fadeEasing="linear"
                    background={css`
                    var(--tooltip--background)
                  `}
                    content="Show in explorer"
                    customCss={css`
                    white-space: nowrap;
                  `}
                >
                    <a href={"http://explorer.unigrid.org/tx/" + data.txid} target="_blank">
                        <FontAwesomeIcon size="lg" icon={faCompass} color="grey" /> </a>
                </Tooltip>
            </div>
            {largeTrans ?
                <div className="trans--item">
                    <Tooltip
                        arrow={10}
                        zIndex={200}
                        fadeDuration={150}
                        radius={10}
                        fontFamily='Roboto'
                        fontSize='5'
                        fadeEasing="linear"
                        background={css`
                    var(--tooltip--background)
                  `}
                        content={data.address}
                        customCss={css`
                    white-space: nowrap;
                  `}
                    >
                        {getAccountName(data.account)}
                    </Tooltip>
                </div>
                : null}
        </div >
    )

    function getTimeObject(epoch) {
        if (largeTrans === true) {
            return calculateDateFromEpoch(epoch);
        } else {
            return <FontAwesomeIcon size="lg" icon={faClock} color="white" />;
        }
    }
    function getBlockCount(conf) {
        return (
            <div>
                <div>{conf}</div>
                <div>confirmations</div>
            </div>
        )

    }
    function getBlockObject(conf) {
        let color = "";
        switch (conf) {
            case 0:
                color = "grey";
                break;
            case 1:
            case 2:
            case 3:
            case 4:
                color = "lightcoral";
                break;
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                color = "lightyellow";
                break;

            default:
                color = "lightgreen";
                break;
        }
        return <FontAwesomeIcon size="lg" icon={faCubes} color={color} />;
    }
    function getCategory(cat) {
        return cat;
    }
    function setAmountColor() {
        let transType = data.category === "send" ? "send--color" : "receive--color";
        return <div className={numWidth + " " + transType}>{data.amount}</div>
    }
    function getArrayIndex(num) {
        let newNum = parseInt(num);
        return newNum + 1;
    }
    function getAccountName(account) {
        if (account === "") {
            return <div className="unnamed-account">unnamed account</div>
        } else {
            return <div>{account}</div>
        }
    }
    function calculateDateFromEpoch(epoch) {
        var recDate = new Date(epoch * 1000);
        const date = recDate.toISOString().split('T')[0];

        return (
            <div>
                <div>{date}</div>
            </div>
        )
    }
    function calculateDateTimeFromEpoch(epoch) {
        var recDate = new Date(epoch * 1000);
        const date = recDate.toISOString().split('T')[0];
        const time = recDate.toTimeString().split(" ")[0];
        return (
            <div>
                <div>{date}</div>
                <div>{time}</div>
            </div>
        )
    }
    function getCategroyIcon(category) {
        switch (category) {
            case "receive":
                return <FontAwesomeIcon size="lg" icon={faSignInAlt} color="lightgreen" />
                break;
            case "send":
                return <FontAwesomeIcon size="lg" icon={faSignOutAlt} color="lightsalmon" />
                break;
            case "stake":
                return <FontAwesomeIcon size="lg" icon={faCoins} color="lightgoldenrodyellow" />
                break;
            default:
                return <FontAwesomeIcon size="lg" icon={faSignInAlt} color="white" />
                break;
        }
    }
}
export default Transaction;