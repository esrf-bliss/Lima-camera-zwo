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

#include <lima/Constants.h>

#include "ZwoShutterCtrlObj.h"
#include "ZwoCamera.h"

lima::Zwo::ShutterCtrlObj::ShutterCtrlObj(Camera &cam)
	: lima::HwShutterCtrlObj()
	, m_camera(cam)
	, m_open_time(0.0)
	, m_close_time(0.0)
	, m_state(false)
{
	DEB_CONSTRUCTOR();
}

lima::Zwo::ShutterCtrlObj::~ShutterCtrlObj()
{
	DEB_DESTRUCTOR();
}

bool lima::Zwo::ShutterCtrlObj::checkMode(ShutterMode shut_mode) const
{
	DEB_MEMBER_FUNCT();
	return shut_mode == ::lima::ShutterAutoFrame;
}

void lima::Zwo::ShutterCtrlObj::getModeList(ShutterModeList &mode_list) const
{
	DEB_MEMBER_FUNCT();
	mode_list = m_camera.getSupportedShutterMode();
}

void lima::Zwo::ShutterCtrlObj::setMode(ShutterMode shut_mode)
{
	DEB_MEMBER_FUNCT();
	if (!checkMode(shut_mode))
		throw LIMA_HW_EXC(InvalidValue, "Invalid Shutter mode");
}

void lima::Zwo::ShutterCtrlObj::getMode(ShutterMode &shut_mode) const
{
	DEB_MEMBER_FUNCT();
	shut_mode = ::lima::ShutterAutoFrame;
}

void lima::Zwo::ShutterCtrlObj::setState(bool shut_open)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(shut_open);
	m_state = shut_open;
}

void lima::Zwo::ShutterCtrlObj::getState(bool &shut_open) const
{
	DEB_MEMBER_FUNCT();
	shut_open = m_state;
	DEB_RETURN() << DEB_VAR1(shut_open);
}

void lima::Zwo::ShutterCtrlObj::setOpenTime(double shut_open_time)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(shut_open_time);
	if (shut_open_time >= 0)
		m_open_time = shut_open_time;
	DEB_ERROR() << "Open time must be >= 0";
}

void lima::Zwo::ShutterCtrlObj::getOpenTime(double &shut_open_time) const
{
	DEB_MEMBER_FUNCT();
	shut_open_time = m_open_time;
	DEB_RETURN() << DEB_VAR1(shut_open_time);
}

void lima::Zwo::ShutterCtrlObj::setCloseTime(double shut_close_time)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(shut_close_time);
	if (shut_close_time >= 0)
		m_close_time = shut_close_time;
	DEB_ERROR() << "Close time must be >= 0";
}

void lima::Zwo::ShutterCtrlObj::getCloseTime(double &shut_close_time) const
{
	DEB_MEMBER_FUNCT();
	shut_close_time = m_close_time;
	DEB_RETURN() << DEB_VAR1(shut_close_time);
}
