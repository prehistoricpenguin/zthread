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

#ifndef __ZTRUNNABLE_H__
#define __ZTRUNNABLE_H__

#include "zthread/Config.h"

namespace ZThread {

/**
 * @class Runnable
 * 
 * @author Eric Crahen <crahen at code-foo dot com>
 * @date <2002-12-21T04:31:53-0500>
 * @version 2.2.11
 *
 * Encapsulates a Runnable task.
 */
class Runnable {
  public:

  /**
   * Runnables should never throw in thier destructors
   */
	  virtual ~Runnable() throw() { }

  /**
   * Task to be performed in another thread of execution
   */
  virtual void run() throw() = 0;

};
 

}

#endif // __ZTRUNNABLE_H__



