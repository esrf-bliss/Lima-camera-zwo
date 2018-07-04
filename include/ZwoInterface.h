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

#ifndef ZWO_INTERFACE_H
#define ZWO_INTERFACE_H

#include "lima/HwInterface.h"
#include "lima/Debug.h"
#include "ZwoCamera.h"
#include "ZwoBinCtrlObj.h"
#include "ZwoBufferCtrlObj.h"
#include "ZwoDetInfoCtrObj.h"
#include "ZwoFlipCtrlObj.h"
#include "ZwoRoiCtrlObj.h"
#include "ZwoSavingCtrlObj.h"
#include "ZwoShutterCtrlObj.h"
#include "ZwoSyncCtrlObj.h"
#include "ZwoVideoCtrlObj.h"

namespace lima {
	namespace Zwo {
		class Interface;
		class Camera;
	}
}

class lima::Zwo::Interface : public lima::HwInterface
{
	DEB_CLASS_NAMESPC(DebModCamera, "Interface", "Zwo");

public:
	Interface(Camera &);

	virtual ~Interface();

	virtual void getCapList(CapList &) const;

	virtual void reset(ResetLevel reset_level);

	virtual void prepareAcq(void);

	virtual void startAcq(void);

	virtual void stopAcq(void);

	virtual void getStatus(StatusType& status);

	virtual int getNbHwAcquiredFrames();

	virtual Camera& getCamera(void) const;

private:
	Camera			&m_cam;

	HwInterface::CapList	m_cap_list;

	BinCtrlObj		m_bin;

	// BufferCtrlObj	m_buffer;

	DetInfoCtrObj		m_det_info;

	FlipCtrlObj		m_flip;

	RoiCtrlObj		m_roi;

	SavingCtrlObj		m_saving;

	ShutterCtrlObj		m_shutter;

	SyncCtrlObj		m_sync;

	VideoCtrlObj		m_video;
};

#endif
