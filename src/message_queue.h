//
// Created by Derek Wood on 2019-08-26.
//

#ifndef PROCESS_KEYFRAMES_MESSAGE_QUEUE_H
#define PROCESS_KEYFRAMES_MESSAGE_QUEUE_H

#include <deque>
#include <mutex>
#include <condition_variable>

template<typename T>
class message_queue
{
public:
    T receive()
    {
        // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait()
        // to wait for and receive new messages and pull them from the queue using move semantics.
        // The received object should then be returned by the receive function.
        std::unique_lock<std::mutex> uLock(_mutex);
        _condition.wait(uLock, [this] { return !_queue.empty(); }); // pass unique lock to condition variable

        // remove last vector element from queue
        T msg = std::move(_queue.back());
        _queue.pop_back();
        return msg;
    }
    void send(T &&msg)
    {
        // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex>
        // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.

        // perform vector modification under the lock
        std::lock_guard<std::mutex> uLock(_mutex);

        // add vector to queue
        //std::cout << "   Message " << msg << " has been sent to the queue" << std::endl;
        _queue.push_back(std::move(msg));
        _condition.notify_one(); // notify client after pushing new Vehicle into vector
    }

private:
    std::deque<T> _queue;
    std::mutex _mutex;
    std::condition_variable _condition;
};

#endif //PROCESS_KEYFRAMES_MESSAGE_QUEUE_H
