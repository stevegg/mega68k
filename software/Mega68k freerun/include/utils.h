#ifndef __UTILS_H
#define __UTILS_H

unsigned long getAddress();
word getDataBus();
void setDataBus(word data);
word getROMorRAM(word address);
void setRAM(word address, word data);

#endif