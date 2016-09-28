# G4FanControl

**G4FanControl** is a software I've developed more or less 10 years ago, when I faced overheating problems with my old PowerBook G4. 
I sold it for some years and now I decided to make it free and opensource. The code is available on [GitHub](https://github.com/andreafabrizi/G4FanControl/)

As anyone who has put their hands on this wonderful laptop can confirm, the operating temperatures quickly climb and it soon becomes 
practically impossible to touch, especially on the bottom under the display. 
What makes this situation truly crazy is that this laptop actually has two powerful FANs which are not used at all by the operating system, probably
to save battery power. 

This utility enables you to manually program the hardware fan controller of your iBook/PowerBook G4 laptop, increasing the temperature threshold and the 
fan RPM.

The G4 Powerbook and IBook cooling system is managed by the **ADT7467** controller from Analog Devices. 
The **ADT7467** chip has one integrated temperature sensor (Local Sensor) and two more remote sensors and everything can be controlled using a convenient I2C interface.

In the next graph, the Temperatures change can be observed before and after the fan was set to the maximum power! 

![G4FanControl](https://www.andreafabrizi.it/img/g4fancontrol_temp_graph.png)

## Build from source
```
$ git clone https://github.com/andreafabrizi/G4FanControl.git
$ cd G4FanControl
$ make
```

## Precompiled packages
[G4FanControl 0.6.1 with GUI](https://github.com/andreafabrizi/G4FanControl/blob/master/dist/G4FanControl_0.6.1.dmg?raw=true)

[Widget 0.1](https://www.andreafabrizi.it/files/g4fancontrol_widget_0.1.zip) (Require G4FanControl 0.6.1)

# Changelog

## Version 0.6.1
* Fixed issue with Leopard 10.5.3

## Version 0.6
* Now G4FanControl is fully compatible with Leopard!
* Retro-compatibility with Tiger
* Fixed incompatibility issues with JVM&lt;1.5
* Removed password prompt on start-up
* Auto-installation package
* Other minor fixes


## Version 0.5
* Introduced GUI


## Version 0.4
* Independent temperature limit control for each sensor
* New temperature sensors limits range: min:20 - max:100
* Fixed issues with some iBook and Powerbook models
* Added a single-line formatted output (to easy control from external applications, like gui)


## Screenshots: 

![G4FanControl](https://www.andreafabrizi.it/img/g4fancontrol_0.6_screenshot_1.png)

![G4FanControl](https://www.andreafabrizi.it/img/g4fancontrol_screenshot_2.png)

![G4FanControl](https://www.andreafabrizi.it/img/g4fancontrol_screenshot_3.png)

![G4FanControl](https://www.andreafabrizi.it/img/g4fancontrol_wdg_0.1_screenshot_1.png)

![G4FanControl](https://www.andreafabrizi.it/img/g4fancontrol_wdg_0.1_screenshot_2.png)
