/**
 * @file
 * @brief     Superclass for any UART connection, hardware or mock based.
 * @author    Wiebe van Breukelen
 * @license   See LICENSE
 */

#ifndef UART_COMM_HPP
#define UART_COMM_HPP

#include "queue.hpp"
#include "wrap-hwlib.hpp"

namespace UARTLib {

/**
 * @brief Used to select the UART controllers available on the Arduino Due.
 *
 * Pins:
 * One   - 18 and 19
 * Two   - 16 and 17
 * Three - 14 and 15
 */
enum class UARTController { ONE, TWO, THREE };

/**
 * @brief Superclass for any UART connection, hardware or mock based.
 * Using polymorphism, we can use the same interface for both implementations.
 *
 */
class UARTConnection : public hwlib::ostream, public hwlib::istream {
  public:
    /**
     * @brief Begin a UART connection.
     *
     * This method initializes the selected USART (universal synchronous and asynchronous receiver-transmitter) controller located
     * on the Arduino Due. By default, this method is called at object construction, but this can be disabled.
     *
     */
    virtual void begin() = 0;

    /**
     * @brief Check how many bytes are available to read.
     *
     * By checking the size of the receive buffer, we determine if new data has arrived.
     *
     * @return unsigned int Amount of bytes available to read.
     */
    virtual unsigned int available() = 0;

    /**
     * @brief Enables the internal USART controller.
     *
     */
    virtual inline void enable() = 0;

    /**
     * @brief Disables the internal USART controller.
     *
     */
    virtual inline void disable() = 0;

    /**
     * @brief Send a single byte.
     *
     * @param c Byte.
     * @return true Byte send.
     * @return false Byte has not been send, USART controller not initialized.
     */
    virtual bool send(const uint8_t c) = 0;

    /**
     * @brief Send a string.
     *
     * @param str String.
     * @return true String send.
     * @return false String has not been send, USART controller not initialized.
     */
    virtual bool send(const uint8_t *str) = 0;

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
    virtual bool send(const char *data) = 0;

    /**
     * @brief Send a array of bytes with a specified length.
     *
     * @param data Array of bytes.
     * @param length Length of array.
     * @return true Array of bytes send.
     * @return false Array of bytes has not been send, USART controller not initialized.
     */
    virtual bool send(const uint8_t *data, size_t length) = 0;

    /**
     * @brief Receive a single byte.
     *
     * By popping the first element of the receive buffer, we return a received byte in a FIFO manner.
     *
     * @return uint8_t Received byte.
     */
    virtual uint8_t receive() = 0;

    /**
     * @brief Checks if the internal USART controller has been initialized.
     *
     * @return true Internal USART controller is initialized.
     * @return false Internal USART has not been initialized. See the begin() method.
     */
    virtual bool isInitialized() = 0;

    virtual void putc(char c) = 0;

    virtual bool char_available() = 0;

    virtual char getc() = 0;

  private:
    /**
     * @brief Checks if the USART controller reports that the transmitter is ready to send.
     *
     * @return true Ready to send.
     * @return false Not ready to send.
     */
    virtual inline bool txReady() = 0;

    /**
     * @brief Send a byte of the serial connection.
     *
     * @param char Byte to send.
     */
    virtual void sendByte(const uint8_t &b) = 0;

    /**
     * @brief Receive a single byte by reading the US_RHR register.
     *
     * @return char
     */
    virtual inline uint8_t receiveByte() = 0;
};

}

#endif