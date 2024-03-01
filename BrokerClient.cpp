// Author: Ivan Arizanovic <ivanarizanovic@yahoo.com>

#include "BrokerClient.h"

void
BrokerClient::on_container_start(proton::container& c) {
    c.connect(mConnUrl);
}

void
BrokerClient::on_connection_open(proton::connection& c) {
    c.open_receiver(mAddr);
    c.open_sender(mAddr);
}

void
BrokerClient::on_sendable(proton::sender& s) {
    if (!mMessageSent) {
        mMessageSent = true;

        if (!mSilent)
            std::cout << "Send msg: " << mMessage << std::endl;

        // Send message to the message broker
        proton::message m(mMessage);
        s.send(m);

        s.close();
    }
}

void
BrokerClient::on_message(proton::delivery& d, proton::message& m) {
    if (!mSilent)
        std::cout << "Received: " << m.body() << std::endl;

    // Persist message in SQLite
    Database::persistMessageInSQLite(to_string(m.body()));

    // Set the ACCEPTED state
    d.accept();

    // Return invalid Message to Sender
    if (mSendInvalidMessage && !mInvalidMessageSent) {
        mInvalidMessageSent = true;
        if (!mSilent)
            std::cout << "Send msg: " << INVALID_MESSAGE << std::endl;

        proton::message successMessage(INVALID_MESSAGE);
        d.connection().open_sender(mAddr).send(successMessage);
    }
    // Close the connection after receiving the messages
    else if((!mSendInvalidMessage && m.body() != INVALID_MESSAGE) ||
            (mInvalidMessageSent && m.body() == INVALID_MESSAGE))
        d.connection().close();
}
