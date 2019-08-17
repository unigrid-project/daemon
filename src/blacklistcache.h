// Copyright (c) 2018-2019 The UNIGRID organization
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BLACKLIST_CACHE_H
#define BLACKLIST_CACHE_H

#include <boost/filesystem.hpp>
#include "amount.h"
#include "base58.h"
#include "primitives/block.h"

class BlacklistCache
{
private:
    std::set<CBitcoinAddress> bannedAddresses;
    boost::filesystem::path pathDB;
    std::string strMagicMessage;
    CAmount sum;
    uint64_t sporkValue;

public:
    enum ReadResult {
        Ok,
        FileError,
        HashReadError,
        IncorrectHash,
        IncorrectMagicMessage,
        IncorrectFormat
    };

    void Initialize();
    bool IsInitialized();
    bool HasReferenceList();
    void RefreshReferenceList();
    void Add(CAmount amount);
    CAmount GetSum();
    CAmount SumBlacklistedAmounts(CBlock& block);
    void SumBlacklistedAmounts();
    bool IsDirty();
    bool Write();
    ReadResult Read(CAmount& sum, uint64_t& sporkValue);
    ReadResult Read();
};

#endif // BLACKLIST_CACHE_H

