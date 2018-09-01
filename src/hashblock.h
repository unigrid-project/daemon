#ifndef HASHBLOCK_H
#define HASHBLOCK_H

#include "uint256.h"
#include "crypto/sph_blake.h"
#include "crypto/sph_bmw.h"
#include "crypto/sph_groestl.h"
#include "crypto/sph_jh.h"
#include "crypto/sph_keccak.h"
#include "crypto/sph_skein.h"
#include "crypto/ripemd160.h"

#include "crypto/sph_luffa.h"
#include "crypto/sph_cubehash.h"
#include "crypto/sph_shavite.h"
#include "crypto/sph_simd.h"
#include "crypto/sph_echo.h"
#include "crypto/sph_hamsi.h"
#include "crypto/sph_fugue.h"
#include "crypto/sph_shabal.h"
#include "crypto/sph_whirlpool.h"

extern "C" {
#include "crypto/sph_sha2.h"
}

#ifndef QT_NO_DEBUG
#include <string>
#endif

#ifdef GLOBALDEFINED
#define GLOBAL
#else
#define GLOBAL extern
#endif

GLOBAL sph_blake512_context     z_blake;
GLOBAL sph_bmw512_context       z_bmw;
GLOBAL sph_groestl512_context   z_groestl;
GLOBAL sph_jh512_context        z_jh;
GLOBAL sph_keccak512_context    z_keccak;
GLOBAL sph_skein512_context     z_skein;
GLOBAL sph_luffa512_context     z_luffa;
GLOBAL sph_cubehash512_context  z_cubehash;
GLOBAL sph_shavite512_context   z_shavite;
GLOBAL sph_simd512_context      z_simd;
GLOBAL sph_echo512_context      z_echo;
GLOBAL sph_hamsi512_context      z_hamsi;
GLOBAL sph_fugue512_context      z_fugue;
GLOBAL sph_shabal512_context      z_shabal;
GLOBAL sph_whirlpool_context      z_whirlpool;
GLOBAL sph_sha512_context      z_sha;
#define fillz() do { \
    sph_blake512_init(&z_blake); \
    sph_bmw512_init(&z_bmw); \
    sph_groestl512_init(&z_groestl); \
    sph_jh512_init(&z_jh); \
    sph_keccak512_init(&z_keccak); \
    sph_skein512_init(&z_skein); \
    sph_luffa512_init(&z_luffa); \
    sph_cubehash512_init(&z_cubehash); \
    sph_shavite512_init(&z_shavite); \
    sph_simd512_init(&z_simd); \
    sph_echo512_init(&z_echo); \
    sph_hamsi512_init(&z_hamsi); \
    sph_fugue512_init(&z_fugue); \
    sph_shabal512_init(&z_shabal); \
    sph_whirlpool_init(&z_whirlpool; \
    sph_sha512_init(&z_sha); \
} while (0)
#define ZBLAKE (memcpy(&ctx_blake, &z_blake, sizeof(z_blake)))
#define ZBMW (memcpy(&ctx_bmw, &z_bmw, sizeof(z_bmw)))
#define ZGROESTL (memcpy(&ctx_groestl, &z_groestl, sizeof(z_groestl)))
#define ZJH (memcpy(&ctx_jh, &z_jh, sizeof(z_jh)))
#define ZKECCAK (memcpy(&ctx_keccak, &z_keccak, sizeof(z_keccak)))
#define ZSKEIN (memcpy(&ctx_skein, &z_skein, sizeof(z_skein)))

template<typename T1>
inline uint256 Hash9(const T1 pbegin, const T1 pend)

{
    sph_blake512_context     ctx_blake;
    sph_bmw512_context       ctx_bmw;
    sph_groestl512_context   ctx_groestl;
    sph_jh512_context        ctx_jh;
    sph_keccak512_context    ctx_keccak;
    sph_skein512_context     ctx_skein;
    static unsigned char pblank[1];

#ifndef QT_NO_DEBUG
    //std::string strhash;
    //strhash = "";
#endif

    uint512 mask = 8;
    uint512 zero = 0;

    uint512 hash[9];

    sph_blake512_init(&ctx_blake);
    // ZBLAKE;
    sph_blake512 (&ctx_blake, (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0])), (pend - pbegin) * sizeof(pbegin[0]));
    sph_blake512_close(&ctx_blake, static_cast<void*>(&hash[0]));

    sph_bmw512_init(&ctx_bmw);
    // ZBMW;
    sph_bmw512 (&ctx_bmw, static_cast<const void*>(&hash[0]), 64);
    sph_bmw512_close(&ctx_bmw, static_cast<void*>(&hash[1]));

    if ((hash[1] & mask) != zero)
    {
        sph_groestl512_init(&ctx_groestl);
        // ZGROESTL;
        sph_groestl512 (&ctx_groestl, static_cast<const void*>(&hash[1]), 64);
        sph_groestl512_close(&ctx_groestl, static_cast<void*>(&hash[2]));
    }
    else
    {
        sph_skein512_init(&ctx_skein);
        // ZSKEIN;
        sph_skein512 (&ctx_skein, static_cast<const void*>(&hash[1]), 64);
        sph_skein512_close(&ctx_skein, static_cast<void*>(&hash[2]));
    }

    sph_groestl512_init(&ctx_groestl);
    // ZGROESTL;
    sph_groestl512 (&ctx_groestl, static_cast<const void*>(&hash[2]), 64);
    sph_groestl512_close(&ctx_groestl, static_cast<void*>(&hash[3]));

    sph_jh512_init(&ctx_jh);
    // ZJH;
    sph_jh512 (&ctx_jh, static_cast<const void*>(&hash[3]), 64);
    sph_jh512_close(&ctx_jh, static_cast<void*>(&hash[4]));

    if ((hash[4] & mask) != zero)
    {
        sph_blake512_init(&ctx_blake);
        // ZBLAKE;
        sph_blake512 (&ctx_blake, static_cast<const void*>(&hash[4]), 64);
        sph_blake512_close(&ctx_blake, static_cast<void*>(&hash[5]));
    }
    else
    {
        sph_bmw512_init(&ctx_bmw);
        // ZBMW;
        sph_bmw512 (&ctx_bmw, static_cast<const void*>(&hash[4]), 64);
        sph_bmw512_close(&ctx_bmw, static_cast<void*>(&hash[5]));
    }

    sph_keccak512_init(&ctx_keccak);
    // ZKECCAK;
    sph_keccak512 (&ctx_keccak, static_cast<const void*>(&hash[5]), 64);
    sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[6]));

    sph_skein512_init(&ctx_skein);
    // SKEIN;
    sph_skein512 (&ctx_skein, static_cast<const void*>(&hash[6]), 64);
    sph_skein512_close(&ctx_skein, static_cast<void*>(&hash[7]));

    if ((hash[7] & mask) != zero)
    {
        sph_keccak512_init(&ctx_keccak);
        // ZKECCAK;
        sph_keccak512 (&ctx_keccak, static_cast<const void*>(&hash[7]), 64);
        sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[8]));
    }
    else
    {
        sph_jh512_init(&ctx_jh);
        // ZJH;
        sph_jh512 (&ctx_jh, static_cast<const void*>(&hash[7]), 64);
        sph_jh512_close(&ctx_jh, static_cast<void*>(&hash[8]));
    }

    return hash[8].trim256();
}

inline int GetHashSelection(const uint256 PrevBlockHash, int index) {
    assert(index >= 0);
    assert(index < 16);

    #define START_OF_LAST_16_NIBBLES_OF_HASH 48
    int hashSelection = PrevBlockHash.GetNibble(START_OF_LAST_16_NIBBLES_OF_HASH + index);
    return(hashSelection);
}

template<typename T1>
inline uint256 HashX16R(const T1 pbegin, const T1 pend, const uint256 PrevBlockHash)
{
//	static std::chrono::duration<double>[16];
    int hashSelection;

    sph_blake512_context     ctx_blake;      //0
    sph_bmw512_context       ctx_bmw;        //1
    sph_groestl512_context   ctx_groestl;    //2
    sph_jh512_context        ctx_jh;         //3
    sph_keccak512_context    ctx_keccak;     //4
    sph_skein512_context     ctx_skein;      //5
    sph_luffa512_context     ctx_luffa;      //6
    sph_cubehash512_context  ctx_cubehash;   //7
    sph_shavite512_context   ctx_shavite;    //8
    sph_simd512_context      ctx_simd;       //9
    sph_echo512_context      ctx_echo;       //A
    sph_hamsi512_context     ctx_hamsi;      //B
    sph_fugue512_context     ctx_fugue;      //C
    sph_shabal512_context    ctx_shabal;     //D
    sph_whirlpool_context    ctx_whirlpool;  //E
    sph_sha512_context       ctx_sha512;     //F

    static unsigned char pblank[1];

    uint512 hash[16];

    for (int i=0;i<16;i++)
    {
        const void *toHash;
        int lenToHash;
        if (i == 0) {
            toHash = (pbegin == pend ? pblank : static_cast<const void*>(&pbegin[0]));
            lenToHash = (pend - pbegin) * sizeof(pbegin[0]);
        } else {
            toHash = static_cast<const void*>(&hash[i-1]);
            lenToHash = 64;
        }

        hashSelection = GetHashSelection(PrevBlockHash, i);

        switch(hashSelection) {
            case 0:
                sph_blake512_init(&ctx_blake);
                sph_blake512 (&ctx_blake, toHash, lenToHash);
                sph_blake512_close(&ctx_blake, static_cast<void*>(&hash[i]));
                break;
            case 1:
                sph_bmw512_init(&ctx_bmw);
                sph_bmw512 (&ctx_bmw, toHash, lenToHash);
                sph_bmw512_close(&ctx_bmw, static_cast<void*>(&hash[i]));
                break;
            case 2:
                sph_groestl512_init(&ctx_groestl);
                sph_groestl512 (&ctx_groestl, toHash, lenToHash);
                sph_groestl512_close(&ctx_groestl, static_cast<void*>(&hash[i]));
                break;
            case 3:
                sph_jh512_init(&ctx_jh);
                sph_jh512 (&ctx_jh, toHash, lenToHash);
                sph_jh512_close(&ctx_jh, static_cast<void*>(&hash[i]));
                break;
            case 4:
                sph_keccak512_init(&ctx_keccak);
                sph_keccak512 (&ctx_keccak, toHash, lenToHash);
                sph_keccak512_close(&ctx_keccak, static_cast<void*>(&hash[i]));
                break;
            case 5:
                sph_skein512_init(&ctx_skein);
                sph_skein512 (&ctx_skein, toHash, lenToHash);
                sph_skein512_close(&ctx_skein, static_cast<void*>(&hash[i]));
                break;
            case 6:
                sph_luffa512_init(&ctx_luffa);
                sph_luffa512 (&ctx_luffa, toHash, lenToHash);
                sph_luffa512_close(&ctx_luffa, static_cast<void*>(&hash[i]));
                break;
            case 7:
                sph_cubehash512_init(&ctx_cubehash);
                sph_cubehash512 (&ctx_cubehash, toHash, lenToHash);
                sph_cubehash512_close(&ctx_cubehash, static_cast<void*>(&hash[i]));
                break;
            case 8:
                sph_shavite512_init(&ctx_shavite);
                sph_shavite512(&ctx_shavite, toHash, lenToHash);
                sph_shavite512_close(&ctx_shavite, static_cast<void*>(&hash[i]));
                break;
            case 9:
                sph_simd512_init(&ctx_simd);
                sph_simd512 (&ctx_simd, toHash, lenToHash);
                sph_simd512_close(&ctx_simd, static_cast<void*>(&hash[i]));
                break;
            case 10:
                sph_echo512_init(&ctx_echo);
                sph_echo512 (&ctx_echo, toHash, lenToHash);
                sph_echo512_close(&ctx_echo, static_cast<void*>(&hash[i]));
                break;
           case 11:
                sph_hamsi512_init(&ctx_hamsi);
                sph_hamsi512 (&ctx_hamsi, toHash, lenToHash);
                sph_hamsi512_close(&ctx_hamsi, static_cast<void*>(&hash[i]));
                break;
           case 12:
                sph_fugue512_init(&ctx_fugue);
                sph_fugue512 (&ctx_fugue, toHash, lenToHash);
                sph_fugue512_close(&ctx_fugue, static_cast<void*>(&hash[i]));
                break;
           case 13:
                sph_shabal512_init(&ctx_shabal);
                sph_shabal512 (&ctx_shabal, toHash, lenToHash);
                sph_shabal512_close(&ctx_shabal, static_cast<void*>(&hash[i]));
                break;
           case 14:
                sph_whirlpool_init(&ctx_whirlpool);
                sph_whirlpool(&ctx_whirlpool, toHash, lenToHash);
                sph_whirlpool_close(&ctx_whirlpool, static_cast<void*>(&hash[i]));
                break;
           case 15:
                sph_sha512_init(&ctx_sha512);
                sph_sha512 (&ctx_sha512, toHash, lenToHash);
                sph_sha512_close(&ctx_sha512, static_cast<void*>(&hash[i]));
                break;
        }
    }

    return hash[15].trim256();
}



#endif // HASHBLOCK_H
