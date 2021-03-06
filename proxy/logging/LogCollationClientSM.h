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

//-------------------------------------------------------------------------
// includes
//-------------------------------------------------------------------------
#include "P_HostDB.h"
#include "P_Net.h"
#include "LogCollationBase.h"

//-------------------------------------------------------------------------
// pre-declarations
//-------------------------------------------------------------------------

class LogBuffer;
class LogHost;

//-------------------------------------------------------------------------
// LogCollationClientSM
//-------------------------------------------------------------------------

class LogCollationClientSM : public LogCollationBase, public Continuation
{
public:
  LogCollationClientSM(LogHost *log_host);
  ~LogCollationClientSM();

  int client_handler(int event, void *data);

  // public interface (for LogFile)
  int send(LogBuffer *log_buffer);

private:
  enum ClientState {
    LOG_COLL_CLIENT_AUTH,
    LOG_COLL_CLIENT_DNS,
    LOG_COLL_CLIENT_DONE,
    LOG_COLL_CLIENT_FAIL,
    LOG_COLL_CLIENT_IDLE,
    LOG_COLL_CLIENT_INIT,
    LOG_COLL_CLIENT_OPEN,
    LOG_COLL_CLIENT_SEND
  };

  enum ClientFlowControl {
    LOG_COLL_FLOW_ALLOW,
    LOG_COLL_FLOW_DENY,
  };

private:
  // client states
  int client_auth(int event, VIO *vio);
  int client_dns(int event, HostDBInfo *hostdb_info);
  int client_done(int event, void *data);
  int client_fail(int event, void *data);
  int client_idle(int event, void *data);
  int client_init(int event, void *data);
  int client_open(int event, NetVConnection *net_vc);
  int client_send(int event, VIO *vio);
  ClientState m_client_state;

  // support functions
  void flush_to_orphan();

  // iocore stuff (two buffers to avoid races)
  NetVConnection *m_host_vc;
  VIO *m_host_vio;
  MIOBuffer *m_auth_buffer;
  IOBufferReader *m_auth_reader;
  MIOBuffer *m_send_buffer;
  IOBufferReader *m_send_reader;
  Action *m_pending_action;
  Event *m_pending_event;

  // to detect server closes (there's got to be a better way to do this)
  VIO *m_abort_vio;
  MIOBuffer *m_abort_buffer;
  bool m_host_is_up;

  // send stuff
  LogBufferList *m_buffer_send_list;
  LogBuffer *m_buffer_in_iocore;
  ClientFlowControl m_flow;

  // back pointer to LogHost container
  LogHost *m_log_host;

  // debugging
  static int ID;
  int m_id;
};

typedef int (LogCollationClientSM::*LogCollationClientSMHandler)(int, void *);
