----------------------------------------------------
--희망하는 Duty를 입력하면 UART가 받아서 PULSE 시간으로 보냄
----------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_unsigned.all;

entity sol is
port(
		sol_clk			:	in	std_logic; --PLL의 CLK
		sol_reset		:	in	std_logic; --PLL의 RESET
		sol_in			:	in	std_logic_vector(31 downto 0); --UART에서 입력받아 sdk연산 후 WDARA에 저장된 RPM
		sol_out			:	out	std_logic_vector(35 downto 0);  --PULSE		
		sol_wren		:	in	std_logic
		);
end sol;

architecture Behavioral of sol is
	signal	PULSE_TEM	:std_logic_vector(31 downto 0)	:=(others=>'0'); --PULSe 임의 저장
	signal	COUNT_CLK	:std_logic_vector(31 downto 0)	:=(others=>'0'); --CLK셀때 사용
	constant BASICHZ	:std_logic_vector(31 downto 0)	:= X"0000_2710"; --5KHz가 기준 X"0007_A120"; 100HZ
	
	attribute keep : string;	
	attribute keep of PULSE_TEM			: signal is "true";

begin

	rpm_sped_proc : process(sol_clk)
	begin
		if (sol_reset='0') then
			PULSE_TEM	<=	(others=>'0');
			COUNT_CLK	<=	(others=>'0');
		elsif rising_edge(fan_clk) then
			if (sol_wren='1') then					--pwm입력 값이 들어오면
				PULSE_TEM	<=	(others=>'0');
				COUNT_CLK	<=	(others=>'0');
			else
				COUNT_CLK	<=	COUNT_CLK + '1';
				if (COUNT_CLK>=sol_in) then
					PULSE_TEM	<=	(others=>'0');
					if(COUNT_CLK= BASICHZ) then
						COUNT_CLK	<=	(others=>'0');

					end if;
				elsif(COUNT_CLK< sol_in) then
					PULSE_TEM 	<=	(others=>'1');				
				end if;			
			end if;
			sol_out(0)	<=	 not PULSE_TEM(0); --pulse임의 값 out으로 넘김
			sol_out(1)	<=	 not PULSE_TEM(1); --pulse임의 값 out으로 넘김
			sol_out(2)	<=	 not PULSE_TEM(2); --pulse임의 값 out으로 넘김
		end if;
	end process;	

end Behavioral;
