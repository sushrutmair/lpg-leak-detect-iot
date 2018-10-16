## LPG Leak Detector

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

### System Overview:

Goal was to design a system as shown in the figure below:

<p align="center">
  <img width="450" height="300" src="https://raw.githubusercontent.com/sushrutmair/lpg-leak-detect-iot/master/docs/assets/sysoverview.jpg">
</p>

The sensor would be put in a compartment where the LPG cylinder resides. It needs to be as low as possible as the LPG gas itself is heavier than air and tends to collect at the bottom first, in case of any leakage. Considering safety and also the fact that this compartment is not easy to access frequently, I decided to lay a cable from the MQ6 to the Detector circuit. This also allows me enough flexibility to put the Detector where I wish to. I used extension cord cables and soldered them to the four pins of the MQ6 – A, B, H+ and H-. These cables entwined together (each with its own shielding), then lead into the Detector. I used cable ties to manage the cable bulk and avoid it spreading out and becoming snagged in something. The Detector overview is below.

<p align="center">
  <img width="450" height="300" src="https://raw.githubusercontent.com/sushrutmair/lpg-leak-detect-iot/master/docs/assets/detector.jpg">
</p>

The MQ6 cable ingress is indicated. In addition there is a voltage divider to allow the Node MCU’s analog IN to measure the voltage level change due to gas levels. Apart from that, there is a power supply that supplies 3.3V to the Node MCU and 5V to the MQ6, each separate and independent. There are 3 LEDs; green LED indicates power to the system, a second blue LED blinks every time the Detector and the client script successfully communicate (COMM) and the third red LED lights up when the detected LPG level crosses a set threshold (CRIT). The circuit was first prototyped as a loose breadboard based system and a video and image is shown below. Note that the video shows the Node MCU powered off a battery but in the final circuit, I chose to run everything off a wall wart. Also the battery isn't really connected if you look carefully. Everything was powered by a power module.


<p align="center">
  <img width="450" height="300" src="https://raw.githubusercontent.com/sushrutmair/lpg-leak-detect-iot/master/docs/assets/bb_system.jpg">
</p>

A video of the above setup:

<p align="center">
<video src="https://raw.githubusercontent.com/sushrutmair/lpg-leak-detect-iot/master/docs/assets/lpg_detect_bb_proto.mp4" poster="https://raw.githubusercontent.com/sushrutmair/lpg-leak-detect-iot/master/docs/assets/video_preload.jpg" width="450" height="300" controls preload></video>
</p>

The Node MCU is connected to an 802.11 router and is also hosting a simple HTTP based web server inside it. The code is explained later in this article. The Wi-Fi network that the Detector connects to is also connected to by a laptop. In this case, my laptop is running Win 10 and a Debian Linux Win 10 app is running on top of it. There is a python script running under Debian which periodically requests the LPG level from the Detector, over HTTP. The script is explained later in this article itself.

### Detector Code:

This code runs in the Node MCU. Full source is available [here](https://github.com/sushrutmair/lpg-leak-detect-iot/blob/master/lpg-leak-detect.ino). The code logic is as follows:


<p align="center">
  <img width="650" height="250" src="https://raw.githubusercontent.com/sushrutmair/lpg-leak-detect-iot/master/docs/assets/detector_flow.jpg">
</p>

- Setup pin modes: Here we set the required pins as OUTPUT and also HIGH as needed. We also initialize Serial communications to be able to debug during development via a serial port.
-	Set as WiFi STA and connect to WiFi: Node MCU will run in 802.11 station mode and connect to the specified router network.
-	Register MDNS name: MDNS (Mobile DNS) allows us to name IoT devices and refer to them by that name always (as against remembering cumbersome IP addresses).
-	Register callbacks: Since the Node MCU is running as a web server, we need to register callbacks to handle incoming clients. That is done here and also we finally start the web server service.
-	Loop: All of the above is setup code that runs once after power up. The loop logic simply calls for handling clients. The NodeMCU framework takes care of handling the linking of each client HTTP request to the afore-registered callback handlers.

### Python Client Script Code"

The client is pretty simple and written in python. The full source is [here](https://github.com/sushrutmair/lpg-leak-detect-iot/blob/master/lpgmon_client.py). The code logic is as follows:

<p align="center">
  <img width="650" height="250" src="https://raw.githubusercontent.com/sushrutmair/lpg-leak-detect-iot/master/docs/assets/python_flow.jpg">
</p>
