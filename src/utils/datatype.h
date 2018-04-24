#ifndef DATATYPE_H
#define DATATYPE_H

//! An enum DataType.
/*! Contains data types for client communication. */
enum DataType {
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


static const QList<QString> dataTypeStr{"VERSION", "VERACK", "PING",
                                           "PONG", "TX", "GETTX", "BLOCK",
                                           "GETBLOCK", "ADDR", "GETADDR",
                                           "BCHAINSTATE", "GETBCHAINSTATE",
                                           "NOTFOUND", "MEMPOOL", "GETMEMPOOL",
                                           "UTXO", "GETUTXO", "REJECT",
                                           "UNDEFINED"};
#endif // DATATYPE_H
