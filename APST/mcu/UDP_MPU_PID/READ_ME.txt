***** SCI *******

SCI Baud Rate : 230400
sciREG1



***** lwIP *******

Static IP : 192.168.3.77
GateAddress : 192.168.3.1
Mask : 255.255.255.0



***** UDP Send/Receive *****

hdkif.c 안에
hdkif_transmit
hdkif_output <- 이 함수에서 IRQ disable함 (protect를 위해서).


****** EMAC *******


****** etPWM ******

VCLK3 : 37.5MHz
etpwmREG1
	period : 1msec (1kHz)
	duty   : 수시로 알아서 ( 엔코더 값 받아서 PID 제어)


****** eQEP ********

Control period : 10ms
Quadrature MODE


****** PIN MAP ******

etpwm1A - J4-7
eQEP1A	- J1-7
eQEP1B	- J5-8
I2C2_SCL- J10-8
I2C2_SDA- J10-9
