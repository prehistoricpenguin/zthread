/*
 *  ZThreads, a platform-independant, multithreading and 
 *  synchroniation library
 *
 *  Copyright (C) 2000-2002, Eric Crahen, See LGPL.TXT for details
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

#ifndef __ZTDEFAULTTHREADFACTORY_H__
#define __ZTDEFAULTTHREADFACTORY_H__

#include "zthread/ThreadFactory.h"
#include "zthread/Thread.h"

namespace ZThread {

/**
 * @class DefaultThreadFactory
 *
 * @author Eric Crahen <zthread@code-foo.com>
 * @date <2002-05-31T13:40:52-0400>
 * @version 2.2.0
 *
 * A DefaultThreadFactory produces stock Thread objects. These are
 * simply returned to the caller after creation without performing
 * any special initialization.
 *
 * @see ThreadFactory
 */
class DefaultThreadFactory : public ThreadFactory {

public:

  //! Destroy the ThreadFactory.
  virtual ~DefaultThreadFactory() throw() {}

  /**
   * Create a new Thread. The ownership of the Thread is passed to
   * the code that calls this function.
   *
   * @exception Synchronization_Exception thrown if a new Thread
   * cannot be created.
   */
  virtual Thread* create() /* throw(Synchronization_Exception) */ {
    return new Thread();
  }

};
 
} // namespace ZThread

#endif // __ZTDEFAULTTHREADFACTORY_H__



