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


#ifndef __ZTATOMICCOUNTIMPL_H__
#define __ZTATOMICCOUNTIMPL_H__

#include "../FastLock.h"
#include <assert.h>

namespace ZThread {

typedef struct atomic_count_t {

  FastLock lock;
  unsigned long count;
  
  atomic_count_t() : count(1) {}

} ATOMIC_COUNT;

AtomicCount::AtomicCount() {

  ATOMIC_COUNT* c = new ATOMIC_COUNT;
  _value = reinterpret_cast<void*>(c);
  
}

AtomicCount::~AtomicCount() {

  ATOMIC_COUNT* c = reinterpret_cast<ATOMIC_COUNT*>(_value);
  assert(c->count == 0);

  delete c;

}
  
void AtomicCount::increment() {

  ATOMIC_COUNT* c = reinterpret_cast<ATOMIC_COUNT*>(_value);
  
  c->lock.acquire();
  ++c->count;
  c->lock.release();  
  
}
  
bool AtomicCount::decrement() {

  ATOMIC_COUNT* c = reinterpret_cast<ATOMIC_COUNT*>(_value);
  bool n;

  c->lock.acquire();
  n = (--c->count == 0);
  c->lock.release();  

  return n;
  
}
 

};

#endif // __ZTATOMICCOUNTIMPL_H__
