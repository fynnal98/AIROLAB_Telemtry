#include "AsyncExecutor.h"

using namespace aerolab::Core;

void AsyncExecutor::Start()
{
    m_thread = std::thread([this]()
                           { m_ioContext.run(); });
}

void AsyncExecutor::Stop()
{
    m_workGuard.reset();
    m_ioContext.stop();
    if (m_thread.joinable())
        m_thread.join();
}