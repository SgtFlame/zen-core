//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2006 - 2008 John Givler
// Copyright (C) 2008        Tony Richards
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
//
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
#ifdef THREADMODEL_POSIX

#include "Mutex_posix.hpp"

// C++
#include <stdexcept>
#include <sstream>
#include <iostream>

// C
#include <errno.h>

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Threading {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

Mutex_posix::Mutex_posix()
{
    const int rc = ::pthread_mutex_init(&m_mutex, NULL);
    if (rc != 0)
    {
        #ifndef NDEBUG
        std::cerr << "Mutex_posix::Mutex_posix(): pthread_mutex_init() failed, rc="<<rc<<": "<<getNameOfError(rc)<<"." << std::endl;
        #endif  // ! NDEBUG
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
Mutex_posix::~Mutex_posix()
{
    const int rc = ::pthread_mutex_destroy(&m_mutex);
    if (rc != 0)
    {
        #ifndef NDEBUG
        std::cerr << "Mutex_posix::~Mutex_posix(): pthread_mutex_destroy() failed, rc="<<rc<<": "<<getNameOfError(rc)<<"." << std::endl;
        #endif  // ! NDEBUG
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Mutex_posix::acquire()
{
    const int rc = ::pthread_mutex_lock(&m_mutex);
    if (rc != 0)
    {
        std::ostringstream ErrMsg;
        ErrMsg << "Mutex_posix::Acquire(): pthread_mutex_lock() failed, rc="<<rc<<": "<<getNameOfError(rc)<<".";
        throw std::runtime_error(ErrMsg.str());
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
Mutex_posix::release()
{
    const int rc = ::pthread_mutex_unlock(&m_mutex);
    if (rc != 0)
    {
        std::ostringstream ErrMsg;
        ErrMsg << "Mutex_posix::Release(): pthread_mutex_unlock() failed, rc="<<rc<<": "<<getNameOfError(rc)<<".";
        throw std::runtime_error(ErrMsg.str());
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
const std::string&
Mutex_posix::getNameOfError(const int _errNo)
{
    // similar to strerror()

    static const std::string eTimedOut  ("ETIMEDOUT");
    static const std::string eDeadLk    ("EDEADLK");
    static const std::string eNoMem     ("ENOMEM");
    static const std::string eAgain     ("EAGAIN");
    static const std::string eInval     ("EINVAL");
    static const std::string eBusy      ("EBUSY");
    static const std::string eSrch      ("ESRCH");
    static const std::string ePerm      ("EPERM");
    static const std::string NoClue     ("<unknown>");

    switch (_errNo)
    {
    case ETIMEDOUT:
        return eTimedOut;
    case EDEADLK:
        return eDeadLk;
    case ENOMEM:
        return eNoMem;
    case EAGAIN:
        return eAgain;
    case EINVAL:
        return eInval;
    case EBUSY:
        return eBusy;
    case ESRCH:
        return eSrch;
    case EPERM:
        return ePerm;
    default:
        return NoClue;
    }
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Threading
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

#endif // THREADMODEL_POSIX
