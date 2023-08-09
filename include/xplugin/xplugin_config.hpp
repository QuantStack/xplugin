/***************************************************************************
 * Copyright (c) 2018, Johan Mabille, Sylvain Corlay, Wolf Vollprecht,      *
 * Martin Renou and Dr. Thorsten Beier                                      *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/
#ifndef XPLUGIN_CONFIG_HPP
#define XPLUGIN_CONFIG_HPP

#ifdef _WIN32
#ifdef XPLUGIN_EXPORTS
#define XPLUGIN_API __declspec(dllexport)
#else
#define XPLUGIN_API __declspec(dllimport)
#endif

#else
#define XPLUGIN_API
#endif

#define XPLUGIN_VERSION_MAJOR 0
#define XPLUGIN_VERSION_MINOR 2
#define XPLUGIN_VERSION_PATCH 0

#endif
