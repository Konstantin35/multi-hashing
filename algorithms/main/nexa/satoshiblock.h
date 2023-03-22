// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Copyright (c) 2015-2022 The Bitcoin Unlimited developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef NEXA_PRIMITIVES_SATOSHI_BLOCK_H
#define NEXA_PRIMITIVES_SATOSHI_BLOCK_H

#include "serialize.h"
#include "../common/ethash/uint256.h"
class arith_uint256;

const uint32_t BIP_009_MASK = 0x20000000;
const uint32_t BASE_VERSION = 0x20000000;
const uint32_t FORK_BIT_2MB = 0x10000000; // Vote for 2MB fork
const bool DEFAULT_2MB_VOTE = false;
static const int PROTOCOL_VERSION = 80003;

/** Get the work equivalent for the supplied nBits of difficulty */
arith_uint256 GetWorkForDifficultyBits(uint32_t nBits);

class SatoshiBlockHeader
{
public:
    // header
    static const int32_t CURRENT_VERSION = BASE_VERSION;
    int32_t nVersion;
    uint256 hashPrevBlock;
    uint256 hashMerkleRoot;
    uint32_t nTime;
    uint32_t nBits;
    uint32_t nNonce;

    SatoshiBlockHeader() { SetNull(); }
    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream &s, Operation ser_action)
    {
        READWRITE(this->nVersion);
        READWRITE(hashPrevBlock);
        READWRITE(hashMerkleRoot);
        READWRITE(nTime);
        READWRITE(nBits);
        READWRITE(nNonce);
    }

    bool operator==(const SatoshiBlockHeader &b)
    {
        return (nVersion == b.nVersion && hashPrevBlock == b.hashPrevBlock && hashMerkleRoot == b.hashMerkleRoot &&
                nTime == b.nTime && nBits == b.nBits && nNonce == b.nNonce);
    }

    void SetNull()
    {
        nVersion = 0;
        hashPrevBlock.SetNull();
        hashMerkleRoot.SetNull();
        nTime = 0;
        nBits = 0;
        nNonce = 0;
    }

    bool IsNull() const { return (nBits == 0); }
    uint256 GetHash() const;

    int64_t GetBlockTime() const { return (int64_t)nTime; }
};

/** The expected size of a serialized block header */
static const unsigned int SATOSHI_SERIALIZED_HEADER_SIZE = ::GetSerializeSize(SatoshiBlockHeader(), SER_NETWORK, PROTOCOL_VERSION);

class SatoshiBlock : public SatoshiBlockHeader
{
private:
    // memory only
    mutable uint64_t nBlockSize; // Serialized block size in bytes

public:
    // Xpress Validation: (memory only)
    //! Orphans, or Missing transactions that have been re-requested, are stored here.
    std::set<uint256> setUnVerifiedTxns;

    // Xpress Validation: (memory only)
    //! A flag which when true indicates that Xpress validation is enabled for this block.
    bool fXVal;

public:
    // memory only
    // 0.11: mutable std::vector<uint256> vMerkleTree;
    mutable bool fChecked;
    mutable bool fExcessive; // Is the block "excessive"

    SatoshiBlock() { SetNull(); }
    SatoshiBlock(const SatoshiBlockHeader &header)
    {
        SetNull();
        *((SatoshiBlockHeader *)this) = header;
    }

    static bool VersionKnown(int32_t nVersion, int32_t voteBits)
    {
        if (nVersion >= 1 && nVersion <= 4)
            return true;
        // BIP009 / versionbits:
        if (nVersion & BIP_009_MASK)
        {
            uint32_t v = nVersion & ~BIP_009_MASK;
            if ((v & ~voteBits) == 0)
                return true;
        }
        return false;
    }

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream &s, Operation ser_action)
    {
        READWRITE(*(SatoshiBlockHeader *)this);
        READWRITE(vtx);
    }

    void SetNull()
    {
        SatoshiBlockHeader::SetNull();
        vtx.clear();
        fChecked = false;
        fExcessive = false;
        fXVal = false;
        nBlockSize = 0;
    }

    SatoshiBlockHeader GetBlockHeader() const
    {
        SatoshiBlockHeader block;
        block.nVersion = nVersion;
        block.hashPrevBlock = hashPrevBlock;
        block.hashMerkleRoot = hashMerkleRoot;
        block.nTime = nTime;
        block.nBits = nBits;
        block.nNonce = nNonce;
        return block;
    }

    std::string ToString() const;

    // Return the serialized block size in bytes. This is only done once and then the result stored
    // in nBlockSize for future reference, saving unncessary and expensive serializations.
    uint64_t GetBlockSize() const;
};

#endif // NEXA_PRIMITIVES_SATOSHI_BLOCK_H
