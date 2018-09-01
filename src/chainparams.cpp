// Copyright c 2009-2010 Satoshi Nakamoto
// Copyright c 2009-2014 The Bitcoin developers
// Copyright c 2014-2015 The Dash developers
// Copyright c 2015-2018 The PIVX developers
// Copyright c 2018 The HUZU developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
	uint8_t addr[16];
	uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut,
		const SeedSpec6 *data, unsigned int count) {
	// It'll only connect to one or two seed nodes because once it connects,
	// it'll get a pile of addresses with newer timestamps.
	// Seed nodes are given a random 'last seen time' of between one and two
	// weeks ago.
	const int64_t nOneWeek = 7 * 24 * 60 * 60;
	for (unsigned int i = 0; i < count; i++) {
		struct in6_addr ip;
		memcpy(&ip, data[i].addr, sizeof(ip));
		CAddress addr(CService(ip, data[i].port));
		addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
		vSeedsOut.push_back(addr);
	}
}

static const unsigned int timeMainGenesisBlock = 1531330000;
uint256 hashMainGenesisBlock(
		"0x0000035b4678be6c8e523cc2e63d445129adbe0a6394aaef89a7b372adb8fd02");
static uint256 nMainProofOfWorkLimit(~uint256(0) >> 21);

static const int64_t nGenesisBlockRewardCoin = 0 * COIN;
static const int64_t nBlockRewardStartCoin = 2048 * COIN;
static const int64_t nBlockRewardMinimumCoin = 1 * COIN;

/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
static Checkpoints::MapCheckpoints mapCheckpoints =
		boost::assign::map_list_of(0,
				uint256(
						"0x0000035b4678be6c8e523cc2e63d445129adbe0a6394aaef89a7b372adb8fd02"))(
				10,
				uint256(
						"0x00000545bb805de915a8b41ed8435749e4ce9f7caee2d2933abb1fd213bda490"));
static const Checkpoints::CCheckpointData data = { &mapCheckpoints, 1531330000, // * UNIX timestamp of last checkpoint block
		0, // * total number of transactions between genesis and last checkpoint
		   //   (the tx=... number in the SetBestChain debug.log lines)
		2880.0   // * estimated number of transactions per day after checkpoint
		};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
		boost::assign::map_list_of(0,
				uint256(
						"0x000001635370b5dcbc86367c369d21443d56f2625c50a1b140b718f516cc820c"))/*(
				100,
				uint256(
						"0x0000040a8297a0501e865bef31871b144a5dcaf7807b051eb15a5ad179a483d9"))*/;
static const Checkpoints::CCheckpointData dataTestnet = {
		&mapCheckpointsTestnet, 1531330001, 0, 2880 };

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
		boost::assign::map_list_of(0,
				uint256(
						"0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206"));
static const Checkpoints::CCheckpointData dataRegtest = {
		&mapCheckpointsRegtest, 0, 0, 0 };

class CMainParams: public CChainParams {
public:
	CMainParams() {
		networkID = CBaseChainParams::MAIN;
		strNetworkID = "main";
		/**
		 * The message start string is designed to be unlikely to occur in normal data.
		 * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
		 * a large 4-byte int at any alignment.
		 */
		pchMessageStart[0] = 0xfe;
		pchMessageStart[1] = 0xa5;
		pchMessageStart[2] = 0x03;
		pchMessageStart[3] = 0xdd;
		vAlertPubKey =
				ParseHex(
						"0426d1c5aac0e7b98f37f5f8ca10a18bb915820516723a727093cca65108ac24cdf3467ff06a39ad388ccc3d83802c85df73dba14e3db3835cec6892b9647e92fa");
		nDefaultPort = 29921;
		bnProofOfWorkLimit = nMainProofOfWorkLimit;
		nSubsidyHalvingInterval = 259000;
		nEnforceBlockUpgradeMajority = 7500;
		nRejectBlockOutdatedMajority = 9000;
		nToCheckBlockUpgradeMajority = 10000;
		nMinerThreads = 0;
		nTargetTimespan = 8 * 60; // eight minutes
		nTargetSpacing = 1 * 60; // 60 seconds
		nMaxTipAge = 24 * 60 * 60;
		nModifierUpdateBlock = 615800;
		nFirstPOSBlock = 1000; // est 25 Nov 2018
		nFirstMasternodePaymentBlock = 700;
		nMasternodeCollateral = 2000;
		nStakeMinAge = 60 * 60; // 1 hour
		strDevpubkey = "03552bb69394b5a1c82fb83df38464042dee0e36fd7f39080e286b6217e8eca8ad";

		/**
		 * Build the genesis block. Note that the output of the genesis coinbase cannot
		 * be spent as it did not originally exist in the database.
		 *
		 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
		 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
		 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
		 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
		 *   vMerkleTree: 4a5e1e
		 */
		const char* pszTimestamp = "I create HUZU in 2018/07/11 17:26:00.";
		CMutableTransaction txNew;
		txNew.vin.resize(1);
		txNew.vout.resize(1);
		txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4)
				<< vector<unsigned char>((const unsigned char*) pszTimestamp,
						(const unsigned char*) pszTimestamp
								+ strlen(pszTimestamp));
		txNew.vout[0].nValue = nGenesisBlockRewardCoin;
		txNew.vout[0].scriptPubKey =
				CScript()
						<< ParseHex(
								"04b36556d4e6822708431cce73eaf447a0ec89a8ae6eb48aa412cb5b56bb6410acaa7cda7000e270b9900eb77667bb421728cab77e720c7ca2118150430c4f418a")
						<< OP_CHECKSIG;

		genesis.vtx.push_back(txNew);
		genesis.hashPrevBlock = 0;
		genesis.hashMerkleRoot = genesis.BuildMerkleTree();
		genesis.nVersion = 112;
		genesis.nTime = timeMainGenesisBlock;
		genesis.nBits = nMainProofOfWorkLimit.GetCompact();

		genesis.nNonce = 1194940;

		assert(
				genesis.hashMerkleRoot
						== uint256(
								"0xa48b07ee851065ef85231c27685241685a683be7650e8ba041b896a0e50d9c91"));

		hashGenesisBlock = genesis.GetHash();
		assert(hashGenesisBlock == hashMainGenesisBlock);

		vSeeds.push_back(CDNSSeedData("54.39.98.89", "54.39.98.89"));
		vSeeds.push_back(CDNSSeedData("51.68.212.21", "51.68.212.21"));
		vSeeds.push_back(CDNSSeedData("51.38.129.175", "51.38.129.175"));
		vSeeds.push_back(CDNSSeedData("183.182.104.121", "183.182.104.121"));

		// HUZU addresses start with 'H'
		base58Prefixes[PUBKEY_ADDRESS] = list_of(40);
		base58Prefixes[SCRIPT_ADDRESS] = list_of(100);
		base58Prefixes[SECRET_KEY] = list_of(153);
		base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E);
		base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4);

		convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

		fRequireRPCPassword = true;
		fMiningRequiresPeers = true;
		fAllowMinDifficultyBlocks = false;
		fDefaultConsistencyChecks = false;
		fRequireStandard = true;
		fMineBlocksOnDemand = false;
		fSkipProofOfWorkCheck = false;
		fTestnetToBeDeprecatedFieldRPC = false;

		nPoolMaxTransactions = 3;
		strSporkKey =
				"044B4A273C4B3E0DB2B0953402D94335C636BA5517D2ACDC7AB7609DC0B86D1861698524A6305024A42318D4B46389EBEEB43CF26A255B0F86AC40F1D757E67527";
		strObfuscationPoolDummyAddress = "y57cqfGRkekRyDRNeJiLtYVEbvhXrNbmox";
		nStartMasternodePayments = 2145916800; // 2038
	}

	const Checkpoints::CCheckpointData& Checkpoints() const {
		return data;
	}
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams: public CMainParams {
public:
	CTestNetParams() {
		networkID = CBaseChainParams::TESTNET;
		strNetworkID = "test";
		pchMessageStart[0] = 0x01;
		pchMessageStart[1] = 0x1a;
		pchMessageStart[2] = 0x39;
		pchMessageStart[3] = 0xf7;
		vAlertPubKey =
				ParseHex(
						"04218bc3f08237baa077cb1b0e5a81695fcf3f5b4e220b4ad274d05a31d762dd4e191efa7b736a24a32d6fd9ac1b5ebb2787c70e9dfad0016a8b32f7bd2520dbd5");
		nDefaultPort = 39922;
		nEnforceBlockUpgradeMajority = 51;
		nRejectBlockOutdatedMajority = 75;
		nToCheckBlockUpgradeMajority = 100;
		nMinerThreads = 0;
		nTargetTimespan = 8 * 60; // ten minutes
		nTargetSpacing = 1 * 60; // 120 seconds
		nMaxTipAge = 0x7fffffff;
		nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2018 04:00:00 GMT
		nFirstPOSBlock = 1000;
		nFirstMasternodePaymentBlock = 200;
		nMasternodeCollateral = 2000;
		nStakeMinAge = 60 * 60; // 1 hour

		//! Modify the testnet genesis block so the timestamp is valid for a later start.

		bnProofOfWorkLimit = nMainProofOfWorkLimit;
		genesis.nBits = nMainProofOfWorkLimit.GetCompact();

		genesis.nTime = 1531330001;

		genesis.nNonce = 426937;
		hashGenesisBlock = genesis.GetHash();

		assert(
				hashGenesisBlock
						== uint256(
								"0x000001635370b5dcbc86367c369d21443d56f2625c50a1b140b718f516cc820c"));

		vFixedSeeds.clear();
		vSeeds.clear();
		vSeeds.push_back(CDNSSeedData("45.76.37.150", "45.76.37.150"));
		vSeeds.push_back(CDNSSeedData("45.76.85.141", "45.76.85.141"));
		vSeeds.push_back(
				CDNSSeedData("testseed1.HUZUnet.info",
						"testseed1.HUZUnet.info"));

		// HUZU addresses start with 'h'
		base58Prefixes[PUBKEY_ADDRESS] = list_of(100);
		base58Prefixes[SCRIPT_ADDRESS] = list_of(199);
		base58Prefixes[SECRET_KEY] = list_of(247);
		base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x35)(0x87)(0xCF);
		base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x35)(0x83)(0x94);

		convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

		fRequireRPCPassword = true;
		fMiningRequiresPeers = true;
		fAllowMinDifficultyBlocks = true;
		fDefaultConsistencyChecks = false;
		fRequireStandard = false;
		fMineBlocksOnDemand = false;
		fTestnetToBeDeprecatedFieldRPC = true;

		nPoolMaxTransactions = 2;
		strSporkKey =
				"046C0C49800E806FAC756EAC67C2A55196805589D30A54B2BF4129DCA75EFC0CBA5527E694BD1ED5606ADD47CE8A8C4A41984178EE38BA091604CD69FFCC77A2BD";
		strObfuscationPoolDummyAddress = "y57cqfGRkekRyDRNeJiLtYVEbvhXrNbmox";
		// nStartMasternodePayments = 1420837558; //Fri, 09 Jan 2015 21:05:58 GMT
	}
	const Checkpoints::CCheckpointData& Checkpoints() const {
		return dataTestnet;
	}
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams: public CTestNetParams {
public:
	CRegTestParams() {
		networkID = CBaseChainParams::REGTEST;
		strNetworkID = "regtest";
		pchMessageStart[0] = 0xfa;
		pchMessageStart[1] = 0xbf;
		pchMessageStart[2] = 0xb5;
		pchMessageStart[3] = 0xda;
		nSubsidyHalvingInterval = 150;
		nEnforceBlockUpgradeMajority = 750;
		nRejectBlockOutdatedMajority = 950;
		nToCheckBlockUpgradeMajority = 1000;
		nMinerThreads = 1;
		nTargetTimespan = 10 * 60; // ten minutes
		nTargetSpacing = 30; // 30 seconds
		bnProofOfWorkLimit = ~uint256(0);
		nMaxTipAge = 24 * 60 * 60;
		genesis.nTime = 1531330000;
		genesis.nBits = bnProofOfWorkLimit.GetCompact();
		genesis.nNonce = 3;
		hashGenesisBlock = genesis.GetHash();
		nDefaultPort = 18444;
		nFirstPOSBlock = 20;
		nFirstMasternodePaymentBlock = 20;
		// assert(hashGenesisBlock == uint256("0x0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206"));

		vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
		vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

		fRequireRPCPassword = false;
		fMiningRequiresPeers = false;
		fAllowMinDifficultyBlocks = true;
		fDefaultConsistencyChecks = true;
		fRequireStandard = false;
		fMineBlocksOnDemand = true;
		fTestnetToBeDeprecatedFieldRPC = false;
	}
	const Checkpoints::CCheckpointData& Checkpoints() const {
		return dataRegtest;
	}
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams: public CMainParams, public CModifiableParams {
public:
	CUnitTestParams() {
		networkID = CBaseChainParams::UNITTEST;
		strNetworkID = "unittest";
		nDefaultPort = 18445;
		vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
		vSeeds.clear();  //! Unit test mode doesn't have any DNS seeds.

		fRequireRPCPassword = false;
		fMiningRequiresPeers = false;
		fDefaultConsistencyChecks = true;
		fAllowMinDifficultyBlocks = false;
		fMineBlocksOnDemand = true;
	}

	const Checkpoints::CCheckpointData& Checkpoints() const {
		// UnitTest share the same checkpoints as MAIN
		return data;
	}

	//! Published setters to allow changing values in unit test cases
	virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) {
		nSubsidyHalvingInterval = anSubsidyHalvingInterval;
	}
	virtual void setEnforceBlockUpgradeMajority(
			int anEnforceBlockUpgradeMajority) {
		nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority;
	}
	virtual void setRejectBlockOutdatedMajority(
			int anRejectBlockOutdatedMajority) {
		nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority;
	}
	virtual void setToCheckBlockUpgradeMajority(
			int anToCheckBlockUpgradeMajority) {
		nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority;
	}
	virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) {
		fDefaultConsistencyChecks = afDefaultConsistencyChecks;
	}
	virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) {
		fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks;
	}
	virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) {
		fSkipProofOfWorkCheck = afSkipProofOfWorkCheck;
	}
};
static CUnitTestParams unitTestParams;

static CChainParams *pCurrentParams = 0;

CModifiableParams *ModifiableParams() {
	assert(pCurrentParams);
	assert(pCurrentParams == &unitTestParams);
	return (CModifiableParams*) &unitTestParams;
}

const CChainParams &Params() {
	assert(pCurrentParams);
	return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
	switch (network) {
	case CBaseChainParams::MAIN:
		return mainParams;
	case CBaseChainParams::TESTNET:
		return testNetParams;
	case CBaseChainParams::REGTEST:
		return regTestParams;
	case CBaseChainParams::UNITTEST:
		return unitTestParams;
	default:
		assert(false && "Unimplemented network");
		return mainParams;
	}
}

void SelectParams(CBaseChainParams::Network network) {
	SelectBaseParams(network);
	pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine() {
	CBaseChainParams::Network network = NetworkIdFromCommandLine();
	if (network == CBaseChainParams::MAX_NETWORK_TYPES)
		return false;

	SelectParams(network);
	return true;
}
