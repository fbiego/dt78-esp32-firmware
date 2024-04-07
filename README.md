# dt78-esp32-firmware
 Firmware for the DT78 watch based on ESP32 microcontroller. This reuses the DT78 watch components except the mother board which has been redesigned with ESP32.
 
 + [`dt78 research`](https://github.com/fbiego/dt78)

#### Videos

+ [`LCD Touch Test video`](https://youtu.be/1j1_iY0G4Cs)
+ [`PCB Assembly`](https://www.youtube.com/watch?v=O0CNbZo6Kx4)
+ [`Watch Screens`](https://www.youtube.com/watch?v=tZKffwjmkNU)
+ [`Watchfaces`](https://youtu.be/2PwVdIeqmRQ)

### PCB
![3](dt78_pcb.jpeg?raw=true "3")
![4](schematic.png?raw=true "4")

+ [`PCB Design files`](https://oshwlab.com/fbiego.fb/dt78)


#### UI

The UI has been taken from [`esp32-c3-mini`](https://github.com/fbiego/esp32-c3-mini) projects with a few tweaks

![screens](c3-screens.png?raw=true "screens")

#### Watchfaces

| | | |
| -- | -- | -- |
| !["Analog"](src/faces/75_2_dial/watchface.png?raw=true "75_2_dial") | !["Shadow"](src/faces/34_2_dial/watchface.png?raw=true "34_2_dial") | !["Blue"](src/faces/79_2_dial/watchface.png?raw=true "79_2_dial") |
| !["Radar"](src/faces/radar/watchface.png?raw=true "radar") | !["Outline"](src/faces/116_2_dial/watchface.png?raw=true "116_2_dial") | !["Red"](src/faces/756_2_dial/watchface.png?raw=true "756_2_dial") |
| !["Tix"](src/faces/tix_resized/watchface.png?raw=true "tix_resized") | !["Pixel"](src/faces/pixel_resized/watchface.png?raw=true "pixel_resized") | !["Smart"](src/faces/smart_resized/watchface.png?raw=true "smart_resized") |
| !["Kenya"](src/faces/kenya/watchface.png?raw=true "kenya") | !["B & W"](src/faces/b_w_resized/watchface.png?raw=true "b_w_resized") | !["WFB"](src/faces/wfb_resized/watchface.png?raw=true "wfb_resized") |

Check out [`esp32-lvgl-watchface`](https://github.com/fbiego/esp32-lvgl-watchface) project to see how watchfaces are converted from binary to LVGL code. You can add more watchfaces but you will be limited by the ESP32 flash size. 
In that case you can only compile your favorite watchfaces. Links to the pre-built binary watchfaces are included.


## Screens
 - Time (Time, Date, Day, Weather[Icon, Temp]) + Custom Watchfaces
 - Weather (City,Icon, Temp, Update time) (1 week forecast [Day, Icon, Temp])
 - Notifications (Icon, Time, Text) (List [Icon, Text] - 10 notifications)
 - Settings (Brightness, Timeout, Battery, About)
 - Control (Music Control, Find Phone, Bluetooth State)

## Chronos App
Click to download
[<img src="chronos.png?raw=true" width=100 align=left>](https://fbiego.com/chronos/app?id=c3-mini)
<br><br><br><br>

### App functions
- Sync time to device
- Send notifications to device
- Send weather info to device

