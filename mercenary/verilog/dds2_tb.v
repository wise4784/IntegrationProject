`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: KOITT
// Engineer: Sanghoon Lee
// 
// Create Date: 12/01/2018 04:31:40 PM
// Design Name: 
// Module Name: dds2_tb
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module dds2_tb;
    reg clk_50, clk_25, reset;
	
	reg [31:0] index;
	wire signed [15:0]  testbench_out;
	
	//Initialize clocks and index
	initial begin
		clk_50 = 1'b0;
		clk_25 = 1'b0;
		index  = 32'd0;
		//testbench_out = 15'd0 ;
	end
	
	always begin
		#10
		clk_50  = !clk_50;
	end
	
	always begin
		#20
		clk_25  = !clk_25;
	end
	
	initial begin
		reset  = 1'b0;
		#10 
		reset  = 1'b1;
		#30
		reset  = 1'b0;
	end
	
	always @ (posedge clk_50) begin
		index  <= index + 32'd1;
	end

    dds2 u (.clock(clk_50), 
            .reset(reset),
            .increment({18'h02000, 14'b0}), 
            .phase(8'd0),
            .sine_out(testbench_out));
	
endmodule
