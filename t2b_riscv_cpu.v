// AstroTinker Bot : Task 2B : RISC-V CPU
/*
Instructions
-------------------
Students are not allowed to make any changes in the Module declaration.

This file is the top-level verilog design for RISC-V CPU Implementation

Recommended Quartus Version : 20.1
The submitted project file must be 20.1 compatible as the evaluation will be done on Quartus Prime Lite 20.1.

Warning: The error due to compatibility will not be entertained.
-------------------
*/

// t2b_riscv_cpu module declaration
module t2b_riscv_cpu (
    input clk, reset, 
	 output dummy_out
    // input Ext_MemWrite,
    // input [31:0] Ext_WriteData, Ext_DataAdr,
    // output MemWrite,
    // output [31:0] WriteData, DataAdr, ReadData
);

// wire lines from other modules
wire [31:0] PC, Instr;
wire MemWrite_rv32,MemWrite;
wire [31:0] DataAdr_rv32, WriteData_rv32;
wire [31:0] DataAdr,WriteData,ReadData;

reg [79:0] node_store;

always @(posedge clk_3M125) begin
    if (MemWrite && DataAdr == 32'h02000008)
        node_store <= {node_store[71:0], WriteData[7:0]};
end

Frequency_Scaling fs (clk, clk_3M125);

// instantiate processor and memories
riscv_cpu rvsingle (clk_3M125, reset, PC, Instr, MemWrite_rv32, DataAdr_rv32, WriteData_rv32, ReadData);
instr_mem imem (PC, Instr);
data_mem dmem (clk_3M125, MemWrite, DataAdr, WriteData, ReadData);

// output assignments
//assign MemWrite = (Ext_MemWrite && reset) ? 1 : MemWrite_rv32;
//assign WriteData = (Ext_MemWrite && reset) ? Ext_WriteData : WriteData_rv32;
//assign DataAdr = (reset) ? Ext_DataAdr : DataAdr_rv32;

assign MemWrite = MemWrite_rv32;
assign WriteData = WriteData_rv32;
assign DataAdr = DataAdr_rv32;

assign dummy_out = node_store == 0;

endmodule

