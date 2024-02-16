/*
 *
 * Copyright (c) [2024] by InvenSense, Inc.
 * 
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */
 
#include "Arduino.h"
#include "ICP101xx.h"
#include <math.h>

static int i2c_write(void * ctx, uint8_t reg, const uint8_t * wbuffer, uint32_t wlen);
static int i2c_read(void * ctx, uint8_t reg, uint8_t * rbuffer, uint32_t rlen);

#define ICP101xx_I2C_SPEED 400000
#define ICP101xx_MEAS_TIMEOUT_MS 25

// ICP101xx constructor for I2c interface
ICP101xx::ICP101xx(TwoWire &i2c_ref, bool address_bit) {
  i2c = &i2c_ref; 
  i2c_address = ICP101XX_I2C_ADDR + (address_bit ? 1 : 0);
}

ICP101xx::ICP101xx(TwoWire &i2c_ref) {
  i2c = &i2c_ref; 
  i2c_address = ICP101XX_I2C_ADDR;
}

/* starts communication with the ICP101xx */
int ICP101xx::begin() {
  inv_icp101xx_serif icp101xx_serif;
  int rc = 0;
  uint8_t who_am_i;
  uint8_t icp_version;

    i2c->begin();
    i2c->setClock(ICP101xx_I2C_SPEED);
	
	icp101xx_serif.context   = this;
	icp101xx_serif.read_reg  = i2c_read;
	icp101xx_serif.write_reg = i2c_write;
	icp101xx_serif.max_read  = 2048; /* maximum number of bytes allowed per serial read */
	icp101xx_serif.max_write = 2048; /* maximum number of bytes allowed per serial write */
	icp101xx_serif.is_spi    = 0;

	/*
	 * Reset pressure sensor driver states
	 */
	inv_icp101xx_reset_states(&icp_device, &icp101xx_serif);

	rc = inv_icp101xx_soft_reset(&icp_device);
	if (rc != 0) {
      return rc;
    }
	inv_icp101xx_init(&icp_device);
	if (rc != 0) {
      return rc;
    }
  
  // successful init, return 0
  return 0;
}

int ICP101xx::start(void) {
  int rc = 0;
  inv_icp101xx_enable_sensor(&icp_device, 1);
  return rc;
}


int ICP101xx::getData(float& pressure, float& temperature) {
	int rc = 0;
	int raw_pressure, raw_temp;
	unsigned long timeout = millis();
	
	do{
		rc = inv_icp101xx_get_data(&icp_device,&raw_pressure, &raw_temp, &pressure, &temperature);
	} while ((rc != 0) && ((millis() - timeout) > ICP101xx_MEAS_TIMEOUT_MS));
	(void)raw_pressure;
	(void)raw_temp;
	pressure = round(pressure);
	temperature = round(temperature*10.0)/10.0;
	return rc;
}


static int i2c_write(void * ctx, uint8_t reg, const uint8_t * wbuffer, uint32_t wlen) {
  ICP101xx* obj = (ICP101xx*)ctx;
  obj->i2c->beginTransmission(obj->i2c_address);
  for(uint8_t i = 0; i < wlen; i++) {
    obj->i2c->write(wbuffer[i]);
  }
  obj->i2c->endTransmission();
  return 0;
}

static int i2c_read(void * ctx, uint8_t reg, uint8_t * rbuffer, uint32_t rlen) {
  ICP101xx* obj = (ICP101xx*)ctx;
  uint16_t rx_bytes = 0;

  obj->i2c->beginTransmission(obj->i2c_address);
  rx_bytes = obj->i2c->requestFrom(obj->i2c_address, rlen);
  if (rlen == rx_bytes) {
    for(uint8_t i = 0; i < rx_bytes; i++) {
      rbuffer[i] = obj->i2c->read();
    }
    obj->i2c->endTransmission(false);
    return 0;
  } else {
    return -1;
  }
}
