/* 
 * File:   data_storage.h
 * Author: user
 *
 * Created on May 13, 2020, 9:22 AM
 */

#ifndef DATA_STORAGE_H
#define	DATA_STORAGE_H

unsigned char data_storage[6];   //array for mirroring eeprom data


void data_write(unsigned char address, unsigned char value);
unsigned char data_read(unsigned char address);

void eeprom_write(unsigned char address, unsigned char value);
unsigned char eeprom_read(unsigned char address);

void read_data_from_eeprom(void);
void save_data_to_eeprom(void);

#endif	/* DATA_STORAGE_H */

