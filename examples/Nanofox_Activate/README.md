# Tutorial for nanofox IoT KiT activation.

![](https://i.postimg.cc/NjzpBQ3r/kit-editeduppp.png)

This page presents a step by step procedure to activate Nanofox IoT Kit, as well as send and monitor the incomming of first message over the Sigfox Network.  

Recommendations:
- Arduino IDE Installed on the computer. https://www.arduino.cc/en/Guide/HomePage

## Step 1: Getting Device ID and PAC of the WISOL Modem

The default firmware loaded with Nanofox initializes the WISOL modem, and prints out the DEVICE ID and PAC codes every 20 seconds. PAC and DEVICE ID will be required in the next step for device activation. 
Follow this steps to get Device ID and PAC:

1.  Connect Nanofox kit in the PC via USB port. The kit should be recognized as a serial device. (If you-re using windows, you can check the device manager. Note: COM port number will vary depending on your computer)

![](https://i.postimg.cc/nrDmJJtp/image.png)  
![](https://i.postimg.cc/66MHYVM0/image.png) 

2.  Open a serial terminal and set communication to 9600 baud, 8 data bits, no parity, one stop bit. (Hint: You can use Arduino IDE Serial Monitor) 
3.  You should see every 20 seconds the DEVICE ID and PAC printed out in your serial terminal. You can click in the Arduino Nano Reset button and watch the initialization procedure.

![](https://i.postimg.cc/7ZhbS4f9/image.png) 

4.  Take note of DEVICE ID and PAC.


## Step 2: Kit activation in Sigfox Backend

Note: In case your kit was already activated by the manufacturer, you can skip this step and jump to step 3.

Follow the Step-by-step guide:

1.  Browse to https://buy.sigfox.com
2.  Click on Activate my devkit
3.  Pick your country
4.  Click on Next
5.  Enter Device ID (HEX) and PAC and click on Next
6.  Enter account details and click on Subscribe
7.  Your subscription is active!

## Step 3: Sigfox Backend Login

Before sending the first message, we will login in the Sigfox Backend and make sure our device has been registered, and that we are in the correct screen to monitor the incoming data.
Follow this steps:

1.  Browse to http://backend.sigfox.com and login with your credentials.
2.  Click in DEVICE on top of the screen. 
  Now you should see a list of your devices, and you should be able to identify your Nanofox kit by the DEVICE ID
3.  Click in the ID code of the Nanofox Kit
4.  Click in the MESSAGE on the left menu
  Now you should see a list of the latest messages received from your device. You are ready now to send the first message using the Nanofox kit.

![](https://i.postimg.cc/jSPmgwwS/image.png)  
  
  ## Step 4: Sending the first Sigfox uplink message

Now we will send the first message through the Sigfox network and check if it has reached backend database. 
Follow this steps:

1.  Make sure your Nanofox kit is still powered up. (If you did not removed it from USB port you should be good to go!) 
2.  In the Nanofox Board, click in the user button and monitor the serial console. You should see some messages indicating the button has been pressed and an uplink message is underway. 

![](https://i.postimg.cc/854jhzBy/image.png) 
![](https://i.postimg.cc/cJmsz5gC/image.png)  

3.  Now monitor the Sigfox Backend. In a few seconds a message should pop up in the list.

![](https://i.postimg.cc/FHvznSWP/image.png) 

Congratulations! You are now read to start experimenting with Nanofox IoT kit!
Brownse to https://github.com/Gridya/nanofox , install our library and get started.
  

  



