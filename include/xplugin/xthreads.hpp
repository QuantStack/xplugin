/***************************************************************************
 * Copyright (c) 2023, Dr. Thorsten Beier                                   *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/
#ifndef XPLUGIN_XTHREADS_HPP
#define XPLUGIN_XTHREADS_HPP

#ifdef XPLUGIN_NO_THREADS
#else
#include <mutex>
#endif

// for environments without threads (like emscripten)
// we define a dummy mutex and xscoped_lock
namespace xp
{
#ifdef XPLUGIN_NO_THREADS
struct xmutex
{
};
template <class... MutexTypes>
class xscoped_lock
{
  public:
    xscoped_lock(MutexTypes &...)
    {
    }
};
#else
using xmutex = std::mutex;
template <class... MutexTypes>
using xscoped_lock = std::scoped_lock<MutexTypes...>;
#endif
} // namespace xp

#endif // XPLUGIN_XTHREADS_HPP
