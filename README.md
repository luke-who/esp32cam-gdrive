# Power Efficient Smart Water Meter Measuring with ESP32-CAM, Google Apps Script and Google Drive

<p align="center">
    <a href="assets/esp32-cam.drawio.svg">
        <img src="assets/esp32-cam.drawio.svg" width = 700/ height=auto>
    </a>
</p>

This project automates the utility measuring unit(water meter in this case) and record-keeping process with ESP32-CAM. This is achieved by taking an image of the water meter, then uploading the image directly from ESP32-CAM to Google Drive with the desired frequency.

# Getting Started
1. Make sure the camera is correctly connected to the ESP32 board.
3. Log into your Google account, create a project on your Google App Script home page, and upload the code from [upload.gs](/upload.gs) to the new project.
4. Deploy the project and copy the Deployment ID
5. Paste the Deployment ID into [MyCamera.h](/MyCamera.h)
6. Modify the WiFi SSID and Password in [esp32cam-gdrive.ino](/esp32cam-gdrive.ino) to enable WiFi connectivity for the board.
7. Adjust the sleep timer and wake-up frequency accordingly in [esp32cam-gdrive.ino](/esp32cam-gdrive.ino) & [sleep_time.h](/sleep_time.h). The default is the 10th of every month around mid-day.
8. Connect your ESP32-CAM to a computer via a USB cable.
9. Upload all the sketch/code in the current folder except upload.gs to the board via Arduino IDE.
10. Push the reset button on the ESP32-CAM if necessary to see some output for debugging.
11. The final step is to hook up a stand-alone power source (battery) and power management module and install them in your local cabinet where the water meter is located. This step will vary depending on the type of building, accessibility of the meter and possibly weather concerns which might affect the circuit. It is recommended to encapsulate the board and power source to prevent unwanted interference or degradation from the outside environment.

# Sleep Mode
There are currently five [sleep modes](https://lastminuteengineers.com/esp32-sleep-modes-power-consumption/) on the ESP32 to preserve power when it's not in use. Different onboard modules will be disabled depending on which sleep mode it is set to. The sleep mode for this project is deep sleep. 

In deep sleep mode, the chip consumes anywhere between 0.15 mA (when the ULP coprocessor is on) and 10 µA according to ESP32 Series Datasheet. It is worth noting the actual current consumption is higher than the specs in the datasheet. However, with an optimised board such as [FireBeetle 2 ESP32-E IoT Microcontroller](https://www.dfrobot.com/product-2195.html) it could consume as low as around 10 µA.

# Reference
1. Inspired by [gsampallo/esp32cam-gdrive](https://github.com/gsampallo/esp32cam-gdrive)
2. [ESP32 Series Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf
