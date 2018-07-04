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

#include "ZwoBinCtrlObj.h"

lima::Zwo::BinCtrlObj::BinCtrlObj(Camera &cam)
	: lima::HwBinCtrlObj()
	, m_camera(cam)
{
	DEB_CONSTRUCTOR();
}

lima::Zwo::BinCtrlObj::~BinCtrlObj()
{
	DEB_DESTRUCTOR();
}

void lima::Zwo::BinCtrlObj::setBin(const Bin &bin)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(bin);
	m_camera.setBin(bin);
}

void lima::Zwo::BinCtrlObj::getBin(Bin &bin)
{
	DEB_MEMBER_FUNCT();
	bin = m_camera.getBin();
	DEB_RETURN() << DEB_VAR1(bin);
}

void lima::Zwo::BinCtrlObj::checkBin(Bin &bin)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(bin);
	m_camera.checkBin(bin);
}
