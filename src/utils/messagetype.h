#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H

//! An enum MessageType.
/*! Contains message types for client communication. */
enum MessageType {
    VERSION,  /*!< 0Connection */
    VERACK,  /*!< 1Connection */
    PING,  /*!< Connection */
    PONG,  /*!< Connection */
    TX,  /*!< Data */
    GETTX,  /*!< Request */
    BLOCK,  /*!< Data */
    GETBLOCK,  /*!< Request */
    ADDR,  /*!< Client Data */
    GETADDR,  /*!< Client Request */
    BCHAINSTATE,  /*!< Data */
    GETBCHAINSTATE,  /*!< Request */
    NOTFOUND,  /*!< ??? */
    MEMPOOL,  /*!< Data */
    GETMEMPOOL,  /*!< Request */
    UTXO,  /*!< Data */
    GETUTXO,  /*!< Request */
    REJECT,  /*!< ??? */
    UNDEFINED  /*!< 18??? */
};


static const QList<QString> messageTypeStr{"VERSION", "VERACK", "PING",
                                           "PONG", "TX", "GETTX", "BLOCK",
                                           "GETBLOCK", "ADDR", "GETADDR",
                                           "BCHAINSTATE", "GETBCHAINSTATE",
                                           "NOTFOUND", "MEMPOOL", "GETMEMPOOL",
                                           "UTXO", "GETUTXO", "REJECT",
                                           "UNDEFINED"};
#endif // MESSAGETYPE_H
