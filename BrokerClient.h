// Author: Ivan Arizanovic <ivanarizanovic@yahoo.com>

#pragma once
#include "Database.h"
#include <proton/connection.hpp>
#include <proton/container.hpp>
#include <proton/delivery.hpp>
#include <proton/message.hpp>
#include <proton/messaging_handler.hpp>
#include <proton/tracker.hpp>

#define INVALID_MESSAGE "Invalid message"

class BrokerClient : public proton::messaging_handler {
public:
    //--------------------------------------------------------------------------
    // Broker Client interface
    // It sends and receives messages to/from MQ broker
    // 
    // @param url       Connection URL (e.g. "//127.0.0.1:5672")
    // @param addr      Address
    // @param msg       Message which will be sent to MQ
    // @param invalid   Return the invalid message to sender after received
    //                  message (by default false)
    // @param silent    Enable silent mode (by default false)
    //--------------------------------------------------------------------------
    BrokerClient(const std::string& url, const std::string& addr,
                 const std::string& msg, const bool& invalid = false,
                 const bool& silent = false) : mConnUrl(url), mAddr(addr),
                 mMessage(msg), mSendInvalidMessage(invalid), mSilent(silent)
    {
        proton::container(*this).run();
    }

private:
    std::string mConnUrl;
    std::string mAddr;
    std::string mMessage;
    bool mSilent;
    bool mMessageSent = false;
    bool mInvalidMessageSent = false;
    bool mSendInvalidMessage = true;

    // Override the Qpid-proton's functions
    void on_container_start(proton::container& c) override;
    void on_connection_open(proton::connection& c) override;
    void on_sendable(proton::sender& s) override;
    void on_message(proton::delivery& d, proton::message& m) override;
};
