# IoT-PUSL2021-Smart-Aquarium
-----------------------------

Developers: Sahan K. Karunaratne, Kasun Amarasinghe, Dharani Pinsarani & Jayani Prabodha.

Type: Group Project.

Year: 2022.

Imporatnt languages, technologies & equipments used:
Java, C++, Arduino Mega (2560), NodeMCU (ESP32), Temperature Sensor (DS18B20), Ultrasonic Sensor (HC-HR04), pH Sensor (E-201C), Arduino Water Pump (Roinco-R365), Arduino heater, LED Panel...

Functionality:
We all have seen aquariums. Different aquatic flora & fauna in an aquarium need different environmental conditions for a better survival. Therefore, a good caring is essential. By eliminating some major drawbacks in a typical aquarium, AquaSmart is an IoT based solution alongside a mobile application to monitor and control the environmental conditions within an aquarium preferable for the better existance of the aquatic animals and plants.

Features.

01. Temperature Control.
    Temperature of the aquarium is mesuared by a temperature sensor (DS18B20) and the heater is set to turn on automatically when the temperature is bellow the
    average and the heater turns off when the temperature reaches normal.

02. Water Level Control.
    The water level is callibrated using an ultrasonic sensor (HC-HR04) and the arduino water pump (Roinco-R365) works accordingly. Simply, when the water level of
    the aquarium is below a predefined value, water pump turns on and when the water level is normal, the water pump turns off.

03. pH Level Measure.
    Most of the marine (sea water) fish prefer to live in a salinity environment. Therefore, in an aquarium, maintaining the correct salt level is important. pH
    sensor (E-201C) is used to measure the salinity of the water. The salinity should be controlled manually according to the measurements.

04. Mobile Application.
    Mobile Application is always communicating with all the IoT peripherals in realtime. The NodeMCU (ESP32) module is used to maintain connection between the IoT
    componets and the mobile app over Wi-Fi. Therefore, the temperature, later level & the pH level can be monitored through the mobile app in realtime.

05. Fancy & Cool RGB LED Panel.
    As a fancy & cool feature to increase the aesthetic, a RGB LED panel has been introduced to this system. According to the user's willingness, the color of this
    RGB panel can be customized via the mobile application.
