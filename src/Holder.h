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

#ifndef __ZTHOLDER_H__
#define __ZTHOLDER_H__

namespace ZThread {

/**
 * @class Holder
 * @author Eric Crahen <zthread@code-foo.com>
 * @date <2002-05-25T21:27:07-0400>
 *
 * A Holder is, well, exactly that; an object used to hold a value
 * for another. Useful for storing things in an STL collection and 
 * keeping a direct reference to them,
 */
class Holder {
  
  void* _value;

 public:
  
  /**
   * Create a new Holder with given value.
   *
   * @param void* value
   */
  inline Holder(void* value) : _value(value) { }

  /**
   * Get the current value held.
   *
   * @return void*
   */
  inline void* getValue() const {
    return _value;
  }

  /**
   * Set the current value held by this object and return
   * the old value.
   *
   * @param void* new value
   * @return void* old value
   */
  inline void* setValue(void* value) {

    void* oldValue = _value;
    _value = value;

    return oldValue;

  }

};

};

#endif // __ZTHOLDER_H__
