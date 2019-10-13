/* PREHEADER */

`define true  1'b1

`define false 1'b0

/* END OF PREHEADER */
module wrapper(
__ILA_I_en,
clk,
dummy_reset,
rst,
__ILA_SO_v,
__m1__,
__m3__,
invariant_assume__m10__,
invariant_assume__m11__,
invariant_assume__m12__,
invariant_assume__m5__,
invariant_assume__m6__,
invariant_assume__m7__,
invariant_assume__m8__,
invariant_assume__m9__,
issue_decode__m13__,
issue_valid__m14__,
m1__DOT__imp,
m1__DOT__out,
m1__DOT__v,
noreset__m0__,
out,
variable_map_assert__p4__,
variable_map_assume__m2__,
__CYCLE_CNT__,
__START__,
__STARTED__,
__ENDED__,
__2ndENDED__,
__RESETED__
);
input            __ILA_I_en;
input            clk;
input            dummy_reset;
input            rst;
output      [3:0] __ILA_SO_v;
output            __m1__;
output            __m3__;
output            invariant_assume__m10__;
output            invariant_assume__m11__;
output            invariant_assume__m12__;
output            invariant_assume__m5__;
output            invariant_assume__m6__;
output            invariant_assume__m7__;
output            invariant_assume__m8__;
output            invariant_assume__m9__;
output            issue_decode__m13__;
output            issue_valid__m14__;
output      [3:0] m1__DOT__imp;
output      [3:0] m1__DOT__out;
output      [3:0] m1__DOT__v;
output            noreset__m0__;
output      [3:0] out;
output            variable_map_assert__p4__;
output            variable_map_assume__m2__;
output reg      [3:0] __CYCLE_CNT__;
output reg            __START__;
output reg            __STARTED__;
output reg            __ENDED__;
output reg            __2ndENDED__;
output reg            __RESETED__;
wire            __2ndIEND__;
(* keep *) wire            __EDCOND__;
(* keep *) wire            __IEND__;
(* keep *) wire            __ILA_I_en;
(* keep *) wire      [3:0] __ILA_SO_v;
(* keep *) wire            __ILA_counter_decode_of_INC__;
(* keep *) wire            __ILA_counter_valid__;
(* keep *) wire            __ISSUE__;
(* keep *) wire            __m1__;
(* keep *) wire            __m3__;
wire            clk;
(* keep *) wire            dummy_reset;
(* keep *) wire            invariant_assume__m10__;
(* keep *) wire            invariant_assume__m11__;
(* keep *) wire            invariant_assume__m12__;
(* keep *) wire            invariant_assume__m5__;
(* keep *) wire            invariant_assume__m6__;
(* keep *) wire            invariant_assume__m7__;
(* keep *) wire            invariant_assume__m8__;
(* keep *) wire            invariant_assume__m9__;
(* keep *) wire            issue_decode__m13__;
(* keep *) wire            issue_valid__m14__;
(* keep *) wire      [3:0] m1__DOT__imp;
(* keep *) wire      [3:0] m1__DOT__out;
(* keep *) wire      [3:0] m1__DOT__v;
(* keep *) wire            noreset__m0__;
(* keep *) wire      [3:0] out;
wire            rst;
(* keep *) wire            variable_map_assert__p4__;
(* keep *) wire            variable_map_assume__m2__;
assign noreset__m0__ =  (~__RESETED__) || (dummy_reset == 0)  ;
always @(posedge clk) begin
if (rst) __CYCLE_CNT__ <= 0;
else if ( ( __START__ || __STARTED__ ) &&  __CYCLE_CNT__ < 6) __CYCLE_CNT__ <= __CYCLE_CNT__ + 1;
end
always @(posedge clk) begin
if (rst) __START__ <= 1;
else if (__START__ || __STARTED__) __START__ <= 0;
end
always @(posedge clk) begin
if (rst) __STARTED__ <= 0;
else if (__START__) __STARTED__ <= 1;
end
always @(posedge clk) begin
if (rst) __ENDED__ <= 0;
else if (__IEND__) __ENDED__ <= 1;
end
always @(posedge clk) begin
if (rst) __2ndENDED__ <= 1'b0;
else if (__ENDED__ && __EDCOND__ && ~__2ndENDED__)  __2ndENDED__ <= 1'b1; end
assign __2ndIEND__ = __ENDED__ && __EDCOND__ && ~__2ndENDED__ ;
always @(posedge clk) begin
if (rst) __RESETED__ <= 1;
end
assign __m1__ = m1__DOT__out == __ILA_SO_v ;
assign variable_map_assume__m2__ = (~ __START__ )|| (__m1__) ;
assign __m3__ = m1__DOT__out == __ILA_SO_v ;
assign variable_map_assert__p4__ = (~ __IEND__) || (__m3__) ;
assign invariant_assume__m5__ = (1'b1) == ((((m1__DOT__v[0:0]) == (1'b1)) ||((m1__DOT__imp[0:0]) == (1'b1))) &&((!((m1__DOT__v[0:0]) == (1'b1))) ||(!((m1__DOT__imp[0:0]) == (1'b1))))) ;
assign invariant_assume__m6__ = (1'b1) == (((!((m1__DOT__v) == (4'ha))) ||(!((m1__DOT__imp) == (4'hf)))) &&((!((m1__DOT__v) == (4'h1))) ||(!((m1__DOT__imp) == (4'h8)))) &&((!((m1__DOT__imp) == (4'h6))) ||(!((m1__DOT__v) == (4'h3)))) &&((!((m1__DOT__v) == (4'h4))) ||(!((m1__DOT__imp) == (4'h5)))) &&((!((m1__DOT__imp) == (4'h4))) ||(!((m1__DOT__v) == (4'h5)))) &&((!((m1__DOT__imp) == (4'h7))) ||(!((m1__DOT__v) == (4'h2)))) &&((!((m1__DOT__v) == (4'hb))) ||(!((m1__DOT__imp) == (4'he)))) &&((!((m1__DOT__imp) == (4'ha))) ||(!((m1__DOT__v) == (4'hf)))) &&((!((m1__DOT__imp) == (4'h9))) ||(!((m1__DOT__v) == (4'h0)))) &&((!((m1__DOT__imp) == (4'hb))) ||(!(m1__DOT__v[1:1])) ||(!(m1__DOT__v[2:2])) ||(!(m1__DOT__v[3:3])) ||(m1__DOT__v[0:0])) &&((!((m1__DOT__imp) == (4'hd))) ||(!((m1__DOT__v) == (4'hc)))) &&((!((m1__DOT__imp) == (4'hc))) ||(!((m1__DOT__v) == (4'hd)))) &&((!((m1__DOT__imp) == (4'h3))) ||(!((m1__DOT__v) == (4'h6)))) &&((!((m1__DOT__imp) == (4'h2))) ||(!((m1__DOT__v) == (4'h7)))) &&((!((m1__DOT__imp) == (4'h1))) ||(!((m1__DOT__v) == (4'h8)))) &&((!((m1__DOT__v) == (4'h9))) ||(m1__DOT__imp[0:0]) ||(m1__DOT__imp[2:2]) ||(m1__DOT__imp[3:3]) ||(m1__DOT__imp[1:1]))) ;
assign invariant_assume__m7__ = (1'b1) == (((!((m1__DOT__imp) == (4'h5))) ||(!((m1__DOT__v) == (4'h0)))) &&((!((m1__DOT__v) == (4'hb))) ||(!((m1__DOT__imp) == (4'ha)))) &&((!((m1__DOT__imp) == (4'h9))) ||(!((m1__DOT__v) == (4'hc)))) &&((!((m1__DOT__imp) == (4'h8))) ||(!((m1__DOT__v) == (4'hd)))) &&((!((m1__DOT__imp) == (4'h6))) ||(!((m1__DOT__v) == (4'hf)))) &&((!((m1__DOT__imp) == (4'hb))) ||(!((m1__DOT__v) == (4'ha)))) &&((!((m1__DOT__v) == (4'he))) ||(!((m1__DOT__imp) == (4'h7)))) &&((!((m1__DOT__imp) == (4'hc))) ||(!((m1__DOT__v) == (4'h9)))) &&((!((m1__DOT__v) == (4'h8))) ||(!((m1__DOT__imp) == (4'hd)))) &&((!((m1__DOT__imp) == (4'he))) ||(!((m1__DOT__v) == (4'h7)))) &&((!((m1__DOT__v) == (4'h1))) ||(!((m1__DOT__imp) == (4'h4)))) &&((!((m1__DOT__imp) == (4'h3))) ||(!((m1__DOT__v) == (4'h2)))) &&((!((m1__DOT__imp) == (4'h2))) ||(!((m1__DOT__v) == (4'h3)))) &&((!((m1__DOT__imp) == (4'h1))) ||(!((m1__DOT__v) == (4'h4)))) &&((!((m1__DOT__v) == (4'h5))) ||(!((m1__DOT__imp) == (4'h0)))) &&((!((m1__DOT__v) == (4'h6))) ||(!((m1__DOT__imp) == (4'hf))))) ;
assign invariant_assume__m8__ = (1'b1) == (((m1__DOT__imp[0:0]) ||(!(m1__DOT__imp[3:3])) ||(!(m1__DOT__v[0:0])) ||(!(m1__DOT__v[1:1])) ||(!(m1__DOT__v[2:2])) ||(!(m1__DOT__v[3:3])) ||(!(m1__DOT__imp[1:1])) ||(!(m1__DOT__imp[2:2]))) &&((!((m1__DOT__v) == (4'h5))) ||(!((m1__DOT__imp) == (4'h8)))) &&((!((m1__DOT__imp) == (4'h6))) ||(!((m1__DOT__v) == (4'h7)))) &&((!((m1__DOT__imp) == (4'h4))) ||(!((m1__DOT__v) == (4'h9)))) &&((!((m1__DOT__imp) == (4'h3))) ||(!((m1__DOT__v) == (4'ha)))) &&((!((m1__DOT__imp) == (4'h2))) ||(!((m1__DOT__v) == (4'hb)))) &&((!((m1__DOT__v) == (4'hc))) ||(!((m1__DOT__imp) == (4'h1)))) &&((!((m1__DOT__imp) == (4'h7))) ||(!((m1__DOT__v) == (4'h6)))) &&((!((m1__DOT__v) == (4'h2))) ||(!((m1__DOT__imp) == (4'hb)))) &&((m1__DOT__imp[1:1]) ||(!((m1__DOT__v) == (4'hd))) ||(m1__DOT__imp[0:0]) ||(m1__DOT__imp[2:2]) ||(m1__DOT__imp[3:3])) &&((!((m1__DOT__imp) == (4'h9))) ||(!((m1__DOT__v) == (4'h4)))) &&((!((m1__DOT__v) == (4'h8))) ||(!(m1__DOT__imp[0:0])) ||(!(m1__DOT__imp[2:2])) ||(m1__DOT__imp[3:3]) ||(m1__DOT__imp[1:1])) &&((!((m1__DOT__imp) == (4'hd))) ||(!((m1__DOT__v) == (4'h0)))) &&((!((m1__DOT__v) == (4'h3))) ||(!((m1__DOT__imp) == (4'ha)))) &&((!((m1__DOT__imp) == (4'hc))) ||(!((m1__DOT__v) == (4'h1)))) &&((!((m1__DOT__v) == (4'he))) ||(!((m1__DOT__imp) == (4'hf))))) ;
assign invariant_assume__m9__ = (1'b1) == (((!((m1__DOT__imp) == (4'h6))) ||(!(m1__DOT__v[0:0])) ||(!(m1__DOT__v[1:1])) ||(!(m1__DOT__v[3:3])) ||(m1__DOT__v[2:2])) &&((!((m1__DOT__imp) == (4'he))) ||(!((m1__DOT__v) == (4'h3)))) &&((!((m1__DOT__v) == (4'h4))) ||(!((m1__DOT__imp) == (4'hd)))) &&((!((m1__DOT__imp) == (4'hc))) ||(!((m1__DOT__v) == (4'h5)))) &&((!((m1__DOT__imp) == (4'hb))) ||(!((m1__DOT__v) == (4'h6)))) &&((!((m1__DOT__imp) == (4'ha))) ||(!((m1__DOT__v) == (4'h7)))) &&((!((m1__DOT__v) == (4'h8))) ||(!((m1__DOT__imp) == (4'h9)))) &&((!((m1__DOT__imp) == (4'h8))) ||(!((m1__DOT__v) == (4'h9)))) &&((!((m1__DOT__v) == (4'h2))) ||(!((m1__DOT__imp) == (4'hf)))) &&((!((m1__DOT__imp) == (4'h5))) ||(!((m1__DOT__v) == (4'hc)))) &&((!((m1__DOT__v) == (4'hd))) ||(!((m1__DOT__imp) == (4'h4)))) &&((!((m1__DOT__imp) == (4'h3))) ||(!((m1__DOT__v) == (4'he)))) &&((!((m1__DOT__v) == (4'hf))) ||(!((m1__DOT__imp) == (4'h2)))) &&((!((m1__DOT__imp) == (4'h1))) ||(!((m1__DOT__v) == (4'h0)))) &&((!((m1__DOT__imp) == (4'h0))) ||(!(m1__DOT__v[0:0])) ||(m1__DOT__v[1:1]) ||(m1__DOT__v[2:2]) ||(m1__DOT__v[3:3])) &&((!((m1__DOT__imp) == (4'h7))) ||(!(m1__DOT__v[1:1])) ||(!(m1__DOT__v[3:3])) ||(m1__DOT__v[0:0]) ||(m1__DOT__v[2:2]))) ;
assign invariant_assume__m10__ = (1'b1) == (((!((m1__DOT__imp) == (4'h2))) ||(!((m1__DOT__v) == (4'h9)))) &&((!((m1__DOT__v) == (4'h1))) ||(!((m1__DOT__imp) == (4'ha)))) &&((!((m1__DOT__v) == (4'h2))) ||(!((m1__DOT__imp) == (4'h9)))) &&((!((m1__DOT__imp) == (4'h8))) ||(!((m1__DOT__v) == (4'h3)))) &&((!((m1__DOT__imp) == (4'he))) ||(!((m1__DOT__v) == (4'hd)))) &&((!((m1__DOT__imp) == (4'hc))) ||(!((m1__DOT__v) == (4'hf)))) &&((!((m1__DOT__imp) == (4'hb))) ||(!((m1__DOT__v) == (4'h0)))) &&((!((m1__DOT__imp) == (4'h1))) ||(!((m1__DOT__v) == (4'ha)))) &&((!((m1__DOT__v) == (4'hc))) ||(!((m1__DOT__imp) == (4'hf)))) &&((!((m1__DOT__v) == (4'he))) ||(!((m1__DOT__imp) == (4'hd)))) &&((!((m1__DOT__imp) == (4'h6))) ||(!((m1__DOT__v) == (4'h5)))) &&((!((m1__DOT__imp) == (4'h5))) ||(!((m1__DOT__v) == (4'h6)))) &&((!((m1__DOT__imp) == (4'h0))) ||(!((m1__DOT__v) == (4'hb)))) &&((!((m1__DOT__imp) == (4'h7))) ||(!((m1__DOT__v) == (4'h4)))) &&((!((m1__DOT__imp) == (4'h4))) ||(!((m1__DOT__v) == (4'h7)))) &&((!((m1__DOT__imp) == (4'h3))) ||(!(m1__DOT__v[3:3])) ||(m1__DOT__v[0:0]) ||(m1__DOT__v[1:1]) ||(m1__DOT__v[2:2]))) ;
assign invariant_assume__m11__ = (1'b1) == (((!((m1__DOT__v) == (4'h1))) ||(!(m1__DOT__imp[1:1])) ||(m1__DOT__imp[0:0]) ||(m1__DOT__imp[2:2]) ||(m1__DOT__imp[3:3])) &&((!((m1__DOT__v) == (4'h7))) ||(!((m1__DOT__imp) == (4'hc)))) &&((!((m1__DOT__imp) == (4'h9))) ||(!((m1__DOT__v) == (4'ha)))) &&((!((m1__DOT__imp) == (4'h8))) ||(!((m1__DOT__v) == (4'hb)))) &&((!((m1__DOT__imp) == (4'h7))) ||(!((m1__DOT__v) == (4'hc)))) &&((!((m1__DOT__imp) == (4'h6))) ||(!((m1__DOT__v) == (4'hd)))) &&((!((m1__DOT__v) == (4'he))) ||(!((m1__DOT__imp) == (4'h5)))) &&((!((m1__DOT__imp) == (4'h4))) ||(!((m1__DOT__v) == (4'hf)))) &&((!((m1__DOT__v) == (4'h5))) ||(!((m1__DOT__imp) == (4'he)))) &&((!((m1__DOT__imp) == (4'ha))) ||(!((m1__DOT__v) == (4'h9)))) &&((!((m1__DOT__imp) == (4'hb))) ||(!((m1__DOT__v) == (4'h8)))) &&((!((m1__DOT__v) == (4'h6))) ||(!((m1__DOT__imp) == (4'hd)))) &&((!((m1__DOT__imp) == (4'h1))) ||(!((m1__DOT__v) == (4'h2)))) &&((!((m1__DOT__v) == (4'h3))) ||(m1__DOT__imp[0:0]) ||(m1__DOT__imp[2:2]) ||(m1__DOT__imp[3:3]) ||(m1__DOT__imp[1:1])) &&((!((m1__DOT__v) == (4'h4))) ||(!((m1__DOT__imp) == (4'hf)))) &&((!((m1__DOT__imp) == (4'h3))) ||(!((m1__DOT__v) == (4'h0))))) ;
assign invariant_assume__m12__ = (1'b1) == (((!((m1__DOT__v) == (4'h3))) ||(!((m1__DOT__imp) == (4'h4)))) &&((!((m1__DOT__imp) == (4'he))) ||(!((m1__DOT__v) == (4'h9)))) &&((!((m1__DOT__imp) == (4'hd))) ||(!((m1__DOT__v) == (4'ha)))) &&((!((m1__DOT__imp) == (4'hc))) ||(!((m1__DOT__v) == (4'hb)))) &&((!((m1__DOT__imp) == (4'hb))) ||(!((m1__DOT__v) == (4'hc)))) &&((!((m1__DOT__imp) == (4'ha))) ||(!((m1__DOT__v) == (4'hd)))) &&((!((m1__DOT__v) == (4'he))) ||(!((m1__DOT__imp) == (4'h9)))) &&((!((m1__DOT__imp) == (4'h8))) ||(!((m1__DOT__v) == (4'hf)))) &&((!((m1__DOT__imp) == (4'h3))) ||(!((m1__DOT__v) == (4'h4)))) &&((!((m1__DOT__v) == (4'h6))) ||(!((m1__DOT__imp) == (4'h1)))) &&((!((m1__DOT__v) == (4'h7))) ||(!((m1__DOT__imp) == (4'h0)))) &&((!((m1__DOT__v) == (4'h8))) ||(!((m1__DOT__imp) == (4'hf)))) &&((!((m1__DOT__imp) == (4'h5))) ||(!((m1__DOT__v) == (4'h2)))) &&((!((m1__DOT__imp) == (4'h7))) ||(!((m1__DOT__v) == (4'h0)))) &&((!((m1__DOT__v) == (4'h5))) ||(!(m1__DOT__imp[1:1])) ||(m1__DOT__imp[0:0]) ||(m1__DOT__imp[2:2]) ||(m1__DOT__imp[3:3])) &&((!((m1__DOT__v) == (4'h1))) ||(!((m1__DOT__imp) == (4'h6))))) ;
assign __EDCOND__ = (`false|| ( __CYCLE_CNT__ == 4'd1)) && __STARTED__  ;
assign __IEND__ = (`false|| ( __CYCLE_CNT__ == 4'd1)) && __STARTED__ && __RESETED__ && (~ __ENDED__) ;
assign issue_decode__m13__ = (~ __START__) || (__ILA_counter_decode_of_INC__) ;
assign issue_valid__m14__ = (~ __START__) || (__ILA_counter_valid__) ;
assign __ISSUE__ = 1 ;

counter__DOT__INC m0 (
   .__START__(__START__),
   .clk(clk),
   .en(__ILA_I_en),
   .rst(rst),
   .__ILA_counter_decode_of_INC__(__ILA_counter_decode_of_INC__),
   .__ILA_counter_valid__(__ILA_counter_valid__),
   .v(__ILA_SO_v),
   .__COUNTER_start__n1()
);
opposite m1(
    .clk(clk),
    .en(__ILA_I_en),
    .m1__DOT__imp(m1__DOT__imp),
    .m1__DOT__out(m1__DOT__out),
    .m1__DOT__v(m1__DOT__v),
    .out(out),
    .rst(dummy_reset)
);
endmodule
module counter__DOT__INC(
__START__,
clk,
en,
rst,
__ILA_counter_decode_of_INC__,
__ILA_counter_valid__,
v,
__COUNTER_start__n1
);
input            __START__;
input            clk;
input            en;
input            rst;
output            __ILA_counter_decode_of_INC__;
output            __ILA_counter_valid__;
output reg      [3:0] v;
output reg      [7:0] __COUNTER_start__n1;
wire            __ILA_counter_decode_of_INC__;
wire            __ILA_counter_valid__;
wire            __START__;
wire            clk;
wire            en;
wire            n0;
wire      [3:0] n2;
wire            rst;
(* keep *) wire      [3:0] v_randinit;
assign __ILA_counter_valid__ = 1'b1 ;
assign n0 =  ( en ) == ( 1'd1 )  ;
assign __ILA_counter_decode_of_INC__ = n0 ;
assign n2 =  ( v ) + ( 4'd1 )  ;
always @(posedge clk) begin
   if(rst) begin
       v <= v_randinit ;
       __COUNTER_start__n1 <= 0;
   end
   else if(__START__ && __ILA_counter_valid__) begin
       if ( __ILA_counter_decode_of_INC__ ) begin 
           __COUNTER_start__n1 <= 1; end
       else if( (__COUNTER_start__n1 >= 1 ) && ( __COUNTER_start__n1 < 255 )) begin
           __COUNTER_start__n1 <= __COUNTER_start__n1 + 1; end
       if (__ILA_counter_decode_of_INC__) begin
           v <= n2 ;
       end
   end
end
endmodule
module opposite(input clk, input rst, input en, output [3:0] out, output wire [3:0] m1__DOT__imp, output wire [3:0] m1__DOT__out, output wire [3:0] m1__DOT__v);

reg [3:0] v; // explicit state variable (modeled in ILA)
reg [3:0] imp; // implicit (micro-arch, not modeled, but need constraints on it)
// the constraint we want is
// p : v == 4'hf - imp

always @(posedge clk) begin
  if (rst)
    v <= 0;
  else if(en)
    v <= v + 1;
end

always @(posedge clk) begin
  if (rst)
    imp <= 4'hf;
  else if(en)
    imp <= imp - 1;
end

assign out = v & ( 4'hf - imp ) ; 
// 4'hf - imp == v
// so, `out` should be the same as `v`

 assign m1__DOT__v = v;
 assign m1__DOT__out = out;
 assign m1__DOT__imp = imp;
endmodule
