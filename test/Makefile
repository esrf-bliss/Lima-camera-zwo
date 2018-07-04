############################################################################
# This file is part of LImA, a Library for Image Acquisition
#
# Copyright (C) : 2016-2018 Jens Krüger
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, see <http://www.gnu.org/licenses/>.
############################################################################

this-dir	:= $(shell pwd)
lima-dir	:= $(this-dir)/../../..
LIMA_DIR = $(lima-dir)

zwo-objs = ../src/Zwo.o

CPPFLAGS = -I ../include \
	   -I ../../../include \
	   -I ../../../common/include \
	   -I ../../../hardware/include \
	   -I ../../../control/include \
	   -I ../../../third-party/Processlib/core/include \
	   -I ../sdk/include \
	   -Wall -pthread -g
# -fPIC

PROCESSOR_TYPE := $(shell uname -m)
ifeq ($(PROCESSOR_TYPE),x86_64)
ZWO_LDFLAGS	:= -L$(LIMA_DIR)/camera/zwo/sdk/lib/x64
else ifeq ($(PROCESSOR_TYPE),armv7l)
ZWO_LDFLAGS	:= -L$(LIMA_DIR)/camera/zwo/sdk/lib/armv7
else
ZWO_LDFLAGS	:= -L$(LIMA_DIR)/camera/zwo/sdk/lib/$(PROCESSOR_TYPE)
endif

LDFLAGS = -L../../../build -llimazwo -llimacore $(ZWO_LDFLAGS) -lASICamera2

all:	test_camera

clean:
	rm -f *.o test_camera
