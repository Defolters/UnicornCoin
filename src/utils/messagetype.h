#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H

//! An enum MessageType.
/*! Contains message types for client communication. */
enum class MessageType {
    VERSION,  /*!< Data */
    VERACK,  /*!< */
    PING,  /*!< */
    PONG,  /*!< */
    TX,  /*!< Data */
    GETTX,  /*!< Request */
    BLOCK,  /*!< Data */
    GETBLOCK,  /*!< Request */
    ADDR,  /*!< Data */
    GETADDR,  /*!< Request */
    BCHAINSTATE,  /*!< Data */
    GETBCHAINSTATE,  /*!< Request */
    NOTFOUND,  /*!< Data */
    MEMPOOL,  /*!< Data */
    GETMEMPOOL,  /*!< Request */
    UTXO,  /*!< Data */
    GETUTXO,  /*!< Request */
    REJECT,  /*!< Data */
    UNDEFINED  /*!< */

};

#endif // MESSAGETYPE_H
