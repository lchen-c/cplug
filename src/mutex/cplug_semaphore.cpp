#include "cplug_semaphore.h"

Semaphore::Semaphore(unsigned int initial, unsigned int max) : m_count(initial), m_max(max)
{

}

bool Semaphore::signal()
{
    if (m_count >= m_max)
        return false;
    boost::lock_guard<boost::mutex> lock(m_mutex);
    ++m_count;
    m_condition.notify_all();
    return true;
}

void Semaphore::wait()
{
    boost::unique_lock<boost::mutex> lock(m_mutex);
    while (m_count == 0)
    {
        m_condition.wait(lock);
    }
    --m_count;
}
