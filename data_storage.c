#include "data_storage.h"


/*
    This storage replace 
 */

void data_write(unsigned char address, unsigned char value){
    //eeprom_write(address,value);
    data_storage[address]=value;
};

unsigned char data_read(unsigned char address){
    //return eeprom_read(address);
    return data_storage[address];
};

void read_data_from_eeprom(void){
    for (unsigned char i=0;i<6;i++){
        data_storage[i] = eeprom_read(i);
    };
};
void save_data_to_eeprom(void){
    for (unsigned char i=0;i<6;i++){
        eeprom_write(i,data_storage[i]);
    };
};