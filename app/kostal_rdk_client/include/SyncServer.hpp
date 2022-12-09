/**
 * @file SyncServer.hpp
 * @copyright Copyright (C) 2016-2022 Flexiv Ltd. All Rights Reserved.
 */
#ifndef FLEXIVRDK_SYNCSERVER_HPP_
#define FLEXIVRDK_SYNCSERVER_HPP_

#include "JsonParser.hpp"

namespace kostal {

typedef boost::asio::ip::tcp::acceptor acc;
typedef boost::asio::ip::tcp::endpoint end;
typedef boost::asio::ip::tcp::socket soc;

class SyncServer
{
protected:
    // create socket pointer
    std::unique_ptr<soc> m_socketPtr;

    // create acceptor pointer
    std::unique_ptr<acc> m_acceptorPtr;

private:
    // create kostal log object
    kostal::KostalLog k_log;

    // create a json parser
    kostal::JsonParser m_parserHandler;

    // create asio io_context and use it all the time
    boost::asio::io_context m_ioContext;

    // create a endpoint
    end m_endpoint;

    // set the port number with global value
    unsigned short m_portNumber = g_COMMPORT;

    // set the token with global value
    const std::string m_token = g_TOKEN;

    // set the message length to global value
    const int msgLength = g_MSGMAXSIZE;

    // received message
    std::string m_recvMsg;

    // reply message
    std::string m_replyMsg;

    // error code
    boost::system::error_code m_ec;

public:
    SyncServer() = default;
    virtual ~SyncServer() = default;

    /**
     * @brief Initialize the socket and get the spi config from the shakehand
     * message of the client
     * @return Status code
     */
    Status init();

    /**
     * @brief Receive buffer from the established talking session
     * @return Status code
     */
    Status recv();

    /**
     * @brief Check whether the server can not be able to listen to any request
     * from client for specific seconds, which means timeout interval
     * @return Status code
     */
    Status monitor();

    /**
     * @brief Set the port number of the socket
     */
    void setPortNumber(unsigned short port);

    /**
     * @brief Get the port number of the socket
     * @return recv message content
     */
    unsigned short getPortNumber();

    /**
     * @brief Get the content of the recv message
     * @return recv message content
     */
    std::string getRecvMsg();

    /**
     * @brief Get the content of the reply message
     * @return reply message content
     */
    std::string getReplyMsg();

    /**
     * @brief Clear the received message buffer
     */
    void clearMsg();

    /**
     * @brief Set the content of the replying message with param reply
     * @param[in] reply
     */
    void setReplyMsg(std::string reply);

    /**
     * @brief Disconnect the current socket and reset socket and acceptor in
     * asio
     */
    void disconnect();
};

} /* namespace kostal */

#endif /* FLEXIVRDK_SYNCSERVER_HPP_ */