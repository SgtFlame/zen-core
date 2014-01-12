//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
// Zen Core Framework
//
// Copyright (C) 2001 - 2009 Tony Richards
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
//-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
/// @author  John S. Givler, Ph.D.(Computer Science)
#ifndef ZEN_THREADING_CONFIGURAGION_HPP_INCLUDED
#define ZEN_THREADING_CONFIGURAGION_HPP_INCLUDED

#ifdef ZENCORE_EXPORTS
#define THREADING_EXPORTS
#endif

#ifdef  _MSC_VER
#    pragma once
#    pragma inline_depth(255)
#    pragma inline_recursion(off)
#    pragma warning(disable:4251)   // 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
#    pragma warning(disable:4275)   // non – DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
#ifdef VLD
#include <vld.h>
#endif // VLD
#endif  // _MSC_VER


#ifdef  _MSC_VER
#    ifndef HOST_WIN32              // this probably should be done elsewhere ...
#        ifdef _WIN32
#            define HOST_WIN32
#            define HOST_CPU_X86    // TODO this could be incorrect
#        endif
#    endif
#    ifndef HOST_WIN64
#        ifdef _WIN64
#            define HOST_WIN64
#            define HOST_CPU_X86_64
#        endif
#    endif

#    ifndef THREADMODEL_WIN32
#        define THREADMODEL_WIN32
#    endif

#endif  // _MSC_VER


// Microsoft C++ compiler
#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
#    ifdef  THREADING_EXPORTS
#        define THREADING_DLL_LINK __declspec(dllexport)
#    else
#        define THREADING_DLL_LINK __declspec(dllimport)
#    endif
#endif  // _MSC_VER


// GNU C++ compiler
#if defined(__GNUG__) && !defined(__INTEL_COMPILER)
#    if 0 // (__GNUC__ >= 4) && !defined(HOST_CYGWIN) && !defined(HOST_SOLARIS)
#        ifdef  THREADING_EXPORTS
#            define THREADING_DLL_LINK __attribute__ ((visibility ("default")))
#        else
#            define THREADING_DLL_LINK __attribute__ ((visibility ("hidden")))
#        endif
#    else
#        ifdef  THREADING_EXPORTS
#            define THREADING_DLL_LINK // __declspec(dllexport)
#        else
#            define THREADING_DLL_LINK // __declspec(dllimport)
#        endif
#    endif
#endif  // __GNUG__


// Sun C++ compiler
#if defined(__SUNPRO_CC)
#    ifdef  THREADING_EXPORTS
#        define THREADING_DLL_LINK   // ?
#    else
#        define THREADING_DLL_LINK   // ?
#    endif
#endif  // __SUNPRO_CC


// Intel C++ compiler
#if defined(__INTEL_COMPILER)
#    ifdef  THREADING_EXPORTS
#        define THREADING_DLL_LINK __declspec(dllexport)
#    else
#        define THREADING_DLL_LINK __declspec(dllimport)
#    endif
#endif  // __INTEL_COMPILER


#ifndef THREADING_DLL_LINK
#    define THREADING_DLL_LINK
#endif  // ! THREADING_EXPORTS


#endif  // ZEN_THREADING_CONFIGURAGION_HPP_INCLUDED

