#include "wrap-hwlib.hpp"

#include "uart_connection.hpp"


int main() {
    WDT->WDT_MR = WDT_MR_WDDIS;

    namespace target = hwlib::target;

    hwlib::wait_ms(500);

    hwlib::cout << "Initialized transceiver" << hwlib::endl;


    /// Initialize UART controller 3. Use a 115200 baudrate to send and receive (used by the uArm Swift Pro to send Gcode commands).
    UARTConnection conn(115200, UARTController::THREE);
    hwlib::wait_ms(500);

    long startMsReceive = hwlib::now_us() / 1000;
    long startMsSend = hwlib::now_us() / 1000;

    while (true) {
        if ((hwlib::now_us() / 1000) - startMsSend > 2500) {
            startMsSend = hwlib::now_us() / 1000;
            conn << "Hello World!";
        }

        if (conn.available() > 0 && (hwlib::now_us() / 1000) - startMsReceive > 30) {
            startMsReceive = hwlib::now_us() / 1000;

            for (unsigned int i = 0; i < conn.available(); i++) {
                hwlib::cout << conn.receive();
            }
        }
    }
}
