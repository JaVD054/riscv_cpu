
// controller.v - controller for RISC-V CPU

module controller (
    input [6:0]  op,
    input [2:0]  funct3,
    input        funct7b5,
    input        Zero, Carry,
    output       [1:0] ResultSrc,
    output       MemWrite,
    output reg   PCSrc, 
    output       ALUSrc,
    output       RegWrite, Jump, Jalr,
    output [2:0] ImmSrc,
    output [2:0] ALUControl
);

wire [1:0] ALUOp;
wire [2:0] Branch;

main_decoder    md (op,funct3, Branch, ResultSrc, MemWrite, 
                    ALUSrc, RegWrite, Jump, Jalr, ImmSrc, ALUOp);

alu_decoder     ad (op[5], funct3, funct7b5, ALUOp, ALUControl);

// for jump and branch
always @(*) begin
    case (Branch)
        3'b100: PCSrc <= Zero | Jump; // beq
        3'b101: PCSrc <= ~Zero | Jump; // bne
        3'b110: PCSrc <= Carry | Jump; // blt
        3'b111: PCSrc <= ~Carry | Zero | Jump; // bge
        default: PCSrc <= Jump; // jal, jalr
    endcase
end

endmodule
