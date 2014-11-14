module HLSM(Clk, Rst, Start, Done, b, c, y, m, n, r, s, f, h, i, p, q, W); 

 
input Clk, Rst, Start; 
input[31:0] b, c, y, m, n, r, s, f, h, i, p, q;
output reg Done; 
output reg[31:0] W;

reg[31:0] a, d, l, t, e, x, g;

reg[4:0] State;
parameter Wait = 0, S1 = 1, S2 = 2, S3 = 3, S4 = 4, S5 = 5, S6 = 6, S7 = 7, S8 = 8, S9 = 9, S10 = 10, S11 = 11, S12 = 12, S13 = 13, S14 = 14, S15 = 15, S16 = 16, S17 = 17, Final = 18;

always @ (posedge Clk)
begin
	if(Rst == 1)
	begin
		a <= 0;
 		d <= 0;
 		l <= 0;
 		t <= 0;
 		e <= 0;
 		x <= 0;
 		g <= 0;
 		W <= 0;
 		Done <= 0;
		State <= Wait;
	end
	else
	begin
		case (State)
			Wait: begin
				Done <= 0;
				if (Start == 1)
					State <= S1;
				else
					State <= Wait;
				end

			S1: begin
				a <= b + c;
				State <= S2;
			end
			S2: begin
				d <= x + y;
				State <= S3;
			end
			S3: begin
				if ( z )
				 begin
					 State <= S4;
				 end
				 else
				 begin
					 State <= S15;
				 end
			end
			S4: begin
				l <= m + n;
				State <= S5;
			end
			S5: begin
				t <= r + s;
				State <= S6;
			end
			S6: begin
				if ( y )
				 begin
					 State <= S7;
				 end
				 else
				 begin
					 State <= S13;
				 end
			end
			S7: begin
				if ( y )
				 begin
					 State <= S8;
				 end
				 else
				 begin
					 State <= S10;
				 end
			end
			S8: begin
				a <= b + c;
				State <= S9;
			end
			S9: begin
				e <= f + g;
				State <= S7;
			end
			S10: begin
				a <= b + c;
				State <= S11;
			end
			S11: begin
				e <= f + g;
				State <= S12;
			end
			S12: begin
				x <= c + h;
				State <= S13;
			end
			S13: begin
				g <= h + i;
				State <= S14;
			end
			S14: begin
				l <= p + q;
				State <= S3;
			end
			S15: begin
				a <= b + c;
				State <= S16;
			end
			S16: begin
				d <= x + y;
				State <= S17;
			end
			S17: begin
				W <= d;
				State <= Final;
			end
			Final: begin
				Done <= 1;
				State <= Wait;
			end
		endcase
	end
end

 
endmodule