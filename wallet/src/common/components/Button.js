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

import React, { useState } from "react";
import "./Button.css"

const STYLES = [
    "btn--primary--solid",
    "btn--warning--solid",
    "btn--danger--solid",
    "btn--success--solid",
    "btn--white--solid",
    "btn--dark--solid",
    "btn--primary--outline",
    "btn--warning--outline",
    "btn--danger--outline",
    "btn--success--outline",
    "btn--dark--outline",
    "btn--white--outline",
];

const SIZES = ["btn--medium", "btn--large", "btn--small"];

function Button(props) {
    const [children] = useState(props.children);
    const [buttonStyle] = useState(props.buttonStyle);
    const [titleStyle] = useState(props.titleStyle);
    const [buttonSize] = useState(props.buttonSize);
    const [name] = useState(props.name);
    const checkButtonStyles = STYLES.includes(buttonStyle)
        ? buttonStyle : STYLES[0];
    const checkButtonSize = SIZES.includes(buttonSize)
        ? buttonSize : SIZES[0];

    return (
        <div >
            <button
                key={name}
                className={`btn ${checkButtonStyles} ${checkButtonSize}`}
                onClick={() => props.handleClick(name)}>
                <div className={titleStyle}>
                    {children}
                </div>
            </button>
        </div>
    )
}

export default Button;