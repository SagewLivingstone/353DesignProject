#include "io_expander.h"

void io_expander_write_data(uint8_t reg, uint8_t data)
{
	i2c_status_t status;
  
	// Before doing anything, make sure the I2C device is idle
  while ( I2CMasterBusy(IO_EXPANDER_I2C_BASE)) {};
		
  //==============================================================
  // Set the I2C device address 
  //==============================================================
	status = i2cSetSlaveAddr(IO_EXPANDER_I2C_BASE, MCP23017_DEV_ID, I2C_WRITE);

	//==============================================================
  // Send the byte of the address
  //==============================================================
	status = i2cSendByte(
			 IO_EXPANDER_I2C_BASE,
			 reg,
			 I2C_MCS_START | I2C_MCS_RUN
	);

  //==============================================================
  // Send data
  //==============================================================
	status = i2cSendByte(
			 IO_EXPANDER_I2C_BASE,
			 data,
			 I2C_MCS_RUN | I2C_MCS_STOP
	);
}

uint8_t io_expander_read_data(uint8_t reg)
{
	uint8_t data;
  i2c_status_t status;
  
  // Before doing anything, make sure the I2C device is idle
  while ( I2CMasterBusy(IO_EXPANDER_I2C_BASE)) {};

  //==============================================================
  // Set the I2C device address 
  //==============================================================
	status = i2cSetSlaveAddr(IO_EXPANDER_I2C_BASE, MCP23017_DEV_ID, I2C_WRITE);

	//==============================================================
  // Send the byte of the address
  //==============================================================
	status = i2cSendByte(
			 IO_EXPANDER_I2C_BASE,
			 reg,
			 I2C_MCS_START | I2C_MCS_RUN
	);

  //==============================================================
  // Set the I2C device address 
  //==============================================================
	status = i2cSetSlaveAddr(IO_EXPANDER_I2C_BASE, MCP23017_DEV_ID, I2C_READ);	

	// READ from ADDR
	 status = i2cGetByte(IO_EXPANDER_I2C_BASE, &data, I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_STOP);
	
		return data;
}


// LEDs -> GPIOA and BTN -> GPIOB
i2c_status_t io_gpio_init()
{  
	i2c_status_t status;
  
	// Before doing anything, make sure the I2C device is idle
  while ( I2CMasterBusy(IO_EXPANDER_I2C_BASE)) {};
		
  //==============================================================
  // Set the I2C device address 
  //==============================================================
	status = i2cSetSlaveAddr(IO_EXPANDER_I2C_BASE, MCP23017_DEV_ID, I2C_WRITE);
  if (status != I2C_OK)
		return status;
	
	//========================SET UP LEDs (GPIOA)===================
	//==============================================================
  // Send the byte of the address
  //==============================================================
	status = i2cSendByte(
			 IO_EXPANDER_I2C_BASE,
			 MCP23017_IODIRA_R,
			 I2C_MCS_START | I2C_MCS_RUN
	);
	 if (status != I2C_OK)
		return status;
	
  //==============================================================
  // Set as OUTPUT
  //==============================================================
	status = i2cSendByte(
			 IO_EXPANDER_I2C_BASE,
			 0x00,
			 I2C_MCS_RUN | I2C_MCS_STOP
	);
	if (status != I2C_OK)
		return status;

	//========================SET UP Push Buttons (GPIOB)===========
	//==============================================================
  // Send the direction byte of the address
  //==============================================================
	status = i2cSendByte(
			 IO_EXPANDER_I2C_BASE,
			 MCP23017_IODIRB_R,
			 I2C_MCS_START | I2C_MCS_RUN
	);
	 if (status != I2C_OK)
		return status;
	
  //==============================================================
  // Set as INPUT
  //==============================================================
	status = i2cSendByte(
			 IO_EXPANDER_I2C_BASE,
			 0xF,
			 I2C_MCS_RUN | I2C_MCS_STOP
	);
	if (status != I2C_OK)
		return status;
	//==============================================================
  // Send the pull up byte of the address
  //==============================================================
	status = i2cSendByte(
			 IO_EXPANDER_I2C_BASE,
			 MCP23017_GPPUB_R,
			 I2C_MCS_START | I2C_MCS_RUN
	);
	 if (status != I2C_OK)
		return status;
	
  //==============================================================
  // Set as PULL UP
  //==============================================================
	status = i2cSendByte(
			 IO_EXPANDER_I2C_BASE,
			 0xF,
			 I2C_MCS_RUN | I2C_MCS_STOP
	);
	if (status != I2C_OK)
		return status;
	//==============================================================
  // Send the interrupt on change byte of the address
  //==============================================================
	status = i2cSendByte(
			 IO_EXPANDER_I2C_BASE,
			 MCP23017_GPINTENB_R,
			 I2C_MCS_START | I2C_MCS_RUN
	);
	 if (status != I2C_OK)
		return status;
	
  //==============================================================
  // Set as interrupt on change
  //==============================================================
	status = i2cSendByte(
			 IO_EXPANDER_I2C_BASE,
			 0xF,
			 I2C_MCS_RUN | I2C_MCS_STOP
	);
	if (status != I2C_OK)
		return status;
	//==============================================================
  // Send the change on defualt byte of the address
  //==============================================================
	status = i2cSendByte(
			 IO_EXPANDER_I2C_BASE,
			 MCP23017_INTCONB_R,
			 I2C_MCS_START | I2C_MCS_RUN
	);
	 if (status != I2C_OK)
		return status;
  //==============================================================
  // Set as interrupt on change of default
  //==============================================================
	status = i2cSendByte(
			 IO_EXPANDER_I2C_BASE,
			 0xFF,
			 I2C_MCS_RUN | I2C_MCS_STOP
	);
	if (status != I2C_OK)
		return status;
	//==============================================================
  // Send the defualt val byte of the address
  //==============================================================
	status = i2cSendByte(
			 IO_EXPANDER_I2C_BASE,
			 MCP23017_DEFVALB_R,
			 I2C_MCS_START | I2C_MCS_RUN
	);
	 if (status != I2C_OK)
		return status;
  //==============================================================
  // Set the defualt
  //==============================================================
	status = i2cSendByte(
			 IO_EXPANDER_I2C_BASE,
			 0xFF,
			 I2C_MCS_RUN | I2C_MCS_STOP
	);
	if (status != I2C_OK)
		return status;
	
  return I2C_OK;
}

//*****************************************************************************
// Initialize the I2C peripheral
//*****************************************************************************
bool io_expander_init(void)
{
  
  if(gpio_enable_port(IO_EXPANDER_GPIO_BASE) == false)
  {
    return false;
  }
  
  // Configure SCL 
  if(gpio_config_digital_enable(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SCL_PIN)== false)
  {
    return false;
  }
    
  if(gpio_config_alternate_function(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SCL_PIN)== false)
  {
    return false;
  }
    
  if(gpio_config_port_control(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SCL_PCTL_M, IO_EXPANDER_I2C_SCL_PIN_PCTL)== false)
  {
    return false;
  }
    

  
  // Configure SDA 
  if(gpio_config_digital_enable(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SDA_PIN)== false)
  {
    return false;
  }
    
  if(gpio_config_open_drain(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SDA_PIN)== false)
  {
    return false;
  }
    
  if(gpio_config_alternate_function(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SDA_PIN)== false)
  {
    return false;
  }
    
  if(gpio_config_port_control(IO_EXPANDER_GPIO_BASE, IO_EXPANDER_I2C_SDA_PCTL_M, IO_EXPANDER_I2C_SDA_PIN_PCTL)== false)
  {
    return false;
  }
    
  //  Initialize the I2C peripheral
  if( initializeI2CMaster(IO_EXPANDER_I2C_BASE)!= I2C_OK)
  {
    return false;
  }
  
	return (io_gpio_init() == I2C_OK);
}

