/*
 *  ZThreads, a platform-independant, multithreading and 
 *  synchroniation library
 *
 *  Copyright (C) 2001, 2002 Eric Crahen, See LGPL.TXT for details
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#ifndef __ZTDEFERREDINTERRUPTIONSCOPE_H__
#define __ZTDEFERREDINTERRUPTIONSCOPE_H__

#include <assert.h>

namespace ZThread {

/**
 * @class DeferredInterruptionScope
 * @author Eric Crahen <zthread@code-foo.com>
 * @date <2002-05-29T08:00:49-0400>
 * @version 2.2.0
 *
 * Locking policy for a Guard that will defer any state reported 
 * for the reported Status of a thread except SIGNALED until the 
 * scope has ended. This allows a Guard to be used to create an
 * uninterruptible region in code. 
 */
class DeferredInterruptionScope {
 public:
  
  template <class LockType>
  static void createScope(LockHolder<LockType>& l) {

    l.getLock().interest(Monitor::SIGNALED);

  }

  template <class LockType>
  static void destroyScope(LockHolder<LockType>& l) {

    l.getLock().interest(Monitor::ANYTHING);

  }

};

}

#endif // __ZTDEFERREDINTERRUPTIONSCOPE_H__
