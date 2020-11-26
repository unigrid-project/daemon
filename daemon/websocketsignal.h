// Copyright (c) 2020 The UNIGRID organization
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef _WEBSOCKET_SIGNAL_H_
#define _WEBSOCKET_SIGNAL_H_

enum WebSocketSignal {
        BLOCK_HEIGHT,
        BOOTSTRAPING_STATUS,
        MASTERNODE_COUNT,
        MASTERNODE_STATUS,
        PEER_COUNT,
        STAKING_STATUS,
        WALLET_TRANSACTION,
        WALLET_LOCK_STATUS
};

#endif /* _WEBSOCKET_SIGNAL_H_ */
