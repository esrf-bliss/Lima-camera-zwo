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

#include "ZwoDetInfoCtrlObj.h"
#include "ZwoCamera.h"


lima::Zwo::DetInfoCtrObj::DetInfoCtrObj(Camera &cam)
	: lima::HwDetInfoCtrlObj()
	, m_camera(cam)
{
	DEB_CONSTRUCTOR();
}

lima::Zwo::DetInfoCtrObj::~DetInfoCtrObj()
{
	DEB_DESTRUCTOR();
}

void lima::Zwo::DetInfoCtrObj::getMaxImageSize(Size &max_image_size)
{
	DEB_MEMBER_FUNCT();
	max_image_size = m_camera.hwSize();
	DEB_RETURN() << DEB_VAR1(max_image_size);
}

void lima::Zwo::DetInfoCtrObj::getDetectorImageSize(Size &det_image_size)
{
	DEB_MEMBER_FUNCT();
	det_image_size = m_camera.hwSize();
	DEB_RETURN() << DEB_VAR1(det_image_size);
}

void lima::Zwo::DetInfoCtrObj::getDefImageType(ImageType &def_image_type)
{
	DEB_MEMBER_FUNCT();
	def_image_type = m_camera.getImageType();
	DEB_RETURN() << DEB_VAR1(def_image_type);
}

void lima::Zwo::DetInfoCtrObj::getCurrImageType(ImageType &curr_image_type)
{
	DEB_MEMBER_FUNCT();
	curr_image_type = m_camera.getImageType();
	DEB_RETURN() << DEB_VAR1(curr_image_type);
}

void lima::Zwo::DetInfoCtrObj::setCurrImageType(ImageType curr_image_type)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(curr_image_type);
	m_camera.setImageType(curr_image_type);
}

void lima::Zwo::DetInfoCtrObj::getPixelSize(double &x_size, double &y_size)
{
	DEB_MEMBER_FUNCT();
	x_size = y_size = m_camera.getPixelSize();
	DEB_RETURN() << DEB_VAR2(x_size, y_size);
}

void lima::Zwo::DetInfoCtrObj::getDetectorType(std::string &det_type)
{
	DEB_MEMBER_FUNCT();
	det_type = m_camera.getModel();
	DEB_RETURN() << DEB_VAR1(det_type);
}

void lima::Zwo::DetInfoCtrObj::getDetectorModel(std::string &det_model)
{
	DEB_MEMBER_FUNCT();
	det_model = m_camera.getDetectorModel();
	DEB_RETURN() << DEB_VAR1(det_model);
}

void lima::Zwo::DetInfoCtrObj::registerMaxImageSizeCallback(HwMaxImageSizeCallback &cb)
{
	DEB_MEMBER_FUNCT();
}

void lima::Zwo::DetInfoCtrObj::unregisterMaxImageSizeCallback(HwMaxImageSizeCallback &cb)
{
	DEB_MEMBER_FUNCT();
}
