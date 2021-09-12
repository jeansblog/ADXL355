# ADXL355 for Jetson TX2
Python and C code to read data from an ADXL355 accelerometer

NVIDIA jetson  install

sudo apt-get install libi2c-dev i2c-tools <BR>
sudo apt-get install i2c-tools python3-smbus

sudo i2cdetect -y -r 1  

### Python
  $ /usr/bin/python3 adxl355example.py  
  
### C code
  $ cd cp_jetson  
  $ make  
  $ ./loop  

### Jetson TX2 and ADXL355
Jetson TX2 J21
https://www.jetsonhacks.com/nvidia-jetson-tx2-j21-header-pinout/

J21
| Pin | Label  | Pin | Label    |
|:---:|:-----:|:---:|:-------:|
| 1   |3.3VDC | 2   |~~5.0 VDC~~|
| 3   |SDA1   | 4   |~~5.0 VDC~~|
| 5   |SCL1   | 6   |GND      |
| 7   |~~GPIO_GCLK~~| 8   |~~TXD0~~|
| 9   |~~GND~~| 10  |~~RXDO~~ |
| 11  |~~GPIO_GEN0~~| 12  |~~GPIO_GEN1~~|
| 13  |~~GPIO_GEN2~~| 14  |~~GND~~|
| 15  |~~GPIO_GEN3~~| 16  |~~GPIO_GEN4~~|
| 17  |3.3VDC | 18  |GPIO_GEN5|

EVAL-ADXL355Z
https://www.analog.com/media/en/technical-documentation/user-guides/EVAL-ADXL354-355-UG-1030.pdf

P1
| Pin | Label | Pin | Label |
|:---:|:----:|:---:|:----:|
| 1   |VDDIO | 2   |~~INT1~~  |
| 3   |VDD   | 4   |~~INT2~~  |
| 5   |GND   | 5   |~~DRDY~~  |

P2
| Pin | Label   | Pin | Label     |
|:---:|:------:|:---:|:--------:|
| 1   |~~V1P8ANA~~ | 2   |CS/SCL    |
| 3   |~~V1P8DIG~~ | 4   |SCLK/VSSIO|
| 5   |MISO/SDA| 6   |MOSI/SDA  |


I2C Address  
MISO/SDA    1(High) ->  0b101 0011(0x53)  or  0(Low) -> 0b001 1101(0x1D) 

I2C needs setting  
SCLK/VSSIO    0(Low)   

VDDIO = VDD = 3.3VDC

https://strawberry-linux.com/pub/adxl355-manual.pdf

