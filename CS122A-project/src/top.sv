`include "src/lcd.sv"
`include "src/SPI.sv"

module top (
    /** Input Ports */
    /** Output Ports */

    input  CLK, //FPGA's clock,
    input SPIclk, //SPI clock,
    input MOSI,
    input CS,
	output LCD_CLK,//LCD clock. 
	output LCD_DEN,
	output [4:0] LCD_R,
	output [5:0] LCD_G,
	output [4:0] LCD_B

);

/** Logic */
logic rst = 0;
assign LCD_CLK = CLK; 
logic [7:0] data;
logic UPData;

lcd top_lcd(
    .rst(rst),
    .pclk(CLK),
    .LCD_DE(LCD_DEN), 
    .LCD_R(LCD_R[4:0]),  // 5-bit red color data
    .LCD_G(LCD_G[5:0]), // 6-bit green color data
    .LCD_B(LCD_B[4:0]), // 5-bit blue color data
    .data(data),
    .memCLK(UPData)
);

SPI top_SPI(
    .clk(SPIclk),
    .MOSI(MOSI),
    .CS(CS),
    .data(data),
    .UPData(UPData)
);


endmodule