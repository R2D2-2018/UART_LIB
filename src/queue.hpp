/**
 * @file
 * @brief     FIFO Queue.
 *
 * Queue without using the internal heap.
 * Based on the following code: https://github.com/sdesalas/Arduino-Queue.h/blob/master/Queue.h
 * @author    Wiebe van Breukelen, Steven de Salas
 * @license   See LICENSE
 */

#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "wrap-hwlib.hpp"

template <class T, size_t QUEUE_SIZE>
class Queue {
  private:
    unsigned int _front, _back, _count;
    T _data[QUEUE_SIZE];

  public:
    Queue() {
        _front = 0;
        _back = 0;
        _count = 0;
    }
    inline int count();
    inline int front();
    inline int back();
    void push(const T &item);
    T peek();
    T pop();
    void clear();
};

template <class T, size_t QUEUE_SIZE>
inline int Queue<T, QUEUE_SIZE>::count() {
    return _count;
}

template <class T, size_t QUEUE_SIZE>
inline int Queue<T, QUEUE_SIZE>::front() {
    return _front;
}

template <class T, size_t QUEUE_SIZE>
inline int Queue<T, QUEUE_SIZE>::back() {
    return _back;
}

template <class T, size_t QUEUE_SIZE>
void Queue<T, QUEUE_SIZE>::push(const T &item) {
    if (_count < QUEUE_SIZE) { // Drops out when full
        _data[_back++] = item;
        ++_count;
        // Check wrap around
        if (_back > QUEUE_SIZE)
            _back -= (QUEUE_SIZE + 1);
    }
}

template <class T, size_t QUEUE_SIZE>
T Queue<T, QUEUE_SIZE>::pop() {
    if (_count <= 0)
        return T(); // Returns empty
    else {
        T result = _data[_front];
        _front++;
        --_count;
        // Check wrap around
        if (_front > QUEUE_SIZE)
            _front -= (QUEUE_SIZE + 1);
        return result;
    }
}

template <class T, size_t QUEUE_SIZE>
T Queue<T, QUEUE_SIZE>::peek() {
    if (_count <= 0)
        return T(); // Returns empty
    else
        return _data[_front];
}

template <class T, size_t QUEUE_SIZE>
void Queue<T, QUEUE_SIZE>::clear() {
    _front = _back;
    _count = 0;
}

#endif
