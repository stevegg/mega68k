#ifndef __UTILS_H
#define __UTILS_H

void allocateMemory();
unsigned long getAddress();
word getDataBus();
void setDataBus(word data);
word getROMorRAM(word address);
void setRAM(word address, word data);

#endif