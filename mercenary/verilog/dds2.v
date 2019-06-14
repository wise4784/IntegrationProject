`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: KOITT
// Engineer: Sanghoon Lee
// 
// Create Date: 12/01/2018 04:30:13 PM
// Design Name: 
// Module Name: dds2
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


module dds2 (clock, reset, increment, phase, sine_out);
    input clock, reset;
    input [31:0] increment ;
    input [7:0]  phase;
    output wire signed [15:0] sine_out;
    reg [31:0]	accumulator;

    always@(posedge clock) begin
	   if (reset) 
	       accumulator <= 0;
	   else
	       accumulator <= accumulator + increment  ;
    end
    
    sine_rom sine_table(clock, accumulator[31:24]+phase, sine_out);
endmodule
