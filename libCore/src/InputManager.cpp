#include "InputManager.h"

using namespace aerolab::Core;

void logReceivedData(const std::vector<uint8_t> &data);

/**
 * @brief Constructor of the InputManager
 * @param exec The IO Executor for asynchronous operations
 * @param ip The IPv4 address to bind the socket to
 * @param port The port to bind the socket to
 */
InputManager::InputManager(boost::asio::any_io_executor exec, std::string ip, int port) : m_ioExecutor(exec),
                                                                                          m_socket(m_ioExecutor)
{
    m_localEndpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4::from_string(ip), port);
    LOG_INFO("Created InputManager");
}

/**
 * Destructor for the InputManger
 */
InputManager::~InputManager()
{
    Stop();
}

// ============================================================================
// Livecycle Management
// ============================================================================

/**
 * @brief Starts the InputManager
 * @details This function sets up the necessary resources and begins listening for incoming data.
 */
void InputManager::Start()
{
    LOG_INFO("*** Starting InputManager ***");

    boost::system::error_code ec;

    LOG_INFO("Opening UDP Socket");
    m_socket.open(boost::asio::ip::udp::v4(), ec);
    if (ec)
    {
        LOG_ERROR("open failed: " + ec.message());
        return;
    }

    m_socket.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if (ec)
    {
        LOG_ERROR("set_option failed: " + ec.message());
        return;
    }

    LOG_INFO("Binding socket to IP " + m_localEndpoint.address().to_string() + ":" + std::to_string(m_localEndpoint.port()));
    m_socket.bind(m_localEndpoint, ec);
    if (ec)
    {
        LOG_ERROR("bind failed: " + ec.message());
        return;
    }

    LOG_INFO("Starting to receive Data");
    receiveData;

    LOG_INFO("*** InputManager started ***\n");
}

/**
 * @brief Stops the InputManager
 * @details This function cleans up resources and stops ongoing operations.
 */
void InputManager::Stop()
{
    LOG_INFO("*** Stopping InputManager ***");

    boost::system::error_code ec;

    LOG_INFO("Closing UDP socket...");
    if (m_socket.is_open())
    {
        m_socket.cancel(ec);
        m_socket.close(ec);
        if (ec)
            LOG_WARNING("close reported: " + ec.message());

        LOG_INFO("UDP socket closed");
    }

    LOG_INFO("*** InputManager stopped ***\n");
}

// ============================================================================
// Data Reception
// ============================================================================

/**
 * @brief Initiates asynchronous data reception
 * @details This function sets up an asynchronous receive operation on the UDP socket.
 * When data is received, it logs the number of bytes, the data itself and the sender's address.
 * It then recursively calls itself to continue listening for incoming data.
 */
void InputManager::receiveData()
{
    auto buffer = std::make_shared<std::vector<char>>(4096);
    auto senderEndpoint = std::make_shared<boost::asio::ip::udp::endpoint>();

    if (!m_socket.is_open())
    {
        LOG_WARNING("Trying to receive data before opening the socket!");
        return;
    }

    m_socket.async_receive_from(
        boost::asio::buffer(*buffer), *senderEndpoint,
        [this, buffer, senderEndpoint](boost::system::error_code ec, std::size_t bytesReceived)
        {
            if (!ec)
            {
                std::vector<uint8_t> receivedData(buffer->begin(), buffer->begin() + bytesReceived);
                LOG_DEBUG("Received " + std::to_string(bytesReceived) + " bytes from " + senderEndpoint->address().to_string());
                logReceivedData(receivedData);

                if (m_messageReceivedCallback)
                    m_messageReceivedCallback(receivedData);
            }
            else
                LOG_ERROR("Error receiving data: " + ec.message());

            if (m_socket.is_open())
                receiveData();
        });
}

// ============================================================================
// Utility Functions
// ============================================================================

/**
 * @brief Logs incoming data in hex
 * @param data The vector of incoming data to log
 */
void logReceivedData(const std::vector<uint8_t> &data)
{
    std::ostringstream oss;
    oss << "Received data: " << std::hex << std::uppercase;

    for (const auto &byte : data)
        oss << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";

    LOG_DEBUG(oss.str());
}
