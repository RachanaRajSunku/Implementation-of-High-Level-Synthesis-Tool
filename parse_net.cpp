#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "parse_net.h"

using namespace std;

net_data* parse_net(char *netfilename, int *linecount, char ***INPUTS, char ***OUTPUTS, char ***REGS, char ***WIRES, int *inputcount, int *outputcount, int *regcount, int *wirecount)
{	
	char netline[500], dupnetline[500], tripnetline[500], newnetline[500];
	char *temp, *tmp;
	char *iftok, *whtok;
	FILE *netfile, *dupnetfile;
	int count = 0;
	int index = 0;
	int i = 0, j = 0, k = 0;
	*linecount = 0;
	net_data *N = NULL;
	int lineindex = 0, linelength = 0;
	bool flaginput = false, flagoutput = false, flagregs = false, flagwires = false;
	bool inputsfound = false, outputsfound = false, regsfound = false, wiresfound = false;
	*inputcount = -1; *outputcount = -1; *regcount = -1; *wirecount = -1;
	int index_input = 0;
	bool index_mult = false;
	bool empty;
	int posinfile = 0;
	int nested_count = 0, nested_paran = 0;
	bool STATE_ASSIGNED = false;  

	netfile = fopen(netfilename, "r");
	if(netfile == NULL)
	{
		cout << "Unable to open the netlist file" << endl;
		exit(0);
	}
	if(netfile != NULL)
	{
		while (fgets(netline, sizeof(netline), netfile) != NULL)
		{	
			if(strlen(netline) == 1)
				continue;
			strcpy(newnetline, netline);
			if(newnetline[strlen(netline)-1] == 10)
				newnetline[strlen(netline)-1] = '\0';		
			temp = strtok(newnetline, " \t");
			empty = true;
			for(i=0; i<strlen(netline); i++)
			{
				if( (netline[i] == 32) || (netline[i] == 9) )
				{
					empty = true;
				}
				else
				{
					if(netline[i] != 10)
					{	
						empty = false;
						break;
					}
				}
			}
			if(empty == true)
			{
				continue;
			}
			if(strcmp(temp, "INPUTS") == 0)
			{
				if(inputsfound == true)
				{
					cout << "Multiple lines with input declaration" << endl;
					exit(0);
				}
				while(temp != NULL)
				{
					if(strcmp(temp,":") == 0)
					{
					}	
					else
					{
						(*inputcount)++;
					}
					temp = strtok(NULL, " \t");
				}
				inputsfound = true;
			}
			else if (strcmp(temp, "OUTPUTS") == 0)
			{
				if(outputsfound == true)
				{
					cout << "Multiple lines with output declaration" << endl;
					exit(0);
				}
				while(temp != NULL)
				{
					if(strcmp(temp,":") == 0)
					{
					}
					else
					{
						(*outputcount)++;
					}
					temp = strtok(NULL, " \t");
				}
				outputsfound = true;
			}
			else if (strcmp(temp, "REGS") == 0)
			{
				if(regsfound == true)
				{
					cout << "Multiple lines with reg declaration" << endl;
					exit(0);
				}	
				while(temp != NULL)
				{
					if(strcmp(temp,":") == 0)
					{	
					}
					else
					{
						(*regcount)++;
					}
					temp = strtok(NULL, " \t");
				}
				regsfound = true;
			}
			else if (strcmp(temp, "WIRES") == 0)
			{
				if(wiresfound == true)
				{
					cout << "Multiple lines with wire declaration" << endl;
					exit(0);
				}
				while(temp != NULL)
				{
					if(strcmp(temp,":") == 0)
					{
					}	
					else
					{
						(*wirecount)++;
					}
					temp = strtok(NULL, " \t");
				}
				wiresfound = true;
			}
			else
			{
				if( (strlen(netline) > 1) && (netline[0] != '/') && (netline[1] != '/') )
				{
					(*linecount)++;
				}
			}
		}
		
	}
	fclose(netfile);
	
	if((*inputcount) <= 0)
	{
		cout << "No inputs provided in the netlist file" << endl;
		exit(0);
	}
	
	if((*outputcount) <= 0)
	{
		cout << "No outputs provided in the netlist file" << endl;
		exit(0);
	}
	
	
	if(*wirecount == -1)
		*wirecount = 0;
	if(*regcount == -1)
		*regcount = 0;
	
	*INPUTS = (char **)malloc((*inputcount)*sizeof(char *));
	*OUTPUTS = (char **)malloc((*outputcount)*sizeof(char *));		
	*REGS = (char **)malloc((*regcount)*sizeof(char *));
	*WIRES = (char **)malloc((*wirecount)*sizeof(char *));	
	
	netfile = fopen(netfilename, "r");
	if(netfile == NULL)
	{
		cout << "Cannot open the netlist file" << endl;
		exit(0);
	}
	else if(netfile != NULL)
	{
		while (fgets(netline, sizeof(netline), netfile) != NULL)
		{
			if(strlen(netline) == 1)
				continue;
			strcpy(newnetline, netline);
			if(newnetline[strlen(netline)-1] == 10)
				newnetline[strlen(netline)-1] = '\0';
			temp = strtok(newnetline, " \t");
			empty = true;
			for(i=0; i<strlen(netline); i++)
			{
				if( (netline[i] == 32) || (netline[i] == 9) )
				{
					empty = true;
				}
				else
				{
					if(netline[i] != 10)
					{	
						empty = false;
						break;
					}
				}
			}
			if(empty == true)
			{
				continue;
			}
			if(strcmp(temp, "INPUTS") == 0)
			{
				while(temp != NULL)
				{
					if( (strcmp(temp,"INPUTS") == 0) || (strcmp(temp, ":") == 0) )
					{
					}
					else
					{
						(*INPUTS)[index] = (char *)malloc(strlen(temp)*sizeof(char));
						strcpy((*INPUTS)[index], temp);
						index++;							
					}
					temp = strtok(NULL, " \t");
				}
			}
			else if(strcmp(temp, "OUTPUTS") == 0)
			{
				while(temp != NULL)
				{
					if ((strcmp(temp, "OUTPUTS") == 0) || (strcmp(temp, ":") == 0))
					{
					}
					else
					{
						(*OUTPUTS)[index] = (char *)malloc(strlen(temp)*sizeof(char));
						strcpy((*OUTPUTS)[index], temp);
						index++; 
					}
					temp = strtok(NULL, " \t");
				}
			}
			else if(strcmp(temp, "REGS") == 0)
			{
				while(temp != NULL)
				{
					if ((strcmp(temp, "REGS") == 0) || (strcmp(temp, ":") == 0))
					{
					}
					else
					{
						(*REGS)[index] = (char *)malloc(strlen(temp)*sizeof(char));
						strcpy((*REGS)[index], temp);
						index++; 
					}
					temp = strtok(NULL, " \t");
				}
			}
			else if(strcmp(temp, "WIRES") == 0)
			{
				while(temp != NULL)
				{
					if ((strcmp(temp, "WIRES") == 0) || (strcmp(temp, ":") == 0))
					{
					}
					else
					{
						(*WIRES)[index] = (char *)malloc(strlen(temp)*sizeof(char));
						strcpy((*WIRES)[index], temp);
						index++; 
					}
					temp = strtok(NULL, " \t");
				}
			}
			index = 0;			
		}
	}

	fclose(netfile);

	N = (net_data *)malloc((*linecount)*sizeof(net_data));
	for(i=0; i<(*linecount); i++)
	{
		N[i].oper1 = NULL;
		N[i].oper2 = NULL;
		N[i].spoper = NULL;
		N[i].input = NULL;
		N[i].numinputs = -1;
		N[i].output = NULL;
		N[i].opername = NULL;
		N[i].ifstmt = NULL;
		N[i].endifpos = -1;
		N[i].type = NORMAL;
		N[i].stateid = -1;
		N[i].paran = NULL;
		N[i].gobacktowhile = -1;
	}
	
	netfile = fopen(netfilename, "r");
	index = 0;
	
	if(netfile == NULL)
	{
		cout << "Cannot open the netlist file" << endl;
		exit(0);
	}
	if(netfile != NULL)
	{
		while(fgets(netline, sizeof(netline), netfile) != NULL)
		{
			empty = true;
			strcpy(dupnetline, netline);
			for(i=0; i<strlen(netline); i++)
			{
				if( (netline[i] == 32) || (netline[i] == 9) )
				{
					empty = true;
				}
				else
				{
					if(netline[i] != 10)
					{	
						empty = false;
						break;
					}
				}
			}
			if(empty == true)
			{
				continue;
			}

			if(dupnetline[strlen(dupnetline)-1] == 10)
					dupnetline[strlen(dupnetline)-1] = '\0';
			temp = strtok(dupnetline, " \t");

			if((strcmp(temp, "INPUTS") == 0) || (strcmp(temp, "OUTPUTS") == 0) || (strcmp(temp, "WIRES") == 0) || (strcmp(temp, "REGS") == 0))
			{
				if(index_mult == true)
				{
					cout << "Invalid declaration location for " << temp << endl;
					exit(0);	
				}	
			}
			else if(strcmp(temp, "if") == 0)
			{
				strcpy(tripnetline, netline);
				iftok = strtok(tripnetline, " \t");
				while(iftok != NULL)
				{
					if( (strcmp(iftok, "if") == 0) || (strcmp(iftok, "(") == 0) ) 
					{
					}
					else
					{
						N[index].ifstmt = (char *)malloc(strlen(iftok)*sizeof(char));
						break;	
					}
					iftok = strtok(NULL, " \t");
				}
				strcpy(N[index].ifstmt, iftok);
				posinfile++;
				N[index].stateid = posinfile;
				N[index].type = IF;
				index++;
			}
			else if(strcmp(temp, "while") == 0)
			{
				strcpy(tripnetline, netline);
				whtok = strtok(tripnetline, " \t");
				while(whtok != NULL)
				{
					if( (strcmp(whtok, "while") == 0) || (strcmp(whtok, "(") == 0) ) 
					{
					}
					else
					{
						N[index].whilestmt = (char *)malloc(strlen(whtok)*sizeof(char));
						break;	
					}
					whtok = strtok(NULL, " \t");
				}
				strcpy(N[index].whilestmt, whtok);
				posinfile++;
				N[index].stateid = posinfile;
				N[index].type = WHILE;
				index++;
			}
			else if(strcmp(temp, "}") == 0)
			{
				N[index].paran = (char *)malloc(2*sizeof(char));
				strcpy(N[index].paran, "}");
				N[index].type = CLOSE;
				index++;
			}
			else if(strlen(netline) == 1)
			{
			}
			else if( (netline[0] == '/') && (netline[1] == '/') )
			{
			}
			else
			{	
				count++;
				linelength = 0;
				strcpy(newnetline, netline);
				if(newnetline[strlen(newnetline)-1] == 10)
					newnetline[strlen(newnetline)-1] = '\0';
				tmp = strtok(newnetline, " \t");	

				while(tmp != NULL)
				{
					linelength++;
					tmp = strtok(NULL, " \t");
				}
				if(linelength < 3)
				{
					//cout << "Error in netlist file: Unrecognized format" << endl;
					//exit(0);
				}
				else if(linelength == 3)	
				{
					posinfile++;
					index_mult = true;
					N[index].input = (char **)malloc(1*sizeof(char *));
					strcpy(newnetline, netline);
					if(newnetline[strlen(newnetline)-1] == 10)
						newnetline[strlen(newnetline)-1] = '\0';
					tmp = strtok(newnetline, " \t");
					lineindex = 0;
					while(tmp != NULL)
					{
						if(lineindex == 0)
						{
							N[index].output = (char *)malloc(strlen(tmp)*sizeof(char));
							strcpy(N[index].output, tmp);
						}
						else if(lineindex == 1)
						{
							if(strcmp(tmp, "=") != 0)
							{
								//cout << "Error in netlist file: format problem, missing = sign at node " << index << endl;
								//exit(0);
							}
							else
							{
								N[index].oper1 = (char *)malloc(2*sizeof(char));
								strcpy(N[index].oper1, tmp);
								N[index].spoper = (char *)malloc(2*sizeof(char));
								strcpy(N[index].spoper, tmp);
							}
						}
						else if(lineindex == 2)
						{
							N[index].input[0] = (char *)malloc(strlen(tmp)*sizeof(char));
							strcpy(N[index].input[0], tmp);
						}
						tmp = strtok(NULL, " \t");
						lineindex++;
					}
					N[index].numinputs = 1;
					//To store the index of the line of cfile which will be later used to determine the next state
					N[index].stateid = posinfile;    
				}
				else
				{
					posinfile++;
					index_mult = true;
					N[index].input = (char **)malloc(((linelength-1)/2)*sizeof(char *));
					N[index].numinputs = (linelength-1)/2;
					strcpy(newnetline, netline);
					if(newnetline[strlen(newnetline)-1] == 10)
						newnetline[strlen(newnetline)-1] = '\0';
					tmp = strtok(newnetline, " \t");
					lineindex = 0;
					index_input = 0;
					while(tmp != NULL)
					{
						if(lineindex == 0)
						{
							N[index].output = (char *)malloc(strlen(tmp)*sizeof(char));
							strcpy(N[index].output, tmp);
							
							for(i=0;i<(*outputcount);i++)
							{
								if(strcmp(N[index].output, (*OUTPUTS)[i]) == 0)
								{
									cout<< "Invalid operation in netlist file: Output " << N[index].output << " acts as a register" << endl;
									exit(0);
								}

							}

								
						}
						else if((lineindex%2) == 1)
						{
							if(lineindex == 1)
							{
								if(strcmp(tmp,"=") != 0)
								{
									//cout << "Error in the netlist file: format problem, missing = sign at node " << index << endl;
									//exit(0);
								}
								else
								{
									N[index].spoper = (char *)malloc(2*sizeof(char));
									strcpy(N[index].spoper, tmp);
								}
							}
							else if(lineindex == 3)
							{
								N[index].oper1 = (char *)malloc(2*sizeof(char));
								strcpy(N[index].oper1, tmp);	
							}
							else
							{
								if(strcmp(tmp,N[index].oper1) == 0)
								{
								}
								else if((strcmp(tmp, ":") == 0) && (strcmp(N[index].oper1,"?") == 0))
								{
									N[index].oper2 = (char *)malloc(1*sizeof(char));
									strcpy(N[index].oper2, tmp);
								}
								else
								{
									//cout << "Error in netlist file: Wrong combination of operators, ? should be followed by : " << endl;
									//exit(0);
								}
								
							}
						}
						else if(lineindex%2 == 0)
						{
							N[index].input[index_input] = (char *)malloc(strlen(tmp)*sizeof(char));
							strcpy(N[index].input[index_input], tmp);
							index_input++;	
						}
						tmp = strtok(NULL, " \t");
						lineindex++;
					}
					N[index].stateid = posinfile;   
				}
				index++;
			}

		}
	}
 
	fclose(netfile);

	// calculating the end position of an if statement
	for(i=0; i<(*linecount); i++)
	{
		nested_count = 0;
		if(N[i].type == IF)
		{
			for(j=i+1; j<(*linecount); j++)
			{
				if( (N[j].type == IF) || (N[j].type == WHILE) )
				{
					nested_count++;
				}
				if(N[j].type == CLOSE)
				{
					if(nested_count != 0)
						nested_count--;
					else if(nested_count == 0)
					{
						if(j == ((*linecount)-1))
						{
							N[i].endifpos = -100;
							break;
						}
						else
						{
							k = j+1;
							STATE_ASSIGNED = false;
							while(k < (*linecount))
							{
								if(N[k].type == NORMAL)
								{
									N[i].endifpos = N[k].stateid;
									STATE_ASSIGNED = true;
									break;
								}
								k++;
							}
							if(STATE_ASSIGNED == false)
							{
								N[i].endifpos = -100;
							}
							break;
						}
					}
				}	
			}
		}
		else if(N[i].type == WHILE)
		{
			for(j=i+1; j<(*linecount); j++)
			{
				if( (N[j].type == IF) || (N[j].type == WHILE) )
				{
					nested_count++;
				}
				if(N[j].type == CLOSE)
				{
					if(nested_count != 0)
						nested_count--;
					else if(nested_count == 0)
					{
						if(j == ((*linecount)-1))
						{
							N[i].endifpos = -100;
							break;
						}
						else
						{
							k = j+1;
							STATE_ASSIGNED = false;
							while(k < (*linecount))
							{
								if(N[k].type == NORMAL)
								{
									N[i].endwhilepos = N[k].stateid;
									STATE_ASSIGNED = true;
									break;
								}
								k++;
							}
							if(STATE_ASSIGNED == false)
							{
								N[i].endwhilepos = -100;
							}
							break;
						}
					}
				}	
			}
		}
	}

	// figuring out end statements of a while condition

	for(i=0; i<(*linecount); i++)
	{
		nested_paran = 0;
		if(i <= ( (*linecount)-2) )
		{
			if( (N[i].type == NORMAL) && (N[i+1].type == CLOSE) )
			{
				k = i-1;
				while(k>=0)
				{
					if(N[k].type == NORMAL)
					{
					}
					else if(N[k].type == CLOSE)
						nested_paran++;
					else if(N[k].type == WHILE)
					{
						if(nested_paran != 0)
							nested_paran--;
						else if(nested_paran == 0)
						{
							N[i].gobacktowhile = N[k].stateid;
							break;
						}
					}
					else if(N[k].type == IF)
					{
						if(nested_paran != 0)
							nested_paran--;
						else if(nested_paran == 0)
							break;
					}
					k--;
				}
			} 
		}
	}
	
	// Error checking for duplication of output node in the cfile 
	
	/*for(i=0; i<(*linecount); i++)
	{
		for(j=0; j<(*linecount); j++)
		{
			if(strcmp(N[i].output, N[j].output) == 0)
			{
				if(i != j)
				{
					//cout << "Error in netlist file: duplication of output node" << endl;
					//exit(0);	
				}
			}
		}
	}*/
	
	// Error checking for duplicacy across INPUTS, OUTPUTS, REGS, WIRES
	
	// Compare inputs with outputs
	for(i=0; i<(*inputcount); i++)
	{
		for(j=0; j<(*outputcount); j++)
		{
			if(strcmp((*INPUTS)[i], (*OUTPUTS)[j]) == 0)
			{
				cout << "Error in netlist file: Duplicate declaration between inputs and outputs" << endl;
				exit(0); 
			}
		}
	}

	// Compare inputs with regs
	for(i=0; i<(*inputcount); i++)
	{
		for(j=0; j<(*regcount); j++)
		{
			if(strcmp((*INPUTS)[i], (*REGS)[j]) == 0)
			{
				cout << "Error in netlist file: Duplicate declaration between inputs and registers" << (*INPUTS)[i] << endl;
				exit(0); 
			}
		}
	}
	
	// Compare inputs with wires
	for(i=0; i<(*inputcount); i++)
	{
		for(j=0; j<(*wirecount); j++)
		{
			if(strcmp((*INPUTS)[i], (*WIRES)[j]) == 0)
			{
				cout << "Error in netlist file: Duplicate declaration between inputs and wires" << endl;
				exit(0); 
			}
		}
	}

	// Compare outputs with regs
	for(i=0; i<(*outputcount); i++)
	{
		for(j=0; j<(*regcount); j++)
		{
			if(strcmp((*OUTPUTS)[i], (*REGS)[j]) == 0)
			{
				cout << "Error in netlist file: Duplicate declaration between outputs and registers" << endl;
				exit(0); 
			}
		}
	}

	// Compare outputs with wires
	for(i=0; i<(*outputcount); i++)
	{
		for(j=0; j<(*wirecount); j++)
		{
			if(strcmp((*OUTPUTS)[i], (*WIRES)[j]) == 0)
			{
				cout << "Error in netlist file: Duplicate declaration between outputs and wires" << endl;
				exit(0); 
			}
		}
	}

	// Compare regs with wires
	for(i=0; i<(*regcount); i++)
	{
		for(j=0; j<(*wirecount); j++)
		{
			if(strcmp((*REGS)[i], (*WIRES)[j]) == 0)
			{
				cout << "Error in netlist file: Duplicate declaration between registers and wires" << endl;
				exit(0); 
			}
		}
	}
 
	// Duplicate declarations of inputs
	for(i=0; i<(*inputcount); i++)
	{
		for(j=0; j<(*inputcount); j++)
		{
			if(strcmp((*INPUTS)[i],(*INPUTS)[j]) == 0)
			{
				if(i != j)
				{
					cout << "Error in netlist file: Duplicate declaration between inputs" << endl;
					exit(0);
				}
			}
		}
	}
	
	// Duplicate declarations of outputs
	for(i=0; i<(*outputcount); i++)
	{
		for(j=0; j<(*outputcount); j++)
		{
			if(strcmp((*OUTPUTS)[i],(*OUTPUTS)[j]) == 0)
			{
				if(i != j)
				{
					cout << "Error in netlist file: Duplicate declaration between outputs" << endl;
					exit(0);
				}
			}
		}
	}
	
	// Duplicate declarations of regs
	for(i=0; i<(*regcount); i++)
	{
		for(j=0; j<(*regcount); j++)
		{
			if(strcmp((*REGS)[i],(*REGS)[j]) == 0)
			{
				if(i != j)
				{
					cout << "Error in netlist file: Duplicate declaration between registers" << endl;
					exit(0);
				}
			}
		}
	}
	
	// Duplicate declarations of wires
	for(i=0; i<(*wirecount); i++)
	{
		for(j=0; j<(*wirecount); j++)
		{
			if(strcmp((*WIRES)[i],(*WIRES)[j]) == 0)
			{
				if(i != j)
				{
					cout << "Error in netlist file: Duplicate declaration between wires" << endl;
					exit(0);
				}
			}
		}
	}

	// Check if everything on the LHS of equations is either OUTPUT or REG or WIRE

	/*for(index=0; index<(*linecount); index++)
	{
		flagoutput = false;
		flagregs = false;
		flagwires = false;
		
		for(i=0; i<(*outputcount); i++)
		{
			if(N[index].output != NULL)
			{
				if(strcmp(N[index].output, (*OUTPUTS)[i]) == 0)
				{
					flagoutput = true;
					break;
				}
			}	
		}
		if(flagoutput == false)
		{
			for(i=0; i<(*regcount); i++)
			{
				if(N[index].output != NULL)
				{
					if(strcmp(N[index].output, (*REGS)[i]) == 0)
					{
						flagregs = true;
						break;
					}
				}
			}
	
		}
		if( (flagregs == false) && (flagoutput == false) )
		{
			for(i=0; i<(*wirecount); i++)
			{
				if(strcmp(N[index].output, (*WIRES)[i]) == 0)
				{
					flagwires = true;
					break;
				}
			}
	
		}
		if((flagoutput == false) && (flagregs == false) && (flagwires == false))
		{
			cout << "Error in netlist file: LHS side of node index " << index << " not an output or reg or wire variable" << endl;
			exit(0);
		}	
			
	}*/
	
	// Check if everything on the RHS of equations is either an input or reg or wire
	
	/*for(index=0; index<(*linecount); index++)
	{
		for(i=0; i<N[index].numinputs; i++)
		{
			flaginput = false;
			flagregs = false;
			flagwires = false;
			
			for(j=0; j<(*inputcount); j++)
			{
				if(strcmp(N[index].input[i], (*INPUTS)[j]) == 0)
				{
					flaginput = true;
					break;
				}
			}

			if(flaginput == false)
			{
				for(j=0; j<(*regcount); j++)
				{
					if(strcmp(N[index].input[i], (*REGS)[j]) == 0)
					{
						flagregs = true;
						break;
					}
				}
			}

			if((flaginput == false) && (flagregs == false))
			{
				for(j=0; j<(*wirecount); j++)
				{
					if(strcmp(N[index].input[i], (*WIRES)[j]) == 0)
					{
						flagwires = true;
						break;
					}
				}	
			}
			
			if((flaginput == false) && (flagregs == false) && (flagwires == false))
			{
				cout << "Error in netlist file: RHS side of node index " << index << " not an input or register or wire variable " << N[index].input[i] << endl;
				exit(0);	
			}		
		}
	}*/ 

	return N;
}
