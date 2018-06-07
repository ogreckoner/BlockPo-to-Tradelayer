#ifndef OMNICORE_OMNICORE_H
#define OMNICORE_OMNICORE_H

class CBitcoinAddress;
class CBlockIndex;
class CCoinsView;
class CCoinsViewCache;
class CTransaction;

#include "omnicore/log.h"
#include "omnicore/persistence.h"
#include "omnicore/tally.h"

#include "sync.h"
#include "uint256.h"
#include "util.h"

#include <univalue.h>

#include <boost/filesystem/path.hpp>

#include "leveldb/status.h"

#include <stdint.h>

#include <map>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>

using std::string;

int const MAX_STATE_HISTORY = 50;

#define TEST_ECO_PROPERTY_1 (0x80000003UL)

// increment this value to force a refresh of the state (similar to --startclean)
#define DB_VERSION 1

// could probably also use: int64_t maxInt64 = std::numeric_limits<int64_t>::max();
// maximum numeric values from the spec:
#define MAX_INT_8_BYTES (9223372036854775807UL)

// maximum size of string fields
#define SP_STRING_FIELD_LEN 256

// Omni Layer Transaction Class
#define NO_MARKER    0
#define OMNI_CLASS_C 3 // uncompressed OP_RETURN
#define OMNI_CLASS_D 4 // compressed OP_RETURN

// Omni Layer Transaction (Packet) Version
#define MP_TX_PKT_V0  0
#define MP_TX_PKT_V1  1

#define MIN_PAYLOAD_SIZE     5

#define MAX_CLASS_D_SEARCH_BYTES   200

// Transaction types, from the spec
enum TransactionType {
  MSC_TYPE_SIMPLE_SEND                =  0,
  MSC_TYPE_RESTRICTED_SEND            =  2,
  MSC_TYPE_SEND_ALL                   =  4,
  MSC_TYPE_SAVINGS_MARK               = 10,
  MSC_TYPE_SAVINGS_COMPROMISED        = 11,
  MSC_TYPE_CREATE_PROPERTY_FIXED      = 50,
  MSC_TYPE_CREATE_PROPERTY_VARIABLE   = 51,
  MSC_TYPE_CLOSE_CROWDSALE            = 53,
  MSC_TYPE_CREATE_PROPERTY_MANUAL     = 54,
  MSC_TYPE_GRANT_PROPERTY_TOKENS      = 55,
  MSC_TYPE_REVOKE_PROPERTY_TOKENS     = 56,
  MSC_TYPE_CHANGE_ISSUER_ADDRESS      = 70,
  OMNICORE_MESSAGE_TYPE_DEACTIVATION  = 65533,
  OMNICORE_MESSAGE_TYPE_ACTIVATION    = 65534,
  OMNICORE_MESSAGE_TYPE_ALERT         = 65535,

  ////////////////////////////////////
  /** New things for Contract */
  MSC_TYPE_CONTRACTDEX_TRADE          = 29,
  MSC_TYPE_CONTRACTDEX_CANCEL_PRICE   = 30,
  MSC_TYPE_CONTRACTDEX_CANCEL_ECOSYSTEM   = 32,
  MSC_TYPE_CONTRACTDEX_CLOSE_POSITION   = 33,
  /** !Here we changed "MSC_TYPE_OFFER_ACCEPT_A_BET = 40" */
  MSC_TYPE_CREATE_CONTRACT            = 40,
  MSC_TYPE_PEGGED_CURRENCY            = 100,
  MSC_TYPE_REDEMPTION_PEGGED          = 101,
  MSC_TYPE_SEND_PEGGED_CURRENCY       = 102,


  ////////////////////////////////////

};

#define MSC_PROPERTY_TYPE_INDIVISIBLE             1
#define MSC_PROPERTY_TYPE_DIVISIBLE               2

enum FILETYPES {
  FILETYPE_BALANCES = 0,
  FILETYPE_GLOBALS,
  FILETYPE_CROWDSALES,
  FILETYPE_CDEXORDERS,
  NUM_FILETYPES
};

#define PKT_RETURNED_OBJECT    (1000)

#define PKT_ERROR             ( -9000)
// Smart Properties
#define PKT_ERROR_SP          (-40000)
#define PKT_ERROR_CROWD       (-45000)
// Send To Owners
#define PKT_ERROR_SEND        (-60000)
#define PKT_ERROR_TOKENS      (-82000)
#define PKT_ERROR_SEND_ALL    (-83000)
#define PKT_ERROR_METADEX     (-80000)
#define METADEX_ERROR         (-81000)

#define OMNI_PROPERTY_BTC             0
#define OMNI_PROPERTY_MSC             1
#define OMNI_PROPERTY_TMSC            2
//////////////////////////////////////
/** New things for Contracts */
#define BUY   1
#define SELL  2
#define ACTIONINVALID  3
//////////////////////////////////////

/** New for future contracts port */
#define MSC_PROPERTY_TYPE_CONTRACT    3

// forward declarations
std::string FormatDivisibleMP(int64_t amount, bool fSign = false);
std::string FormatDivisibleShortMP(int64_t amount);
std::string FormatMP(uint32_t propertyId, int64_t amount, bool fSign = false);
std::string FormatShortMP(uint32_t propertyId, int64_t amount);
std::string FormatByType(int64_t amount, uint16_t propertyType);
std::string FormatByDivisibility(int64_t amount, bool divisible);
double FormatContractShortMP(int64_t n);

/** Returns the marker for transactions. */
const std::vector<unsigned char> GetOmMarker();

//! Used to indicate, whether to automatically commit created transactions
extern bool autoCommit;

//! Global lock for state objects
extern CCriticalSection cs_tally;

/** LevelDB based storage for storing Omni transaction data.  This will become the new master database, holding serialized Omni transactions.
 *  Note, intention is to consolidate and clean up data storage
 */
class COmniTransactionDB : public CDBBase
{
public:
    COmniTransactionDB(const boost::filesystem::path& path, bool fWipe)
    {
        leveldb::Status status = Open(path, fWipe);
        PrintToConsole("Loading master transactions database: %s\n", status.ToString());
    }

    virtual ~COmniTransactionDB()
    {
        if (msc_debug_persistence) PrintToLog("COmniTransactionDB closed\n");
    }

    /* These functions would be expanded upon to store a serialized version of the transaction and associated state data
     *
     * void RecordTransaction(const uint256& txid, uint32_t posInBlock, various, other, data);
     * int FetchTransactionPosition(const uint256& txid);
     * bool FetchTransactionValidity(const uint256& txid);
     *
     * and so on...
     */
    void RecordTransaction(const uint256& txid, uint32_t posInBlock);
    uint32_t FetchTransactionPosition(const uint256& txid);
};

/** LevelDB based storage for transactions, with txid as key and validity bit, and other data as value.
 */
class CMPTxList : public CDBBase
{
public:
    CMPTxList(const boost::filesystem::path& path, bool fWipe)
    {
        leveldb::Status status = Open(path, fWipe);
        PrintToConsole("Loading tx meta-info database: %s\n", status.ToString());
    }

    virtual ~CMPTxList()
    {
        if (msc_debug_persistence) PrintToLog("CMPTxList closed\n");
    }

    void recordTX(const uint256 &txid, bool fValid, int nBlock, unsigned int type, uint64_t nValue);
    /** Records a "send all" sub record. */
    void recordSendAllSubRecord(const uint256& txid, int subRecordNumber, uint32_t propertyId, int64_t nvalue);

    string getKeyValue(string key);
    /** Returns the number of sub records. */
    int getNumberOfSubRecords(const uint256& txid);
    /** Retrieves details about a "send all" record. */
    bool getSendAllDetails(const uint256& txid, int subSend, uint32_t& propertyId, int64_t& amount);
    int getMPTransactionCountTotal();
    int getMPTransactionCountBlock(int block);

    int getDBVersion();
    int setDBVersion();

    bool exists(const uint256 &txid);
    bool getTX(const uint256 &txid, string &value);

    std::set<int> GetSeedBlocks(int startHeight, int endHeight);
    void LoadAlerts(int blockHeight);
    void LoadActivations(int blockHeight);

    void printStats();
    void printAll();

    bool isMPinBlockRange(int, int, bool);

    void recordPaymentTX(const uint256 &txid, bool fValid, int nBlock, unsigned int vout, unsigned int propertyId, uint64_t nValue, string buyer, string seller);
    void recordMetaDExCancelTX(const uint256 &txidMaster, const uint256 &txidSub, bool fValid, int nBlock, unsigned int propertyId, uint64_t nValue);

    /////////////////////////////////////////////
    /** New things for Contracts */
    void recordContractDexCancelTX(const uint256 &txidMaster, const uint256 &txidSub, bool fValid, int nBlock, unsigned int propertyId, uint64_t nValue);
    /////////////////////////////////////////////

    uint256 findMetaDExCancel(const uint256 txid);
    /** Returns the number of sub records. */
    int getNumberOfMetaDExCancels(const uint256 txid);

    //////////////////////////////////////
    /** New things for Contracts */
    int getNumberOfContractDexCancels(const uint256 txid);
    //////////////////////////////////////

};
/** LevelDB based storage for the trade history. Trades are listed with key "txid1+txid2".
 */
class CMPTradeList : public CDBBase
{
public:
    CMPTradeList(const boost::filesystem::path& path, bool fWipe)
    {
        leveldb::Status status = Open(path, fWipe);
        PrintToConsole("Loading trades database: %s\n", status.ToString());
    }

    virtual ~CMPTradeList()
    {
        if (msc_debug_persistence) PrintToLog("CMPTradeList closed\n");
    }

    void recordMatchedTrade(const uint256 txid1, const uint256 txid2, string address1, string address2, unsigned int prop1, unsigned int prop2, uint64_t amount1, uint64_t amount2, int blockNum, int64_t fee);

    /////////////////////////////////
    /** New things for Contract */
    /** New things for Contract */
    void recordMatchedTrade(const uint256 txid1, const uint256 txid2, string address1, string address2, int64_t nCouldbuy, int64_t amountForsale, int64_t amountStillForsale, int blockNum1, int blockNum2, string s_status1, string s_status2, int64_t lives_maker, int64_t lives_taker, uint32_t property_traded, string tradeStatus, int64_t pricepold, int64_t pricepnew, uint8_t pnewAction);
    void recordForUPNL(const uint256 txid, string address, uint32_t property_traded, int64_t effectivePrice);
    // void recordMatchedTrade(const uint256 txid1, const uint256 txid2, string address1, string address2, unsigned int prop1, unsigned int prop2, uint64_t amount1, uint64_t amount2, int blockNum, int64_t fee, string t_status, std::vector<uint256> &vecTxid);
    /////////////////////////////////

    void recordNewTrade(const uint256& txid, const std::string& address, uint32_t propertyIdForSale, uint32_t propertyIdDesired, int blockNum, int blockIndex);
    int deleteAboveBlock(int blockNum);
    bool exists(const uint256 &txid);
    void printStats();
    void printAll();
    bool getMatchingTrades(const uint256& txid, uint32_t propertyId, UniValue& tradeArray, int64_t& totalSold, int64_t& totalBought);

    ///////////////////////////////////////
    /** New things for Contract */
    bool getMatchingTrades(uint32_t propertyId, UniValue& tradeArray);
    double getPNL(string address, int64_t contractsClosed, int64_t price, uint32_t property, uint32_t marginRequirementContract, uint32_t notionalSize, std::string Status);
    void marginLogic(uint32_t property);
    double getUPNL(string address, uint32_t contractId);
    //////////////////////////////////////

    bool getMatchingTrades(const uint256& txid);
    void getTradesForAddress(std::string address, std::vector<uint256>& vecTransactions, uint32_t propertyIdFilter = 0);
    void getTradesForPair(uint32_t propertyIdSideA, uint32_t propertyIdSideB, UniValue& response, uint64_t count);
    int getMPTradeCountTotal();
};


//! Available balances of wallet properties
extern std::map<uint32_t, int64_t> global_balance_money;
//! Vector containing a list of properties relative to the wallet
extern std::set<uint32_t> global_wallet_property_list;

int64_t getMPbalance(const std::string& address, uint32_t propertyId, TallyType ttype);
int64_t getUserAvailableMPbalance(const std::string& address, uint32_t propertyId);

/** Global handler to initialize Omni Core. */
int omnicorelite_init();

/** Global handler to shut down Omni Core. */
int omnicorelite_shutdown();

/** Global handler to total wallet balances. */
void CheckWalletUpdate(bool forceUpdate = false);

/** Used to notify that the number of tokens for a property has changed. */
void NotifyTotalTokensChanged(uint32_t propertyId);

int mastercore_handler_disc_begin(int nBlockNow, CBlockIndex const * pBlockIndex);
int mastercore_handler_disc_end(int nBlockNow, CBlockIndex const * pBlockIndex);
int mastercore_handler_block_begin(int nBlockNow, CBlockIndex const * pBlockIndex);
int mastercore_handler_block_end(int nBlockNow, CBlockIndex const * pBlockIndex, unsigned int);
bool mastercore_handler_tx(const CTransaction& tx, int nBlock, unsigned int idx, const CBlockIndex* pBlockIndex);
int mastercore_save_state( CBlockIndex const *pBlockIndex );

namespace mastercore
{
extern std::unordered_map<std::string, CMPTally> mp_tally_map;
/*New things for contracts*///////////////////////////////////
// extern std::unordered_map<std::string, CDexTally> cd_tally_map;
//////////////////////////////////////////////////////////////
extern CMPTxList *p_txlistdb;
extern COmniTransactionDB *p_OmniTXDB;
extern CMPTradeList *t_tradelistdb;

// TODO: move, rename
extern CCoinsView viewDummy;
extern CCoinsViewCache view;
//! Guards coins view cache
extern CCriticalSection cs_tx_cache;

std::string strMPProperty(uint32_t propertyId);

bool isMPinBlockRange(int starting_block, int ending_block, bool bDeleteFound);

std::string FormatIndivisibleMP(int64_t n);

int WalletTxBuilder(const std::string& senderAddress, const std::string& receiverAddress, int64_t referenceAmount,
     const std::vector<unsigned char>& data, uint256& txid, std::string& rawHex, bool commit, unsigned int minInputs = 1);

bool isTestEcosystemProperty(uint32_t propertyId);
bool isMainEcosystemProperty(uint32_t propertyId);
uint32_t GetNextPropertyId(bool maineco); // maybe move into sp

CMPTally* getTally(const std::string& address);

int64_t getTotalTokens(uint32_t propertyId, int64_t* n_owners_total = NULL);

std::string strTransactionType(uint16_t txType);

/** Returns the encoding class, used to embed a payload. */
int GetEncodingClass(const CTransaction& tx, int nBlock);

/** Determines, whether it is valid to use a Class C transaction for a given payload size. */
bool UseEncodingClassC(size_t nDataSize);

bool getValidMPTX(const uint256 &txid, int *block = NULL, unsigned int *type = NULL, uint64_t *nAmended = NULL);

bool update_tally_map(const std::string& who, uint32_t propertyId, int64_t amount, TallyType ttype);

std::string getTokenLabel(uint32_t propertyId);
}


#endif // OMNICORE_OMNICORE_H
