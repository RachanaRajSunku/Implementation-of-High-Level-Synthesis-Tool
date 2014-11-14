#include<iostream>
#include<string.h>
#include<cstdlib>
#include "parse_net.h"
#include "verilogfile.h"

using namespace std;

int main(int argc, char *argv[])
{
//Check for commandline arguments

	//Commandline argument for help 	
	if(argc == 2)
	{
		if(strcmp(argv[0], "./hlsyn") == 0)
		//if(strcmp(argv[0], "./src/hlsyn") == 0)
		{
			if(strcmp(argv[1], "-help") == 0)
			{
				cout << "The available commandline options are: \n" << endl;
    				cout << "hlsyn -ns cfile verilogfile" << endl;
				cout << "Generates High-Level State Machine (HLSM) description in Verilog with no scheduling\n" << endl;
    				cout << "hlsyn -listl cfile verilogfile mul add logic" << endl;
				cout << "Generates High-Level State Machine (HLSM) description in Verilog with LISTL scheduling\n" << endl;
    				cout << "hlsyn -listr cfile verilogfile latency" << endl;
				cout << "Generates High-Level State Machine (HLSM) description in Verilog with LISTR scheduling\n" << endl;
			}
			else
			{
				cout << "Invalid commandline arguments" <<endl;
				cout << "Usage: hlsyn -help" << endl;
				exit(0);
			}
		}
		else
		{
			cout << "Invalid commandline arguments" <<endl;
			cout << "Usage: hlsyn -help" << endl;
			exit(0);
		}	
	}

// Commandline argument for no scheduling
	else if(argc == 4)
	{
		if(strcmp(argv[0], "./hlsyn") == 0)
		//if(strcmp(argv[0], "./src/hlsyn") == 0)
		{
			if(strcmp(argv[1], "-ns") == 0)
			{
			}
			else
			{
				cout << "Invalid commandline arguments" <<endl;
				cout << "Usage: hlsyn -ns cfile verilogfile" << endl;
				exit(0);
			}
		}
		else
		{
			cout << "Invalid commandline arguments" <<endl;
			cout << "Usage: hlsyn -ns cfile verilogfile" << endl;
			exit(0);
		}
	}

// Commandline argument for LISTL scheduling
	else if (argc == 7)
	{
		if(strcmp(argv[0], "./hlsyn") == 0)
		//if(strcmp(argv[0], "./src/hlsyn") == 0)
		{
			if(strcmp(argv[1], "-listl") == 0)
			{
			}
			else
			{
				cout << "Invalid commandline arguments" <<endl;
				cout << "Usage: hlsyn -listl cfile verilogfile mul add logic" << endl;
				exit(0);
			}
		}
		else
		{
			cout << "Invalid commandline arguments" <<endl;
			cout << "Usage: hlsyn -listl cfile verilogfile mul add logic" << endl;
			exit(0);
		}
	}

// Commandline argument for LISTR scheduling
	else if (argc == 5)
	{
		if(strcmp(argv[0], "./hlsyn") == 0)
		//if(strcmp(argv[0], "./src/hlsyn") == 0)
		{
			if(strcmp(argv[1], "-listr") == 0)
			{
			}
			else
			{
				cout << "Invalid commandline arguments" <<endl;
				cout << "Usage: hlsyn -listr cfile verilogfile latency" << endl;
				exit(0);
			}
		}
		else
		{
			cout << "Invalid commandline arguments" <<endl;
			cout << "Usage: hlsyn -listr cfile verilogfile latency" << endl;
			exit(0);
		}
	}
	
	else 
	{
		cout << "Invalid commandline arguments" <<endl;
		cout << "Usage: hlsyn -ns cfile verilogfile" << endl;
		cout << "or" << endl;
		cout << "Usage: hlsyn -listl cfile verilogfile mul add logic" << endl;
		cout << "or" << endl;
		cout << "Usage: hlsyn -listr cfile verilogfile latency" << endl;
		cout << "or" << endl;
		cout << "Usage: hlsyn -help" << endl;
		exit(0);
	}
 
	
	net_data *NET;
	int len_net_file;
	char **INPUTS, **OUTPUTS, **REGS, **WIRES;
	int inputcount, outputcount, regcount, wirecount;
	
	//To get the netlist file components
	NET = parse_net(argv[2], &len_net_file, &INPUTS, &OUTPUTS, &REGS, &WIRES, &inputcount, &outputcount, &regcount, &wirecount);
	
	//To generate the verilogfile with no scheduling
	verilog_file(argv[3], &NET, len_net_file, &INPUTS, inputcount, &OUTPUTS, outputcount, &REGS, regcount, &WIRES, wirecount);
	
	return 0;
} 
