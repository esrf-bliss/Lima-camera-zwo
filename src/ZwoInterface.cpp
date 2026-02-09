//###########################################################################
// This file is part of LImA, a Library for Image Acquisition
//
// Copyright (C) : 2016-2023, Jens Krueger
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

#include "ZwoInterface.h"

lima::Zwo::Interface::Interface(Camera &cam)
	: lima::HwInterface()
	, m_cam(cam)
	, m_bin(cam)
	// , m_buffer(/* cam */)
	, m_det_info(cam)
	, m_flip(cam)
	, m_roi(cam)
	, m_saving(cam)
	, m_shutter(cam)
	, m_sync(cam)
	, m_video(cam)
{
	DEB_CONSTRUCTOR();

	if (m_cam.hasHwBinning())
		m_cap_list.push_back(HwCap(&m_bin));
	m_cap_list.push_back(HwCap(&m_det_info));
	m_cap_list.push_back(HwCap(&m_flip));
	m_cap_list.push_back(HwCap(&m_roi));
	m_cap_list.push_back(HwCap(&m_sync));

	// m_cap_list.push_back(HwCap(&m_buffer));
	m_cap_list.push_back(HwCap(cam.getBufferCtrlObj()));
	// m_cap_list.push_back(HwCap(&m_saving));
	// m_cap_list.push_back(HwCap(&m_video));
	m_cap_list.push_back(HwCap(&m_shutter));
}

lima::Zwo::Interface::~Interface()
{
	DEB_DESTRUCTOR();
}

void lima::Zwo::Interface::getCapList(CapList &caplist) const
{
	DEB_MEMBER_FUNCT();
	caplist = m_cap_list;
}

void lima::Zwo::Interface::reset(ResetLevel)
{
	DEB_MEMBER_FUNCT();
	m_cam.reset();
}

void lima::Zwo::Interface::prepareAcq(void)
{
	DEB_MEMBER_FUNCT();
	m_cam.prepareAcq();
}

void lima::Zwo::Interface::startAcq(void)
{
	DEB_MEMBER_FUNCT();
	m_cam.startAcq();
}

void lima::Zwo::Interface::stopAcq(void)
{
	DEB_MEMBER_FUNCT();
	m_cam.stopAcq();
}

void lima::Zwo::Interface::getStatus(StatusType& status)
{
	DEB_MEMBER_FUNCT();
	status = m_cam.getStatus();
	DEB_RETURN() << DEB_VAR1(status);
}

lima::Zwo::Camera& lima::Zwo::Interface::getCamera(void) const
{
	DEB_MEMBER_FUNCT();
	return m_cam;
}

int lima::Zwo::Interface::getNbHwAcquiredFrames(void)
{
	DEB_MEMBER_FUNCT();
	int frames = m_cam.getNbHwAcquiredFrames();
	DEB_RETURN() << DEB_VAR1(frames);
	return frames;
}
