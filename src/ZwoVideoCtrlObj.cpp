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

#include "ZwoVideoCtrlObj.h"
#include "ZwoCamera.h"

lima::Zwo::VideoCtrlObj::VideoCtrlObj(Camera &cam)
	: lima::HwVideoCtrlObj()
	, m_camera(cam)
{
	DEB_CONSTRUCTOR();
}

lima::Zwo::VideoCtrlObj::~VideoCtrlObj()
{
	DEB_DESTRUCTOR();
}

void lima::Zwo::VideoCtrlObj::getSupportedVideoMode(std::list<VideoMode> &aList) const
{
	DEB_MEMBER_FUNCT();
	aList = m_camera.getSupportedVideoMode();
}

void lima::Zwo::VideoCtrlObj::setVideoMode(VideoMode)
{
	DEB_MEMBER_FUNCT();
}

void lima::Zwo::VideoCtrlObj::getVideoMode(VideoMode&) const
{
	DEB_MEMBER_FUNCT();
}

void lima::Zwo::VideoCtrlObj::setLive(bool)
{
	DEB_MEMBER_FUNCT();
}

void lima::Zwo::VideoCtrlObj::getLive(bool &) const
{
	DEB_MEMBER_FUNCT();
}

void lima::Zwo::VideoCtrlObj::getGain(double &) const
{
	DEB_MEMBER_FUNCT();
}

void lima::Zwo::VideoCtrlObj::setGain(double)
{
	DEB_MEMBER_FUNCT();
}

void lima::Zwo::VideoCtrlObj::checkBin(Bin &bin)
{
	DEB_MEMBER_FUNCT();
}

void lima::Zwo::VideoCtrlObj::checkRoi(const Roi &set_roi, Roi &hw_roi)
{
	DEB_MEMBER_FUNCT();
}

void lima::Zwo::VideoCtrlObj::setBin(const Bin &)
{
	DEB_MEMBER_FUNCT();
}

void lima::Zwo::VideoCtrlObj::setRoi(const Roi &)
{
	DEB_MEMBER_FUNCT();
}
