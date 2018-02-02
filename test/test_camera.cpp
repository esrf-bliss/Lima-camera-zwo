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

#include <iostream>
#include <vector>
#include <lima/CtControl.h>
#include <lima/CtAcquisition.h>

#include "ZwoCamera.h"
#include "ZwoInterface.h"

void print_camera(::lima::Zwo::Camera &cam)
{
	std::cout << "Camera has ID " << cam.id() << std::endl;
	std::cout << "Camera has hardware size: " << cam.hwSize() << std::endl;
	std::cout << "Camera vendor " << cam.getModel() << std::endl;
	std::cout << "Camera model " << cam.getDetectorModel() << std::endl;

	std::cout << "Camera is USB3 type model: " << cam.isUsb3Camera() << std::endl;
	std::cout << "Camera has a pixel size of : " << cam.getPixelSize() << " um" << std::endl;
	std::cout << "Camera has a current roi of : " << cam.getRoi() << std::endl;
	std::cout << "Camera is in status : " << cam.getStatus() << std::endl;
	std::cout << "Camera exposure time is set to: " << cam.getExpTime() << " s" << std::endl;
	std::cout << "Camera will be triggered by: " << cam.getTrigMode() << std::endl;
	std::cout << "Camera has taken " << cam.getNbFrames() << " Frames" << std::endl;

	::lima::Zwo::Interface camif(cam);
	std::vector< ::lima::HwCap> caplist;
	camif.getCapList(caplist);
	std::cout << caplist.size() << std::endl;
	std::cout << "Capability list" << std::endl;
	for (std::vector< ::lima::HwCap>::iterator it = caplist.begin(); it != caplist.end(); ++it)
	{
		std::cout << it->getType() <<
		//	it->getCtrlObj() <<
		std::endl;
	}
	std::cout << std::endl;
}

void take_picture(::lima::Zwo::Camera &cam)
{
	::lima::Zwo::Interface camif(cam);
	::lima::CtControl ctl(&camif);
	::lima::CtAcquisition *acq = ctl.acquisition();
	double expTime(0.1);

	std::cout << "Set exposure time to: " << expTime << std::endl;
	acq->setAcqExpoTime(expTime);
	acq->setAcqNbFrames(1);
	std::cout << "Exposure time is : " << cam.getExpTime() << std::endl;
	std::cout << "Camera has a current roi of : " << cam.getRoi() << std::endl;
	std::cout << "Prepare acquisition" << std::endl;
	ctl.prepareAcq();
	// ctl.image().setRoi(roi);
	std::cout << "Camera has a current roi of : " << cam.getRoi() << std::endl;
	std::cout << "Take a picture " << std::endl; // , time.time()
	ctl.startAcq();
	std::cout << "started" << std::endl;
}

int main(int, char **)
{
	::lima::Zwo::Camera cam(0);
	print_camera(cam);
	take_picture(cam);
}

// from Lima import Core, Zwo, Simulator

// def take_picture(cam):
//


// stat = cam.getStatus()
// # print stat, stat.det, stat.acq
// # print stat.det_mask
// while stat.det == 8:
//     print time.time(), stat.det, stat.acq
//     stat = cam.getStatus()
//     time.sleep(0.1)
// #   print stat
// #   if stat.det = Idle
// #       break
// print time.time()
// print 'Exposure time is : %f' % cam.getExpTime()

// scam = Simulator.Camera()
// scamif = Simulator.Interface(scam)
// #
// for cap in scamif.getCapList():
//      # print cap.getCtrlObj()
//      print sip.isdeleted(cap)
//      sip.dump(cap)
//      if not sip.isdeleted(cap):
//          print cap.getType()
// #    else:
// #        print dir(cap)
