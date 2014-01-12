//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2006 - 2008 John Givler
// Copyright (C) 2008 Tony Richards
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

#include "../ConditionFactory.hpp"

#if defined(THREADMODEL_POSIX)
#include "Condition_posix.hpp"
#endif

#if defined(THREADMODEL_SOLARIS)
#include "Condition_solaris.hpp"
#endif

#if defined(THREADMODEL_POSIX)
#include "Condition_posix.hpp"
#endif

#if defined(THREADMODEL_WIN32)
#include "Condition_win32.hpp"
#endif

#if     !defined(THREADMODEL_POSIX) && !defined(THREADMODEL_SOLARIS) && !defined(THREADMODEL_WIN32)
#error  Unrecognized (or unspecified) thread model.  Try THREADMODEL_POSIX or THREADMODEL_SOLARIS or THREADMODEL_WIN32.
#endif

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
namespace Zen {
namespace Threading {
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

I_Condition*
ConditionFactory::create(const bool _initialState)
{
    // TR - I deviated slightly from DrG's more flexible design in that I only
    //  support one thread model at a time.
#ifdef THREADMODEL_WIN32
	return new Condition_win32(_initialState);
#else
#ifdef THREADMODEL_SOLARIS
    return new Condition_solaris(_initialState);
#else
#ifdef THREADMODEL_POSIX
    return new Condition_posix(_initialState);
#else
#error ConditionFactory::create not implemented for this platform
#endif  // THREADMODEL_POSIX
#endif  // THREADMODEL_SOLARIS
#endif  // THREADMODEL_WIN32
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
void
ConditionFactory::destroy(I_Condition* const _pCondition)
{
    delete _pCondition;
}

//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
}   // namespace Threading
}   // namespace Zen
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
