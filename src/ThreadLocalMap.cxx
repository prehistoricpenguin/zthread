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

#include "Debug.h"

#include "ThreadLocalMap.h"
#include "zthread/AbstractThreadLocal.h"
#include <assert.h>

namespace ZThread {

ThreadLocalMap::~ThreadLocalMap() throw() {
}

bool ThreadLocalMap::getValue(const AbstractThreadLocal* atl, void* &oldValue) {

  Map::iterator i = _map.find(atl);
  if(i == _map.end() || &oldValue == 0)
    return false;

  oldValue = i->second;
  return true;

}

bool ThreadLocalMap::setValue(const AbstractThreadLocal* atl, void* &oldValue, void* newValue) {

  Map::iterator i = _map.find(atl);
  if(i != _map.end() && &oldValue != 0) {

    oldValue = i->second;
    i->second = newValue;

    return true;

  }

  _map.insert(std::make_pair(atl, newValue));
  return false;

}

ThreadLocalMap& ThreadLocalMap::operator=(const ThreadLocalMap& map) throw() {

  ZTDEBUG("Propogating %d values.\n", map._map.size());

  // Propogate values from the given map into this one.
  for(Map::const_iterator i = map._map.begin(); i != map._map.end(); ++i) {

    const AbstractThreadLocal* atl = i->first;
    void* value = i->second;

    assert(atl);
    
    // When a ThreadLocal that is willing to propogate is found, 
    // calculate the child value
    if(atl->propogateValue()) {
      
      value = atl->childValue(value);
      _map.insert(std::make_pair(atl,value));
  
    }
    
  }
  
  return *this;

}

void ThreadLocalMap::clear() throw() {

  // Notify the ThreadLocal's
  for(Map::const_iterator i = _map.begin(); i != _map.end(); ++i) {

    const AbstractThreadLocal* atl = i->first;
    assert(atl);

    atl->destroyValue(i->second);

  }

  _map.clear();

}


} // namespace ZThread


