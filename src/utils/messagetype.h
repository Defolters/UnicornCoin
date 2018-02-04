#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H

//! An enum.
/*! More detailed enum description. */
enum class MessageType {
    VERSION, /*!< Enum value TVal1. */
    VERACK,
    PING,
    PONG,
    TX,
    GETTX,
    BLOCK,
    GETBLOCK,
    ADDR,
    GETADDR,
    BCHAINSTATE,
    GETBCHAINSTATE,
    NOTFOUND,
    MEMPOOL,
    GETMEMPOOL,
    UTXO,
    GETUTXO,
    REJECT

};

#endif // MESSAGETYPE_H
