***** GPIO *******

GIO A :	0 - 자기유지용 (입력, Active LOW)
	1 - 장치, System On
	2 - Step On-off (pwm대용) -> STEP이 IC로 안됐을 경우
	3 - Ethernet (DPS83630)_ON (저전력모드 off)
	4 - Ethernet (DPS83630)_Reset
	5 - FPGA_ON
	6 - STEP, DC 모터 ON
	7 - 10V on (DC - DC) (PWM은 따로 투입)

GIO B :	0 - Fire (Active Low)
	1 - Laser On
	2 - DC모터 방향
	3 - Step모터 방향


***** SCI *******

SCI Baud Rate : 230400
sciREG1

***** AHRSv1 ******

SCI Baud Rate : 115200
sciREG3


***** lwIP *******

Static IP : 192.168.3.77
GateAddress : 192.168.3.1
Mask : 255.255.255.0


***** UDP Send/Receive *****

hdkif.c 안에
hdkif_transmit
hdkif_output <- 이 함수에서 IRQ disable함 (protect를 위해서).


****** EMAC *******

lwIP protocol
UDP 형식

****** etPWM ******

VCLK3 : 37.5MHz

etpwmREG1 (DC 모터)
	period : 1msec (1kHz)
	duty   : 수시로 알아서 ( 엔코더 값 받아서 PID 제어)

etpwmREG2 (DC-DC Input용 TR)
	period : 0.125msec (80kHz)
	duty : 점점 증가 ( 증가 폭은 설정값에 따라 바뀜 )

etpwmREG3 (Step 모터 / 각도 조절)
	period : 1msec (1kHz) => DC모터 드라이브랑 똑같이
	duty : 상관 x (걍 50퍼)

etpwmREG4 (Servo 모터 / 총알 장전)
	period : 20msec (50Hz)
	duty : CMPA = 420 ~ 1910
	최소 증감값 : 15



****** eQEP ********

DRIVE 2
Control period : 10ms
Quadrature MODE


****** ADC ********

adcREG1

****** PIN MAP ******

etpwm1A - J10-17
etpwm2B - J10-20
etpwm3B - J10-25
etpwm4A - J10-26

eQEP2A	- J10-21
eQEP2B	- J10-22

I2C2_SCL- J10-8
I2C2_SDA- J10-9

sciREG3_Tx - J1-4 ( Max232 - 10 )
sciREG3_Rx - J1-3 ( Max232 - 9	)

ADC1_9	- J5-2

HET
