#ifndef __UTILS_H
#define __UTILS_H

// 2k of Memory
#define RAM_SIZE 2*1024
#define RAM_BASE 0x8000

#ifdef DATA_BUS_WIDTH_16
typedef word data_t;
#else
typedef byte data_t;
#endif

void allocateMemory();
unsigned long getAddress();
data_t getDataBus();
void setDataBus(data_t data);
data_t getROMorRAM(word address);
void setRAM(word address, data_t data);
void setD38(boolean set);

#endif