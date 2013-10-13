HighFive - A Brainihack project
===============================

HighFive is a game when one tries to hit his opponent hands while he tries to escape.

We use Emotive Epoc to read, in real time, the "attacker" brain activity and alert the "defender" when the "attacker" is thinking about hitting him. We send a signal via to the defender via sound and vibration on an Android device.

Brainihack TLV 2013
-------------------

This project was built in just 2 days of [Brainhack] [1]

  [1]: http://brainihack.org/       "Brainihack"

The Code
--------

The code consist of:

* Classifier code (C++, matlab) - this is the algorithm the gets the live data and determine whether the "attacker" is about to hit or not. Later this code was integrated with the RealTimeInfra.

* RealTimeInfra (C++) - this bit is about reading the data (22 channels) form the Emotive head set and send it to the classifier. Then, if needed, it will send a message (via bluetooth) to the Android app.

* Android (Java) - This is the android app that open a bluetooth server socket and wait get the signal. When it does, it vibrates and make the "HI-Yha" sound

* Python code - this part was used just to stitch the RealTimeInfra code with the bluetooth socket (faster to write then c++). We also used it for some testing.

This code is now a mess! hopefully we'll sort it out, with time...

Links:
------

* Youtube: ...

Team Members:
-------------
Avi, Yonatan, Shahar, Naama, Ido, Ereli, Clara, Roy, Galit
