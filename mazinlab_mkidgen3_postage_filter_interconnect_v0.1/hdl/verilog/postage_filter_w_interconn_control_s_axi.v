// ==============================================================
// Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.1 (64-bit)
// Tool Version Limit: 2022.04
// Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
// ==============================================================
`timescale 1ns/1ps
module postage_filter_w_interconn_control_s_axi
#(parameter
    C_S_AXI_ADDR_WIDTH = 7,
    C_S_AXI_DATA_WIDTH = 32
)(
    input  wire                          ACLK,
    input  wire                          ARESET,
    input  wire                          ACLK_EN,
    input  wire [C_S_AXI_ADDR_WIDTH-1:0] AWADDR,
    input  wire                          AWVALID,
    output wire                          AWREADY,
    input  wire [C_S_AXI_DATA_WIDTH-1:0] WDATA,
    input  wire [C_S_AXI_DATA_WIDTH/8-1:0] WSTRB,
    input  wire                          WVALID,
    output wire                          WREADY,
    output wire [1:0]                    BRESP,
    output wire                          BVALID,
    input  wire                          BREADY,
    input  wire [C_S_AXI_ADDR_WIDTH-1:0] ARADDR,
    input  wire                          ARVALID,
    output wire                          ARREADY,
    output wire [C_S_AXI_DATA_WIDTH-1:0] RDATA,
    output wire [1:0]                    RRESP,
    output wire                          RVALID,
    input  wire                          RREADY,
    output wire [11:0]                   monitor_0,
    output wire [11:0]                   monitor_1,
    output wire [11:0]                   monitor_2,
    output wire [11:0]                   monitor_3,
    output wire [11:0]                   monitor_4,
    output wire [11:0]                   monitor_5,
    output wire [11:0]                   monitor_6,
    output wire [11:0]                   monitor_7
);
//------------------------Address Info-------------------
// 0x00 : reserved
// 0x04 : reserved
// 0x08 : reserved
// 0x0c : reserved
// 0x10 : Data signal of monitor_0
//        bit 11~0 - monitor_0[11:0] (Read/Write)
//        others   - reserved
// 0x14 : reserved
// 0x18 : Data signal of monitor_1
//        bit 11~0 - monitor_1[11:0] (Read/Write)
//        others   - reserved
// 0x1c : reserved
// 0x20 : Data signal of monitor_2
//        bit 11~0 - monitor_2[11:0] (Read/Write)
//        others   - reserved
// 0x24 : reserved
// 0x28 : Data signal of monitor_3
//        bit 11~0 - monitor_3[11:0] (Read/Write)
//        others   - reserved
// 0x2c : reserved
// 0x30 : Data signal of monitor_4
//        bit 11~0 - monitor_4[11:0] (Read/Write)
//        others   - reserved
// 0x34 : reserved
// 0x38 : Data signal of monitor_5
//        bit 11~0 - monitor_5[11:0] (Read/Write)
//        others   - reserved
// 0x3c : reserved
// 0x40 : Data signal of monitor_6
//        bit 11~0 - monitor_6[11:0] (Read/Write)
//        others   - reserved
// 0x44 : reserved
// 0x48 : Data signal of monitor_7
//        bit 11~0 - monitor_7[11:0] (Read/Write)
//        others   - reserved
// 0x4c : reserved
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

//------------------------Parameter----------------------
localparam
    ADDR_MONITOR_0_DATA_0 = 7'h10,
    ADDR_MONITOR_0_CTRL   = 7'h14,
    ADDR_MONITOR_1_DATA_0 = 7'h18,
    ADDR_MONITOR_1_CTRL   = 7'h1c,
    ADDR_MONITOR_2_DATA_0 = 7'h20,
    ADDR_MONITOR_2_CTRL   = 7'h24,
    ADDR_MONITOR_3_DATA_0 = 7'h28,
    ADDR_MONITOR_3_CTRL   = 7'h2c,
    ADDR_MONITOR_4_DATA_0 = 7'h30,
    ADDR_MONITOR_4_CTRL   = 7'h34,
    ADDR_MONITOR_5_DATA_0 = 7'h38,
    ADDR_MONITOR_5_CTRL   = 7'h3c,
    ADDR_MONITOR_6_DATA_0 = 7'h40,
    ADDR_MONITOR_6_CTRL   = 7'h44,
    ADDR_MONITOR_7_DATA_0 = 7'h48,
    ADDR_MONITOR_7_CTRL   = 7'h4c,
    WRIDLE                = 2'd0,
    WRDATA                = 2'd1,
    WRRESP                = 2'd2,
    WRRESET               = 2'd3,
    RDIDLE                = 2'd0,
    RDDATA                = 2'd1,
    RDRESET               = 2'd2,
    ADDR_BITS                = 7;

//------------------------Local signal-------------------
    reg  [1:0]                    wstate = WRRESET;
    reg  [1:0]                    wnext;
    reg  [ADDR_BITS-1:0]          waddr;
    wire [C_S_AXI_DATA_WIDTH-1:0] wmask;
    wire                          aw_hs;
    wire                          w_hs;
    reg  [1:0]                    rstate = RDRESET;
    reg  [1:0]                    rnext;
    reg  [C_S_AXI_DATA_WIDTH-1:0] rdata;
    wire                          ar_hs;
    wire [ADDR_BITS-1:0]          raddr;
    // internal registers
    reg  [11:0]                   int_monitor_0 = 'b0;
    reg  [11:0]                   int_monitor_1 = 'b0;
    reg  [11:0]                   int_monitor_2 = 'b0;
    reg  [11:0]                   int_monitor_3 = 'b0;
    reg  [11:0]                   int_monitor_4 = 'b0;
    reg  [11:0]                   int_monitor_5 = 'b0;
    reg  [11:0]                   int_monitor_6 = 'b0;
    reg  [11:0]                   int_monitor_7 = 'b0;

//------------------------Instantiation------------------


//------------------------AXI write fsm------------------
assign AWREADY = (wstate == WRIDLE);
assign WREADY  = (wstate == WRDATA);
assign BRESP   = 2'b00;  // OKAY
assign BVALID  = (wstate == WRRESP);
assign wmask   = { {8{WSTRB[3]}}, {8{WSTRB[2]}}, {8{WSTRB[1]}}, {8{WSTRB[0]}} };
assign aw_hs   = AWVALID & AWREADY;
assign w_hs    = WVALID & WREADY;

// wstate
always @(posedge ACLK) begin
    if (ARESET)
        wstate <= WRRESET;
    else if (ACLK_EN)
        wstate <= wnext;
end

// wnext
always @(*) begin
    case (wstate)
        WRIDLE:
            if (AWVALID)
                wnext = WRDATA;
            else
                wnext = WRIDLE;
        WRDATA:
            if (WVALID)
                wnext = WRRESP;
            else
                wnext = WRDATA;
        WRRESP:
            if (BREADY)
                wnext = WRIDLE;
            else
                wnext = WRRESP;
        default:
            wnext = WRIDLE;
    endcase
end

// waddr
always @(posedge ACLK) begin
    if (ACLK_EN) begin
        if (aw_hs)
            waddr <= AWADDR[ADDR_BITS-1:0];
    end
end

//------------------------AXI read fsm-------------------
assign ARREADY = (rstate == RDIDLE);
assign RDATA   = rdata;
assign RRESP   = 2'b00;  // OKAY
assign RVALID  = (rstate == RDDATA);
assign ar_hs   = ARVALID & ARREADY;
assign raddr   = ARADDR[ADDR_BITS-1:0];

// rstate
always @(posedge ACLK) begin
    if (ARESET)
        rstate <= RDRESET;
    else if (ACLK_EN)
        rstate <= rnext;
end

// rnext
always @(*) begin
    case (rstate)
        RDIDLE:
            if (ARVALID)
                rnext = RDDATA;
            else
                rnext = RDIDLE;
        RDDATA:
            if (RREADY & RVALID)
                rnext = RDIDLE;
            else
                rnext = RDDATA;
        default:
            rnext = RDIDLE;
    endcase
end

// rdata
always @(posedge ACLK) begin
    if (ACLK_EN) begin
        if (ar_hs) begin
            rdata <= 'b0;
            case (raddr)
                ADDR_MONITOR_0_DATA_0: begin
                    rdata <= int_monitor_0[11:0];
                end
                ADDR_MONITOR_1_DATA_0: begin
                    rdata <= int_monitor_1[11:0];
                end
                ADDR_MONITOR_2_DATA_0: begin
                    rdata <= int_monitor_2[11:0];
                end
                ADDR_MONITOR_3_DATA_0: begin
                    rdata <= int_monitor_3[11:0];
                end
                ADDR_MONITOR_4_DATA_0: begin
                    rdata <= int_monitor_4[11:0];
                end
                ADDR_MONITOR_5_DATA_0: begin
                    rdata <= int_monitor_5[11:0];
                end
                ADDR_MONITOR_6_DATA_0: begin
                    rdata <= int_monitor_6[11:0];
                end
                ADDR_MONITOR_7_DATA_0: begin
                    rdata <= int_monitor_7[11:0];
                end
            endcase
        end
    end
end


//------------------------Register logic-----------------
assign monitor_0 = int_monitor_0;
assign monitor_1 = int_monitor_1;
assign monitor_2 = int_monitor_2;
assign monitor_3 = int_monitor_3;
assign monitor_4 = int_monitor_4;
assign monitor_5 = int_monitor_5;
assign monitor_6 = int_monitor_6;
assign monitor_7 = int_monitor_7;
// int_monitor_0[11:0]
always @(posedge ACLK) begin
    if (ARESET)
        int_monitor_0[11:0] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_MONITOR_0_DATA_0)
            int_monitor_0[11:0] <= (WDATA[31:0] & wmask) | (int_monitor_0[11:0] & ~wmask);
    end
end

// int_monitor_1[11:0]
always @(posedge ACLK) begin
    if (ARESET)
        int_monitor_1[11:0] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_MONITOR_1_DATA_0)
            int_monitor_1[11:0] <= (WDATA[31:0] & wmask) | (int_monitor_1[11:0] & ~wmask);
    end
end

// int_monitor_2[11:0]
always @(posedge ACLK) begin
    if (ARESET)
        int_monitor_2[11:0] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_MONITOR_2_DATA_0)
            int_monitor_2[11:0] <= (WDATA[31:0] & wmask) | (int_monitor_2[11:0] & ~wmask);
    end
end

// int_monitor_3[11:0]
always @(posedge ACLK) begin
    if (ARESET)
        int_monitor_3[11:0] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_MONITOR_3_DATA_0)
            int_monitor_3[11:0] <= (WDATA[31:0] & wmask) | (int_monitor_3[11:0] & ~wmask);
    end
end

// int_monitor_4[11:0]
always @(posedge ACLK) begin
    if (ARESET)
        int_monitor_4[11:0] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_MONITOR_4_DATA_0)
            int_monitor_4[11:0] <= (WDATA[31:0] & wmask) | (int_monitor_4[11:0] & ~wmask);
    end
end

// int_monitor_5[11:0]
always @(posedge ACLK) begin
    if (ARESET)
        int_monitor_5[11:0] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_MONITOR_5_DATA_0)
            int_monitor_5[11:0] <= (WDATA[31:0] & wmask) | (int_monitor_5[11:0] & ~wmask);
    end
end

// int_monitor_6[11:0]
always @(posedge ACLK) begin
    if (ARESET)
        int_monitor_6[11:0] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_MONITOR_6_DATA_0)
            int_monitor_6[11:0] <= (WDATA[31:0] & wmask) | (int_monitor_6[11:0] & ~wmask);
    end
end

// int_monitor_7[11:0]
always @(posedge ACLK) begin
    if (ARESET)
        int_monitor_7[11:0] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_MONITOR_7_DATA_0)
            int_monitor_7[11:0] <= (WDATA[31:0] & wmask) | (int_monitor_7[11:0] & ~wmask);
    end
end


//------------------------Memory logic-------------------

endmodule
