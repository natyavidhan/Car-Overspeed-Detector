# Car-Overspeed-Detector
## [Car Overspeeding Detection System](https://overspeed-backend-server.studiousgamer.repl.co/) Is An Open source system. Basically its main function is to reduce overspeeding of cars by detecting its speed and reporting the overspeeding condition in the database.

### Storage
Data of every overspeeding case is stored inside a MongoDB cluster which is very safe.

### IoT
Data is sent through a NodeMCU i.e. An ESP8266 which is simple but effective.

### Server
The backend server is hosted on repl.it for now, but it will be changed to heroku after the development is complete.

### Language
The Backend server is coded in python while the system itself is coded in c++ (arduino language)

### Hardware
The Data is sended with a NodeMCU (ESP8266) and the In-Car Components i.e. the buzzer, led's and LCD screen are controlled with an Arduino UNO

## How It Works?

First of all the **IR sensor** detects the speed of the car. then it is processed in the **Arduino UNO**. If the speed is under limit, it will do nothing. But if the speed is exceeding the speed limit, it turns on the **NodeMCU**. the NodeMCU then gets the current **latitude** and **longitude** of the car with the help of a **Neo6M GPS module**. after that, it **compares** the lat. and lon. of the car with lat. and lon. of 10 **police stations** to find the police station having the **shortest distance**. after this, the latitude, the longitude, **Car number**, **current date** and **current time** is sent to the **database** under the particular police station with the help of the **backend server**.
