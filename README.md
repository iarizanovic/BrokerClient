# BrokerClient

BrokerClient is package which interact with Message Broker.

There are two libraries in the package and one test executable. First library is main one for Broker client and second library is helper one for database operations.

Dependencies for Windows x86 platform are implemented in the project (*bin/*, *include/* and *lib/* folders).

## Build
To build the libraries and test executable, one needs *MS Visual Studio* (tested with *Visual Studio 2022*).

Just open the project folder in Visual Studio and click the *Build* button.

## Usage of the main Broker client library
Library simply send the message (**msg** parameter) to the MQ (configured with **url** and **addr** parameters) and received it. Then the received message will be stored in the database using the database library.

If **invalid** parameter is enabled, it returns the invalid message to sender after received message.

If **silent** parameter is enabled, it doesn't have any output.

```cpp
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
             const bool& silent = false);
```

## Usage of the helper database library
This library saves the messages in SQLite database. One can do it with the following function:
```cpp
//--------------------------------------------------------------------------
// Save received message to database
// @param message   Input message
// @return          Error code from SQLite
//--------------------------------------------------------------------------
static int persistMessageInSQLite(const std::string& message);
```
To get the latest saved message, one can use the following function:
```cpp
//--------------------------------------------------------------------------
// Get the latest message
// @return          The latest message
//--------------------------------------------------------------------------
static std::string getLatestValue();
```

## Example
Following example presents how to simply use the BrokerClient and Database libraries:
```c
#include "BrokerClient.h"

// Default patameters
std::string gConnUrl = "//127.0.0.1:5672";
std::string gAddr = "testing";
std::string gMsg = "Broker Test";
bool gSendInvalidMessage = false;
bool gSilent = false;

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
        else if (arg == "--invalid")
            gSendInvalidMessage = true;
        else if (arg == "--silent")
            gSilent = true;
        else {
            std::cout << "Error: '" << arg << "' is unknown argument." << std::endl;
            return -1;
        }
    }

    // Initialize
    BrokerClient handler(gConnUrl, gAddr, gMsg, gSendInvalidMessage, gSilent);

    // Get the latest received message
    std::cout << "The latest received message: " << Database::getLatestValue();
    return 0;
}
```

## Test binary
Test binary runs two tests with following scenarios:
1. Send and receive the message
2. Send and receive the message and then return the invalid message to the sender and receive the invalid message.
```
[==========] Running 2 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 2 tests from BrokerClientTest
[ RUN      ] BrokerClientTest.SendAndRecive
Send msg: Broker Test
Received: Broker Test
[       OK ] BrokerClientTest.SendAndRecive (10 ms)
[ RUN      ] BrokerClientTest.ReturnInvalidMessage
Send msg: Broker Test
Received: Broker Test
Send msg: Invalid message
Received: Invalid message
[       OK ] BrokerClientTest.ReturnInvalidMessage (12 ms)
[----------] 2 tests from BrokerClientTest (22 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test suite ran. (23 ms total)
[  PASSED  ] 2 tests.
```

### Usage
```
.\Test.exe [OPTIONS]
```
The test executable can be configured with following options:
* *--url <Connection_URL>* : Set the connection URL of the MQ (by default is *"//127.0.0.1:5672"*)
* *--addr <Address>* : Set the address (by default is *"testing"*)
* *--msg <Message>* : Set the message which will be sent (by default is *"Broker Test"*)

## Structure
Here is the structure for the project:
```
project_root/
|-- bin/                     <- Dynamic-link libraries of the dependencies
|   |-- gmock.dll
|   |-- gmock_main.dll
|   |-- gtest.dll
|   |-- gtest_main.dll
|   |-- jsoncpp.dll
|   |-- qpid-proton-core.dll
|   |-- qpid-proton-cpp.dll
|   |-- qpid-proton-proactor.dll
|   |-- qpid-proton.dll
|   |-- sqlite3.dll
|-- include/                 <- Source code of the dependencies
|   |-- gmock/...
|   |-- gtest/...
|   |-- proton/...
|   |-- sqlite3-vcpkg-config.h
|   |-- sqlite3.h
|   |-- sqlite3ext.h
|-- lib/                     <- Static libraries of the dependencies
|   |-- gmock.lib
|   |-- gmock_main.lib
|   |-- gtest.lib
|   |-- gtest_main.lib
|   |-- qpid-proton-core.lib
|   |-- qpid-proton-cpp.lib
|   |-- qpid-proton-proactor.lib
|   |-- qpid-proton.lib
|   |-- sqlite3.lib
|-- BrokerClient.cpp         <- C++ of the main BrokerClient library
|-- BrokerClient.h           <- Header of the main BrokerClient library
|-- CMakeLists.txt           <- Cmake file
|-- CMakePresets.json        <- Cmake project preset
|-- Database.cpp             <- Cpp of the helper Database library
|-- Database.h               <- Header of the helper Database library
|-- README.MD                <- Readme file of the project
|-- Test.cpp                 <- Cpp of the Google test executable
```

## Author
Ivan Arizanovic (<ivanarizanovic@yahoo.com>)