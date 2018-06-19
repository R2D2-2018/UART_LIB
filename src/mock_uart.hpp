#ifndef MOCK_UART_HPP
#define MOCK_UART_HPP

#include "wrap-hwlib.hpp"
#include "uart_connection.hpp"

class MockUART : public UARTConnection {
public:
/**
   * @brief Construct a new MockUART object.
   * 
   * @param baudrate Transmit and receive baudrate.
   * @param controller Controller used to transmit and receive.
   * 
   * By default, controller one is selected (pins 18 and 19 on the Arduino Due).
   * 
   * @param initializeController Initialize the USART controller directly within the object constructor.
   */
  MockUART(unsigned int baudrate, UARTController controller = UARTController::ONE, bool initializeController = true);

  /**
   * @brief Begin a UART connection.
   * 
   * This method initializes the selected USART (universal synchronous and asynchronous receiver-transmitter) controller located
   * on the Arduino Due. By default, this method is called at object construction, but this can be disabled.
   * 
   */
  void begin();

  /**
   * @brief Check how many bytes are available to read.
   * 
   * By checking the size of the receive buffer, we determine if new data has arrived.
   * 
   * @return unsigned int Amount of bytes available to read.
   */
  unsigned int available();

  /**
   * @brief Enables the internal USART controller.
   * 
   */
  inline void enable();

  /**
   * @brief Disables the internal USART controller.
   * 
   */
  inline void disable();

  /**
   * @brief Send a single byte.
   * 
   * @param c Byte.
   * @return true Byte send.
   * @return false Byte has not been send, USART controller not initialized.
   */
  bool send(const uint8_t c);

  /**
   * @brief Send a string.
   * 
   * @param str String.
   * @return true String send.
   * @return false String has not been send, USART controller not initialized.
   */
  bool send(const uint8_t *str);

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
  bool send(const char *data);

  /**
   * @brief Send a array of bytes with a specified length.
   * 
   * @param data Array of bytes.
   * @param length Length of array.
   * @return true Array of bytes send.
   * @return false Array of bytes has not been send, USART controller not initialized.
   */
  bool send(const uint8_t* data, size_t length);

  /**
   * @brief Receive a single byte.
   * 
   * By popping the first element of the receive buffer, we return a received byte in a FIFO manner.
   * 
   * @return uint8_t Received byte.
   */
  uint8_t receive();

  /**
   * @brief Checks if the internal USART controller has been initialized.
   * 
   * @return true Internal USART controller is initialized.
   * @return false Internal USART has not been initialized. See the begin() method.
   */
  bool isInitialized();

  /**
   * @brief Send a single byte.
   * 
   * @param c Byte.
   */
  void operator<<(const uint8_t c);

  /**
   * @brief Send a string.
   * 
   * As we want to easily send strings using the shifting operator, we need to declare a operator<<(const char* ).
   * The use of a operator<<(const uint8_t* ) would be favourable, but unfortunately not possible.
   * 
   * @param str String (must be null terminated).
   */
  void operator<<(const char *str);

  /**
   * @brief Receive a byte.
   * 
   * @param c Byte.
   */
  void operator>>(uint8_t &c);

  /**
   * @brief Destroy the MockUART object.
   * 
   * Disables the UART controller to save resources.
   * 
   */
  ~MockUART();

private:
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
  inline bool txReady();

  /**
   * @brief Send a byte of the serial connection.
   *
   * @param char Byte to send.
   */
  void sendByte(const uint8_t &b);

  /**
   * @brief Receive a single byte by reading the US_RHR register.
   * 
   * @return char 
   */
  inline uint8_t receiveByte();
};

#endif