module SPI (
    output logic UPData,
    input logic clk,
    input logic MOSI,
    input logic CS,
    output logic[7:0] data
);


/** Logic */
logic[7:0] tmp = 0;
logic[4:0] cnt = 0;

always @(posedge clk) begin
    if(CS == 1'b0) begin
        
        tmp <= {tmp[6:0], MOSI};
        cnt <= (cnt + 1) % 8;
        
        if(cnt == 7) begin
            data <= {tmp[6:0], MOSI};
            UPData <= 1;
        end
        else begin
            UPData <= 0;
        end
    end
    else begin
        cnt <= 0;
        tmp <= 0;
        UPData <= 0;
    end

end

endmodule
