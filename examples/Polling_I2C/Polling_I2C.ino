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
 
#include "ICP101xx.h"
#include "conversion_helper.h"

#define DISPLAY_HEIGHT 1
#define DISPLAY_DEPTH 0

// Instantiate an ICP101xx 
ICP101xx ICP(Wire);

void setup() {
  int ret;
  Serial.begin(115200);
  while(!Serial) {}

  // Initializing the ICP101xx
  ret = ICP.begin();
  if (ret != 0) {
    Serial.print("ICP101xx initialization failed: ");
    Serial.println(ret);
    while(1);
  }

  ICP.start();
}

void loop() {
  float pressure_P = 0;
  float temperature_C = 0;

  // Read Sensor data until a data is available
  if(ICP.getData(pressure_P,temperature_C) == 0)
  {
    // Format data for Serial Plotter
    Serial.print("Pressure (Pa):");
    Serial.println(pressure_P);
    Serial.print("Temp (C):");
    Serial.println(temperature_C);
#if DISPLAY_HEIGHT
      Serial.print("Height (m):");
      Serial.println(convertToHeight(pressure_P,temperature_C));
#endif
#if DISPLAY_DEPTH
      Serial.print("Depth (m):");
      Serial.println(convertToDepth(pressure_P));
#endif
  }
}
