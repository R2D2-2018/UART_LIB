#include "uart_connection.hpp"

UARTConnection::UARTConnection(unsigned int baudrate, UARTController controller, bool initializeController) : baudrate(baudrate), controller(controller), USARTControllerInitialized(false) {
    if (initializeController) {
        begin();
    }
}

UARTConnection::~UARTConnection() {
    ///> Disable the UART controller on destruction
    disable();
}

void UARTConnection::begin() {
    ///> Only initialize the UART controller if it hasn't been enabled.
    if (USARTControllerInitialized) {
        return;
    }

    ///> Setup the correct USART controller.
    if (controller == UARTController::ONE) {
        hardwareUSART = USART0;

        ///> Disable PIO control on PA10, PA11 and set up for peripheral A.
        PIOA->PIO_PDR = PIO_PA10;
        PIOA->PIO_ABSR &= ~PIO_PA10;
        PIOA->PIO_PDR = PIO_PA11;
        PIOA->PIO_ABSR &= ~PIO_PA11;

        ///> Enable the clock to USART0.
        PMC->PMC_PCER0 = (0x01 << ID_USART0);
    } else if (controller == UARTController::TWO) {
        hardwareUSART = USART1;

        ///> Disable PIO control on PA12, PA13 and set up for peripheral A.
        PIOA->PIO_PDR = PIO_PA12;
        PIOA->PIO_ABSR &= ~PIO_PA12;
        PIOA->PIO_PDR = PIO_PA13;
        PIOA->PIO_ABSR &= ~PIO_PA13;

        ///> Enable the clock to USART1.
        PMC->PMC_PCER0 = (0x01 << ID_USART1);
    } else {
        hardwareUSART = USART3;

        ///> Disable PIO control on PD4, PD5 and set up for peripheral B (setting a high bit).
        ///> Section 31.7.24 - http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-11057-32-bit-Cortex-M3-Microcontroller-SAM3X-SAM3A_Datasheet.pdf
        PIOD->PIO_PDR = PIO_PD4;
        PIOD->PIO_ABSR |= PIO_PD4;
        PIOD->PIO_PDR = PIO_PD5;
        PIOD->PIO_ABSR |= PIO_PD5;

        ///> Enable the clock to USART3.
        PMC->PMC_PCER0 = (0x01 << ID_USART3);
    }

    ///> Disable the UART connection to make changes.
    disable();

    ///> Set the baudrate to 115200. see page 799
    ///> Calculation: 5241600 / desired baudrate
    ///> 115200: 45
    hardwareUSART->US_BRGR = (5241600u / baudrate);

    ///> No parity, normal channel mode. Use a 8 bit data field.
    hardwareUSART->US_MR = UART_MR_PAR_NO | UART_MR_CHMODE_NORMAL | US_MR_CHRL_8_BIT; // Might check if this definition is compatable with USART as well.

    ///> Disable the interrupt controller.
    hardwareUSART->US_IDR = 0xFFFFFFFF;

    ///> Enable the UART controller
    enable();

    ///> USART Controller initialized
    USARTControllerInitialized = true;
}

unsigned int UARTConnection::available() {
    if (!USARTControllerInitialized) {
        return 0;
    }

    ///> We use the USART Channel status register to check if there is data available.
    if ((hardwareUSART->US_CSR & 1) != 0) {
        rxBuffer.push(receiveByte());
    }

    return rxBuffer.count();
}

bool UARTConnection::send(const uint8_t b) {
    if (!USARTControllerInitialized) {
        return false;
    }

    sendByte(b);

    return true;
}

bool UARTConnection::send(const uint8_t* str) {
    if (!USARTControllerInitialized) {
        return false;
    }

    for (const uint8_t *p = str; *p != '\0'; p++) {
        sendByte(*p);
    }

    return true;
}

bool UARTConnection::send(const char* str) {
    if (!USARTControllerInitialized) {
        return false;
    }

    for (const char *p = str; *p != '\0'; p++) {
        sendByte(*p);
    }

    return true;
}

bool UARTConnection::send(const uint8_t* data, size_t length) {
    if (!USARTControllerInitialized) {
        return false;
    }

    for (unsigned int i = 0; i < length; i++) {
        sendByte(data[i]);
    }
    
    return true;
}

uint8_t UARTConnection::receive() {
    if (!USARTControllerInitialized || !rxBuffer.count()) {
        return 0;
    }

    return rxBuffer.pop();   
}

void UARTConnection::operator<<(const char *str) {
    send(str);
}

void UARTConnection::operator<<(const uint8_t c) {
    send(c);
}

void UARTConnection::operator>>(uint8_t &c) {
    c = receive();
}

bool UARTConnection::isInitialized() {
    return USARTControllerInitialized;
}

void UARTConnection::sendByte(const uint8_t &b) {
    ///> Wait before we can send any more data
    while (!txReady());

    ///> Send it!
    hardwareUSART->US_THR = b;
}

inline uint8_t UARTConnection::receiveByte() {
    return hardwareUSART->US_RHR;
}

inline bool UARTConnection::txReady() {
    ///> We use the USART Channel status register to wait until the TXRDY bit is cleared.
    return (hardwareUSART->US_CSR & 2);
}

inline void UARTConnection::enable() {
    ///> Enable the transmitter and receiver
    hardwareUSART->US_CR = UART_CR_RXEN | UART_CR_TXEN;
}

inline void UARTConnection::disable() {
    ///> Set the control register to reset and disable the receiver and transmitter.
    hardwareUSART->US_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;
}