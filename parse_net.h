#ifndef PARSE_NET_H
#define PARSE_NET_H

#include<iostream>
#define NORMAL 0
#define IF 1
#define WHILE 2
#define CLOSE 3

struct net_data
{
	char *oper1, *oper2;
	char *spoper;
	char **input;
	int numinputs;
	char *output;
	char *opername;
	char *ifstmt;
	char *whilestmt;
	int endifpos;
	int endwhilepos;
	int type;
	int stateid;
	char *paran;
	int gobacktowhile;
};

net_data* parse_net(char *nfilename, int *nlen, char ***INPUTS, char ***OUTPUTS, char ***REGS, char ***WIRES, int *inputcount, int *outputcount, int *regcount, int *wirecount);

#endif
