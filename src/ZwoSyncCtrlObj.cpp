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

#include "ZwoSyncCtrlObj.h"
#include "ZwoCamera.h"


lima::Zwo::SyncCtrlObj::SyncCtrlObj(Camera &cam)
	: lima::HwSyncCtrlObj()
	, m_camera(cam)
{
	DEB_CONSTRUCTOR();
	m_auto_exposure_mode = OFF;
}

lima::Zwo::SyncCtrlObj::~SyncCtrlObj()
{
	DEB_DESTRUCTOR();
}

bool lima::Zwo::SyncCtrlObj::checkTrigMode(TrigMode trig_mode)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(trig_mode);
	return m_camera.checkTrigMode(trig_mode);
}

void lima::Zwo::SyncCtrlObj::setTrigMode(TrigMode trig_mode)
{
	DEB_MEMBER_FUNCT();
	if (checkTrigMode(trig_mode))
		m_camera.setTrigMode(trig_mode);
	else
		DEB_ERROR() << "Invalid trigger mode : " << DEB_VAR1(trig_mode);
	// THROW_HW_ERROR(InvalidValue) << "Invalid " << DEB_VAR1(trig_mode);
}

void lima::Zwo::SyncCtrlObj::getTrigMode(TrigMode &trig_mode)
{
	DEB_MEMBER_FUNCT();
	trig_mode = m_camera.getTrigMode();
}

void lima::Zwo::SyncCtrlObj::setExpTime(double exp_time)
{
	DEB_MEMBER_FUNCT();
	m_camera.setExpTime(exp_time);
}

void lima::Zwo::SyncCtrlObj::getExpTime(double &exp_time)
{
	DEB_MEMBER_FUNCT();
	exp_time = m_camera.getExpTime();
}

void lima::Zwo::SyncCtrlObj::setLatTime(double lat_time)
{
	DEB_MEMBER_FUNCT();
	// m_camera.setLatTime(lat_time);
}

void lima::Zwo::SyncCtrlObj::getLatTime(double &lat_time)
{
	DEB_MEMBER_FUNCT();
	lat_time = 0.; // m_camera.getLatTime();
}

void lima::Zwo::SyncCtrlObj::setNbHwFrames(int nb_frames)
{
	DEB_MEMBER_FUNCT();
	m_camera.setNbFrames(nb_frames);
}

void lima::Zwo::SyncCtrlObj::getNbHwFrames(int &nb_frames)
{
	DEB_MEMBER_FUNCT();
	nb_frames = m_camera.getNbFrames();
}

void lima::Zwo::SyncCtrlObj::getValidRanges(ValidRangesType &valid_ranges)
{
	DEB_MEMBER_FUNCT();

	ASI_CONTROL_CAPS cC = m_camera.getControlCap(ASI_EXPOSURE);

	double min_time = cC.MinValue * 1e-6;
	double max_time = cC.MaxValue * 1e-6;
	valid_ranges.min_exp_time = min_time;
	valid_ranges.max_exp_time = max_time;
	valid_ranges.min_lat_time = min_time;
	valid_ranges.max_lat_time = max_time;
}
