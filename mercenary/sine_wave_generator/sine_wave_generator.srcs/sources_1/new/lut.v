`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: KOITT
// Engineer: Sanghoon Lee
// 
// Create Date: 06/24/2019 02:49:22 PM
// Design Name: 
// Module Name: lut
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

module lut(
    input clk,
    input      [ADDRESS_WIDTH-1:0] addr,
    output reg [DATA_WIDTH-1:0] data
);
    parameter ADDRESS_WIDTH=8,
              DATA_WIDTH=8,
              FILENAME="sin.hex";
    
    reg [DATA_WIDTH-1:0] mem [2**ADDRESS_WIDTH-1:0];
    
    initial $readmemh(FILENAME, mem);
    
    always@(posedge clk)
        data <= mem[addr];
        
endmodule