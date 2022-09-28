#ifndef IIC_H
#define IIC_H
#include "headfile.h"

void i2c_init(uint32_t i2cdev);
void i2c_write(uint32_t i2cdev,uint8_t *data,uint8_t bytes);

#endif




