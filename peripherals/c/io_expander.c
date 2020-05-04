#include "io_expander.h"

void io_expander_write_reg(uint8_t reg, uint8_t data){
	i2c_status_t status;
  
	// set device to idle
  while ( I2CMasterBusy(IO_EXPANDER_I2C_BASE)) {};
		
  
  // set the slave address 
	status = i2cSetSlaveAddr(IO_EXPANDER_I2C_BASE, MCP23017_DEV_ID, I2C_WRITE);

  // send a byte of the address

	status = i2cSendByte(IO_EXPANDER_I2C_BASE, reg, I2C_MCS_START | I2C_MCS_RUN);

  // send data
	status = i2cSendByte(IO_EXPANDER_I2C_BASE, data, I2C_MCS_RUN | I2C_MCS_STOP);
}

uint8_t io_expander_read_reg(uint8_t reg){
	uint8_t data;
  i2c_status_t status;
  
  //idle device
  while ( I2CMasterBusy(IO_EXPANDER_I2C_BASE)) {};

   // set the slave address 
	status = i2cSetSlaveAddr(IO_EXPANDER_I2C_BASE, MCP23017_DEV_ID, I2C_WRITE);

  // send a byte of the address
	status = i2cSendByte(IO_EXPANDER_I2C_BASE, reg, I2C_MCS_START | I2C_MCS_RUN);

  //set device address
	status = i2cSetSlaveAddr(IO_EXPANDER_I2C_BASE, MCP23017_DEV_ID, I2C_READ);	

	//read address
	 status = i2cGetByte(IO_EXPANDER_I2C_BASE, &data, I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_STOP);
	
		return data;
}


i2c_status_t io_gpio_init()
{  
	i2c_status_t status;
  
	//idle device
  while ( I2CMasterBusy(IO_EXPANDER_I2C_BASE)) {};
		
  //set slave address
	status = i2cSetSlaveAddr(IO_EXPANDER_I2C_BASE, MCP23017_DEV_ID, I2C_WRITE);
  if (status != I2C_OK)
		return status;
	
	//LED set up
	status = i2cSendByte(IO_EXPANDER_I2C_BASE, MCP23017_IODIRA_R, I2C_MCS_START | I2C_MCS_RUN);
	 if (status != I2C_OK)
		return status;
	
	//output
	status = i2cSendByte(IO_EXPANDER_I2C_BASE, 0x00, I2C_MCS_RUN | I2C_MCS_STOP);
	if (status != I2C_OK)
		return status;

	//push buttons init
	status = i2cSendByte(IO_EXPANDER_I2C_BASE, MCP23017_IODIRB_R, I2C_MCS_START | I2C_MCS_RUN);
	 if (status != I2C_OK)
		return status;
	
  //input
	status = i2cSendByte(IO_EXPANDER_I2C_BASE, 0xF, I2C_MCS_RUN | I2C_MCS_STOP);
	if (status != I2C_OK)
		return status;
	
	//send pull up
	status = i2cSendByte(IO_EXPANDER_I2C_BASE, MCP23017_GPPUB_R, I2C_MCS_START | I2C_MCS_RUN);
	 if (status != I2C_OK)
		return status;
	
  //set pull up
	status = i2cSendByte(IO_EXPANDER_I2C_BASE, 0xF, I2C_MCS_RUN | I2C_MCS_STOP);
	if (status != I2C_OK)
		return status;
	
	//send interrupt on byte change
	status = i2cSendByte(IO_EXPANDER_I2C_BASE, MCP23017_GPINTENB_R, I2C_MCS_START | I2C_MCS_RUN);
	 if (status != I2C_OK)
		return status;
	
  //set interrupt on byte change
	status = i2cSendByte(IO_EXPANDER_I2C_BASE, 0xF, I2C_MCS_RUN | I2C_MCS_STOP);
	if (status != I2C_OK)
		return status;
	
	//send change of default byte
	status = i2cSendByte(IO_EXPANDER_I2C_BASE, MCP23017_INTCONB_R, I2C_MCS_START | I2C_MCS_RUN);
	 if (status != I2C_OK)
		return status;
  
  //set intterupt on byte change
	status = i2cSendByte(IO_EXPANDER_I2C_BASE, 0xFF, I2C_MCS_RUN | I2C_MCS_STOP);
	if (status != I2C_OK)
		return status;
	
	//send default byte
	status = i2cSendByte(IO_EXPANDER_I2C_BASE, MCP23017_DEFVALB_R, I2C_MCS_START | I2C_MCS_RUN);
	 if (status != I2C_OK)
		return status;
	
    //set default
	status = i2cSendByte(IO_EXPANDER_I2C_BASE, 0x00, I2C_MCS_RUN | I2C_MCS_STOP);
	if (status != I2C_OK)
		return status;
	
  return I2C_OK;
}

//*****************************************************************************
// Initialize the I2C peripheral
//*****************************************************************************
bool io_expander_init(void)
{ 
	//==============================
  // Configure I2C GPIO Pins
  //==============================  
  if(gpio_enable_port(IO_EXPANDER_GPIO_BASE) == false)
    return false;
  if(gpio_enable_port(IO_EXPANDER_IRQ_GPIO_BASE) == false)
		return false;
	
  // Configure SCL 
  if(gpio_config_digital_enable(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SCL_PIN)== false)
    return false;   
  if(gpio_config_alternate_function(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SCL_PIN)== false)
    return false;
  if(gpio_config_port_control(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SCL_PCTL_M, IO_EXPANDER_I2C_SCL_PIN_PCTL)== false)
    return false;
   
  // Configure SDA 
  if(gpio_config_digital_enable(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SDA_PIN)== false)
    return false;
  if(gpio_config_open_drain(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SDA_PIN)== false)
    return false;
  if(gpio_config_alternate_function(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SDA_PIN)== false)
    return false;
  if(gpio_config_port_control(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SDA_PCTL_M, IO_EXPANDER_I2C_SDA_PIN_PCTL)== false)
    return false;
  
  //  Initialize the TIVA board to be master
  if( initializeI2CMaster(IO_EXPANDER_I2C_BASE)!= I2C_OK)
    return false;
	
	i2cSetSlaveAddr(I2C1_BASE, MCP23017_DEV_ID, I2C_WRITE);
	io_expander_write_reg(MCP23017_IODIRA_R, 0x00);
	io_expander_write_reg(MCP23017_IODIRB_R, 0xFF);
	io_expander_write_reg(MCP23017_IOCONB_R, 0);
	io_expander_write_reg(MCP23017_GPINTENB_R, 0xF);
	io_expander_write_reg(MCP23017_GPPUB_R, 0x0F);
	
  gpio_config_digital_enable(IO_EXPANDER_IRQ_GPIO_BASE, IO_EXPANDER_IRQ_PIN_NUM);
  gpio_config_enable_pullup(IO_EXPANDER_IRQ_GPIO_BASE, IO_EXPANDER_IRQ_PIN_NUM);
	if(!gpio_config_falling_edge_irq(GPIOF_BASE, PF0)) 
		return false;
	NVIC_SetPriority(GPIOF_IRQn, 1);
	NVIC_EnableIRQ(GPIOF_IRQn);
	
	// Read cap register in every initialization to clear interrupt
	io_expander_read_reg(MCP23017_INTCAPB_R);

  printf("Finished config\n\r");
  return true;
}

