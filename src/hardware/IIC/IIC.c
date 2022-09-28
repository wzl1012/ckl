#include "headfile.h"
 
void i2c_init(uint32_t i2cdev)
{
	rcu_periph_clock_enable(RCU_I2C1);
    /* I2C clock configure */
    i2c_clock_config(i2cdev, 100000, I2C_DTCY_2);
    /* I2C address configure */
    i2c_mode_addr_config(i2cdev, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, I2C0_SLAVE_ADDRESS7);
    /* enable I2C0 */
    i2c_enable(i2cdev);
    /* enable acknowledge */
    i2c_ack_config(i2cdev, I2C_ACK_ENABLE);
}

void i2c_write(uint32_t i2cdev,uint8_t *data,uint8_t bytes)
{
	 /* wait until I2C bus is idle */
    while(i2c_flag_get(i2cdev, I2C_FLAG_I2CBSY));
    /* send a start condition to I2C bus */
    i2c_start_on_bus(i2cdev);
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(i2cdev, I2C_FLAG_SBSEND));
    /* send slave address to I2C bus */
    i2c_master_addressing(i2cdev, I2C0_SLAVE_ADDRESS7, I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(i2cdev, I2C_FLAG_ADDSEND));
    /* clear ADDSEND bit */
    i2c_flag_clear(i2cdev, I2C_FLAG_ADDSEND);
    /* wait until the transmit data buffer is empty */
    while(!i2c_flag_get(i2cdev, I2C_FLAG_TBE));
    for(uint8_t i=0; i<bytes; i++){
        /* data transmission */
        i2c_data_transmit(i2cdev, data[i]);
        /* wait until the TBE bit is set */
        while(!i2c_flag_get(i2cdev, I2C_FLAG_TBE));
    }
    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(i2cdev);
    while(I2C_CTL0(i2cdev)&0x0200);	
	
}

