#include "baro/bmp/bmp581.h"


BMP581::BMP581()
{
    // Nothing to do
}

int8_t BMP581::begin()
{
    // Variable to track errors returned by API calls
    int8_t err = BMP5_OK;

    // Initialize the sensor
    err = init();
    if(err != BMP5_OK)
    {
        return err;
    }

    // Enable both pressure and temperature sensors
    err = enablePress(BMP5_ENABLE);
    if(err != BMP5_OK)
    {
        return err;
    }

    // Set to normal mode
    return setMode(BMP5_POWERMODE_NORMAL);
}

int8_t BMP581::beginSPI(SPI_HandleTypeDef* hspi, GPIO_TypeDef* hgpio, uint16_t csPin)
{
    // Set up chip select pin
    interfaceData.hspi = hspi;
    interfaceData.hgpio = hgpio;
    interfaceData.csPin = csPin;

    // Set interface
    sensor.intf = BMP5_SPI_INTF;
    interfaceData.interface = BMP5_SPI_INTF;

    // Initialize sensor
    return begin();
}

int8_t BMP581::init()
{
    // Variable to track errors returned by API calls
    int8_t err = BMP5_OK;

    // Initialize config values
    osrOdrConfig = {0,0,0,0};
    fifo = {0,0,0,0,0,0,0,0,0};

    // Set helper function pointers
    sensor.read = readRegisters;
    sensor.write = writeRegisters;
    sensor.delay_us = usDelay;
    sensor.intf_ptr = &interfaceData;

    // Reset the sensor
    err = bmp5_soft_reset(&sensor);
    if(err != BMP5_OK)
    {
        return err;
    }

    // Initialize the sensor
    return bmp5_init(&sensor);
}

int8_t BMP581::setMode(bmp5_powermode mode)
{
    return bmp5_set_power_mode(mode, &sensor);
}

int8_t BMP581::getMode(bmp5_powermode* mode)
{
    return bmp5_get_power_mode(mode, &sensor);
}

int8_t BMP581::enablePress(uint8_t pressEnable)
{
    osrOdrConfig.press_en = pressEnable;
    return bmp5_set_osr_odr_press_config(&osrOdrConfig, &sensor);
}

int8_t BMP581::getSensorData(bmp5_sensor_data* data)
{
    return bmp5_get_sensor_data(data, &osrOdrConfig, &sensor);
}

int8_t BMP581::setODRFrequency(uint8_t odr)
{
    // Check whether ODR is valid
    if(odr > BMP5_ODR_0_125_HZ)
    {
        return BMP5_E_INVALID_SETTING;
    }

    // TODO - Check whether this ODR is compatible with current OSR
    
    osrOdrConfig.odr = odr;
    return bmp5_set_osr_odr_press_config(&osrOdrConfig, &sensor);
}

int8_t BMP581::getODRFrequency(uint8_t* odr)
{
    *odr = osrOdrConfig.odr;
    return BMP5_OK;
}

int8_t BMP581::setOSRMultipliers(bmp5_osr_odr_press_config* config)
{
    // Check whether OSR multipliers are valid
    if(config->osr_t > BMP5_OVERSAMPLING_128X
        || config->osr_p > BMP5_OVERSAMPLING_128X)
    {
        return BMP5_E_INVALID_SETTING;
    }

    // TODO - Check whether this OSR is compatible with current ODR
    
    osrOdrConfig.osr_t = config->osr_t;
    osrOdrConfig.osr_p = config->osr_p;
    return bmp5_set_osr_odr_press_config(&osrOdrConfig, &sensor);
}

int8_t BMP581::getOSRMultipliers(bmp5_osr_odr_press_config* config)
{
    config->osr_t = osrOdrConfig.osr_t;
    config->osr_p = osrOdrConfig.osr_p;
    return BMP5_OK;
}

int8_t BMP581::getOSREffective(bmp5_osr_odr_eff* osrOdrEffective)
{
    return bmp5_get_osr_odr_eff(osrOdrEffective, &sensor);
}

int8_t BMP581::setFilterConfig(bmp5_iir_config* iirConfig)
{
    return bmp5_set_iir_config(iirConfig, &sensor);
}

int8_t BMP581::setOORConfig(bmp5_oor_press_configuration* oorConfig)
{
    return bmp5_set_oor_configuration(oorConfig, &sensor);
}

int8_t BMP581::setInterruptConfig(BMP581_InterruptConfig* config)
{
    // Variable to track errors returned by API calls
    int8_t err = BMP5_OK;

    err = bmp5_configure_interrupt(config->mode, config->polarity, config->drive, config->enable, &sensor);
    if(err != BMP5_OK)
    {
        return err;
    }

    return bmp5_int_source_select(&config->sources, &sensor);
}

int8_t BMP581::getInterruptStatus(uint8_t* status)
{
    return bmp5_get_interrupt_status(status, &sensor);
}

int8_t BMP581::setFIFOConfig(bmp5_fifo* fifoConfig)
{
    // Variable to track errors returned by API calls
    int8_t err = BMP5_OK;

    // Copy desired config
    memcpy(&fifo, fifoConfig, sizeof(bmp5_fifo));

    // The sensor must be in stanby mode for the FIFO config to
    // be set, grab the current power mode
    bmp5_powermode originalMode;
    err = getMode(&originalMode);
    if(err != BMP5_OK)
    {
        return err;
    }

    // Now set to standby
    err = setMode(BMP5_POWERMODE_STANDBY);
    if(err != BMP5_OK)
    {
        return err;
    }

    // Now we can set the FIFO config
    err = bmp5_set_fifo_configuration(&fifo, &sensor);
    if(err != BMP5_OK)
    {
        return err;
    }

    // Finally, set back to original power mode
    return setMode(originalMode);
}

int8_t BMP581::getFIFOLength(uint8_t* numData)
{
    // Variable to track errors returned by API calls
    int8_t err = BMP5_OK;

    uint16_t numBytes = 0;
    err = bmp5_get_fifo_len(&numBytes, &fifo, &sensor);
    if(err != BMP5_OK)
    {
        return err;
    }

    // Determine number of bytes per sample
    uint8_t bytesPerSample = 1;
    if(fifo.frame_sel == BMP5_FIFO_PRESS_TEMP_DATA)
    {
        bytesPerSample = 6;
    }
    else if((fifo.frame_sel == BMP5_FIFO_TEMPERATURE_DATA) || (fifo.frame_sel == BMP5_FIFO_PRESSURE_DATA))
    {
        bytesPerSample = 3;
    }

    // Compute number of samples in the FIFO buffer
    *numData = numBytes / bytesPerSample;

    return BMP5_OK;
}

int8_t BMP581::getFIFOData(bmp5_sensor_data* data, uint8_t numData)
{
    // Variable to track errors returned by API calls
    int8_t err = BMP5_OK;

    // Determine the number of bytes per data frame, 3 bytes per sensor
    uint8_t bytesPerFrame;
    if(fifo.frame_sel == BMP5_FIFO_TEMPERATURE_DATA
        || fifo.frame_sel == BMP5_FIFO_PRESSURE_DATA)
    {
        bytesPerFrame = 3;
    }
    else
    {
        bytesPerFrame = 6;
    }

    // Set number of bytes to read based on user's buffer size. If this is larger
    // than the number of bytes in the buffer, bmp5_get_fifo_data() will
    // automatically limit it
    fifo.length = numData * bytesPerFrame;

    // Create buffer to hold all bytes of FIFO buffer
    uint8_t byteBuffer[fifo.length];
    fifo.data = byteBuffer;

    // Get all bytes out of FIFO buffer
    err = bmp5_get_fifo_data(&fifo, &sensor);
    if(err != BMP5_OK)
    {
        return err;
    }

    // Parse raw data into temperature and pressure data
    return bmp5_extract_fifo_data(&fifo, data);
}

int8_t BMP581::flushFIFO()
{
    // Variable to track errors returned by API calls
    int8_t err = BMP5_OK;

    // There isn't a simple way to flush the FIFO buffer unfortunately. However the
    // FIFO is automatically flushed when certain config settings change, such as
    // the power mode. We can simply change the power mode twice to flush the buffer

    // Grab the current power mode
    bmp5_powermode originalMode;
    err = getMode(&originalMode);
    if(err != BMP5_OK)
    {
        return err;
    }

    // Change the power mode to something else
    if(originalMode != BMP5_POWERMODE_STANDBY)
    {
        // Sensor is not in standby mode, so default to that
        err = setMode(BMP5_POWERMODE_STANDBY);
        if(err != BMP5_OK)
        {
            return err;
        }
    }
    else
    {
        // Already in standby, switch to forced instead
        err = setMode(BMP5_POWERMODE_FORCED);
        if(err != BMP5_OK)
        {
            return err;
        }
    }

    // Finally, set back to original power mode
    return setMode(originalMode);
}

int8_t BMP581::readNVM(uint8_t addr, uint16_t* data)
{
    return bmp5_nvm_read(addr, data, &sensor);
}

int8_t BMP581::writeNVM(uint8_t addr, uint16_t data)
{
    return bmp5_nvm_write(addr, &data, &sensor);
}

BMP5_INTF_RET_TYPE BMP581::readRegisters(uint8_t regAddress, uint8_t* dataBuffer, uint32_t numBytes, void* interfacePtr)
{
    // Make sure the number of bytes is valid
    if(numBytes == 0)
    {
        return BMP5_E_COM_FAIL;
    }

    // Get interface data
    BMP581_InterfaceData* interfaceData = (BMP581_InterfaceData*) interfacePtr;

    switch(interfaceData->interface)
    {
        case BMP5_I2C_INTF:
            return BMP5_E_COM_FAIL; // I2C is not used

        case BMP5_SPI_INTF:
        {
            uint8_t data[1] = {regAddress | 0x80};
            HAL_GPIO_WritePin(interfaceData->hgpio, interfaceData->csPin, GPIO_PIN_RESET);
            if(HAL_SPI_Transmit(interfaceData->hspi, data, sizeof(data), HAL_MAX_DELAY) != HAL_OK){
                HAL_GPIO_WritePin(interfaceData->hgpio, interfaceData->csPin, GPIO_PIN_SET);
                return BMP5_E_COM_FAIL;
            }
            if(HAL_SPI_Receive(interfaceData->hspi, dataBuffer, numBytes, HAL_MAX_DELAY) != HAL_OK){
                HAL_GPIO_WritePin(interfaceData->hgpio, interfaceData->csPin, GPIO_PIN_SET);
                return BMP5_E_COM_FAIL;
            }
            HAL_GPIO_WritePin(interfaceData->hgpio, interfaceData->csPin, GPIO_PIN_SET);
            break;
        }

        case BMP5_I3C_INTF:
            return BMP5_E_COM_FAIL; // I3C is not supported here
    }

    return BMP5_OK;
}

BMP5_INTF_RET_TYPE BMP581::writeRegisters(uint8_t regAddress, const uint8_t* dataBuffer, uint32_t numBytes, void* interfacePtr)
{
    // Make sure the number of bytes is valid
    if(numBytes == 0)
    {
        return BMP5_E_COM_FAIL;
    }
    // Get interface data
    BMP581_InterfaceData* interfaceData = (BMP581_InterfaceData*) interfacePtr;

    // Determine which interface we're using
    switch(interfaceData->interface)
    {
        case BMP5_I2C_INTF:
            return BMP5_E_COM_FAIL;

        case BMP5_SPI_INTF:
        {
            uint16_t i;
            HAL_GPIO_WritePin(interfaceData->hgpio, interfaceData->csPin, GPIO_PIN_RESET);
            for(i = 0; i < numBytes; i++){
                uint8_t data[2] = {regAddress, dataBuffer[i]};
                if(HAL_SPI_Transmit(interfaceData->hspi, data, sizeof(data), HAL_MAX_DELAY) != HAL_OK){
                    HAL_GPIO_WritePin(interfaceData->hgpio, interfaceData->csPin, GPIO_PIN_SET);
                    return BMP5_E_COM_FAIL;
                }
            }
            HAL_GPIO_WritePin(interfaceData->hgpio, interfaceData->csPin, GPIO_PIN_SET);
            break;
        }

        case BMP5_I3C_INTF:
            return BMP5_E_COM_FAIL; // I3C is not supported here
    }

    return BMP5_OK;
}

void Delay_us(__IO uint32_t nCount)
{
	if(nCount>1){
			uint32_t count=nCount*8-6;
			while(count--);
	}
	else{
		uint32_t count=2;
		while(count--);
	}
}

void BMP581::usDelay(uint32_t period, void* interfacePtr)
{
    Delay_us(period);
}
