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

#ifndef ZWO_ROI_CTRL_OBJ_H
#define ZWO_ROI_CTRL_OBJ_H

#include "lima/HwRoiCtrlObj.h"
#include "lima/Debug.h"

namespace lima {
	namespace Zwo {
		class RoiCtrlObj;
		class Camera;
	}
}

class lima::Zwo::RoiCtrlObj : public lima::HwRoiCtrlObj
{
	DEB_CLASS_NAMESPC(DebModCamera, "RoiCtrlObj", "Zwo");

public:
	RoiCtrlObj(Camera &cam);

	virtual ~RoiCtrlObj();

	void checkRoi(const Roi &set_roi, Roi &hw_roi);

	void setRoi(const Roi &set_roi);

	void getRoi(Roi &hw_roi);

private:
	Camera	&m_camera;
};

#endif
