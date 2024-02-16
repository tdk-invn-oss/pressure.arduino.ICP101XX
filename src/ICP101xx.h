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
 
#ifndef ICP101xx_H
#define ICP101xx_H

#include "Arduino.h"
#include "SPI.h"
#include "Wire.h"
#include <stdint.h>

extern "C" {
#include "Devices/Drivers/ICP101xx/Icp101xx.h"
}

class ICP101xx {
  public:
    ICP101xx(TwoWire &i2c);
    ICP101xx(TwoWire &i2c,bool address_bit);
    int begin();
    int start(void);
    int getData(float& pressure, float& temperature);
    struct inv_icp101xx icp_device;
    uint8_t i2c_address;
    TwoWire *i2c;
};

#endif // ICP101xx_H
