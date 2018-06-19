#include "wrap-hwlib.hpp"
#include "hardware_uart.hpp"
#include "mock_uart.hpp"

/**
 * @brief Example for using the UART library.
 */
int main() {
    WDT->WDT_MR = WDT_MR_WDDIS;

    namespace target = hwlib::target;

    ///< Wait a litle bit of proper initialization.
    hwlib::wait_ms(500);

    ///< Initialize UART controller 3. Use a 115200 baudrate to send and receive (used by the uArm Swift Pro to send Gcode commands).
    HardwareUART conn(115200, UARTController::THREE);
    
    ///< For the mock implementation, use MockUART conn(115200, UARTController::ONE);

    long startMsReceive = hwlib::now_us() / 1000;
    long startMsSend = hwlib::now_us() / 1000;

    while (true) {
        ///< Send "Hello World" every 2.5 seconds.
        if ((hwlib::now_us() / 1000) - startMsSend > 2500) {
            startMsSend = hwlib::now_us() / 1000;
            conn << "Hello World!";
        }

        ///< Check for new received data every 30 milliseconds.
        if (conn.available() > 0 && (hwlib::now_us() / 1000) - startMsReceive > 30) {
            startMsReceive = hwlib::now_us() / 1000;

            ///< Print the received data.
            for (unsigned int i = 0; i < conn.available(); i++) {
                hwlib::cout << conn.receive();
            }
        }
    }
}
