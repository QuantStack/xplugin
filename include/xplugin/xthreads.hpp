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

struct xmutex_dummy
{
};
template <class... MutexTypes>
class xscoped_lock_dummy
{
  public:
    xscoped_lock_dummy(MutexTypes &...)
    {
    }
};

#ifdef XPLUGIN_NO_THREADS
using xmutex = xmutex_dummy;
template <class... MutexTypes>
using xscoped_lock = xscoped_lock_dummy<MutexTypes...>;
#else
using xmutex = std::mutex;
template <class... MutexTypes>
using xscoped_lock = std::scoped_lock<MutexTypes...>;
#endif

template <bool THREAD_SAFE>
using xmutex_t = std::conditional_t<THREAD_SAFE, xmutex, xmutex_dummy>;

template <bool THREAD_SAFE, class... MutexTypes>
using xscoped_lock_t = std::conditional_t<THREAD_SAFE, xscoped_lock<MutexTypes...>, xscoped_lock_dummy<MutexTypes...>>;

} // namespace xp

#endif // XPLUGIN_XTHREADS_HPP
