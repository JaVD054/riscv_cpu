
// main_decoder.v - logic for main decoder

module main_decoder (
    input  [6:0] op,
    input [2:0]  funct3, 
    output [2:0] Branch,
    output [1:0] ResultSrc,
    output       MemWrite , ALUSrc,
    output       RegWrite, Jump, Jalr,
    output [2:0] ImmSrc,
    output [1:0] ALUOp
);

reg [14:0] controls;


always @(*) begin
    case (op)
        // RegWrite_ImmSrc_ALUSrc_MemWrite_ResultSrc_Branch_ALUOp_Jump_jalr
        7'b0000011: controls = 15'b1_000_1_0_01_000_00_0_0; // lw
        7'b0100011: controls = 15'b0_001_1_1_00_000_00_0_0; // sw
        7'b0110011: controls = 15'b1_0xx_0_0_00_000_10_0_0; // R–type
        7'b1100011: begin
            case(funct3)
                3'b000: controls = 15'b0_010_0_0_00_100_01_0_0; // beq
                3'b001: controls = 15'b0_010_0_0_00_101_01_0_0; // bne
                3'b100: controls = 15'b0_010_0_0_00_110_01_0_0; // blt
                3'b101: controls = 15'b0_010_0_0_00_111_01_0_0; // bge
            endcase
        end  // beq
        7'b0010011: controls = 15'b1_000_1_0_00_000_10_0_0; // I–type ALU
        7'b1101111: controls = 15'b1_011_0_0_10_000_00_1_0; // jal
        7'b1100111: controls = 15'b1_000_1_0_10_000_00_1_1; // jalr
        7'b0110111: controls = 15'b1_100_1_0_00_000_00_0_0; // lui
        7'b0010111: controls = 15'b1_100_1_0_11_000_00_0_0; // auipc
        default:    controls = 15'bx_0xx_x_x_xx_0xx_xx_x_x; // ???
    endcase
end



assign {RegWrite, ImmSrc, ALUSrc, MemWrite, ResultSrc, Branch, ALUOp, Jump, Jalr} = controls;

endmodule
