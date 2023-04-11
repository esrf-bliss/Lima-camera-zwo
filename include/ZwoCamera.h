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

#ifndef ZWO_CAMERA_H
#define ZWO_CAMERA_H

#include <list>

#include "lima/SizeUtils.h"
#include "lima/HwInterface.h"
#include "lima/HwBufferMgr.h"
#include "lima/Debug.h"
#include "lima/HwInterface.h" // Only to provide the CapList type
#include "lima/Constants.h"

#include "ASICamera2.h"

namespace lima {
	class HwBufferCtrlObj;
	namespace Zwo {
		class Camera;
	}
}

class lima::Zwo::Camera
{
	DEB_CLASS_NAMESPC(DebModCamera, "Camera", "Zwo");

public:
	Camera(int id);

	~Camera();

	double getExpTime(void);

	void setExpTime(const double exp_time);

	lima::HwInterface::StatusType getStatus(void);

	std::string getDetectorModel(void);

	void startAcq(void);

	void stopAcq(void);

	void prepareAcq(void);

	void reset(void);

	void setNbFrames(const int nb_frames);

	int getNbFrames(void);

	int getNbHwAcquiredFrames(void);

	void setTrigMode(const TrigMode trig_mode);

	TrigMode getTrigMode(void);

	void setImageType(const ImageType type);

	void setBin(const Bin &bin);

	Bin getBin(void);

	void checkBin(Bin &bin);

	void enableHwBinning(const bool);

	void checkRoi(const Roi &set_roi, Roi &hw_roi);

	void setRoi(const Roi &roi);

	Roi getRoi(void);

	std::string errorText(const ASI_ERROR_CODE);

	bool isUsb3Camera(void) const;

	double getPixelSize(void) const;

	std::string getModel(void) const;

	ImageType getImageType(void) const;

	int id(void) const;

	Size hwSize(void) const;

	std::list< ::lima::VideoMode> getSupportedVideoMode(void);

	ShutterModeList getSupportedShutterMode(void);

	bool checkTrigMode(TrigMode trig_mode);

	ASI_CONTROL_CAPS getControlCap(const ASI_CONTROL_TYPE &);

	void setTemperatureSP(const double &temp);

	double getTemperatureSP(void);

	double getTemperature(void);

	void setCooler(const bool flag);

	bool getCooler(void);

	void setFlip(const ASI_FLIP_STATUS);

	ASI_FLIP_STATUS getFlip(void);

	HwBufferCtrlObj *getBufferCtrlObj();

private:
	class _AcqThread;

	friend class _AcqThread;

	bool hasCooler(void);

private:
	ASI_IMG_TYPE			m_imageType;

	ASI_CAMERA_INFO 		m_asiCameraInfo;

	int				m_bin;

	std::list<ASI_CONTROL_CAPS>	m_controlCaps;

	bool				m_cooler;

	Cond				m_cond;

	_AcqThread			*m_acq_thread;

	bool				m_quit;

	int 				m_nb_frames;

	int 				m_acq_frame_id;

	bool 				m_acq_started;

	bool				m_acq_thread_run;

	SoftBufferCtrlObj		m_buffer;
};

#endif
