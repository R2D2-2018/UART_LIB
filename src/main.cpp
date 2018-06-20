#include "hardware_uart.hpp"
#include "mock_uart.hpp"
#include "wrap-hwlib.hpp"

class ExampleUARTUser {
  private:
    UARTConnection &conn;

  public:
    ExampleUARTUser(UARTConnection &conn) : conn(conn){};

    unsigned int bytesAvailable() {
        return conn.available();
    }

    void sendSomething() {
        conn << "Hello World!\r\n";
    }

    uint8_t receiveSomething() {
        return conn.receive();
    }
};

/**
 * @brief Example for using the UART library, with and without real hardware access.
 * Use the one without hardware access (mock) for tests.
 */
int main() {
    WDT->WDT_MR = WDT_MR_WDDIS;

    namespace target = hwlib::target;

    ///< Wait a litle bit for proper initialization.
    hwlib::wait_ms(500);

    ///< Initialize a hardware UART connection, with a baudrate of 115200 and using RX1 and TX1 on the Arduino Due.
    HardwareUART connHw(115200, UARTController::ONE);
    ///< Initiailze a mock/fake UART connection. Use this one in your tests.
    MockUART connMock(115200, UARTController::ONE);

    ///< Create two dummy objects.
    ExampleUARTUser uartHwUser(connHw);
    ExampleUARTUser uartMockUser(connMock);

    char availableRealUART = 0, availableFakeUART = 0;

    while (true) {
        uartHwUser.sendSomething();
        uartMockUser.sendSomething();

        ///< Send something using the real UART hardware.
        availableRealUART = uartHwUser.bytesAvailable();

        ///< Send something using the fake implementation you want to use for unit tests.
        ///< The number of bytes available will quickly end up at the fixed size of the receive buffer, 255.
        ///< As calling the MockUART::available() method will increase the number of received bytes one by one.
        availableFakeUART = uartMockUser.bytesAvailable();

        ///< Receive something using the real UART hardware. You better use a buffer it you expect loads of data (the cout delays
        ///< the receive process).
        if (availableRealUART > 0) {
            hwlib::cout << "Received from real UART: " << uartHwUser.receiveSomething() << hwlib::endl;
        }

        ///< Receive something using the fake/test UART hardware.
        if (availableFakeUART > 9) {
            ///< If you like, you can comment out the following code statement. This will demonstrate that the receive buffer of the
            ///< fake UART is filled constantly with the value of 0xAA hwlib::cout << "Received from fake/testing UART: " <<
            ///< uartMockUser.receiveSomething() << hwlib::endl;
        }
    }
}
