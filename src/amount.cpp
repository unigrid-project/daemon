// Copyright c 2009-2010 Satoshi Nakamoto
// Copyright c 2009-2014 The Bitcoin developers
// Copyright c 2014-2015 The Dash developers
// Copyright c 2015-2018 The PIVX developers
// Copyright c 2018 The HUZU developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "amount.h"

#include "tinyformat.h"

CFeeRate::CFeeRate(const CAmount& nFeePaid, size_t nSize)
{
    if (nSize > 0)
        nHUZUsPerK = nFeePaid*1000/nSize;
    else
        nHUZUsPerK = 0;
}

CAmount CFeeRate::GetFee(size_t nSize) const
{
    CAmount nFee = nHUZUsPerK*nSize / 1000;

    if (nFee == 0 && nHUZUsPerK > 0)
        nFee = nHUZUsPerK;

    return nFee;
}

std::string CFeeRate::ToString() const
{
    return strprintf("%d.%05d HUZU/kB", nHUZUsPerK / COIN, nHUZUsPerK % COIN);
}
