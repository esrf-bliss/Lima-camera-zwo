.. _camera-zwo:

Zwo (Zhen Wang Optical)
-----------------------

.. image:: asi178mm.png


Introduction
````````````

ZWO offers a large choice of cameras for astronomical applications. The cameras
are connected via USB. The delivered driver library is available for Linux,
Mac, and Windows.

The LIMA module has been tested with the following models on Linux::

  - ASI 120MM Mini
  - ASI 178MM-Cool
  - ASI 294MM Pro
  - ASI 2600MM Pro


Prerequisite
````````````

Installation & Module configuration
```````````````````````````````````

Follow the generic instructions in :ref:`build_installation`. If using CMake
directly, add the following flag:

.. code-block:: sh

  -DLIMACAMERA_ZWO=true

For the Tango server installation, refers to :ref:`tango_installation`.

Initialisation and Capabilities
```````````````````````````````
In order to help people to understand how the camera plugin has been implemented
in LImA this section provide some important information about the developer's
choices.

Camera initialisation
.....................

There is nothing specific.
The available cameras must be enumerated. A selected camera can then be inited.
(Note that at the moment only one camera will be handled by the plugin.)

Std capabilites
...............

This plugin has been implement in respect of the mandatory capabilites but with
some limitations according to some programmer's  choices.  We only provide here
extra information for a better understanding of the capabilities for the Zwo
camera.

* HwDetInfo
  
TODO

* HwSync

TODO  
  

Optional capabilites
....................

In addition to the standard capabilities, we make the choice to implement some
optional capabilities in order to have an improved simulation.

TODO

* BinCtrl

  TODO

* BufferCtrl

  TODO

* FlipCtrl

  TODO

* RoiCtrl

  TODO

* ShutterCtrl

  TODO

* SavingCtrl

  TODO

* VideoCtrl

  TODO

Configuration
`````````````

TODO


How to use
``````````
The LimaCCDs tango server provides a complete interface to the zwo plugin so
feel free to test.

For a quick test one can use python, is this a short code example:

.. code-block:: python

  from Lima import Zwo
  from lima import Core
  import time

  cam = Zwo.Camera(0)
  hwint = Zwo.Interface(cam)

  control = Core.CtControl(hwint)

  acq = control.acquisition()

  # setting new file parameters and autosaving mode
  saving = control.saving()

  pars = saving.getParameters()
  pars.directory = '/tmp/'
  pars.prefix = 'testsimul_'
  pars.suffix = '.edf'
  pars.fileFormat = Core.CtSaving.EDF
  pars.savingMode = Core.CtSaving.AutoFrame
  saving.setParameters(pars)

  # now ask for 2 sec. exposure and 10 frames
  acq.setAcqExpoTime(2)
  acq.setNbImages(10) 
  
  control.prepareAcq()
  control.startAcq()

  # wait for last image (#9) ready
  lastimg = control.getStatus().ImageCounters.LastImageReady
  while lastimg != 9:
      time.sleep(0.1)
      lastimg = control.getStatus().ImageCounters.LastImageReady
 
  # read the first image
  im0 = control.ReadImage(0)
