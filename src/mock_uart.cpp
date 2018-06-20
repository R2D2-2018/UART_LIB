#include "mock_uart.hpp"

MockUART::MockUART(unsigned int baudrate, UARTController controller, bool initializeController)
    : baudrate(baudrate), controller(controller), USARTControllerInitialized(false) {
    if (initializeController) {
        begin();
    }
}

MockUART::~MockUART() {
    ///< Disable the UART controller on destruction
    disable();
}

void MockUART::begin() {
    ///< Only initialize the UART controller if it hasn't been enabled.
    if (USARTControllerInitialized) {
        return;
    }

    ///< Normally, we would setup the correct USART controller.
    ///< In the mock implementation, we do nothing.

    ///< Enable the UART controller
    enable();

    ///< USART Controller initialized
    USARTControllerInitialized = true;
}

unsigned int MockUART::available() {
    if (!USARTControllerInitialized) {
        return 0;
    }

    ///< In the hardware implementation we use the USART Channel status register to check if there is data available.
    ///< In the mock implementation, we just put in that we received from receiveByte() (will is always 0x80).
    rxBuffer.push(receiveByte());

    return rxBuffer.count();
}

bool MockUART::send(const uint8_t b) {
    if (!USARTControllerInitialized) {
        return false;
    }

    sendByte(b);

    return true;
}

bool MockUART::send(const uint8_t *str) {
    if (!USARTControllerInitialized) {
        return false;
    }

    for (const uint8_t *p = str; *p != '\0'; p++) {
        sendByte(*p);
    }

    return true;
}

bool MockUART::send(const char *str) {
    if (!USARTControllerInitialized) {
        return false;
    }

    for (const char *p = str; *p != '\0'; p++) {
        sendByte(*p);
    }

    return true;
}

bool MockUART::send(const uint8_t *data, size_t length) {
    if (!USARTControllerInitialized) {
        return false;
    }

    for (unsigned int i = 0; i < length; i++) {
        sendByte(data[i]);
    }

    return true;
}

uint8_t MockUART::receive() {
    if (!USARTControllerInitialized || !rxBuffer.count()) {
        return 0;
    }

    return rxBuffer.pop();
}

void MockUART::putc(char c) {
    sendByte(c);
}

char MockUART::getc() {
    return receive();
}

bool MockUART::char_available() {
    return (available() > 0);
}

bool MockUART::isInitialized() {
    return USARTControllerInitialized;
}

void MockUART::sendByte(const uint8_t &b) {
    ///< Wait before we can send any more data
    while (!txReady()) {
    }

    ///< Normally, we would send right now. Since it's a mock implementation, we don't do that.
}

inline uint8_t MockUART::receiveByte() {
    ///< Normally, we would receive right now. Since it's a mock implementation, we don't do that.
    ///< Instead, we always send a fixed byte.

    return 0xAA;
}

inline bool MockUART::txReady() {
    ///< Normally, we would wait for the tx line to be ready. Since it's a mock implementation, we don't do that.

    return true;
}

inline void MockUART::enable() {
    ///< Normally, we would enable the USART controller right now. Since it's a mock implementation, we don't do that.
}

inline void MockUART::disable() {
    ///< Normally, we would disable the USART controller right now. Since it's a mock implementation, we don't do that.
}