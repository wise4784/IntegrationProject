library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

library unisim;
use unisim.vcomponents.all;

entity top_m is
port	(
			MAIN_CLK		:in		std_logic;
			
			--UART      	
			UART_OUT1		: out	std_logic;
			UART_IN1		: in	std_logic;
		
			--PWM       	
			SOL_PWM			: out	std_logic_vector(35 downto 0)
			
			);
end top_m;

architecture Behavioral of top_m is


	component	pwm_wrapper
	port(
			RESET_N		:in		std_logic;	-- Master Reset
			CPU_CLOCK	:in		std_logic;
			CPU_CSEL	:in		std_logic;
			CPU_WREN	:in		std_logic;
			CPU_ADDR	:in		std_logic_vector(7 downto 0);	-- Byte Address
			CPU_WDATA	:in		std_logic_vector(31 downto 0);
			CPU_RDATA	:out	std_logic_vector(31 downto 0);
			CPU_READY	:out	std_logic;
			
			PWM_OUT		:out	std_logic_vector(35 downto 0)			
			);
	end component;

	component	pll
	port(
			CLK_IN1		:in		std_logic;
			reset		:in		std_logic;
			clk_out1	:out	std_logic;
			locked		:out	std_logic
			);
	end component;

	signal	pll_lock		: std_logic;
	signal	clk050m			: std_logic;
	signal	cpu_rst_0		: std_logic;
	signal	cpu_rst_p		: std_logic;
	signal	cpu_rst_n		: std_logic;

	component cpu_wrapper is
	generic(
				FLASH_ADDR_WIDTH	:	natural	:=	26
				);
	port(
		EPC_CLK		:in		std_logic;
		CORE_CLK	:in		std_logic;
		RST_P		:in		std_logic;
		UART_OUT	:out	std_logic;
		UART_IN		:in		std_logic;

		EPC_CSEL	: out	std_logic_vector(7 downto 0);
		EPC_WREN	: out	std_logic_vector(7 downto 0);
		EPC_ADDR00	: out	std_logic_vector(15 downto 0);
		EPC_ADDR01	: out	std_logic_vector(15 downto 0);
		EPC_ADDR02	: out	std_logic_vector(15 downto 0);
		EPC_ADDR03	: out	std_logic_vector(15 downto 0);
		EPC_ADDR04	: out	std_logic_vector(15 downto 0);
		EPC_ADDR05	: out	std_logic_vector(15 downto 0);
		EPC_ADDR06	: out	std_logic_vector(15 downto 0);
		EPC_ADDR07	: out	std_logic_vector(15 downto 0);
		EPC_WDATA00	: out	std_logic_vector(31 downto 0);
		EPC_WDATA01	: out	std_logic_vector(31 downto 0);
		EPC_WDATA02	: out	std_logic_vector(31 downto 0);
		EPC_WDATA03	: out	std_logic_vector(31 downto 0);
		EPC_WDATA04	: out	std_logic_vector(31 downto 0);
		EPC_WDATA05	: out	std_logic_vector(31 downto 0);
		EPC_WDATA06	: out	std_logic_vector(31 downto 0);
		EPC_WDATA07	: out	std_logic_vector(31 downto 0);
		EPC_RDATA00	: in	std_logic_vector(31 downto 0);
		EPC_RDATA01	: in	std_logic_vector(31 downto 0);
		EPC_RDATA02	: in	std_logic_vector(31 downto 0);
		EPC_RDATA03	: in	std_logic_vector(31 downto 0);
		EPC_RDATA04	: in	std_logic_vector(31 downto 0);
		EPC_RDATA05	: in	std_logic_vector(31 downto 0);
		EPC_RDATA06	: in	std_logic_vector(31 downto 0);
		EPC_RDATA07	: in	std_logic_vector(31 downto 0);
		EPC_READY	: in	std_logic_vector(7 downto 0)
	);
	end component;

	signal	epc_csel		: std_logic_vector(7 downto 0);
	signal	epc_wren		: std_logic_Vector(7 downto 0);
	signal	epc_addr00		: std_logic_vector(15 downto 0);
	signal	epc_addr01		: std_logic_vector(15 downto 0);
	signal	epc_addr02		: std_logic_vector(15 downto 0);
	signal	epc_addr03		: std_logic_vector(15 downto 0);
	signal	epc_addr04		: std_logic_vector(15 downto 0);
	signal	epc_addr05		: std_logic_vector(15 downto 0);
	signal	epc_addr06		: std_logic_vector(15 downto 0);
	signal	epc_addr07		: std_logic_vector(15 downto 0);
	signal	epc_wdata00		: std_logic_vector(31 downto 0);
	signal	epc_wdata01		: std_logic_vector(31 downto 0);
	signal	epc_wdata02		: std_logic_vector(31 downto 0);
	signal	epc_wdata03		: std_logic_vector(31 downto 0);
	signal	epc_wdata04		: std_logic_vector(31 downto 0);
	signal	epc_wdata05		: std_logic_vector(31 downto 0);
	signal	epc_wdata06		: std_logic_vector(31 downto 0);
	signal	epc_wdata07		: std_logic_vector(31 downto 0);
	signal	epc_rdata00		: std_logic_vector(31 downto 0);
	signal	epc_rdata01		: std_logic_vector(31 downto 0);
	signal	epc_rdata02		: std_logic_vector(31 downto 0);
	signal	epc_rdata03		: std_logic_vector(31 downto 0);
	signal	epc_rdata04		: std_logic_vector(31 downto 0);
	signal	epc_rdata05		: std_logic_vector(31 downto 0);
	signal	epc_rdata06		: std_logic_vector(31 downto 0);
	signal	epc_rdata07		: std_logic_vector(31 downto 0);
	signal	epc_ready       : std_logic_vector(7 downto 0);
	signal	acry_reset_req	: std_logic;

begin
	u0:pll port map
	(
		clk_in1		=>	MAIN_CLK		,
		reset		=>	'0'	 			,
		locked		=>	pll_lock		,
		clk_out1	=>	clk050m
	);

	u1:pwm_wrapper
	port map
	(
		RESET_N				=>	cpu_rst_n					,
		CPU_CLOCK			=>	clk050m						,
		CPU_CSEL			=>	epc_csel(0)					,
		CPU_WREN			=>	epc_wren(0)					,
		CPU_ADDR			=>	epc_addr00(7 downto 0)		,-- Byte Address
		CPU_WDATA			=>	epc_wdata00					,
		CPU_RDATA			=>	epc_rdata00					,
		CPU_READY			=>	epc_ready(0)				,		
		PWM_OUT				=>	SOL_PWM
	);

	

	pll_lock	<=	'1' when (pll_lock='1') else	'0';

	process(pll_lock,clk050m)
		variable	reset_delay	: integer range 0 to 65535;
	begin
		if (pll_lock='0') then
			cpu_rst_0	<=	'1';
			cpu_rst_p	<=	'1';
			cpu_rst_n	<=	'0';
			reset_delay	:=	0;
		elsif rising_edge(clk050m) then
			if (reset_delay=65535) then
				cpu_rst_0	<=	'0';

				if (acry_reset_req='1') then
					reset_delay	:=	0;
				end if;
			else
				reset_delay	:=	reset_delay + 1;
				if (reset_delay=10000) then
					cpu_rst_0	<=	'1';
				end if;
			end if;
			cpu_rst_p	<=	cpu_rst_0;
			cpu_rst_n	<=	not cpu_rst_0;
		end if;
	end process;
	
	
	-------------------------------------------------------------------
	-- Microblaze CPU
	-------------------------------------------------------------------
	cpu_inst : cpu_wrapper
	generic map(
		FLASH_ADDR_WIDTH	=>	26				 --: natural	:=	26
					)
	port map
	(
		EPC_CLK				=>	clk050m			,--: in		std_logic;
		CORE_CLK			=>	clk050m			,--: in		std_logic;
		RST_P				=>	cpu_rst_p		,--: in		std_logic;
		UART_OUT			=>	UART_OUT1		,--: out	std_logic;
		UART_IN				=>	UART_IN1		,--: in		std_logic;
		EPC_CSEL			=>	epc_csel		,--: out	std_logic_vector(7 downto 0);
		EPC_WREN			=>	epc_wren		,--: out	std_logic_vector(7 downto 0);
		EPC_ADDR00			=>	epc_addr00		,--: out	std_logic_vector(15 downto 0);
		EPC_ADDR01			=>	epc_addr01		,--: out	std_logic_vector(15 downto 0);
		EPC_ADDR02			=>	epc_addr02		,--: out	std_logic_vector(15 downto 0);
		EPC_ADDR03			=>	epc_addr03		,--: out	std_logic_vector(15 downto 0);
		EPC_ADDR04			=>	epc_addr04		,--: out	std_logic_vector(15 downto 0);
		EPC_ADDR05			=>	epc_addr05		,--: out	std_logic_vector(15 downto 0);
		EPC_ADDR06			=>	epc_addr06		,--: out	std_logic_vector(15 downto 0);
		EPC_ADDR07			=>	epc_addr07		,--: out	std_logic_vector(15 downto 0);
		EPC_WDATA00			=>	epc_wdata00		,--: out	std_logic_vector(31 downto 0);
		EPC_WDATA01			=>	epc_wdata01		,--: out	std_logic_vector(31 downto 0);
		EPC_WDATA02			=>	epc_wdata02		,--: out	std_logic_vector(31 downto 0);
		EPC_WDATA03			=>	epc_wdata03		,--: out	std_logic_vector(31 downto 0);
		EPC_WDATA04			=>	epc_wdata04		,--: out	std_logic_vector(31 downto 0);
		EPC_WDATA05			=>	epc_wdata05		,--: out	std_logic_vector(31 downto 0);
		EPC_WDATA06			=>	epc_wdata06		,--: out	std_logic_vector(31 downto 0);
		EPC_WDATA07			=>	epc_wdata07		,--: out	std_logic_vector(31 downto 0);
		EPC_RDATA00			=>	epc_rdata00		,--: in		std_logic_vector(31 downto 0);
		EPC_RDATA01			=>	epc_rdata01		,--: in		std_logic_vector(31 downto 0);
		EPC_RDATA02			=>	epc_rdata02		,--: in		std_logic_vector(31 downto 0);
		EPC_RDATA03			=>	epc_rdata03		,--: in		std_logic_vector(31 downto 0);
		EPC_RDATA04			=>	epc_rdata04		,--: in		std_logic_vector(31 downto 0);
		EPC_RDATA05			=>	epc_rdata05		,--: in		std_logic_vector(31 downto 0);
		EPC_RDATA06			=>	epc_rdata06		,--: in		std_logic_vector(31 downto 0);
		EPC_RDATA07			=>	epc_rdata07		,--: in		std_logic_vector(31 downto 0);
		EPC_READY			=>	epc_ready		 --: in		std_logic_vector(7 downto 0)
	);
	epc_rdata01		<=	(others=>'0');
	epc_rdata02		<=	(others=>'0');
	epc_rdata03		<=	(others=>'0');
	epc_rdata04		<=	(others=>'0');
	epc_rdata05		<=	(others=>'0');
	epc_rdata06		<=	(others=>'0');
	epc_rdata07		<=	(others=>'0');
	epc_ready(1)	<=	'0';
	epc_ready(2)	<=	'0';
	epc_ready(3)	<=	'0';
	epc_ready(4)	<=	'0';
	epc_ready(5)	<=	'0';
	epc_ready(6)	<=	'0';
	epc_ready(7)	<=	'0';
end Behavioral;
