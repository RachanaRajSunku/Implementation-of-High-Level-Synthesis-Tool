#include "verilogfile.h"

using namespace std;

void verilog_file(char *verilogfile, net_data **N, int len_net_file, char ***INPUTS, int inputcount, char ***OUTPUTS, int outputcount, char ***REGS, int regcount, char ***WIRES, int wirecount)
{
	FILE *verilog;
	int i, j, k;
	int numstates = 0;
	int numbits = 0;
	int tmp = 0;
	int index = 0;
	
	if(*N == NULL)
	{
		cout << "Empty NetList" << endl;
		exit(0);
	}
	//open the file in write mode
	verilog = fopen(verilogfile ,"w");
	
	//Start writing the file using fprintf command  
	fprintf(verilog, "module HLSM(Clk, Rst, Start, Done, ");
	for(i=0; i<inputcount; i++)
	{
		fprintf(verilog, "%s, ", (*INPUTS)[i]);
	}

	for(i=0; i<outputcount; i++)
	{
		if(i != (outputcount-1))
			fprintf(verilog, "%s, ", (*OUTPUTS)[i]);
		else if(i == (outputcount-1))
			fprintf(verilog, "%s); \n", (*OUTPUTS)[i]);
	}
	

	fprintf(verilog, "\n \n");
	
	//Define the inputs in the file
	fprintf(verilog, "input Clk, Rst, Start; \n");
	fprintf(verilog, "input[31:0] ");
	for(i=0; i<inputcount; i++)
	{
		//if it is not the last input to be declared in the file 
		if(i != (inputcount-1))
			fprintf(verilog, "%s, ", (*INPUTS)[i]);

		//if it is the last input to be declared in the file
		else if(i == (inputcount-1))
			fprintf(verilog, "%s;\n", (*INPUTS)[i]);
			
	}

	//Define the outputs in the file
	fprintf(verilog, "output reg Done; \n");
	fprintf(verilog, "output reg[31:0] ");	
	for(i=0; i<outputcount; i++)
	{
		if(i != (outputcount-1))
			fprintf(verilog, "%s, ", (*OUTPUTS)[i]);
		else if(i == (outputcount-1))
			fprintf(verilog, "%s;\n\n", (*OUTPUTS)[i]);
			
	}

	//Define all the registers as wires in the file	
	if(regcount != 0)
	{
		fprintf(verilog, "reg[31:0] ");	
		for(i=0; i<regcount; i++)
		{
			if(i != (regcount-1))
				fprintf(verilog, "%s, ", (*REGS)[i]);
			else if(i == (regcount-1))
				fprintf(verilog, "%s;\n\n", (*REGS)[i]);
		}
	}			
	
	//Define the wires in the file
	if(wirecount != 0)
	{
		fprintf(verilog, "wire[31:0] ");	
		for(i=0; i<wirecount; i++)
		{
			if(i != (wirecount-1))
				fprintf(verilog, "%s, ", (*WIRES)[i]);
			else if(i == (wirecount-1))
				fprintf(verilog, "%s;\n\n", (*WIRES)[i]);
		}
	}			
	
	for(i=0; i<len_net_file; i++)
	{
		if((*N)[i].type != CLOSE)
			numstates++;
	}
	numstates = numstates + 2;
	tmp = numstates;
	while(tmp != 0)
	{
		tmp = tmp/2;
		numbits++;
	}
	
	fprintf(verilog, "reg[%d:0] State;\n", (numbits-1));
	fprintf(verilog, "parameter ");
	for(i=0; i<numstates; i++)
	{
		if(i == 0)
			fprintf(verilog, "Wait = 0, ");
		else if(i != numstates-1)
			fprintf(verilog, "S%d = %d, ", i, i);
		else if(i == numstates-1)
			fprintf(verilog, "Final = %d;\n", i);
			
	}

	fprintf(verilog, "\n");
	fprintf(verilog, "always @ (posedge Clk)\n");
	fprintf(verilog, "begin\n");
	fprintf(verilog, "\tif(Rst == 1)\n");
	fprintf(verilog, "\tbegin\n");
	for(i=0; i<regcount; i++)
	{
		fprintf(verilog, "\t\t%s <= 0;\n ", (*REGS)[i]); 
	}
	for(i=0; i<outputcount; i++)
	{
		fprintf(verilog, "\t\t%s <= 0;\n ", (*OUTPUTS)[i]);
	}
	fprintf(verilog, "\t\tDone <= 0;\n");
	fprintf(verilog, "\t\tState <= Wait;\n");
	fprintf(verilog, "\tend\n");
	fprintf(verilog, "\telse\n");
	fprintf(verilog, "\tbegin\n");
	fprintf(verilog, "\t\tcase (State)\n");
	
	fprintf(verilog, "\t\t\tWait: begin\n");
	fprintf(verilog, "\t\t\t\tDone <= 0;\n");
	fprintf(verilog, "\t\t\t\tif (Start == 1)\n\t\t\t\t\tState <= S1;\n\t\t\t\telse\n\t\t\t\t\tState <= Wait;\n\t\t\t\tend\n\n");
	
	for(i=1; i<(numstates-1); i++)
	{
		if(index != len_net_file)
		{
			if((*N)[index].type == IF)
			{
				fprintf(verilog, "\t\t\tS%d: begin\n", i);
				fprintf(verilog, "\t\t\t\tif ( %s )\n", (*N)[index].ifstmt);
				fprintf(verilog, "\t\t\t\t begin\n");
				fprintf(verilog, "\t\t\t\t\t State <= S%d;\n", i+1);
				fprintf(verilog, "\t\t\t\t end\n");
				fprintf(verilog, "\t\t\t\t else\n");
				fprintf(verilog, "\t\t\t\t begin\n");
				if((*N)[index].endifpos == -100)
					fprintf(verilog, "\t\t\t\t\t State <= Final;\n");
				else
					fprintf(verilog, "\t\t\t\t\t State <= S%d;\n", (*N)[index].endifpos);	
				fprintf(verilog, "\t\t\t\t end\n");
				fprintf(verilog, "\t\t\tend\n");
			
			}
			else if((*N)[index].type == WHILE)
			{
				fprintf(verilog, "\t\t\tS%d: begin\n", i);
				fprintf(verilog, "\t\t\t\tif ( %s )\n", (*N)[index].whilestmt);
				fprintf(verilog, "\t\t\t\t begin\n");
				fprintf(verilog, "\t\t\t\t\t State <= S%d;\n", i+1);
				fprintf(verilog, "\t\t\t\t end\n");
				fprintf(verilog, "\t\t\t\t else\n");
				fprintf(verilog, "\t\t\t\t begin\n");
				if((*N)[index].endifpos == -100)
					fprintf(verilog, "\t\t\t\t\t State <= Final;\n");
				else
					fprintf(verilog, "\t\t\t\t\t State <= S%d;\n", (*N)[index].endwhilepos);	
				fprintf(verilog, "\t\t\t\t end\n");
				fprintf(verilog, "\t\t\tend\n");
			
			}
			else if ((*N)[index].type == NORMAL)
			{
				fprintf(verilog, "\t\t\tS%d: begin\n", i);
				fprintf(verilog, "\t\t\t\t%s <= ", (*N)[index].output);
				if(strcmp((*N)[index].oper1, "=") == 0)
				{
					fprintf(verilog, "%s;\n", (*N)[index].input[0]);
					if(i == numstates-2)
					{
						fprintf(verilog, "\t\t\t\tState <= Final;\n");
					}
					else
					{	
						if((*N)[index].gobacktowhile == -1)
							fprintf(verilog, "\t\t\t\tState <= S%d;\n", i+1 );
						else
							fprintf(verilog, "\t\t\t\tState <= S%d;\n", (*N)[index].gobacktowhile );
					}
				}
				else if( (strcmp((*N)[index].oper1, ">>") == 0) || (strcmp((*N)[index].oper1, "<<") == 0) || 
					(strcmp((*N)[index].oper1, ">") == 0) || (strcmp((*N)[index].oper1, "<") == 0) )
				{
					fprintf(verilog, "%s %s %s;\n", (*N)[index].input[0], (*N)[index].oper1, (*N)[index].input[1]);
					if(i == numstates-2)
					{
						fprintf(verilog, "\t\t\t\tState <= Final;\n");
					}
					else
					{
						if((*N)[index].gobacktowhile == -1)
							fprintf(verilog, "\t\t\t\tState <= S%d;\n", i+1 );
						else
							fprintf(verilog, "\t\t\t\tState <= S%d;\n", (*N)[index].gobacktowhile );
					}
				}
				else if(strcmp((*N)[index].oper1, "?") == 0)
				{
					fprintf(verilog, "%s %s %s %s %s;\n", (*N)[index].input[0], (*N)[index].oper1, (*N)[index].input[1], (*N)[index].oper2, (*N)[index].input[2]  );
					if(i == numstates-2)
					{
						fprintf(verilog, "\t\t\t\tState <= Final;\n");
					}
					else
					{
						if((*N)[index].gobacktowhile == -1)
							fprintf(verilog, "\t\t\t\tState <= S%d;\n", i+1 );
						else
							fprintf(verilog, "\t\t\t\tState <= S%d;\n", (*N)[index].gobacktowhile );	
					}
				}
				else
				{

					for(j=0; j<(*N)[index].numinputs; j++)
					{
						fprintf(verilog, "%s", (*N)[index].input[j]);
						if(j != ((*N)[index].numinputs-1))
						{
							fprintf(verilog, " %s ", (*N)[index].oper1);
						}
					}
					fprintf(verilog, ";\n");
					if(i == numstates-2)
					{
						fprintf(verilog, "\t\t\t\tState <= Final;\n");
					}
					else
					{
						if((*N)[index].gobacktowhile == -1)
							fprintf(verilog, "\t\t\t\tState <= S%d;\n", i+1 );
						else
							fprintf(verilog, "\t\t\t\tState <= S%d;\n", (*N)[index].gobacktowhile );
					}
				}
				fprintf(verilog, "\t\t\tend\n");
			}
		}
		k = index+1;
		while((*N)[k].type == CLOSE)
		{ 
			k++;
		}
		index = k;
	}
	fprintf(verilog, "\t\t\tFinal: begin\n");
	fprintf(verilog, "\t\t\t\tDone <= 1;\n");
	fprintf(verilog, "\t\t\t\tState <= Wait;\n");
	fprintf(verilog, "\t\t\tend\n");

	fprintf(verilog, "\t\tendcase\n");
	fprintf(verilog, "\tend\n");
	fprintf(verilog, "end\n");	
	fprintf(verilog, "\n \nendmodule");
	fclose(verilog); 	
}
