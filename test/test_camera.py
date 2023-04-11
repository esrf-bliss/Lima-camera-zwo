############################################################################
# This file is part of LImA, a Library for Image Acquisition
#
# Copyright (C) : 2016-2023 Jens Krüger
#
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, see <http://www.gnu.org/licenses/>.
############################################################################
import time
import sip

from Lima import Core, Zwo, Simulator


def print_camera(cam):
    # print dir(cam)
    print 'Camera has ID %d' % cam.id()
    print 'Camera has hardware size: %r' % cam.hwSize()
    print 'Camera vendor %s' % cam.getModel()
    print 'Camera model %s' % cam.getDetectorModel()

    print 'Camera is USB3 type model: %s' % cam.isUsb3Camera()
    print 'Camera has a pixel size of : %s um' % cam.getPixelSize()
    print 'Camera has a current roi of : %r' % cam.getRoi()
    print 'Camera is in status : %r' % cam.getStatus()
    print 'Camera exposure time is set to: %f s' % cam.getExpTime()
    print 'Camera will be triggered by: %r' % cam.getTrigMode()
    print 'Camera has taken %d Frames' % cam.getNbFrames()

    print 'Capability list'
    camif = Zwo.Interface(cam)
    caplist = camif.getCapList()
    print len(caplist)
    for cap in caplist:
        print sip.isdeleted(cap)
        # sip.dump(cap)
        # print cap.getCtrlObj()
        # print cap.getType()
        # print dir(cap)
    print ''


def take_picture(cam):
    camif = Zwo.Interface(cam)
    ctl = Core.CtControl(camif)
    expTime = 0.1
    acq = ctl.acquisition()
    print 'Set exposure time to: %f' % expTime
    acq.setAcqExpoTime(expTime)
    acq.setAcqNbFrames(1)
    # ctl.image().setRoi(roi);
    print 'Exposure time is : %f' % cam.getExpTime()
    print 'Camera has a current roi of : %r' % cam.getRoi()
    print 'Prepare acquisition'
    ctl.prepareAcq()
    print 'Camera has a current roi of : %r' % cam.getRoi()
    print 'Take a picture', time.time()
    ctl.startAcq()
    print 'started'
    stat = ctl.getStatus()
    print time.time(), stat
    while stat.AcquisitionStatus == 1:  # Core.AcqRunning:
        time.sleep(0.1)
        # print time.time()
        print ctl.getStatus()


cam = Zwo.Camera(0)
print_camera(cam)
take_picture(cam)

stat = cam.getStatus()
# print stat, stat.det, stat.acq
# print stat.det_mask
while stat.acq == 1:
    print time.time(), stat.det, stat.acq
    stat = cam.getStatus()
    time.sleep(0.1)
#   print stat
#   if stat.det = Idle
#       break
print time.time()
print 'Exposure time is : %f' % cam.getExpTime()

scam = Simulator.Camera()
scamif = Simulator.Interface(scam)
#
for cap in scamif.getCapList():
     # print cap.getCtrlObj()
     print sip.isdeleted(cap)
     # sip.dump(cap)
     if not sip.isdeleted(cap):
         print cap.getType()
#    else:
#        print dir(cap)
