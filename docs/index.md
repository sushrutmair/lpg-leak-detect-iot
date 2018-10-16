## LPG Leak detector

There are already lots of projects built on top of the MQ family of sensors and also specifically the MQ6. In my case, the goal here was multifold:
-	Work with a new sensor for me (MQ 6)
-	Understand web hosting support inside a Node MCU
-	Be able to create a named IoT device connected to the home network
-	Try out the Debian app on Windows 10 which would run the client script talking to the named IoT device

This article comprises of the following sections:
-	The MQ6 sensor
-	System overview
-	Detector code
-	Script code
-	Results
-	Key learnings

### About the MQ6 sensor:
I did not buy this separately but I had one fixed to a commercial household LPG leak detector. That system was old and wasn’t working since quite some time. I had already salvaged parts of it for other things (battery charging circuit). I decided to desolder the MQ 6 and use it my own project.  However, I wasn’t able to successfully desolder it so in the end, I just cut out the part of the PCB hosting the sensor. The pins were hardly visible but I managed to solder multicore wires to it and via a heatshrink assembly was able to reasonably protect those delicate connections.
The MQ 6 does not have any polarity to its pins so you can decide which is which. Of course you need to respect the A, B and H pin identity but you can decide which is H+ & H-. [This](https://dmohankumar.wordpress.com/2012/05/04/familiarize-electronic-components-part-xviii-gas-sensors/) explains the working principles of all MQ series sensors. The sensor needs some settling time for its heater – first time use is 24 H but subsequent uses are a few minutes. The heater also has a good current draw (~150 mA); you must power it from an external power source and not from your microcontroller. I have not gone into details of connecting the sensor as many people have done that and it is available via a web search easily. Note that practically you would need to convert the analog OUT readings of the sensor to ppm (parts per million) concentration of the gas (I have not incorporated that in my code). My salvaged MQ6 is shown below resting on a steel bowl.

<p align="center">
  <img width="450" height="300" src="https://raw.githubusercontent.com/sushrutmair/lpg-leak-detect-iot/master/docs/assets/mq6.jpg">
</p>

