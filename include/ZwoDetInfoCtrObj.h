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

#ifndef ZWO_DET_INFO_CTR_OBJ_H
#define ZWO_DET_INFO_CTR_OBJ_H

#include "lima/HwDetInfoCtrlObj.h"
#include "lima/Debug.h"
#include "ASICamera2.h"

namespace lima {
	namespace Zwo {
		class DetInfoCtrObj;
		class Camera;
	}
}

class lima::Zwo::DetInfoCtrObj : public lima::HwDetInfoCtrlObj
{
	DEB_CLASS_NAMESPC(DebModCamera, "DetInfoCtrlObj", "Zwo");

public:
	DetInfoCtrObj(Camera &);

	virtual ~DetInfoCtrObj();

	virtual void getMaxImageSize(Size &max_image_size);

	virtual void getDetectorImageSize(Size &det_image_size);

	virtual void getDefImageType(ImageType &def_image_type);

	virtual void getCurrImageType(ImageType &curr_image_type);

	virtual void setCurrImageType(ImageType curr_image_type);

	virtual void getPixelSize(double &x_size, double &y_size);

	virtual void getDetectorType(std::string &det_type);

	virtual void getDetectorModel(std::string &det_model);

	virtual void registerMaxImageSizeCallback(HwMaxImageSizeCallback &cb);

	virtual void unregisterMaxImageSizeCallback(HwMaxImageSizeCallback &cb);

private:
	Camera		&m_camera;
};

#endif
