#include <boost/thread/condition_variable.hpp>
#include <boost/thread/mutex.hpp>    
#include <boost/thread/lock_types.hpp>
#include <atomic>

using namespace std;

class Semaphore
{
public:
    explicit Semaphore(unsigned int initial, unsigned int max);
    bool signal();
    void wait();
private:
    atomic_int m_max;
    int m_count;
    boost::mutex m_mutex;
    boost::condition_variable m_condition;
};