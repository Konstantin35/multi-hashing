#include "../common/utils/common.h"
#include "../nexa/hashwrapper.h"

uint256 SatoshiBlockHeader::GetHash() const { return SerializeHash(*this); }

uint256 CBlockHeader::GetMiningHeaderCommitment() const
{
    CSHA256Writer miniHeader;
    miniHeader << hashPrevBlock << nBits;
    uint256 miniHash = miniHeader.GetHash();

    CSHA256Writer extHeader;
    extHeader << hashAncestor << hashTxFilter << hashMerkleRoot << nTime << ((uint64_t)height) << chainWork << size
              << txCount << feePoolAmt << utxoCommitment << minerData;
    uint256 extHash = extHeader.GetHash();

    CSHA256Writer commitment;
    commitment << miniHash << extHash;
    uint256 ret = commitment.GetHash();
    return ret;
}
