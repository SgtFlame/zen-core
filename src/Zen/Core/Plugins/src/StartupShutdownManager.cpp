//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
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
//  Tony Richards trichards@indiezen.com
//  Matthew Alan Gray mgray@indiezen.org
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#include "StartupShutdownManager.hpp"

#include "../I_StartupShutdownParticipant.hpp"

#include <Zen/Core/Utility/runtime_exception.hpp>

#include <Zen/Core/Threading/MutexFactory.hpp>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Plugins {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
StartupShutdownManager::StartupShutdownManager()
:   m_sharedThreadPool(16, NULL, true, true)
,   m_installQueue(1, NULL, true, false)
,   m_shutdownQueue(1, NULL, true, false)
,   m_pParticipantGuard(Threading::MutexFactory::create())
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
StartupShutdownManager::~StartupShutdownManager()
{
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Zen::Threading::I_Condition*
StartupShutdownManager::start()
{
    class InstallationCompleteTask
    :   public Threading::ThreadPool::Task
    {
    public:
        virtual void call()
        {
            m_condition.assertCondition();
        }

        InstallationCompleteTask(Zen::Threading::I_Condition& _condition)
        :   m_condition(_condition)
        {
        }
    private:
        Zen::Threading::I_Condition&    m_condition;
    };

    // Create an I_Condition to pass to the caller
    Zen::Threading::I_Condition* pCondition = Zen::Threading::ConditionFactory::create(false);

    // Create the installation complete task
    InstallationCompleteTask* pTask = new InstallationCompleteTask(*pCondition);

    // Push the task onto the installQueue
    m_installQueue.pushRequest(pTask);

    // Start the installation queue so that participants will be installed
    m_installQueue.start();

    return pCondition;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
StartupShutdownManager::stop()
{
    class InstallationQueueEmptyTask
    :   public Threading::ThreadPool::Task
    {
    public:
        virtual void call()
        {
            m_condition.assertCondition();
        }

        InstallationQueueEmptyTask(Zen::Threading::I_Condition& _condition)
        :   m_condition(_condition)
        {
        }
    private:
        Zen::Threading::I_Condition&    m_condition;
    };

    class ShutdownCompleteTask
    :   public Threading::ThreadPool::Task
    {
    public:
        virtual void call()
        {
            m_condition.assertCondition();
        }

        ShutdownCompleteTask(Zen::Threading::I_Condition& _condition)
        :   m_condition(_condition)
        {
        }
    private:
        Zen::Threading::I_Condition&    m_condition;
    };

    // Create an I_Condition to know when the installQueue is empty...
    Zen::Threading::I_Condition* pInstallerCondition = Zen::Threading::ConditionFactory::create(false);

    // Create the installation complete task and pass the condition into it.
    InstallationQueueEmptyTask* pInstallerTask = new InstallationQueueEmptyTask(*pInstallerCondition);

    // Push the task onto the end of the queue
    m_installQueue.pushRequest(pInstallerTask);

    // Prepare to stop the installation queue so that no more tasks
    // will be pushed onto it.
    m_installQueue.prepareToStop();

    // Wait for the install queue to be emptied
    pInstallerCondition->requireCondition();

    // Destroy the condition
    Zen::Threading::ConditionFactory::destroy(pInstallerCondition);

    // Stop the install queue
    m_installQueue.stop();

    // Create an I_Condition to know when the shutdownQueue is finished.
    Zen::Threading::I_Condition* pShutdownCondition = Zen::Threading::ConditionFactory::create(false);

    // Create the shutdown complete task and pass the condition into it.
    ShutdownCompleteTask* pShutdownTask = new ShutdownCompleteTask(*pShutdownCondition);

    // Push the task onto the end of the queue
    m_shutdownQueue.pushRequest(pShutdownTask);

    // Start the shutdown queue
    m_shutdownQueue.start();

    // Wait on the shutdown queue to completely start
    m_shutdownQueue.requireStarted();

    // Prepare to stop the shutdown queue so that no more tasks will be
    // pushed onto it.
    m_shutdownQueue.prepareToStop();

    // Wait for the shutdown queue to be emptied.
    pShutdownCondition->requireCondition();

    // Destroy the condition
    Zen::Threading::ConditionFactory::destroy(pShutdownCondition);

    // Stop the shutdown queue
    m_shutdownQueue.stop();

    // Empty participant collections
    {
        Threading::CriticalSection lock(m_pParticipantGuard);
        m_participants.empty();
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
StartupShutdownManager::installParticipant(pParticipant_type _pParticipant)
{
    class InstallParticipantTask
    :   public Threading::ThreadPool::Task
    {
    private:
        class PrepareToStartParticipantTask
            :   public Threading::ThreadPool::Task
        {
        private:
            class StartParticipantTask
            :   public Threading::ThreadPool::Task
            {
            public:
                virtual void call()
                {
                    m_pParticipant->start();
                }

                StartParticipantTask(pParticipant_type _pParticipant)
                :   m_pParticipant(_pParticipant)
                {
                }
            private:
                pParticipant_type   m_pParticipant;
            };

            class StopParticipantTask
            :   public Threading::ThreadPool::Task
            {
            public:
                virtual void call()
                {
                    m_pParticipant->stop();
                }

                StopParticipantTask(pParticipant_type _pParticipant)
                :   m_pParticipant(_pParticipant)
                {
                }
            private:
                pParticipant_type   m_pParticipant;
            };
        public:
            virtual void call()
            {
                Threading::I_Condition* pCondition = m_pParticipant->prepareToStart(m_manager.getSharedThreadPool());
                if( pCondition != NULL )
                {
                    pCondition->requireCondition();
                }

                StartParticipantTask* pStartTask = new StartParticipantTask(m_pParticipant);
                StopParticipantTask* pStopTask = new StopParticipantTask(m_pParticipant);
                m_installQueue.pushRequest(pStartTask);
                m_shutdownQueue.pushRequest(pStopTask);
            }

            PrepareToStartParticipantTask(StartupShutdownManager& _manager, pParticipant_type _pParticipant, Threading::ThreadPool& _installQueue, Threading::ThreadPool& _shutdownQueue)
            :   m_manager(_manager)
            ,   m_pParticipant(_pParticipant)
            ,   m_installQueue(_installQueue)
            ,   m_shutdownQueue(_shutdownQueue)
            {
            }
        private:
            StartupShutdownManager&         m_manager;
            pParticipant_type               m_pParticipant;
            Threading::ThreadPool&          m_installQueue;
            Threading::ThreadPool&          m_shutdownQueue;
        };

        class PrepareToStopParticipantTask
        :   public Threading::ThreadPool::Task
        {
        public:
            virtual void call()
            {
                Threading::I_Condition* pCondition = m_pParticipant->prepareToStop();
                if( pCondition != NULL )
                {
                    pCondition->requireCondition();
                }
            }

            PrepareToStopParticipantTask(pParticipant_type _pParticipant)
            :   m_pParticipant(_pParticipant)
            {
            }
        private:
            pParticipant_type   m_pParticipant;
        };
    public:
        virtual void call()
        {
            m_manager.handleInstallParticipant(m_pParticipant);
            PrepareToStartParticipantTask* pStartTask = new PrepareToStartParticipantTask(m_manager, m_pParticipant, m_installQueue, m_shutdownQueue);
            PrepareToStopParticipantTask* pStopTask = new PrepareToStopParticipantTask(m_pParticipant);
            m_installQueue.pushRequest(pStartTask);
            m_shutdownQueue.pushRequest(pStopTask);
        }

        InstallParticipantTask(StartupShutdownManager& _manager, pParticipant_type _pParticipant, Threading::ThreadPool& _installQueue, Threading::ThreadPool& _shutdownQueue)
        :   m_manager(_manager)
        ,   m_pParticipant(_pParticipant)
        ,   m_installQueue(_installQueue)
        ,   m_shutdownQueue(_shutdownQueue)
        {
        }
    private:
        StartupShutdownManager&     m_manager;
        pParticipant_type           m_pParticipant;
        Threading::ThreadPool&      m_installQueue;
        Threading::ThreadPool&      m_shutdownQueue;
    };

    // Schedule the installation of the participant.
    InstallParticipantTask* pTask = new InstallParticipantTask(*this, _pParticipant, m_installQueue, m_shutdownQueue);
    m_installQueue.pushRequest(pTask);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
StartupShutdownManager::handleInstallParticipant(pParticipant_type _pParticipant)
{
    Threading::CriticalSection lock(m_pParticipantGuard);

    m_participants.insert(_pParticipant);
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Plugins
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
