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

#include "ZwoRoiCtrlObj.h"

#include "ASICamera2.h"

lima::Zwo::RoiCtrlObj::RoiCtrlObj(Camera &cam)
	: lima::HwRoiCtrlObj()
	, m_camera(cam)
{
	DEB_CONSTRUCTOR();
}

lima::Zwo::RoiCtrlObj::~RoiCtrlObj()
{
	DEB_DESTRUCTOR();
}

void lima::Zwo::RoiCtrlObj::checkRoi(const Roi &set_roi, Roi &hw_roi)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(set_roi);
	m_camera.checkRoi(set_roi, hw_roi);
	DEB_RETURN() << DEB_VAR1(hw_roi);
}

void lima::Zwo::RoiCtrlObj::setRoi(const Roi &set_roi)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(set_roi);
	m_camera.setRoi(set_roi);
}

void lima::Zwo::RoiCtrlObj::getRoi(Roi &hw_roi)
{
	DEB_MEMBER_FUNCT();
	hw_roi = m_camera.getRoi();
	DEB_RETURN() << DEB_VAR1(hw_roi);
}
