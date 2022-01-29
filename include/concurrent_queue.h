/*******************************************************************************
 MIT License
 Copyright (c) 2020 Orhan Kupusoglu
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
*******************************************************************************/

/*!
 * \mainpage ConcurrentQueue
 * \anchor ConcurrentQueue
 *
 * Based on the implementation by Anthony Williams
 * public domain
 * [Implementing a Thread-Safe Queue using Condition Variables (Updated)](https://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html)
 * C++11
 * [std::queue](https://en.cppreference.com/w/cpp/container/queue)
 * [std::mutex](https://en.cppreference.com/w/cpp/thread/mutex)
 * [std::condition_variable](https://en.cppreference.com/w/cpp/thread/condition_variable)
 * [std::chrono::duration](https://en.cppreference.com/w/cpp/chrono/duration)
 */

#ifndef CONCURRENT_QUEUE_H
#define CONCURRENT_QUEUE_H

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

template<typename T>
class ConcurrentQueue {
    public:
        ConcurrentQueue() = default;                                    // default constructor
        ConcurrentQueue(const ConcurrentQueue&) = delete;               // copy constructor
        ConcurrentQueue& operator=(const ConcurrentQueue&) = delete;    // copy assignment
        ConcurrentQueue(ConcurrentQueue&&) = delete;                    // move constructor
        ConcurrentQueue& operator=(ConcurrentQueue &&) = delete;        // move assignment

        void clear() {
            std::unique_lock<std::mutex> lock(_mutex);
            std::queue<T> blank;
            std::swap(blank, _queue);
        }

        void push(T const& data) {
            std::unique_lock<std::mutex> lock(_mutex);
            _queue.push(data);
            lock.unlock();
            _condition.notify_one();
        }

        std::size_t size() {
            std::unique_lock<std::mutex> lock(_mutex);
            return _queue.size();
        }

        bool empty() {
            std::unique_lock<std::mutex> lock(_mutex);
            return _queue.empty();
        }

        bool try_pop(T& value) {
            std::unique_lock<std::mutex> lock(_mutex);

            if (_queue.empty()) {
                return false;
            }

            value = std::move(_queue.front());
            _queue.pop();
            return true;
        }

        void wait_and_pop(T& value) {
            std::unique_lock<std::mutex> lock(_mutex);

            while (_queue.empty()) {
                _condition.wait(lock);
            }

            value = std::move(_queue.front());
            _queue.pop();
        }

        bool wait_and_pop_while(T& value,
            std::chrono::milliseconds timeout_duration=std::chrono::seconds(1),
            const std::chrono::milliseconds& check_interval=std::chrono::milliseconds(10)) {
            std::unique_lock<std::mutex> lock(_mutex);

            while (_queue.empty()) {
                if (_condition.wait_for(lock, check_interval) == std::cv_status::timeout) {
                    timeout_duration -= check_interval;
                    if (timeout_duration <= std::chrono::milliseconds::zero() ) {
                        return false;
                    }
                }
            }

            value = std::move(_queue.front());
            _queue.pop();
            return true;
        }

    private:
        std::queue<T> _queue;
        std::mutex _mutex;
        std::condition_variable _condition;
};

#endif
