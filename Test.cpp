// Author: Ivan Arizanovic <ivanarizanovic@yahoo.com>

#include "BrokerClient.h"
#include "gtest/gtest.h"

// Default patameters
std::string gConnUrl = "//127.0.0.1:5672";
std::string gAddr = "testing";
std::string gMsg = "Broker Test";

namespace Tests {
    std::string BrokerClientTest(bool send_invalid_message) {
        BrokerClient handler(gConnUrl, gAddr, gMsg, send_invalid_message);
        return Database::getLatestValue();
    }

    GTEST_TEST(BrokerClientTest, SendAndRecive) {
        EXPECT_EQ(BrokerClientTest(false), gMsg);
    };
    GTEST_TEST(BrokerClientTest, ReturnInvalidMessage) {
        EXPECT_EQ(BrokerClientTest(true), INVALID_MESSAGE);
    };
}

int main(int argc, char** argv) {
    // Get the arguments
    for (auto i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--url" && argc > i + 1)
            gConnUrl = argv[++i];
        else if (arg == "--addr" && argc > i + 1)
            gAddr = argv[++i];
        else if (arg == "--msg" && argc > i + 1)
            gMsg = argv[++i];
        else {
            std::cout << "Error: '" << arg << "' is unknown argument." << std::endl;
            return -1;
        }
    }

    // Initialize the Google tests
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}