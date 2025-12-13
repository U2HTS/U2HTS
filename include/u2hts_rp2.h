/*
  Copyright (C) CNflysky.
  U2HTS stands for "USB to HID TouchScreen".
  This file is licensed under GPL V3.
  All rights reserved.
*/

#ifndef _U2HTS_RP2_H_
#define _U2HTS_RP2_H_

#include <bsp/board_api.h>
#include <hardware/flash.h>
#include <hardware/i2c.h>
#include <pico/flash.h>
#include <pico/stdlib.h>
#include <tusb.h>

#include "tusb_config.h"

#define U2HTS_SWAP16(x) __builtin_bswap16(x)
#define U2HTS_SWAP32(x) __builtin_bswap32(x)

#define U2HTS_I2C i2c1
#define U2HTS_I2C_TIMEOUT 10 * 1000  // 10ms

#define U2HTS_I2C_SDA 10
#define U2HTS_I2C_SCL 11
#define U2HTS_TP_INT 6
#define U2HTS_TP_RST 5
#define U2HTS_USR_KEY 9
// last page
#define U2HTS_CONFIG_STORAGE_OFFSET PICO_FLASH_SIZE_BYTES - 8192

#define U2HTS_PHYSICAL_MAX_X 4096
#define U2HTS_PHYSICAL_MAX_Y 4096
// clang-format off
#define U2HTS_HID_TP_DESC \
      HID_USAGE(0x22), \
      HID_COLLECTION(HID_COLLECTION_LOGICAL), \
      HID_USAGE(0x42), \
      HID_LOGICAL_MAX(1), \
      HID_LOGICAL_MIN(0), \
      HID_REPORT_SIZE(1), \
      HID_REPORT_COUNT(1), \
        HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), \
      HID_USAGE(0x51), \
      HID_REPORT_SIZE(7), \
        HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), \
      HID_USAGE_PAGE(HID_USAGE_PAGE_DESKTOP), \
      HID_LOGICAL_MAX_N(U2HTS_LOGICAL_MAX, 2), \
      HID_REPORT_SIZE(16), \
      HID_USAGE(HID_USAGE_DESKTOP_X), \
      HID_PHYSICAL_MAX_N(U2HTS_PHYSICAL_MAX_X, 2), \
        HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), \
      HID_PHYSICAL_MAX_N(U2HTS_PHYSICAL_MAX_Y, 2), \
      HID_USAGE(HID_USAGE_DESKTOP_Y), \
        HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), \
      HID_USAGE_PAGE(HID_USAGE_PAGE_DIGITIZER), \
      HID_LOGICAL_MAX_N(255, 2), \
      HID_PHYSICAL_MAX_N(255, 2), \
      HID_REPORT_SIZE(8), \
      HID_REPORT_COUNT(3), \
      HID_USAGE(0x48), \
      HID_USAGE(0x49), \
      HID_USAGE(0x30), \
        HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), \
      HID_COLLECTION_END

#define U2HTS_HID_TP_INFO_DESC \
      HID_LOGICAL_MAX_N(0xFFFF, 3), \
      HID_REPORT_SIZE(16), \
      HID_UNIT_EXPONENT(0x0C), \
      HID_UNIT_N(0x1001, 2), \
      HID_REPORT_COUNT(1), \
      HID_USAGE(0x56), \
        HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE), \
      HID_USAGE(0x54), \
      HID_LOGICAL_MAX(10), HID_REPORT_SIZE(8), \
        HID_INPUT(HID_DATA | HID_VARIABLE | HID_ABSOLUTE)

#define U2HTS_HID_TP_MAX_COUNT_DESC \
      HID_USAGE(0x55), \
        HID_FEATURE(HID_DATA | HID_VARIABLE | HID_ABSOLUTE)

#define U2HTS_HID_TP_MS_THQA_CERT_DESC \
      HID_USAGE_PAGE_N(0XFF00, 2), \
      HID_USAGE(0xc5), HID_LOGICAL_MAX_N(255, 2), \
      HID_REPORT_COUNT_N(256, 2), \
        HID_FEATURE(HID_DATA | HID_VARIABLE | HID_ABSOLUTE)
// clang-format on
inline static void u2hts_pins_init() {
  // some touch contoller requires ATTN signal in specified state while
  // resetting.
  gpio_set_function(U2HTS_TP_INT, GPIO_FUNC_SIO);
  gpio_set_dir(U2HTS_TP_INT, GPIO_OUT);
  gpio_put(U2HTS_TP_INT, true);

  gpio_set_function(U2HTS_TP_RST, GPIO_FUNC_SIO);
  gpio_set_dir(U2HTS_TP_RST, GPIO_OUT);
  gpio_put(U2HTS_TP_RST, true);

  gpio_set_function(PICO_DEFAULT_LED_PIN, GPIO_FUNC_SIO);
  gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

  gpio_init(U2HTS_USR_KEY);
  gpio_set_dir(U2HTS_USR_KEY, GPIO_IN);
}
#endif