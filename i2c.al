
###
### Just record everything on each edge of D2
### enable pullup resistors on all D2-D7
###
# trigger posedge D2
# trigger negedge D2
# capture A0 A1 A2 A3 A4 A5
# capture D2 D3 D4 D5 D6 D7
# pullup D2 D3 D4 D5 D6 D7

###
### Capure data with free running trigger
###
label D2 "SCL"
label D3 "SDA"
capture D2 D3 D4 D5
trigger 19kHz

###
### Decode SPI Data
###
### SCK  = D2 (on positive edge)
### CS   = D3 (negated)
### MOSI = D4
### MISO = D5
###
# decode spi posedge lsb D2 !D3 D4 D5

###
### Decode I2C Data
###
### SCL = D2
### SDA = D3
###
#decode i2c D2 D3

###
### Decode JTAG Data
###
### TCK = D2
### TMS = D3
### TDI = D4
### TDO = D5
###
# decode jtag D2 D3 D4 D5

