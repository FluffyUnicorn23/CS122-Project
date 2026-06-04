module lcd
(
    input  rst,
    input  pclk,        
    output reg LCD_DE,
    output reg [4:0] LCD_B,
    output reg [5:0] LCD_G,
    output reg [4:0] LCD_R,      
    input logic [7:0] data,
    input logic memCLK
);

localparam X_TOTAL = 525;
localparam Y_TOTAL = 285;
localparam X_Active = 480;
localparam Y_Active = 272;

reg [9:0] x = 0;
reg [9:0] y = 0;
logic[480:0][7:0] mem; 
logic [8:0] i = 0;
logic loopdone = 0;

always @(posedge pclk) begin
    if (x < X_TOTAL && y < Y_TOTAL) begin
        if(x < X_Active && y < Y_Active) begin
            LCD_DE <= 1;
            
            if(x > 2 && x < 13) begin
                if( ((y % 45) == 18) && y != 243 ) begin
                    LCD_B <= 15;
                    LCD_G <= 0;
                    LCD_R <= 0;
                end
                else if((y % 45) == 10 && y != 100 && y != 235 ) begin
                    LCD_B <= 15;
                    LCD_G <= 0;
                    LCD_R <= 0;
                end
                else if((y % 45) == 26 && y != 116 && y!= 251) begin
                    LCD_B <= 15;
                    LCD_G <= 0;
                    LCD_R <= 0;
                end
                else begin
                    LCD_B <= 15;
                    LCD_G <= 31;
                    LCD_R <= 15;
                end
            end
            else if(x == 1) begin
                if((y % 45) < 18 && (y % 45) > 10 && y < 118) begin
                    LCD_B <= 15;
                    LCD_G <= 0;
                    LCD_R <= 0;
                end
                else if((y > 18 && y < 26) || (y > 198 && y < 206)) begin
                    LCD_B <= 15;
                    LCD_G <= 0;
                    LCD_R <= 0;
                end
                else begin
                    LCD_B <= 15;
                    LCD_G <= 31;
                    LCD_R <= 15;
                end
            end
            else if (x == 14) begin
                if((y % 45) < 18 && (y % 45) > 10 && y > 73) begin
                    LCD_B <= 15;
                    LCD_G <= 0;
                    LCD_R <= 0;
                end
                else if((y % 45) < 26 && (y%45) > 18 && (y < 170 || y > 216)) begin
                    LCD_B <= 15;
                    LCD_G <= 0;
                    LCD_R <= 0;
                end
                else begin
                    LCD_B <= 15;
                    LCD_G <= 31;
                    LCD_R <= 15;
                end
            end
            else if( x < 20 && x > 15) begin
                LCD_B <= 15;
                LCD_G <= 0;
                LCD_R <= 0;
            end
            else if(loopdone) begin
                LCD_B <= 15;
                LCD_G <= 0;
                LCD_R <= 0;
                loopdone <= 0;
            end
            else begin
                LCD_B <= 15;
                LCD_G <= 31;
                LCD_R <= 15;
            end
            
            for(i = 21; i < 480; ++i) begin
                if(i == x && mem[i] == y) begin
                    loopdone <= 1;
                end
            end

        end
        else begin
            LCD_DE <= 0;
            LCD_R <= 0;
            LCD_G <= 0;
            LCD_B <= 0;
        end
    end 
    else begin
        LCD_DE <= 0;
        LCD_R <= 0;
        LCD_G <= 0;
        LCD_B <= 0;
    end
end

always @(posedge pclk or posedge rst) begin
    if (rst) begin
        x <= 0;
        y <= 0;
    end 
    else begin
        if (x == X_TOTAL - 1) begin
            x <= 0;
            if (y == Y_TOTAL - 1) begin
                y <= 0;
            end
            else begin
                y <= y + 1;
            end
        end 
        else begin
            x <= x + 1;
        end
    end
end

always @(posedge memCLK) begin
    mem <= {mem[479:0], data};
end

endmodule