#pragma once

#include "Logger.h"

#include <boost/asio.hpp>
#include <functional>
#include <vector>

namespace aerolab::Core
{
    class InputManager
    {
    public:
        InputManager(boost::asio::any_io_executor exec, std::string, int port);
        ~InputManager();

        void Start();
        void Stop();

        /// @brief Sets the callback for received messages
        /// @param callback The callback function to call on received messages
        void SetCallback(std::function<void(const std::vector<uint8_t> &datagram)> callback) { m_messageReceivedCallback = callback; }

    private:
        void receiveData();

        /// Callback function for received messages
        std::function<void(const std::vector<uint8_t> &datagram)> m_messageReceivedCallback;
        /// IO Executor for async operations
        boost::asio::any_io_executor m_ioExecutor;
        /// UDP input socket
        boost::asio::ip::udp::socket m_socket;
        /// Local endpoint for binding the socket
        boost::asio::ip::udp::endpoint m_localEndpoint;
    };
}