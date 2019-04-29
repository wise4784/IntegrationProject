library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;

Library UNISIM;
use UNISIM.VCOMPONENTS.ALL;

entity pwm_wrapper is
port(
		RESET_N				:	in	std_logic;	-- Master Reset
		-- CPU(Microblaze) Signals
		CPU_CLOCK			:	in	std_logic; --pll clk 50m hz
		CPU_CSEL			:	in	std_logic;
		CPU_WREN			:	in	std_logic;
		CPU_ADDR			:	in	std_logic_vector(7 downto 0);	-- Byte Address
		CPU_WDATA			:	in	std_logic_vector(31 downto 0); --입력받은 rpm
		CPU_RDATA			:	out	std_logic_vector(31 downto 0); --PWM의 펄스
		CPU_READY			:	out	std_logic;		
		PWM_OUT				:	out	std_logic_vector(35 downto 0)	
		);
end pwm_wrapper;

architecture Behavioral of pwm_wrapper is

	type BYTE4_ARRAY_TYPE is array (0 to 5) of std_logic_vector(31 downto 0); --32비트 6 배열 정의
	signal	COUNT_CLK1	: BYTE4_ARRAY_TYPE;
	signal	COUNT_CLK2	: BYTE4_ARRAY_TYPE;	
	signal	rpm_in_0d	  : std_logic_vector(2 downto 0);
	signal	rpm_in_1d	  : std_logic_vector(2 downto 0);
	constant	BASICHZ1  : std_logic_vector(31 downto 0)	:="00000010111110101111000010000001"; --50M (1초 세는 클럭)
	--constant	BASICHZ2  : std_logic_vector(31 downto 0)	:="00000000000000000000000000010110";

component sol
port(
		sol_clk		:	in	std_logic;
		sol_reset	:	in	std_logic;
		sol_in		:	in	std_logic_vector(31 downto 0); --sdk에서 hz로 바꾼 bit값
		sol_out		:	out	std_logic_vector(2 downto 0);
		sol_wren	:	in	std_logic		
		);
end component;


	constant	ADDR_RPM_FROM_UART	:	std_logic_vector(7 downto 0)	:=	X"00";	--uart로 입력받은 rpm값이 저장된 주소	
	
	signal	rst_p_cpu		: std_logic;
	signal	reg_state		: std_logic_vector(1 downto 0);

	signal	reg_rpm_from_uart	:	std_logic_vector(31 downto 0);	--주소에 저장되는 rpm값
	signal	reg_rpm_from_uart_wren	:	std_logic;
	signal	sol_pwr_sw			: 	std_logic;	
	
	
begin


	-- Reset 버퍼
	process(RESET_N,CPU_CLOCK)
	begin
		if (RESET_N='0') then
			rst_p_cpu	<=	'1';
		elsif rising_edge(CPU_CLOCK) then
			rst_p_cpu	<=	'0';
		end if;
	end process;

	
 
	-- Register Access
	process(rst_p_cpu,CPU_CLOCK)
	begin
		if (rst_p_cpu='1') then --초기화
			reg_rpm_from_uart_wren	<=	'0';
			CPU_RDATA			<=	(others=>'0');
			CPU_READY			<=	'0' ;

		elsif rising_edge(CPU_CLOCK) then
			case reg_state is
				when "00" => --레지스터가 00일 때
					if (CPU_CSEL='1') then --칩 셀렉트 1
						if (CPU_WREN='1') then --1일때 쓰기
							------------------------------------------------------------------------------------------------
							-- Register Write
							------------------------------------------------------------------------------------------------
							case CPU_ADDR is --CPU주소가 X00 일때
								when	ADDR_RPM_FROM_UART					=>	reg_rpm_from_uart			<=	cpu_wdata;
																				reg_rpm_from_uart_wren		<=	'1';
																			
								when 	others =>	null;
							end case;

							CPU_RDATA	<=	(others=>'0');
							CPU_READY	<=	'1';
							reg_state	<=	"11";

						else --레지스터 00, cpu_wren=0일 때 읽기
							------------------------------------------------------------------------------------------------
							-- Register Read
							------------------------------------------------------------------------------------------------
							case CPU_ADDR is
								
								when 	others =>	null;
							end case;

							CPU_READY	<=	'1';
							reg_state	<=	"11";
						end if;
					else--칩셀렉트 0 일때
						CPU_RDATA	<=	(others=>'0');
						CPU_READY	<=	'0';
					end if;

				when others =>	-- 레지스터가 "11"
					if (CPU_CSEL='0') then
						CPU_RDATA	<=	(others=>'0');
						CPU_READY	<=	'0';
						reg_state	<=	"00";
					end if;
					reg_rpm_from_uart_wren	<=	'0';
			end case;
		end if;
	end process;
	
	
	

	z1:sol port map
	(
		sol_clk			=>	CPU_CLOCK			,
		sol_reset		=>	reset_n				,
		sol_in			=>	reg_rpm_from_uart	,
		sol_out			=>	PWM_OUT				,		
		sol_wren		=>	reg_rpm_from_uart_wren
	);

	

end Behavioral;

