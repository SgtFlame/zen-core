//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2001 - 2009 John Givler
// Copyright (C) 2001 - 2009 Tony Richards
// Copyright (C) 2008 - 2009 Matthew Alan Gray
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you must not
//     claim that you wrote the original software. If you use this software
//     in a product, an acknowledgment in the product documentation would be
//     appreciated but is not required.
//  2. Altered source versions must be plainly marked as such, and must not be
//     misrepresented as being the original software.
//  3. This notice may not be removed or altered from any source distribution.
//
//  John S. Givler, Ph.D.(Computer Science) - Dr.John.S.Givler@Givler.com
//  Tony Richards                           - trichards@indiezen.com
//  Matthew Alan Gray                       - mgray@indiezen.org
//
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "../ThreadPool.hpp"
#include "../ThreadFactory.hpp"

#include <Zen/Core/Threading/I_Thread.hpp>
#include <Zen/Core/Utility/runtime_exception.hpp>

#include <iostream>
#include <stdexcept>

// Different debug levels to help with debugging.  
// TR - These debug levels are incomplete, so don't use them.  Let me know if you need them
// and I can finish the implementation.
//#define DEBUG 1
//#define DEBUG 40
//#define DEBUG 100

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Threading {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ThreadPool::ThreadPool(unsigned const _numThreads, I_Condition* const _pQueueIsEmpty, bool const _waitForEmptyQueue, bool const _startNow)
:   m_threadPool                ()
,   m_numberOfNotRunningThreads (_numThreads)
,   m_threadsStarted            (false)
,   m_numberOfThreads           (_numThreads)
,   m_pAllThreadsAreRunning     (ConditionFactory::create())
,   m_requestQueue              (_pQueueIsEmpty, true, _numThreads==1)
,   m_amPreparingToStop         (false)
,   m_amStopping                (false)
,   m_amStoppingAsSoonAsPossible(false)
,   m_waitForEmptyQueue         (_waitForEmptyQueue)
,   m_spinLock                  ()
{
    m_threadPool.reserve(_numThreads);

    if (_startNow)
    {
        start();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ThreadPool::~ThreadPool()
{
    // volatile, and visible to all Threads in this pool
    m_amStoppingAsSoonAsPossible = !m_waitForEmptyQueue;

    for (size_t i = m_threadPool.size(); i > 0; --i)
    {
        // wake up a blocked Thread (if any), tell it to quit now
        m_requestQueue.push(0);                         
    }

    for (std::vector<pThread_type>::const_iterator ppThread = m_threadPool.begin(); ppThread != m_threadPool.end(); ++ppThread)
    {
        (*ppThread)->join();

        delete *ppThread;
    }

    // Now that the threads are all shut down, drain any remaining Tasks from the queue
    while (!m_requestQueue.isEmpty())
    {
        Task* pTask;
        m_requestQueue.pop(pTask);
        if (pTask) pTask->dispose();
    }

    ConditionFactory::destroy(m_pAllThreadsAreRunning);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
bool
ThreadPool::isStarted()
{
    return m_threadsStarted;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ThreadPool::start()
{
    // Guard while checking to see if the threads are already started
    {
        xCriticalSection guard(m_spinLock);
        if (m_threadsStarted)
        {
            /// If they're already started, simply return
            return;
        }
        else
        {
            // They're not started, but we're about to start them
            m_threadsStarted = true;
        }
    }

    // Start the threads.
    for(unsigned i = m_numberOfThreads; i > 0; --i)
    {
        ThreadPoolRunnable* const pRunnable = new ThreadPoolRunnable(*this);
        pThread_type pThread (ThreadFactory::create(pRunnable));
        #if DEBUG > 40
        pRunnable->setThread(pThread);
        #endif // DEBUG
        m_threadPool.push_back(pThread);
        pThread->start();
    }

    // Wait for the threads to completely start
    requireStarted();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ThreadPool::requireStarted()
{
    // Wait for the threads to completely start
    m_pAllThreadsAreRunning->requireCondition();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ThreadPool::prepareToStop()
{
    // subsequent PushRequest()s will be ignored (and will return 'false').
    m_amPreparingToStop = true;

    // enables optional requestQueue assertion.  Still accepts push()s.
    m_requestQueue.closing();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ThreadPool::stop()
{
    if (m_amStopping) return;

    m_amPreparingToStop = true;                           // subsequent PushRequest()s will be ignored (and will return 'false').
    m_amStopping      = true;
    if (!m_waitForEmptyQueue)
    {
        m_amStoppingAsSoonAsPossible = true;            // volatile, and visible to all Threads in this pool
    }

    for (size_t i=m_threadPool.size(); i>0; --i)
    {
        m_requestQueue.push(0);                         // wake up a blocked Thread (if any), tell it to quit now
    }
    m_requestQueue.closing();                           // enables optional RequestQueue assertion.  Still accepts push()s.

    for (std::vector<pThread_type>::const_iterator ppThread = m_threadPool.begin(); ppThread != m_threadPool.end(); ++ppThread)
    {
        (*ppThread)->join();
    }

    // subsequent m_requestQueue.push()s will be ignored.  Don't use until threads are done.
    m_requestQueue.close();
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ThreadPool::TaskPool::give(PooledTask& _task)
{
    xCriticalSection cs(m_spinLock);

    m_unusedTasks.push_front(_task);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ThreadPool::PooledTask&
ThreadPool::TaskPool::take()
{
    PooledTask* pTask;

    while(true)
    {
        if(m_amStopping)
        {
            throw Zen::Utility::runtime_exception("Zen::Threading::ThreadPool::TaskPool::take() : The TaskPool is currently being stopped.");
        }

        // Check to see if the collection is empty.
        {
            xCriticalSection cs(m_spinLock);
            
            // If the unused tasks collection is not empty, pop a task off of the list
            // TR - size() is linear time, so use empty() whenever possible 
            //     Use size() only if you really want the size.
            //isEmpty = (0 == m_unusedTasks.size());
            if (!m_unusedTasks.empty())
            {
                // Do this here while still locked, otherwise we could have a race
                // condition.
                pTask = &(m_unusedTasks.front());
                m_unusedTasks.pop_front();
                return *pTask;
            }
        }

        // The unused tasks is empty, so create a new task
        pTask = m_pooledTaskAllocator.create(*this);
        if(pTask != NULL)
        {
            return *pTask;
        }

        // Circle back around and try it again :)
        // Don't sleep.  Leave it to the allocator to decide what's appropriate
        //Zen::Threading::I_Thread::sleepForMilliseconds(1);
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ThreadPool::TaskPool::prepareToStop()
{
    // subsequent request to create new tasks will be ignored.
    m_amPreparingToStop = true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ThreadPool::TaskPool::stop()
{
    if( m_amStopping ) return;

    // subsequent request to create new tasks will be ignored.
    m_amPreparingToStop = true;
    m_amStopping      = true;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void ThreadPool::ThreadPoolRunnable::run() throw ()
{
    try
    {
        // ThreadPool startup
        {
            bool readyToRockAndRoll;
            {
                xCriticalSection cs (m_pool.m_spinLock);
                readyToRockAndRoll = (0 == --m_pool.m_numberOfNotRunningThreads);
            }
            if (readyToRockAndRoll)
            {
                m_pool.m_pAllThreadsAreRunning->assertCondition();
                #if DEBUG >= 40
                std::cout << "ThreadPool::ThreadPoolRunnable::run(): all "<<m_pool.getNumberOfThreads()<<" threads are up & running!" << std::endl;
                #endif // DEBUG
            }
        }

        // ThreadPool inner loop
        std::list<Task*> taskList;
        while (!m_pool.m_amStoppingAsSoonAsPossible)    // volatile
        {
            taskList.clear();
            m_pool.popRequest(taskList);
            for (std::list<Task*>::iterator ppTask = taskList.begin(); ppTask != taskList.end(); ++ppTask)
            {
                Task* const pTask = *ppTask;
                #if DEBUG >= 40
                ++ m_useCount;
                #endif // DEBUG
                if (pTask)
                {
                    if (!m_pool.m_amStoppingAsSoonAsPossible)
                    {
                        try
                        {
                            pTask->call();
                        }
                        #if DEBUG > 0
                        catch (const std::exception& _oops)
                        {
                            xCriticalSection cs (m_pool.m_spinLock);
                            std::cerr << "ThreadPool::ThreadPoolRunnable::run(): pTask->call() failed and threw a std::exception, description='"<<_oops.what()<<"'" << std::endl;
                            std::cerr << "ThreadPool::ThreadPoolRunnable::run(): pTask == '"<<std::hex<<pTask<<std::dec<<"'" << std::endl;
                            std::cerr << "ThreadPool::ThreadPoolRunnable::run(): Queue has "<<m_pool.m_requestQueue.size()<<" unprocessed tasks." << std::endl;
                            std::cerr << "ThreadPool::ThreadPoolRunnable::run(): My thread ran "<<m_useCount<<" tasks." << std::endl;
                            //std::cerr << "ThreadPool::ThreadPoolRunnable::run(): My thread ID is "<<getThread()->getThreadId().toString()<<"." << std::endl;
                        }
                        #endif // DEBUG
                        catch (...)
                        {
                            #if DEBUG > 0
                            xCriticalSection cs (m_pool.m_spinLock);
                            std::cerr << "ThreadPool::ThreadPoolRunnable::run(): pTask->call() failed and threw an unknown exception." << std::endl;
                            std::cerr << "ThreadPool::ThreadPoolRunnable::run(): Queue has "<<m_pool.m_requestQueue.size()<<" unprocessed tasks." << std::endl;
                            std::cerr << "ThreadPool::ThreadPoolRunnable::run(): My thread ran "<<m_useCount<<" tasks." << std::endl;
                            //std::cerr << "ThreadPool::ThreadPoolRunnable::run(): My thread ID is "<<getThread()->getThreadId().toString()<<"." << std::endl;
                            #endif // DEBUG
                        }
                    }
                    else
                    {
                        #if DEBUG >= 40
                        xCriticalSection cs (m_pool.m_spinLock);
                        std::cout << "ThreadPool::ThreadPoolRunnable::run(): quitting, even though there is an unprocessed task." << std::endl;
                        #endif // DEBUG
                    }
                    try
                    {
                        pTask->dispose();       // *this Pool owns the Task object, and now gets rid of it
                    }
                    #if DEBUG > 0
                    catch (const std::exception& _oops)
                    {
                        xCriticalSection cs (m_pool.m_spinLock);
                        std::cerr << "ThreadPool::ThreadPoolRunnable::run(): pTask->Dispose() failed and threw a std::exception, description='"<<_oops.what()<<"'" << std::endl;
                        std::cerr << "ThreadPool::ThreadPoolRunnable::run(): Queue has "<<m_pool.m_requestQueue.size()<<" unprocessed tasks." << std::endl;
                        std::cerr << "ThreadPool::ThreadPoolRunnable::run(): My thread ran "<<m_useCount<<" tasks." << std::endl;
                        //std::cerr << "ThreadPool::ThreadPoolRunnable::run(): My thread ID is "<<getThread()->getThreadId().toString()<<"." << std::endl;
                    }
                    #endif // DEBUG
                    catch (...)
                    {
                        #if DEBUG > 0
                        xCriticalSection cs (m_pool.m_spinLock);
                        std::cerr << "ThreadPool::ThreadPoolRunnable::run(): pTask->Dispose() failed and threw an unknown exception." << std::endl;
                        std::cerr << "ThreadPool::ThreadPoolRunnable::run(): Queue has "<<m_pool.m_requestQueue.size()<<" unprocessed tasks." << std::endl;
                        std::cerr << "ThreadPool::ThreadPoolRunnable::run(): My thread ran "<<m_useCount<<" tasks." << std::endl;
                        //std::cerr << "ThreadPool::ThreadPoolRunnable::run(): My thread ID is "<<getThread()->getThreadId().toString()<<"." << std::endl;
                        #endif // DEBUG
                    }
                }
                else                        // convention: null pointer-to-Task stops this worker thread (only)
                {
                    #if DEBUG >= 40
                    {
                        xCriticalSection cs (m_pool.m_spinLock);
                        std::cout << "ThreadPool::ThreadPoolRunnable::run (): Null task, worker exits normally." << std::endl;
                    }
                    #endif // DEBUG
                    // Return the remaining pointers-to-tasks (presumably all NULLs) back to the queue, for the other threads.
                    // Note that we cannot use m_pool.PushRequest() any longer.
                    m_pool.m_requestQueue.push(taskList, ++ppTask, taskList.end());

                    return;                 // Exit the thread function!
                }
            }
        }
        #if DEBUG >= 40
        {
            xCriticalSection cs (m_pool.m_spinLock);
            std::cout << "ThreadPool::ThreadPoolRunnable::run (): Pool is shutting down, worker exits normally." << std::endl;
        }
        #endif  // DEBUG
    }
    #if DEBUG > 0
    catch (const std::exception& _oops)
    {
        // TBD -- should log the exception, or at least inform the pool
        xCriticalSection cs (m_pool.m_spinLock);
        std::cerr << "ThreadPool::ThreadPoolRunnable::run(): std::exception, description='"<<_oops.what()<<"'" << std::endl;
        std::cerr << "ThreadPool::ThreadPoolRunnable::run(): Queue has "<<m_pool.m_requestQueue.size()<<" unprocessed tasks." << std::endl;
        std::cerr << "ThreadPool::ThreadPoolRunnable::run(): My thread ran "<<m_useCount<<" tasks." << std::endl;
        //std::cerr << "ThreadPool::ThreadPoolRunnable::run(): My thread ID is "<<getThread()->getThreadId().toString()<<"." << std::endl;
    }
    #endif // DEBUG
    catch (...)
    {
        // TBD -- should log the exception, or at least inform the pool
        #if DEBUG > 0
        xCriticalSection cs (m_pool.m_spinLock);
        std::cerr << "ThreadPool::ThreadPoolRunnable::run(): Queue has "<<m_pool.m_requestQueue.size()<<" unprocessed tasks." << std::endl;
        std::cerr << "ThreadPool::ThreadPoolRunnable::run(): My thread ran "<<m_useCount<<" tasks." << std::endl;
        #endif // DEBUG
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void ThreadPool::ThreadPoolRunnable::stop()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ThreadPool::ThreadPoolRunnable::ThreadPoolRunnable(ThreadPool& _pool)
:   m_pool(_pool)
#ifdef DEBUG
,   m_useCount(0)
,   m_pThread (0)
#endif // DEBUG
{}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ThreadPool::ThreadPoolRunnable::~ThreadPoolRunnable()
{
    #if DEBUG >= 40
    xCriticalSection cs (m_pool.m_spinLock);
    std::cout << "ThreadPool::ThreadPoolRunnable::~ThreadPoolRunnable(): Use count for this Thread is "<<m_useCount << std::endl;
    #endif // DEBUG
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
ThreadPool::pThread_type
ThreadPool::ThreadPoolRunnable::getThread() const
{
    return m_pThread;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ThreadPool::ThreadPoolRunnable::setThread(pThread_type _pThread)
{
    m_pThread = _pThread;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Threading
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
