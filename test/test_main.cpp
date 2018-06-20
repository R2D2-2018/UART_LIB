#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "../src/mock_uart.hpp"

TEST_CASE("Construct MockUART instance") {
    UARTLib::MockUART uart(115200, UARTLib::UARTController::THREE, false);

    REQUIRE(!uart.isInitialized());
    REQUIRE(uart.available() == 0);

    uart.begin();

    REQUIRE(uart.isInitialized());
    
    REQUIRE(uart.available() == 1);
    REQUIRE(uart.available() == 2);

    REQUIRE(uart.receive() == 0xAA);
    REQUIRE(uart.receive() == 0xAA);

    REQUIRE(uart.receive() == 0);
}
