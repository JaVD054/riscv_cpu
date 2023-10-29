module sgn_zero_extend(
    input  [31:0] read_data_mem,
    input  [ 2:0] funct3,
    output reg [31:0] ext_out
);

always @(*) begin
    case({funct3[2], funct3[0]})
        // lb
        2'b00: ext_out = {{24{read_data_mem[7]}}, read_data_mem[7:0]};
        // lh
        2'b01: ext_out = {{16{read_data_mem[15]}}, read_data_mem[15:0]};
        // lbu
        2'b10: ext_out = {24'b0, read_data_mem[7:0]};
        // lhu
        2'b11: ext_out = {16'b0, read_data_mem[15:0]};
        default: ext_out = 32'bxxxx_xxxx_xxxx_xxxx_xxxx_xxxx_xxxx_xxxx; // undefined
    endcase
end

endmodule
