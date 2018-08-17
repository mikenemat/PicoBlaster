# PicoBlaster
Lutron makes amazing lighting control hardware, but their artificial limitations on their Caseta line of products is horribly unfriendly to consumers. Everything speaks the same RF protocol, but they've crippled the Caseta Smart Hub to disallow any non-Caseta branded hardware.

The PicoBlaster is a hardware + software project to interface a Pico Keypad with a ESP8266 module, such that any Lutron device compatible with the Pico can be controlled over wifi.

Lutron has dozens if not hundreds of devices compatible with the Pico that are artificially forbidden from communicating with the Lutron Smart Bridge Pro. They want you to buy identical hardware with a fancy label like RadioRA2 and pay $$$$ for an installer. Lame.  Instead, buy the hardware on Ebay, pair it with a Pico remote, and interface it to a ESP8266.

Can be used to control RadioRA2 dimmers, PowPaks, and any other device compatible with the Pico. The pico back case needs to be removed for the modifications, but the front face and original functionality remain unaffected.

**Hardware**

* Pico Keypad (there are different versions - tested with SI4010 IC model with ON/OFF/DIM UP/DIM/DOWN/FAV buttons commonly sold at Home Depot)
* ESP8266 Module (Tested with Adafruit Feather Huzzah ESP8266)
* Connect ESP8266 3.3V and GND Pico battery + and - terminals (remove battery first)
* Connect GPIOs to the SI4010 chip in the Pico as follows:
* GPIO12 ESP = ON = SI4010 PIN 12/GPIO2
* GPIO14 ESP = OFF = SI4010 PIN 10/GPIO4
* GPIO13 ESP = DIM DOWN = SI4010 PIN 14/GPIO8
* GPIO15 ESP = DIM UP = SI4010 PIN 8/GPIO6
* GPIOs are pulled low to trigger buttons. Suspect we should add some resistors in series so that it is safer if the manual buttons on the Pico are used at the same time the GPIOs are pulled high by the ESP. 

**Software**

WifiManager + fauxmoESP + FauxmoESP fork + Custom Sketch. FauxmoESP 3 has some weird bugs that crash the ESP on ESP Core 2.4.0

**Working well**

ON/OFF Functionality

**Working, maybe**

Dimming - because the Pico does not know the state of the target device, it has to either turn it on fully or turn it off fully, and then simulate holding DIM UP / DIM DOWN for a length of time. This works quite well for lighting dimmers, but could be dangerous for things like Shades/Blinds. If you are **exclusively** using the PicoBlaster, remembering the state is trivial. However, I assume you are using local control and/or additional Picos so this is not enabled. Perhaps this can be implemented as an optional feature later.

Watchdog functionality - if the ESP crashes, the watchdog does not reliably resume the firmware. This is very rare but needs fixing.

**Not working**

Customizable name + dimming parameters as a part of wifimanager config

WifiManager reset button

**References**

Pico teardown: https://www.allaboutcircuits.com/news/teardown-tuesday-lutron-caseta-wireless-remote/

Pico reverse engineering: https://hackaday.io/project/2291-integrated-room-sunrise-simulator/log/7223-the-wireless-interface

Si4010 datasheet: https://www.silabs.com/documents/public/data-sheets/Si4010.pdf
