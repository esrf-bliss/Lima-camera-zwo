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

#include "ZwoFlipCtrlObj.h"
#include "ASICamera2.h"
#include "ZwoCamera.h"

lima::Zwo::FlipCtrlObj::FlipCtrlObj(Camera &cam)
	: lima::HwFlipCtrlObj()
	, m_camera(cam)
{
	DEB_CONSTRUCTOR();
}

lima::Zwo::FlipCtrlObj::~FlipCtrlObj()
{
	DEB_DESTRUCTOR();
}

void lima::Zwo::FlipCtrlObj::setFlip(const Flip &flip)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(flip);
	if (m_camera.id() == -1)
	{
		DEB_ERROR() << "No camera available";
		return;
	}
	ASI_FLIP_STATUS asi_flip(ASI_FLIP_NONE);
	if (flip.x)
	{
		if (flip.y)
			asi_flip = ASI_FLIP_BOTH;
		else
			asi_flip = ASI_FLIP_HORIZ;
	}
	else if (flip.y)
		asi_flip = ASI_FLIP_VERT;
	m_camera.setFlip(asi_flip);
}

void lima::Zwo::FlipCtrlObj::getFlip(Flip &flip)
{
	DEB_MEMBER_FUNCT();
	switch(m_camera.getFlip())
	{
		default:
		case ASI_FLIP_NONE:
			flip = Flip(false, false);
			break;
		case ASI_FLIP_HORIZ:
			flip = Flip(true, false);
			break;
		case ASI_FLIP_VERT:
			flip = Flip(false, true);
			break;
		case ASI_FLIP_BOTH:
			flip = Flip(true, true);
			break;
	}
	DEB_RETURN() << DEB_VAR1(flip);
}

void lima::Zwo::FlipCtrlObj::checkFlip(Flip &flip)
{
}
