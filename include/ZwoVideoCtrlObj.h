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

#ifndef ZWO_VIDEO_CTRL_OBJ_H
#define ZWO_VIDEO_CTRL_OBJ_H

#include "lima/HwVideoCtrlObj.h"
#include "lima/Debug.h"
#include "ZwoCamera.h"

namespace lima {
	namespace Zwo {
		class VideoCtrlObj;
	}
}

class lima::Zwo::VideoCtrlObj : public lima::HwVideoCtrlObj
{
	DEB_CLASS_NAMESPC(DebModCamera, "VideoCtrlObj", "Zwo");

public:
	VideoCtrlObj(Camera &);

	virtual ~VideoCtrlObj();

	virtual void getSupportedVideoMode(std::list<VideoMode> &aList) const;

	virtual void setVideoMode(VideoMode);

	virtual void getVideoMode(VideoMode&) const;

	virtual void setLive(bool);

	virtual void getLive(bool &) const;

	virtual void getGain(double &) const;

	virtual void setGain(double);

	virtual void checkBin(Bin &bin);

	virtual void checkRoi(const Roi &set_roi, Roi &hw_roi);

	virtual void setBin(const Bin &);

	virtual void setRoi(const Roi &);

private:
	Camera	&m_camera;
};

#endif
