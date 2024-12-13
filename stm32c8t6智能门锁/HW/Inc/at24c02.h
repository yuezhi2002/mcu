#ifndef __AT24C02_H
#define __AT24C02_H

void at24c02_init();
void at24c02_writepage(SP,data);
uint32_t at24c02_readpage(SP);

#endif
