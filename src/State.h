/*
 *  ZThreads, a platform-independant, multithreading and 
 *  synchroniation library
 *
 *  Copyright (C) 2001 Eric Crahen, See LGPL.TXT for details
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

#ifndef __ZTSTATE_H__
#define __ZTSTATE_H__

namespace ZThread {

/**
 * @class State 
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-06-02T07:09:33-0400>
 * @version 2.2.1
 *
 * Class to encapsulte the current state of the threads life-cycle.
 */
class State {

  //! Various states
  typedef enum { REFERENCE, IDLE, RUNNING, COMPLETE, JOINED } STATE;
  typedef enum { NONE, DAEMON } MODIFIER;

  //! Current state
  STATE _state;
  unsigned short _modifiers;

 public:

  /**
   * Create State with the given flag set.
   */
  State(STATE s=IDLE) : _state(s), _modifiers(NONE) {}

  /**
   * Test for the IDLE state. No task has yet run.
   */
  bool isIdle() const {
    return _state == IDLE;
  }

  /**
   * Test for the RUNNING state. A task is in progress.
   *
   * @return bool
   */
  bool isRunning() const {
    return _state == RUNNING;
  }

  /**
   * Test for the REFERENCE state. A task is in progress but not
   * under control of this library.
   *
   * @return bool
   */
  bool isReference() const {
    return _state == REFERENCE;
  }

  /**
   * Transition to the IDLE state.
   *
   * @return bool true if successful
   */
  bool setIdle() {

    if(_state != RUNNING)
      return false;

    _state = IDLE;
    return true;

  }

  /**
   * Transition to the RUNNING state.
   *
   * @return bool true if successful
   */
  bool setRunning() {

    if(_state != IDLE)
      return false;

    _state = RUNNING;
    return true;

  }

  /**
   * Transition to the REFERENCE state.
   *
   * @return bool true if successful
   */
  bool setReference() {

    if(_state != IDLE)
      return false;

    _state = REFERENCE;
    return true;

  }

  /**
   * Test for the COMPLETE state. A task has completed but 
   * the thread is not yet join()ed.
   *
   * @return bool
   */
  bool isComplete() const {
    return _state == COMPLETE;
  }

  /**
   * Transition to the COMPLETE state.
   *
   * @return bool true if successful
   */
  bool setComplete() {

    if(_state != RUNNING)
      return false;

    _state = COMPLETE;
    return true;

  }

  /**
   * Test for the JOINED state. A task has completed and 
   * the thread is join()ed.
   *
   * @return bool
   */
  bool isJoined() const {
    return _state == JOINED;
  }

  /**
   * Transition to the JOINED state.
   *
   * @return bool true if successful
   */
  bool setJoined() {

    if(_state != COMPLETE)
      return false;

    _state = JOINED;
    return true;

  }

  /**
   * Test for the DAEMON modifier. 
   *
   * @return bool
   */
  bool isDaemon() const {
    return (_modifiers & DAEMON) != 0;
  }


  /**
   * Toggle the DAEMON modifier.
   *
   * @param flag bool 
   * @return bool previous state
   */
  bool setDaemon(bool flag) {

    bool wasDaemon = isDaemon();

    _modifiers &= ~DAEMON;
    if(flag)
      _modifiers |= DAEMON;

    return wasDaemon;

  }

};


};

#endif
