//###########################################################################
// This file is part of LImA, a Library for Image Acquisition
//
// Copyright (C) : 2016-2024, Jens Krueger
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

void lima::Zwo::VideoCtrlObj::setLive(bool live)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(live);
	m_camera.setLive(live);
}

void lima::Zwo::VideoCtrlObj::getLive(bool &live) const
{
	DEB_MEMBER_FUNCT();
	live = m_camera.getLive();
	DEB_RETURN() << DEB_VAR1(live);
}

void lima::Zwo::VideoCtrlObj::getGain(double &gain) const
{
	DEB_MEMBER_FUNCT();
	gain = m_camera.getGain();
	DEB_RETURN() << DEB_VAR1(gain);
}

void lima::Zwo::VideoCtrlObj::setGain(double gain)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(gain);
	m_camera.setGain(gain);
}

void lima::Zwo::VideoCtrlObj::checkBin(Bin &bin)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(bin);
	m_camera.checkBin(bin);
}

void lima::Zwo::VideoCtrlObj::checkRoi(const Roi &set_roi, Roi &hw_roi)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(set_roi);
	m_camera.checkRoi(set_roi, hw_roi);
	DEB_RETURN() << DEB_VAR1(hw_roi);
}

void lima::Zwo::VideoCtrlObj::setBin(const Bin &bin)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(bin);
	m_camera.setBin(bin);
}

void lima::Zwo::VideoCtrlObj::setRoi(const Roi &set_roi)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(set_roi);
	m_camera.setRoi(set_roi);
}
