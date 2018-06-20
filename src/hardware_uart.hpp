/**
 * @file
 * @brief     UART Communication driver for the Arduino Due.
 * @author    Wiebe van Breukelen
 * @license   See LICENSE
 */

#ifndef HARDWARE_UART_HPP
#define HARDWARE_UART_HPP

#include "queue.hpp"
#include "uart_connection.hpp"
#include "wrap-hwlib.hpp"

namespace UARTLib {

/**
 * @brief Establishes an serial/UART connection using on of the three dedicated serial controllers located on the Arduino Due.
 *
 */
class HardwareUART : public UARTConnection {
  public:
    /**
     * @brief Construct a new HardwareUART object.
     *
     * @param baudrate Transmit and receive baudrate.
     * @param controller Controller used to transmit and receive.
     *
     * By default, controller one is selected (pins 18 and 19 on the Arduino Due).
     *
     * @param initializeController Initialize the USART controller directly within the object constructor.
     */
    HardwareUART(unsigned int baudrate, UARTController controller = UARTController::ONE, bool initializeController = true);

    /**
     * @brief Begin a UART connection.
     *
     * This method initializes the selected USART (universal synchronous and asynchronous receiver-transmitter) controller located
     * on the Arduino Due. By default, this method is called at object construction, but this can be disabled.
     *
     */
    void begin() override;

    /**
     * @brief Check how many bytes are available to read.
     *
     * By checking the size of the receive buffer, we determine if new data has arrived.
     *
     * @return unsigned int Amount of bytes available to read.
     */
    unsigned int available() override;

    /**
     * @brief Enables the internal USART controller.
     *
     */
    inline void enable() override;

    /**
     * @brief Disables the internal USART controller.
     *
     */
    inline void disable() override;

    /**
     * @brief Send a single byte.
     *
     * @param c Byte.
     * @return true Byte send.
     * @return false Byte has not been send, USART controller not initialized.
     */
    bool send(const uint8_t c) override;

    /**
     * @brief Send a string.
     *
     * @param str String.
     * @return true String send.
     * @return false String has not been send, USART controller not initialized.
     */
    bool send(const uint8_t *str) override;

    /**
     * @brief Send a string.
     *
     * As we cannot static cast a const char* pointer to a const char* pointer, we overload the send function.
     * Using reinterpreter cast would be a unfavourable solution, as the side effects are unkwown.
     *
     * @param data String.
     * @return true String has been send.
     * @return false String has not been send, USART controller not initialized.
     */
    bool send(const char *data) override;

    /**
     * @brief Send a array of bytes with a specified length.
     *
     * @param data Array of bytes.
     * @param length Length of array.
     * @return true Array of bytes send.
     * @return false Array of bytes has not been send, USART controller not initialized.
     */
    bool send(const uint8_t *data, size_t length) override;

    /**
     * @brief Receive a single byte.
     *
     * By popping the first element of the receive buffer, we return a received byte in a FIFO manner.
     *
     * @return uint8_t Received byte.
     */
    uint8_t receive() override;

    /**
     * @brief Checks if the internal USART controller has been initialized.
     *
     * @return true Internal USART controller is initialized.
     * @return false Internal USART has not been initialized. See the begin() method.
     */
    bool isInitialized() override;

    /**
     * @brief Write a character using UART.
     *
     * Used for interface inheriting between hwlib::istream and hwlib::ostream.
     *
     * @param c Character to send.
     */
    void putc(char c) override;

    /**
     * @brief Check if a character is available to read.
     *
     * Used for interface inheriting between hwlib::istream and hwlib::ostream.
     *
     * @return true
     * @return false
     */
    bool char_available() override;

    /**
     * @brief Read a character using UART.
     *
     * Used for interface inheriting between hwlib::istream and hwlib::ostream.
     *
     * @return char
     */
    char getc() override;

    /**
     * @brief Destroy the HardwareUART object.
     *
     * Disables the UART controller to save resources.
     *
     */
    ~HardwareUART();

  private:
    /**
     * @brief Pointer the access the internal USART controllers.
     *
     */
    Usart *hardwareUSART = nullptr;

    /**
     * @brief Data baudrate used for sending and receiving.
     *
     */
    unsigned int baudrate;

    /**
     * @brief Enumerable type used to select on of the three USART controllers located on the Arduino Due.
     *
     */
    UARTController controller;

    /**
     * @brief Holds the initialization status of the USART controller.
     *
     */
    bool USARTControllerInitialized;

    /**
     * @brief UART receive buffer.
     *
     */
    Queue<uint8_t, 250> rxBuffer;

    /**
     * @brief Checks if the USART controller reports that the transmitter is ready to send.
     *
     * @return true Ready to send.
     * @return false Not ready to send.
     */
    inline bool txReady() override;

    /**
     * @brief Send a byte of the serial connection.
     *
     * @param char Byte to send.
     */
    void sendByte(const uint8_t &b) override;

    /**
     * @brief Receive a single byte by reading the US_RHR register.
     *
     * @return char
     */
    inline uint8_t receiveByte() override;
};

}

#endif