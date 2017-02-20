# fintdet
Arduino physical intrusion detector and logger

## Why
I was bored and had some electronic components on my table, so I decided to build a physical intrusion detector with arduino.

## How it works
It is really simple but extremely useful:  
It keeps track of the distance changes and if it changes more than 10cm every 500ms an intrusion was detected!

Every intrusion is **logged into the EEPROM** so that a reset or lack of energy will not compromise the data.  
Sadly it is not possible to detect the time because I have not an RTC module.

## TODO
Add extra security to the intrusion clearing mechanism

## Coffee
1LCCFc71kszsP5vtRDrg7czihBJPeLQFFh
