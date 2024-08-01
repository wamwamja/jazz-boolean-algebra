/**
 * @file config.h
 */

/*******************************************************************************
 * Copyright (c) 2024 - 2024.  Jiazhen LUO
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 ******************************************************************************/

#ifndef BOOLEAN_ALGEBRA_CONFIG_H
#define BOOLEAN_ALGEBRA_CONFIG_H

//#define JAZZ_DEBUG

#if !defined(JAZZ_ASSERT)
#if defined(JAZZ_DEBUG)
#include <cassert>
#define JAZZ_ASSERT(cond) assert(cond)
#else
#define JAZZ_ASSERT(cond)
#endif
#endif

#ifdef JAZZ_DEBUG
#define JAZZ_DEBUG_VIRTUAL virtual
#else
#define JAZZ_DEBUG_VIRTUAL
#endif

#if !defined(DO_JAZZ_STATIC_ASSERT)
#if defined(DO_JAZZ_ASSERT)
#define JAZZ_STATIC_ASSERT(cond) static_assert(cond)
#else
#define JAZZ_STATIC_ASSERT(cond)
#endif
#endif


#if !defined(JAZZ_API)
#if defined(_WIN32_) || defined(WIN32) || defined(_WIN64_) || defined(WIN64)
#define JAZZ_API __declspec(dllexport)
#elif defined(_ADNROID_) || defined(ANDROID) || defined(__APPLE__) || defined(__linux__)
#define JAZZ_API __attribute__((visibility("default")))
#else
#define JAZZ_API
#endif
#endif

#endif//BOOLEAN_ALGEBRA_CONFIG_H
