#pragma once

#include <boost/asio.hpp>
#include <thread>

namespace aerolab::Core
{
    class AsyncExecutor
    {
    public:
        AsyncExecutor()
            : m_workGuard(boost::asio::make_work_guard(m_ioContext))
        {
        }

        ~AsyncExecutor()
        {
            Stop();
        }

        void Start();
        void Stop();

        boost::asio::any_io_executor GetExecutor() { return m_ioContext.get_executor(); }

    private:
        boost::asio::io_context m_ioContext;
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> m_workGuard;
        std::thread m_thread;
    };
}
