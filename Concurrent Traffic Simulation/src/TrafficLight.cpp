#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <queue>
#include <future>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
    std::unique_lock<std::mutex> lock(_mutex);
    _cond.wait(lock, [this] { return !_queue.empty(); }); 
    T msg = _queue.back();
    _queue.pop_back();

    return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
  	std::lock_guard<std::mutex> lock(_mutex);
  	_queue.push_back(std::move(msg));
  	_cond.notify_one();
}

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
  	while(msg_queue->receive() == green) {
    	std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class.
  	threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. 
  
  	std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(4000, 6000); // define the range
  
  	int gap_time = distr(gen);
  
  	std::chrono::time_point<std::chrono::system_clock> last_time = std::chrono::system_clock::now();
  
  	while(1) {
      	if(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - last_time).count() >= gap_time) {
        	if(_currentPhase == red) {
            	_currentPhase = green;
            } else {
            	_currentPhase = red;
            }
			auto is_sent = std::async(std::launch::async, &MessageQueue<TrafficLightPhase>::send, msg_queue, std::move(_currentPhase));
			is_sent.wait();
          
          	last_time = std::chrono::system_clock::now();
          	gap_time = distr(gen);
        }
    	std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
