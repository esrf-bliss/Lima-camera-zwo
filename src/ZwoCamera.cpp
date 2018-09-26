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

#include <time.h>
#include <unistd.h>

#include "ZwoCamera.h"

#include "ASICamera2.h"

#include "lima/HwBufferMgr.h"
#include "lima/Constants.h"

class lima::Zwo::Camera::_AcqThread : public Thread
{
	DEB_CLASS_NAMESPC(DebModCamera, "Camera", "_AcqThread");
public:
	_AcqThread(Camera &);

protected:
	virtual void threadFunction();

private:
	Camera &m_cam;
};

::lima::Zwo::Camera::_AcqThread::_AcqThread(Camera &cam) :
	m_cam(cam)
{
	pthread_attr_setscope(&m_thread_attr, PTHREAD_SCOPE_PROCESS);
}

void ::lima::Zwo::Camera::_AcqThread::threadFunction()
{
	DEB_MEMBER_FUNCT();

	DEB_TRACE() << "Camera thread started: " << DEB_VAR1(m_cam.m_quit);
	AutoMutex aLock(m_cam.m_cond.mutex());

	while (!m_cam.m_quit)
	{
		DEB_TRACE() << "Camera thread running: " << DEB_VAR1(m_cam.m_quit);
		while(!m_cam.m_acq_started && !m_cam.m_quit)
		{
			m_cam.m_acq_thread_run = false;
			m_cam.m_cond.wait();
		}
		m_cam.m_acq_thread_run = true;
		if(m_cam.m_quit)
			break;

		bool continueAcq = true;

		StdBufferCbMgr &buffer_mgr = m_cam.m_buffer.getBuffer();
		buffer_mgr.setStartTimestamp(Timestamp::now());
		unsigned int imgSize = buffer_mgr.getFrameDim().getMemSize();

		while(continueAcq && (!m_cam.m_nb_frames || m_cam.m_acq_frame_id < (m_cam.m_nb_frames - 1)))
		{
			DEB_TRACE() << "Start camera to exposure";
			ASI_ERROR_CODE status = ASIStartExposure(m_cam.id(), ASI_FALSE); // false for dark image yes/no, see manual
			if (status == ASI_SUCCESS)
			{
				usleep(10000); // 10ms
				DEB_TRACE() << "Waiting for camera";
				ASI_EXPOSURE_STATUS expStatus(ASI_EXP_WORKING);
				while(expStatus == ASI_EXP_WORKING)
				{
					if (ASIGetExpStatus(m_cam.id(), &expStatus) != ASI_SUCCESS)
						break;
					usleep(1000);
				}
				if (expStatus == ASI_EXP_SUCCESS)
				{
					DEB_TRACE() << "Reading image from camera : " << DEB_VAR1(imgSize) << " bytes";
					unsigned char	*imgBuf = (unsigned char *)buffer_mgr.getFrameBufferPtr(m_cam.m_acq_frame_id + 1);
					if (ASIGetDataAfterExp(m_cam.id(), imgBuf, imgSize) == ASI_SUCCESS)
					{
						m_cam.m_acq_frame_id++;
						HwFrameInfoType frame_info;
						frame_info.acq_frame_nb = m_cam.m_acq_frame_id;
						continueAcq = buffer_mgr.newFrameReady(frame_info);
						DEB_TRACE() << "successful :" << DEB_VAR1(m_cam.m_acq_frame_id);
					}
				}
				else
					continueAcq = false;
			}
			else
				DEB_ERROR() << "Could not start exposure : " << m_cam.errorText(status);
		}
		m_cam.m_acq_started = false;
	}
}

lima::Zwo::Camera::Camera(int id)
	: m_imageType(ASI_IMG_END)
	, m_bin(1)
	, m_cooler(false)
	, m_quit(false)
	, m_nb_frames(1)
	, m_acq_frame_id(-1)
	, m_acq_started(false)
	, m_acq_thread_run(false)
{
	DEB_CONSTRUCTOR();
	int nCam = ASIGetNumOfConnectedCameras();
	m_asiCameraInfo.CameraID = -1;
	ASI_ERROR_CODE err = ASIOpenCamera(id);
	if (err == ASI_SUCCESS)
	{
		err = ASIGetCameraProperty(&m_asiCameraInfo, id);
		if (err == ASI_SUCCESS)
		{
			m_cooler = std::string(m_asiCameraInfo.Name).find("-Cool") != std::string::npos;
			err = ASIInitCamera(id);
			if (err == ASI_SUCCESS)
			{
				m_asiCameraInfo.CameraID = id;
				int nCtrls = 0;
				err = ASIGetNumOfControls(id, &nCtrls);
				m_controlCaps.clear();
				if (err == ASI_SUCCESS)
				{
					ASI_CONTROL_CAPS controlCaps;
					DEB_WARNING() << "USB3 : " << isUsb3Camera();
					for (int i = 0; i < nCtrls; i++)
					{
						ASIGetControlCaps(id, i, &controlCaps);
						DEB_WARNING() << controlCaps.ControlType << " :"  << controlCaps.Name << " :" << controlCaps.Description;
						DEB_WARNING() << controlCaps.MinValue << "-" << controlCaps.MaxValue << ":" << controlCaps.DefaultValue;
						if (controlCaps.ControlType == ASI_HARDWARE_BIN)
							enableHwBinning(true);
						m_controlCaps.push_back(controlCaps);
					}
					getTemperature();
					struct timespec t = {0, 70000000}; // 70 ms
					// this sleep is needed to initialize the temperature sensor in the lib
					nanosleep(&t, &t);
				}
				else
					DEB_ERROR() << "err : " << errorText(err);
				int width,
				    height;
				err = ASIGetROIFormat(id, &width, &height, &m_bin, &m_imageType);
				setImageType(Bpp16);
			}
		}
	}
	else
		DEB_ERROR() << "Camera with id " << id << " not found: " << errorText(err);

	m_acq_thread = new _AcqThread(*this);
	m_acq_thread->start();
}

lima::Zwo::Camera::~Camera()
{
	DEB_DESTRUCTOR();
	if (id() != -1)
	{
		ASI_ERROR_CODE err = ASICloseCamera(id());
		if (err != ASI_SUCCESS)
			DEB_ERROR() << "Could not close camera: " << errorText(err);
		m_asiCameraInfo.CameraID = -1;
	}
	m_quit = true;
}

double lima::Zwo::Camera::getExpTime(void)
{
	DEB_MEMBER_FUNCT();
	double exp_time(-1.0);

	if (id() != -1)
	{
		long lRet;
		ASI_BOOL bRet;
		ASI_ERROR_CODE err = ASIGetControlValue(id(), ASI_EXPOSURE, &lRet, &bRet);
		if (err == ASI_SUCCESS)
			exp_time = lRet / 1000000.;
		else
			DEB_ERROR() << "Could not get exposure time : " << errorText(err);
	}
	else
		DEB_ERROR() << "No camera available";
	DEB_RETURN() << DEB_VAR1(exp_time);
	return exp_time;
}

void lima::Zwo::Camera::setExpTime(const double exp_time)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(exp_time);
	if (id() != -1)
	{
		ASI_ERROR_CODE err = ASISetControlValue(id(), ASI_EXPOSURE, long(exp_time * 1000000), ASI_TRUE /* FALSE */);
		if (err != ASI_SUCCESS)
			DEB_ERROR() << "Could not set exposure time : " << errorText(err);
	}
	else
		DEB_ERROR() << "No camera available";
}

lima::HwInterface::StatusType lima::Zwo::Camera::getStatus(void)
{
	DEB_MEMBER_FUNCT();
	lima::HwInterface::StatusType status;
	status.set(lima::HwInterface::StatusType::Ready);
	if (id() != -1)
	{
		ASI_EXPOSURE_STATUS expStatus;
		if (ASIGetExpStatus(id(), &expStatus) == ASI_SUCCESS)
		{
			switch (expStatus)
			{
				case ASI_EXP_IDLE:
					status.set(lima::HwInterface::StatusType::Ready);
					break;
				case ASI_EXP_SUCCESS:
					status.set(lima::HwInterface::StatusType::Ready);
					break;
				case ASI_EXP_WORKING:
					status.set(lima::HwInterface::StatusType::Exposure);
					break;
				default:
					DEB_WARNING() << "Camera lib is in Fault state. Camera has to be reset.";
					status.set(lima::HwInterface::StatusType::Fault);
			}
		}
		else
			status.set(lima::HwInterface::StatusType::Fault);
	}
	else
	{
		DEB_ERROR() << "No camera available";
		status.set(lima::HwInterface::StatusType::Fault);
	}
	return status;
}

std::string lima::Zwo::Camera::getDetectorModel(void)
{
	DEB_MEMBER_FUNCT();
	DEB_RETURN() << DEB_VAR1(m_asiCameraInfo.Name);
	return m_asiCameraInfo.Name;
}

void lima::Zwo::Camera::startAcq(void)
{
	DEB_MEMBER_FUNCT();
	if (id() != -1)
	{
		AutoMutex aLock(m_cond.mutex());
		m_acq_started = true;
		m_cond.broadcast();
	}
	else
		DEB_ERROR() << "No camera available";
}

void lima::Zwo::Camera::stopAcq(void)
{
	DEB_MEMBER_FUNCT();
	if (id() != -1)
	{
		ASI_ERROR_CODE status = ASIStopExposure(id());
		if (status != ASI_SUCCESS)
			DEB_ERROR() << "Could not stop exposure : " << errorText(status);
		AutoMutex aLock(m_cond.mutex());
		m_acq_started = false;
		aLock.unlock();
	}
	else
		DEB_ERROR() << "No camera available";
}

void lima::Zwo::Camera::prepareAcq(void)
{
	DEB_MEMBER_FUNCT();
	m_acq_frame_id = -1;
}

void lima::Zwo::Camera::reset(void)
{
	DEB_MEMBER_FUNCT();
	if (id() != -1)
	{
		// This state sometimes happens but there is no way to get out of it
		// the proposed way is to take another picture
		// the sleep time between start and stop was tested from outside the
		// driver via TANGO interface maybe it could be shorter
		lima::HwInterface::StatusType status = getStatus();
		if (status.acq == AcqFault)
		{
			DEB_ALWAYS() << "Camera lib is in Fault state. Have to take a short time picture.";
			prepareAcq();
			startAcq();
			usleep(500000);
			stopAcq();
		}
	}
}

void lima::Zwo::Camera::setNbFrames(const int nb_frames)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(nb_frames);
	m_nb_frames = nb_frames;
}

int lima::Zwo::Camera::getNbFrames(void)
{
	DEB_MEMBER_FUNCT();
	DEB_RETURN() << DEB_VAR1(m_nb_frames);
	return m_nb_frames;
}

int lima::Zwo::Camera::getNbHwAcquiredFrames(void)
{
	DEB_MEMBER_FUNCT();
	int frames(m_acq_frame_id + 1);
	DEB_RETURN() << DEB_VAR1(frames);
	return frames;
}

bool lima::Zwo::Camera::checkTrigMode(TrigMode trig_mode)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(trig_mode);
	return trig_mode == IntTrig;
}

void lima::Zwo::Camera::setTrigMode(const TrigMode trig_mode)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(trig_mode);
}

lima::TrigMode lima::Zwo::Camera::getTrigMode(void)
{
	DEB_MEMBER_FUNCT();
	TrigMode trig_mode(IntTrig);
	DEB_RETURN() << DEB_VAR1(trig_mode);
	return trig_mode;
}

void lima::Zwo::Camera::setImageType(const ImageType type)
{
	DEB_MEMBER_FUNCT();
	int width(0),
	    height(0),
	    bin(0);
	ASI_IMG_TYPE imgType;

	ASI_ERROR_CODE err = ASIGetROIFormat(id(), &width, &height, &bin, &imgType);

	if (err != ASI_SUCCESS)
	{
		DEB_ERROR() << "Could not detect current image type: " << errorText(err);
	}

	if (type == Bpp8)
		imgType = m_asiCameraInfo.IsColorCam ? ASI_IMG_Y8 : ASI_IMG_RAW8;
	else if	(type != Bpp16)
		imgType = ASI_IMG_RAW16;
	else if (type != Bpp24)
		imgType = ASI_IMG_RGB24;
	else
	{
		DEB_ERROR() << "Requested image type is not useable : " << DEB_VAR1(type);
		return;
	}

	for (int i = 0; i < 8 && m_asiCameraInfo.SupportedVideoFormat[i] != -1; ++i)
		if (imgType == m_asiCameraInfo.SupportedVideoFormat[i])
		{
			err = ASISetROIFormat(id(), width, height, bin, imgType);
			if (err != ASI_SUCCESS)
				DEB_ERROR() << "Could not set image type : " << errorText(err);
			m_imageType = imgType;
			return;
		}
	DEB_ERROR() << "Requested image type is not useable : " << DEB_VAR1(type);
}

void lima::Zwo::Camera::enableHwBinning(const bool enable)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(enable);
	if (id() != -1)
	{
		ASI_ERROR_CODE err = ASISetControlValue(id(), ASI_HARDWARE_BIN, int(enable), ASI_TRUE);
		if (err != ASI_SUCCESS)
		{
			DEB_ERROR() << "Could not enable hardware binning: " << errorText(err);
		}
	}
	else
		DEB_ERROR() << "No camera available";
}

void lima::Zwo::Camera::setBin(const Bin &bin)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(bin);
	if (bin.getX() != bin.getY())
	{
		DEB_ERROR() << "Only quadratic bins allowed";
	}
	else if (id() != -1)
	{
		int width = m_asiCameraInfo.MaxWidth / bin.getX(),
		    height = m_asiCameraInfo.MaxHeight / bin.getY();
		Roi roi(0, 0, width, height);
	        checkRoi(roi, roi);
		width = roi.getSize().getWidth();
		height = roi.getSize().getHeight();
		ASI_ERROR_CODE status = ASISetROIFormat(id(), width, height, bin.getX(), m_imageType);
		if (status != ASI_SUCCESS)
		{
			DEB_ERROR() << "Could not set 'Bins': " << errorText(status);
		}
	}
	else
		DEB_ERROR() << "No camera available";
}

lima::Bin lima::Zwo::Camera::getBin(void)
{
	DEB_MEMBER_FUNCT();
	if (id() != -1)
	{
		int width, height;
		ASI_ERROR_CODE status = ASIGetROIFormat(id(), &width, &height, &m_bin, &m_imageType);
		if (status != ASI_SUCCESS)
		{
			DEB_ERROR() << "Could not get 'Bins': " << errorText(status);
		}
	}
	Bin bin(m_bin, m_bin);
	DEB_RETURN() << DEB_VAR1(bin);
	return bin;
}

void lima::Zwo::Camera::checkBin(Bin &bin)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(bin);
	if (bin.getX() != bin.getY())
	{
		DEB_ERROR() << "Only quadratic bins allowed";
	}
	else
		for (int i = 0; i < 16 && m_asiCameraInfo.SupportedBins[i]; ++i)
			if (bin.getX() == m_asiCameraInfo.SupportedBins[i])
				return;
	bin = Bin(1, 1);
}

void lima::Zwo::Camera::checkRoi(const Roi &set_roi, Roi &hw_roi)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(set_roi);
	Size s = set_roi.getSize();
	Point o = set_roi.getTopLeft();
	if (isUsb3Camera())
	{
		int	x = s.getWidth(),
			y = s.getHeight();
		x -= (x % 8);
		y -= (y % 8);
		if (x > hwSize().getWidth())
		{
			x = hwSize().getWidth();
			o.x = 0;
		}
		if (y > hwSize().getHeight())
		{
			y = hwSize().getHeight();
			o.y = 0;
		}
		s = Size(x, y);
	}
	else if ((s.getWidth() * s.getHeight()) % 1024 != 0)
	{
	}
	hw_roi = Roi(o, s);
	DEB_RETURN() << DEB_VAR1(hw_roi);
}

void lima::Zwo::Camera::setRoi(const Roi &roi)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(roi);
	if (id() != -1)
	{
		Point o = roi.getTopLeft();
		ASI_ERROR_CODE status = ASISetStartPos(id(), o.x, o.y);
		if (status == ASI_SUCCESS)
		{
			Size s = roi.getSize();
			// 0 values in width and/or height will be interpreted as maximum values of the camera
			if (!s.getWidth() || !s.getHeight())
			{
				DEB_TRACE() << "Roi size is: " << s;
				if (!s.getWidth())
					s = Point(hwSize().getWidth(), s.getHeight());
				if (!s.getHeight())
					s = Point(s.getWidth(), hwSize().getHeight());
				DEB_TRACE() << "Roi size changed to: " << s;
			}
			status = ASISetROIFormat(id(), s.getWidth(), s.getHeight(), m_bin, m_imageType);
			if (status != ASI_SUCCESS)
				DEB_ERROR() << "Could not set 'ROI width and height': " << errorText(status);
		}
		else
			DEB_ERROR() << "Could not set 'ROI x and y': " << errorText(status);
	}
	else
		DEB_ERROR() << "No camera available";
}

lima::Roi lima::Zwo::Camera::getRoi(void)
{
	Roi roi;
	DEB_MEMBER_FUNCT();
	if (id() != -1)
	{
		int width(0), height(0);

		int status = ASIGetROIFormat(id(), &width, &height, &m_bin, &m_imageType);
		if (status == ASI_SUCCESS)
		{
			int x(0), y(0);
			status = ASIGetStartPos(id(), &x, &y);
			if (status == ASI_SUCCESS)
				roi = Roi(x, y, width, height);
			else
				DEB_ERROR() << "Could not get 'Roi x and y'";
		}
		else
			DEB_ERROR() << "Could not query 'ROI width and height'";

	}
	else
		DEB_ERROR() << "No camera available";
	DEB_RETURN() << DEB_VAR1(roi);
	return roi;
}

int lima::Zwo::Camera::id(void) const
{
	// DEB_MEMBER_FUNCT();
	return m_asiCameraInfo.CameraID;
}

lima::Size lima::Zwo::Camera::hwSize(void) const
{
	DEB_MEMBER_FUNCT();
	return Size(m_asiCameraInfo.MaxWidth, m_asiCameraInfo.MaxHeight);
}

std::string lima::Zwo::Camera::errorText(const ASI_ERROR_CODE err)
{
	switch (err)
	{
		case ASI_SUCCESS:
			return "No error.";
		case ASI_ERROR_INVALID_INDEX:
			return "Invalid camera index";
		case ASI_ERROR_INVALID_ID:
			return "Camera with id is not available.";
		case ASI_ERROR_INVALID_CONTROL_TYPE:
			return "This control type is not available for this camera.";
		case ASI_ERROR_CAMERA_CLOSED:
			return "camera didn't open.";
		case ASI_ERROR_CAMERA_REMOVED:
			return "failed to find the camera, maybe the camera has been removed.";
		case ASI_ERROR_INVALID_PATH:
			return "cannot find the path of the file.";
		case ASI_ERROR_INVALID_FILEFORMAT:
			return "invalid file format.";
		case ASI_ERROR_INVALID_SIZE:
			return "wrong video format size.";
		case ASI_ERROR_INVALID_IMGTYPE:
			return "unsupported image format.";
		case ASI_ERROR_OUTOF_BOUNDARY:
			return "the startpos is out of boundary.";
		case ASI_ERROR_TIMEOUT:
			return "timeout";
		case ASI_ERROR_INVALID_SEQUENCE:
			return "stop capture first.";
		case ASI_ERROR_BUFFER_TOO_SMALL:
			return "buffer size is not big enough.";
		case ASI_ERROR_VIDEO_MODE_ACTIVE:
			return "video mode is active.";
		case ASI_ERROR_EXPOSURE_IN_PROGRESS:
			return "taking picture is active.";
		case ASI_ERROR_GENERAL_ERROR:
			return "general error, eg: value is out of valid range.";
		default:
			return "unknown error";
	}
}

bool lima::Zwo::Camera::isUsb3Camera(void) const
{
	DEB_MEMBER_FUNCT();
	return m_asiCameraInfo.IsUSB3Camera;
}

double lima::Zwo::Camera::getPixelSize(void) const
{
	DEB_MEMBER_FUNCT();
	return m_asiCameraInfo.PixelSize;
}

std::string lima::Zwo::Camera::getModel(void) const
{
	DEB_MEMBER_FUNCT();
	return "ASI (ZWO)";
}

lima::ImageType lima::Zwo::Camera::getImageType(void) const
{
	DEB_MEMBER_FUNCT();
	int width(0),
	    height(0),
	    bin(0);
	ASI_IMG_TYPE imgType(ASI_IMG_END);

	ASI_ERROR_CODE err = ASIGetROIFormat(id(), &width, &height, &bin, &imgType);

	if (err != ASI_SUCCESS)
	{
		// DEB_ERROR() << "could not detect image type : " << errorText(err);
		imgType = ASI_IMG_END;
	}
	switch (imgType)
	{
		case ASI_IMG_END:
		case ASI_IMG_RAW8:
		case ASI_IMG_Y8:
			return Bpp8;
		case ASI_IMG_RGB24:
			return Bpp32;
		case ASI_IMG_RAW16:
			return Bpp16;
	}
	return Bpp8;
}

std::list<lima::VideoMode> lima::Zwo::Camera::getSupportedVideoMode()
{
	DEB_MEMBER_FUNCT();
	std::list<lima::VideoMode> l;

	for (int i = 0; i < 8 && m_asiCameraInfo.SupportedVideoFormat[i] != ASI_IMG_END; ++i)
		switch (m_asiCameraInfo.SupportedVideoFormat[i])
		{
			case ASI_IMG_RAW8:
			case ASI_IMG_Y8:
				l.push_back(Y8);
				break;
			case ASI_IMG_RGB24:
				l.push_back(RGB24);
				break;
			case ASI_IMG_RAW16:
				l.push_back(Y16);
				break;
			default:
				DEB_ERROR() << "Unknown image type: " << m_asiCameraInfo.SupportedVideoFormat[i];
				break;
		}
	return l;
}

std::vector< ::lima::ShutterMode> lima::Zwo::Camera::getSupportedShutterMode(void)
{
	DEB_MEMBER_FUNCT();
	ShutterModeList l;
	l.push_back(ShutterMode(ShutterAutoFrame));
	return l;
}

ASI_CONTROL_CAPS lima::Zwo::Camera::getControlCap(const ASI_CONTROL_TYPE &t)
{
	DEB_MEMBER_FUNCT();
	for (std::list<ASI_CONTROL_CAPS>::iterator it = m_controlCaps.begin(); it != m_controlCaps.end(); ++it)
		if (it->ControlType == t)
			return *it;
	DEB_ERROR() << "Control type : " << t << "not found";
}

void lima::Zwo::Camera::setFlip(const ASI_FLIP_STATUS flip)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(flip);
	if (id() != -1)
	{
		ASI_ERROR_CODE err = ASISetControlValue(id(), ASI_FLIP, long(flip), ASI_FALSE);
		if (err != ASI_SUCCESS)
			DEB_ERROR() << "Could not set flip value : " << errorText(err);
	}
	else
		DEB_ERROR() << "No camera available";
}

ASI_FLIP_STATUS lima::Zwo::Camera::getFlip(void)
{
	DEB_MEMBER_FUNCT();
	if (id() != -1)
	{
		long ltemp = 0;
		ASI_BOOL bAuto = ASI_FALSE;
		int status = ASIGetControlValue(id(), ASI_FLIP, &ltemp, &bAuto);
		if (status == ASI_SUCCESS)
			return ASI_FLIP_STATUS(ltemp);
	}
	return ASI_FLIP_NONE;
}

::lima::HwBufferCtrlObj* ::lima::Zwo::Camera::getBufferCtrlObj(void)
{
	return &m_buffer;
}

void lima::Zwo::Camera::setTemperatureSP(const double &temp)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(temp);
	if (id() != -1 && hasCooler())
	{
		ASI_ERROR_CODE err = ASISetControlValue(id(), ASI_TARGET_TEMP, long(temp), ASI_FALSE);
		if (err != ASI_SUCCESS)
			DEB_ERROR() << "Could not set cooler temperature : " << errorText(err);
	}
}

double lima::Zwo::Camera::getTemperatureSP(void)
{
	DEB_MEMBER_FUNCT();
	if (id() != -1 && hasCooler())
	{
		long ltemp = 0;
		ASI_BOOL bAuto = ASI_FALSE;
		int status = ASIGetControlValue(id(), ASI_TARGET_TEMP, &ltemp, &bAuto);
		if (status == ASI_SUCCESS)
			return double(ltemp);
	}
	return -274.;
}

double lima::Zwo::Camera::getTemperature(void)
{
	DEB_MEMBER_FUNCT();
	if (id() != -1)
	{
		long ltemp = 0;
		ASI_BOOL bAuto = ASI_FALSE;
		int status = ASIGetControlValue(id(), ASI_TEMPERATURE, &ltemp, &bAuto);
		if (status == ASI_SUCCESS)
			return double(ltemp) / 10.0;
	}
	return -274.;
}

void lima::Zwo::Camera::setCooler(const bool cool)
{
	DEB_MEMBER_FUNCT();
	DEB_PARAM() << DEB_VAR1(cool);
	if (id() != -1 && hasCooler())
	{
		ASI_ERROR_CODE err = ASISetControlValue(id(), ASI_COOLER_ON, long(cool), ASI_FALSE);
		if (err != ASI_SUCCESS)
			DEB_ERROR() << "Could not set cooler value : " << errorText(err);
	}
}

bool lima::Zwo::Camera::getCooler(void)
{
	DEB_MEMBER_FUNCT();
	if (id() != -1 && hasCooler())
	{
		long ltemp = 0;
		ASI_BOOL bAuto = ASI_FALSE;
		int status = ASIGetControlValue(id(), ASI_COOLER_ON, &ltemp, &bAuto);
		if (status == ASI_SUCCESS)
			return bool(ltemp);
	}
	return false;
}

bool lima::Zwo::Camera::hasCooler(void)
{
	return m_cooler;
}
