// Copyright c 2009-2010 Satoshi Nakamoto
// Copyright c 2009-2014 The Bitcoin developers
// Copyright c 2014-2015 The Dash developers
// Copyright c 2015-2018 The PIVX developers
// Copyright c 2018 The HUZU developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "chain.h"
#include "chainparams.h"
#include "primitives/block.h"
#include "uint256.h"
#include "util.h"

unsigned int GetNextPoSTargetRequired(const CBlockIndex* pindexLast)
{
    int64_t nActualSpacing = pindexLast->pprev->GetBlockTime() - pindexLast->pprev->pprev->GetBlockTime();

    // ppcoin: target change every block
    // ppcoin: retarget with exponential moving toward target spacing
    uint256 bnNew;
    bnNew.SetCompact(pindexLast->pprev->nBits);
    bnNew *= ((Params().Interval() - 1) * Params().TargetSpacing() + nActualSpacing + nActualSpacing);
    bnNew /= ((Params().Interval() + 1) * Params().TargetSpacing());

    if (bnNew > Params().ProofOfWorkLimit())
        bnNew = Params().ProofOfWorkLimit();

    return bnNew.GetCompact();
}

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock)
{
    unsigned int nProofOfWorkLimit = Params().ProofOfWorkLimit().GetCompact();

    // Genesis block
    if (pindexLast == NULL)
        return nProofOfWorkLimit;

    // Only change once per interval
    if ((pindexLast->nHeight) <= Params().Interval())
    {
        return pindexLast->nBits;
    }

    // Go back by what we want to be Params().Interval() blocks
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < Params().Interval(); i++)
        pindexFirst = pindexFirst->pprev;
    assert(pindexFirst);

    // Limit adjustment step
    int64_t nActualTimespan = (pindexLast->GetBlockTime() - pindexFirst->GetBlockTime());
    // LogPrintf("  nActualTimespan = %d before bounds\n", nActualTimespan);
    int64_t LimUp = Params().TargetTimespan() * 100 / 130; // 130% up
    int64_t LimDown = Params().TargetTimespan() * 2; // 200% down
    if (nActualTimespan < LimUp)
        nActualTimespan = LimUp;
    if (nActualTimespan > LimDown)
        nActualTimespan = LimDown;

    // Retarget
    uint256 bnNew;
    uint256 bnOld;
    bnNew.SetCompact(pindexLast->nBits);
    bnOld = bnNew;
    bnNew *= nActualTimespan;
    bnNew /= Params().TargetTimespan();

    if (bnNew > Params().ProofOfWorkLimit())
        bnNew = Params().ProofOfWorkLimit();


    return bnNew.GetCompact();
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits)
{
    bool fNegative;
    bool fOverflow;
    uint256 bnTarget;

    if (Params().SkipProofOfWorkCheck())
       return true;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > Params().ProofOfWorkLimit())
        return false;

    // Check proof of work matches claimed amount
    if (hash > bnTarget)
        return false;

    return true;
}

uint256 GetBlockProof(const CBlockIndex& block)
{
    uint256 bnTarget;
    bool fNegative;
    bool fOverflow;
    bnTarget.SetCompact(block.nBits, &fNegative, &fOverflow);
    if (fNegative || fOverflow || bnTarget == 0)
        return 0;
    // We need to compute 2**256 / (bnTarget+1), but we can't represent 2**256
    // as it's too large for a uint256. However, as 2**256 is at least as large
    // as bnTarget+1, it is equal to ((2**256 - bnTarget - 1) / (bnTarget+1)) + 1,
    // or ~bnTarget / (nTarget+1) + 1.
    return (~bnTarget / (bnTarget + 1)) + 1;
}
