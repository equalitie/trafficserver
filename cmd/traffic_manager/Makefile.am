#
#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

bin_PROGRAMS = traffic_manager
check_PROGRAMS = test_metrics

TESTS = $(check_PROGRAMS)

AM_CPPFLAGS += \
  $(LUAJIT_CPPFLAGS) \
  $(iocore_include_dirs) \
  -I$(abs_top_srcdir)/lib/records \
  -I$(abs_top_srcdir)/proxy/hdrs \
  -I$(abs_top_srcdir)/proxy/shared \
  -I$(abs_top_srcdir)/mgmt \
  -I$(abs_top_srcdir)/mgmt/api \
  -I$(abs_top_srcdir)/mgmt/api/include \
  -I$(abs_top_srcdir)/mgmt/cluster \
  -I$(abs_top_srcdir)/mgmt/utils \
  -I$(abs_top_srcdir)/lib \
  -I$(abs_top_srcdir)/lib/luajit/src

AM_LDFLAGS += \
  @OPENSSL_LDFLAGS@

traffic_manager_SOURCES = \
  AddConfigFilesHere.cc \
  MgmtHandlers.cc \
  MgmtHandlers.h \
  WebOverview.cc \
  WebOverview.h \
  metrics.h \
  traffic_manager.cc

traffic_manager_LDADD = \
  $(top_builddir)/mgmt/api/libmgmtapilocal.la \
  $(top_builddir)/mgmt/libmgmt_lm.la \
  $(top_builddir)/proxy/hdrs/libhdrs.a \
  $(top_builddir)/lib/ts/libtsutil.la \
  $(top_builddir)/iocore/eventsystem/libinkevent.a \
  $(top_builddir)/lib/records/librecords_lm.a \
  $(top_builddir)/proxy/shared/libdiagsconfig.a

AM_LDFLAGS += \
  $(LUAJIT_LDFLAGS)

traffic_manager_SOURCES += \
  metrics.cc

traffic_manager_LDADD += \
  $(top_builddir)/lib/bindings/libbindings.la \
  $(top_builddir)/lib/luajit/src/libluajit.a

traffic_manager_LDADD +=\
  $(LIBUNWIND_LIBS) \
  @LIBRESOLV@ @LIBPCRE@ @LIBTCL@ @LIBCAP@ @HWLOC_LIBS@ \
  -lm

test_metrics_SOURCES = test_metrics.cc metrics.cc WebOverview.cc
test_metrics_LDADD = \
  $(top_builddir)/mgmt/libmgmt_lm.la \
  $(top_builddir)/lib/records/librecords_lm.a \
  $(top_builddir)/lib/bindings/libbindings.la \
  $(top_builddir)/lib/luajit/src/libluajit.a \
  $(top_builddir)/lib/ts/libtsutil.la \
  $(top_builddir)/iocore/eventsystem/libinkevent.a \
  @LIBTCL@ @LIBPCRE@

# Must do it this way or the dependencies aren't detected.
if BUILD_WCCP
traffic_manager_LDADD += \
  $(top_builddir)/lib/wccp/libwccp.a \
  $(top_builddir)/lib/tsconfig/libtsconfig.la \
  @OPENSSL_LIBS@

test_metrics_LDADD += \
  $(top_builddir)/lib/wccp/libwccp.a \
  $(top_builddir)/lib/tsconfig/libtsconfig.la \
  @OPENSSL_LIBS@

endif

include $(top_srcdir)/build/tidy.mk

tidy-local: $(DIST_SOURCES)
	$(CXX_Clang_Tidy)
