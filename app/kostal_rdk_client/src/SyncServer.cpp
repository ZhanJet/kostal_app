/**
 * @file SyncServer.cpp
 * @date Nov 29, 2022
 * @author Chenchen Liu
 */

#include "SyncServer.hpp"

namespace kostal {

Status SyncServer::init()
{
    Status result;
    try {
        // create an endpoint
        m_endpoint = std::move(boost::asio::ip::tcp::endpoint(
            boost::asio::ip::tcp::v4(), m_portNumber));

        // create an unique pointer of io_context
        m_socketPtr
            = std::make_unique<boost::asio::ip::tcp::socket>(m_ioContext);

        // create an unique pointer of acceptor
        m_acceptorPtr.reset(
            new boost::asio::ip::tcp::acceptor(m_ioContext, m_endpoint));

        // print hint log
        k_log->info("Waiting for client to send token...");

        // this is a block function, it will block until a new connection is
        // built successfully or an error happens
        m_acceptorPtr->accept(*m_socketPtr);

        // create receive buffer
        char m_recvBuffer[msgLength];

        // only correct shakehand token can pass this while-loop, but error
        // in socket will result in error code SOCKET
        while (true) {
            // try to read some buffer from this sole socket
            size_t read_length = m_socketPtr->read_some(
                boost::asio::buffer(m_recvBuffer, msgLength), m_ec);

            // If the client close the socket
            if (m_ec == boost::asio::error::eof) {
                k_log->error(
                    "===================================================");
                k_log->error("The connection is closed cleanly by client");
                return SOCKET;
                // If there is some other error message
            } else if (m_ec) {
                k_log->error("Socket reads message error: " + m_ec.message());
                return SOCKET;
            }

            // Manually clear the message variable
            clearMsg();
            for (int i = 0; i < read_length; i++) {
                m_recvMsg += m_recvBuffer[i];
            }

            // parse the received spi config
            std::string recvToken = "";
            result
                = m_parserHandler.parseSPIJSON(&m_recvMsg, &recvToken, k_log);

            // Judge whether the received message is matching the token, if
            // not match, reply "wrong" to client
            if (recvToken != m_token) {
                k_log->warn(
                    "The client is sending an unkown token: " + recvToken);
                const char wrongReply[6] = "wrong";
                boost::asio::write(*m_socketPtr,
                    boost::asio::buffer(wrongReply, std::strlen(wrongReply)),
                    m_ec);
                k_log->info("Waiting for client to resend token...");
                continue;
            }

            // If the token is matched, reply received to client
            std::string reply = "received";
            boost::asio::write(
                *m_socketPtr, boost::asio::buffer(reply, reply.size()), m_ec);

            // If the client close the socket
            if (m_ec == boost::asio::error::eof) {
                k_log->error(
                    "===================================================");
                k_log->error("The connection is closed cleanly by client");
                return SOCKET;
                // If there is some other error message
            } else if (m_ec) {
                k_log->error("Socket writes message error: " + m_ec.message());
                return SOCKET;
            }

            // The shakehand is finished
            k_log->info("The initialization is completed");
            return SUCCESS;
        }
    } catch (std::exception& e) {
        k_log->error(e.what());
        return SOCKET;
    }
}

Status SyncServer::recv()
{
    try {
        // receive buffer
        char m_recvBuffer[msgLength];
        size_t read_length = m_socketPtr->read_some(
            boost::asio::buffer(m_recvBuffer, msgLength), m_ec);
        if (m_ec == boost::asio::error::eof) {
            k_log->error("===================================================");
            k_log->error(
                "The connection is closed cleanly by client or timeout");
            return SOCKET;
        } else if (m_ec) {
            k_log->error("Socket reads message error: " + m_ec.message());
            return SOCKET;
        }

        clearMsg();
        for (int i = 0; i < read_length; i++) {
            m_recvMsg += m_recvBuffer[i];
        }
        std::cout<<"The recv msg is: "<<m_recvMsg<<std::endl;

        boost::asio::write(*m_socketPtr,
            boost::asio::buffer(m_replyMsg, m_replyMsg.size()), m_ec);
        if (m_ec == boost::asio::error::eof) {
            k_log->error("===================================================");
            k_log->error(
                "The connection is closed cleanly by client or timeout");
            return SOCKET;
        } else if (m_ec) {
            k_log->error("Socket writes message error: " + m_ec.message());
            return SOCKET;
        }

        return SUCCESS;
    } catch (std::exception& e) {
        k_log->error(e.what());
        return SOCKET;
    }
}

Status SyncServer::monitor()
{
    // receive message in async way
    auto result = std::async(&SyncServer::recv, this);

    // set timeout for communication
    std::future_status status
        = result.wait_for(std::chrono::seconds(g_timeoutInterval));

    // if the situation is timeout
    if (status == std::future_status::timeout) {
        k_log->error("===================================================");
        k_log->error("The connection with client is timeout...");
        this->disconnect();
        return SOCKET;

        // if the situation is successfully
    } else if (status == std::future_status::ready) {

        // check whether the async task is successful
        if (result.get() != SUCCESS) {
            k_log->error("The receiving of client's message fails...");
            this->disconnect();
            return SOCKET;
        } else {
            // all successful heartbeat goes here
            return SUCCESS;
        }
    } else {
        k_log->error("The future status is deferred...");
        this->disconnect();
        return SOCKET;
    }
}

void SyncServer::setPortNumber(unsigned short port)
{
    m_portNumber = port;
}

unsigned short SyncServer::getPortNumber()
{
    return m_portNumber;
}

std::string SyncServer::getRecvMsg()
{
    if (m_recvMsg.empty()) {
        k_log->warn("The received message is empty");
    }
    return m_recvMsg;
}

std::string SyncServer::getReplyMsg()
{
    if (m_replyMsg.empty()) {
        k_log->warn("The reply message is empty");
    }
    return m_replyMsg;
}

void SyncServer::clearMsg()
{
    m_recvMsg = "";
}

void SyncServer::setReplyMsg(std::string reply)
{
    if (reply.empty()) {
        k_log->error("The server is sending empty response");
        m_replyMsg = "FAULT";
    }
    m_replyMsg = reply;
}

void SyncServer::disconnect()
{
    // shut down the socket if it is open
    if (m_socketPtr->is_open()) {
        m_socketPtr->shutdown(soc::shutdown_both, m_ec);
    }

    // just close socket solely
    m_socketPtr->close(m_ec);
    if (m_ec) {
        k_log->error(m_ec.message());
    }

    // reset socket and acceptor
    m_socketPtr.reset();
    m_acceptorPtr.reset();
}

} /* namespace kostal */
