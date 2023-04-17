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

namespace Zwo {

class Camera
{
%TypeHeaderCode
#include <ZwoCamera.h>
#include <ASICamera2.h>
%End
public:
	Camera(int id /In/);

	~Camera();

	double getExpTime();

	void setExpTime(const double exp_time /In/);

	HwInterface::StatusType getStatus();

	std::string getDetectorModel();

	void startAcq();

	void stopAcq();

	void prepareAcq();

	void reset();

	void setNbFrames(const int nb_frames /In/);

	int getNbFrames();

	void setTrigMode(const TrigMode trig_mode /In/);

	TrigMode getTrigMode();

	void setImageType(const ImageType type /In/);

	ImageType getImageType();

	void setBin(const Bin &bin /In/);

	Bin getBin();

	void checkBin(Bin &bin);

	void checkRoi(const Roi &set_roi /In/, Roi &hw_roi /Out/);

	void setRoi(const Roi &roi /In/);

	Roi getRoi();

	// const std::vector<HwCap> getCapList() const;

	// std::string errorText(const ASI_ERROR_CODE);

	bool isUsb3Camera() const;

	double getPixelSize() const;

	std::string getModel() const;

	int id() const;

	Size hwSize();

	std::list<VideoMode> getSupportedVideoMode();

	void setTemperatureSP(const double &temp /In/);

	double getTemperatureSP();

	double getTemperature();

	void setCooler(const bool flag);

	bool getCooler();
	
private:
	Camera(const Zwo::Camera&);
};

};
