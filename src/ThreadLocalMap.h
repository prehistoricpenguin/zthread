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
 *
 *  SUNY @ Buffalo, hereby disclaims all copyright interest in the
 *  ZThreads library written by Eric Crahen
 */

#ifndef __ZTTHREADLOCALMAP_H__
#define __ZTTHREADLOCALMAP_H__

#include "zthread/Config.h" // disable warnings for vc6 stl 
#include <map>
#include <utility>

namespace ZThread {

class AbstractThreadLocal;

/**
 * @class ThreadLocalMap
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-05-27T13:58:02-0400>
 * @version 2.2.0
 *
 * A ThreadLocalMap keeps track of which ThreadLocal object has mapped 
 * which values.
 */
class ThreadLocalMap {

  typedef std::map<const AbstractThreadLocal*, void*> Map;

  //! Map each AbstractThreadLocal to a value
  Map _map;

 public:

  ThreadLocalMap() {};

  ~ThreadLocalMap() throw();

  /**
   * Get the value associated with a ThreadLocal and the current thread.
   */
  bool getValue(const AbstractThreadLocal*, void* &oldValue);

  /**
   * Set the value associated with a TheadLocal and the current thread.
   */
  bool setValue(const AbstractThreadLocal*, void* &oldValue, void* newValue);

  /**
   * Copy associated ThreadLocals and values from another ThreadLocalMap
   * by propogating each value from that map.
   *
   * @param const ThreadLocalMap& - source of new entries 
   */
  ThreadLocalMap& operator=(const ThreadLocalMap&) throw();

  /**
   * Remove all the values associated with the current thread, fire the 
   * destroyValue() method of each ThreadLocal that has an entry.
   */
  void clear() throw();

};

};

#endif // __ZTTHREADLOCALMAP_H__

