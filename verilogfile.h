#ifndef VERILOGFILE_H
#define VERILOGFILE_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse_net.h"

//Create a void function to create the verilog file for the given netlist file
void verilog_file(char *verilogfile, net_data **N, int len_net_file, char ***INPUTS, int inputcount, char ***OUTPUTS, int outputcount, char ***REGS, int regcount, char ***WIRES, int wirecount);

#endif
