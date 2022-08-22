ZWO Tango device
================

This is the reference documentation of the ASI ZWO Tango device.

Properties
----------
=============== ========== ============== =================================
Property name	Mandatory  Default value  Description
=============== ========== ============== =================================
camera          No	   0              Camera number
temperature_sp  No 	   -30            Temparature set poing in Celsius
cooler          No 	   ON             Start or stop the cooler
=============== ========== ============== =================================


Attributes
----------
=============== === ========== ======================================
Attribute name	RW  Type       Description
=============== === ========== ======================================
temperature_sp  rw  DevShort   Cooler temperature setpoint in Celsius
temperature     ro  DevShort   Cooler temperature in Celsius
cooler          rw  DevString  Start/Stop the cooler:
                                - ON
                                - OFF
=============== === ========== ======================================

Commands
--------

============= ======== ========= ============
Command name  Arg. in  Arg. out	 Description
============= ======== ========= ============
============= ======== ========= ============
