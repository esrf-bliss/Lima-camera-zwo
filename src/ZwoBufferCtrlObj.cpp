//###########################################################################
// This file is part of LImA, a Library for Image Acquisition
//
// Copyright (C) : 2016-2018, Jens Krueger
//
// This is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//###########################################################################
//

#include "ZwoBufferCtrlObj.h"

lima::Zwo::BufferCtrlObj::BufferCtrlObj(Camera &)
	: lima::HwBufferCtrlObj()
{
	DEB_CONSTRUCTOR();
}

lima::Zwo::BufferCtrlObj::~BufferCtrlObj()
{
	DEB_DESTRUCTOR();
}

void lima::Zwo::BufferCtrlObj::setFrameDim(const FrameDim &frame_dim)
{
}

void lima::Zwo::BufferCtrlObj::getFrameDim(FrameDim &frame_dim)
{
}

void lima::Zwo::BufferCtrlObj::setNbBuffers(int nb_buffers)
{
}

void lima::Zwo::BufferCtrlObj::getNbBuffers(int &nb_buffers)
{
}

void lima::Zwo::BufferCtrlObj::setNbConcatFrames(int nb_concat_frames)
{
}

void lima::Zwo::BufferCtrlObj::getNbConcatFrames(int &nb_concat_frames)
{
}

void lima::Zwo::BufferCtrlObj::getMaxNbBuffers(int &max_nb_buffers)
{
}

void *lima::Zwo::BufferCtrlObj::getBufferPtr(int buffer_nb, int concat_frame_nb)
{
}

void *lima::Zwo::BufferCtrlObj::getFramePtr(int acq_frame_nb)
{
}

void lima::Zwo::BufferCtrlObj::getStartTimestamp(Timestamp &start_ts)
{
}

void lima::Zwo::BufferCtrlObj::getFrameInfo(int acq_frame_nb, HwFrameInfoType &info)
{
}

void lima::Zwo::BufferCtrlObj::registerFrameCallback(HwFrameCallback &frame_cb)
{
}

void lima::Zwo::BufferCtrlObj::unregisterFrameCallback(HwFrameCallback &frame_cb)
{
}
