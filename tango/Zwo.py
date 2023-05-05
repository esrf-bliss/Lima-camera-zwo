#  -*- coding: utf-8 -*-
############################################################################
# This file is part of LImA, a Library for Image Acquisition
#
# Copyright (C) : 2016-2024
# Jens Krüger <jens.krueger@frm2.tum.de>
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
#
# file :        Zwo.py
#
# description : Python source for the ZWO cameras and its commands.
#               The class is derived from Device. It represents the
#               CORBA servant object which will be accessed from the
#               network. All commands which can be executed on the
#               ZWO device are implemented in this file.
#
# project :     TANGO Device Server
#
# copyleft :    Jens Krüger
#

"""Sources for the ZWO cameras."""

import PyTango
from Lima import Core
from Lima import Zwo as ZwoHw
from Lima.Server import AttrHelper


class Zwo(PyTango.Device_4Impl):
    """Zwo control class."""

    Core.DEB_CLASS(Core.DebModApplication, 'LimaZwo')

    def __init__(self, *args):
        """Device constructor."""
        PyTango.Device_4Impl.__init__(self, *args)
        self.__Attribute2FunctionBase = {'temperature_sp': 'TemperatureSP'}
        self.__Cooler = {'ON': True, 'OFF': False}
        self.init_device()

    def delete_device(self):
        """Device destructor."""
        pass

    @Core.DEB_MEMBER_FUNCT
    def init_device(self):
        """Device initialization."""
        self.set_state(PyTango.DevState.ON)
        self.get_device_properties(self.get_device_class())
        self.cam = ZwoHw.Camera(self.camera)
        self.model = self.cam.getModel()
        if self.cooler:
            self.cam.setCooler(self.__Cooler[self.cooler])
        if self.temperature_sp:
            self.cam.setTemperatureSP(self.temperature_sp)

    def __getattr__(self, name):
        return AttrHelper.get_attr_4u(self, name, self.cam)

    @Core.DEB_MEMBER_FUNCT
    def write_cooler(self, attr):
        """Write the cooler attribute."""
        self.cam.setCooler(self.__Cooler[attr.get_write_value()])


class ZwoClass(PyTango.DeviceClass):
    """Zwo cooler class."""

    class_property_list = {}

    device_property_list = {
        'camera': [
            PyTango.DevShort,
            'Camera number',
            [0],
        ],
        'temperature_sp': [
            PyTango.DevShort,
            'Temperature set point in Celsius',
            [-30],
        ],
        'cooler': [
            PyTango.DevString,
            'Start or stop the cooler ("ON"/"OFF")',
            ['ON'],
        ]
    }

    cmd_list = {}

    attr_list = {
        'temperature_sp': [
            [PyTango.DevShort, PyTango.SCALAR, PyTango.READ_WRITE],
            {
                'description': 'Cooler temperature setpoint in Celsius',
                'unit': 'C',
                'format': '%1d',
                'memorized': 'True',
            }],
        'temperature': [
            [PyTango.DevShort, PyTango.SCALAR, PyTango.READ],
            {
                'description': 'Cooler temperature in Celsius',
                'unit': 'C',
                'format': '%.1f',
            }],
        'cooler': [
            [PyTango.DevString, PyTango.SCALAR, PyTango.READ_WRITE],
            {
                'description': 'Start/stop the cooler, OFF or ON',
                'unit': '',
                'format': '',
                'memorized': 'True',
            }],
    }

    def __init__(self, name):
        """Constructor."""
        PyTango.DeviceClass.__init__(self, name)
        self.set_type(name)


def get_control(camera=0, **keys):
    """Get the camera control object."""
    print('\n\nStarting and configuring the Zwo camera ...')
    camera = int(camera)
    cam = ZwoHw.Camera(camera)
    control = Core.CtControl(ZwoHw.Interface(cam))
    print('\n\nZwo Camera #%d (%s:%s) is started' % (
        camera, '',  # cam.getDetectorType(),
        cam.getDetectorModel()))
    return control


def get_tango_specific_class_n_device():
    """Return the module specific classes."""
    return (ZwoClass, Zwo)
