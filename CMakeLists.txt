###########################################################################
# This file is part of LImA, a Library for Image Acquisition
#
#  Copyright (C) : 2016-2018 Jens Krüger
#
#  This is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 3 of the License, or
#  (at your option) any later version.
#
#  This software is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, see <http://www.gnu.org/licenses/>.
############################################################################
set(NAME "zwo")

if(UNIX)
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--add-needed")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pthread -fPIC -g")
endif()


set(${NAME}_srcs src/ZwoBinCtrlObj.cpp src/ZwoDetInfoCtrObj.cpp  src/ZwoRoiCtrlObj.cpp
	src/ZwoSyncCtrlObj.cpp src/ZwoBufferCtrlObj.cpp  src/ZwoFlipCtrlObj.cpp
	src/ZwoSavingCtrlObj.cpp   src/ZwoVideoCtrlObj.cpp src/ZwoCamera.cpp
	src/ZwoInterface.cpp  src/ZwoShutterCtrlObj.cpp)

add_library(lima${NAME} SHARED ${${NAME}_srcs})

#INCLUDES
target_include_directories(lima${NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(lima${NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/sdk/include")

#LIBRARY
find_library(LIB_ZWO ASICamera2 "${CMAKE_CURRENT_SOURCE_DIR}/sdk/lib/")
if (("${LIB_ZWO}" STREQUAL "LIB_ZWO-NOTFOUND"))
	message(FATAL_ERROR "${NAME} : libASICamera2 not found")
endif()

target_link_libraries(lima${NAME} limacore)
target_link_libraries(lima${NAME} ${LIB_ZWO})

limatools_set_library_soversion(lima${NAME} "VERSION")

limatools_set_install_libdir(lima${NAME})

if(LIMA_ENABLE_PYTHON)
  limatools_run_sip_for_camera(${NAME})
  install(FILES python/__init__.py DESTINATION "${PYTHON_SITE_PACKAGES_DIR}/Lima/Zwo")
endif()

if(LIMA_ENABLE_TESTS)
  add_subdirectory(test)
endif()
