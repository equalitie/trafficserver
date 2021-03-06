/** @file

  A brief file description

  @section license License

  Licensed to the Apache Software Foundation (ASF) under one
  or more contributor license agreements.  See the NOTICE file
  distributed with this work for additional information
  regarding copyright ownership.  The ASF licenses this file
  to you under the Apache License, Version 2.0 (the
  "License"); you may not use this file except in compliance
  with the License.  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
 */

#pragma once
#include "I_EventSystem.h"

TS_INLINE const char *
get_vc_event_name(int event)
{
  switch (event) {
  default:
    return "unknown event";
  case VC_EVENT_NONE:
    return "VC_EVENT_NONE";
  case VC_EVENT_IMMEDIATE:
    return "VC_EVENT_IMMEDIATE";
  case VC_EVENT_READ_READY:
    return "VC_EVENT_READ_READY";
  case VC_EVENT_WRITE_READY:
    return "VC_EVENT_WRITE_READY";
  case VC_EVENT_READ_COMPLETE:
    return "VC_EVENT_READ_COMPLETE";
  case VC_EVENT_WRITE_COMPLETE:
    return "VC_EVENT_WRITE_COMPLETE";
  case VC_EVENT_EOS:
    return "VC_EVENT_EOS";
  case VC_EVENT_ERROR:
    return "VC_EVENT_ERROR";
  case VC_EVENT_INACTIVITY_TIMEOUT:
    return "VC_EVENT_INACTIVITY_TIMEOUT";
  case VC_EVENT_ACTIVE_TIMEOUT:
    return "VC_EVENT_ACTIVE_TIMEOUT";
  }
}

TS_INLINE
VConnection::VConnection(ProxyMutex *aMutex) : Continuation(aMutex), lerrno(0)
{
  SET_HANDLER(0);
}

TS_INLINE
VConnection::VConnection(Ptr<ProxyMutex> &aMutex) : Continuation(aMutex), lerrno(0)
{
  SET_HANDLER(0);
}

TS_INLINE
VConnection::~VConnection()
{
}

//////////////////////////////////////////////////////////////////////////////
//
//      DEPRECATED DEPRECATED DEPRECATED
//
//      inline VIO * VConnection::do_io()
//
//      This method enqueues a VIO operation onto the VIO queue, and
//      activates the I/O operation if and operation of that type isn't
//      already underway.
//
//////////////////////////////////////////////////////////////////////////////

TS_INLINE VIO *
vc_do_io_write(VConnection *vc, Continuation *cont, int64_t nbytes, MIOBuffer *buf, int64_t offset)
{
  IOBufferReader *reader = buf->alloc_reader();

  if (offset > 0)
    reader->consume(offset);

  return vc->do_io_write(cont, nbytes, reader, true);
}

TS_INLINE VIO *
VConnection::do_io(int op, Continuation *c, int64_t nbytes, MIOBuffer *cb, int data)
{
  switch (op) {
  case VIO::READ:
    return do_io_read(c, nbytes, cb);
  case VIO::WRITE:
    return vc_do_io_write(this, c, nbytes, cb, data);
  case VIO::CLOSE:
    do_io_close();
    return nullptr;
  case VIO::ABORT:
    do_io_close(data);
    return nullptr;
  case VIO::SHUTDOWN_READ:
    do_io_shutdown(IO_SHUTDOWN_READ);
    return nullptr;
  case VIO::SHUTDOWN_WRITE:
    do_io_shutdown(IO_SHUTDOWN_WRITE);
    return nullptr;
  case VIO::SHUTDOWN_READWRITE:
    do_io_shutdown(IO_SHUTDOWN_READWRITE);
    return nullptr;
  }
  ink_assert(!"cannot use default implementation for do_io operation");
  return nullptr;
}

TS_INLINE void
VConnection::set_continuation(VIO *, Continuation *)
{
}
TS_INLINE void
VConnection::reenable(VIO *)
{
}
TS_INLINE void
VConnection::reenable_re(VIO *vio)
{
  reenable(vio);
}
