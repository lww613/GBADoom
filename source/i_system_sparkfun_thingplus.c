#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#ifdef RP2040

#include "d_event.h"
#include "d_main.h"
#include "doomdef.h"
#include "doomtype.h"

#include "global_data.h"

#include "tables.h"

#include "i_system_e32.h"

#include "lprintf.h"

#include "hardware/spi.h"
#include "pico/runtime.h"
#include "pico/stdlib.h"
#include "st7789.h"

#define DCNT_PAGE 0x0010

#define TM_FREQ_1024 0x0003
#define TM_ENABLE 0x0080
#define TM_CASCADE 0x0004
#define TM_FREQ_1024 0x0003
#define TM_FREQ_256 0x0002

#define REG_WAITCNT *((vu16 *)(0x4000204))

//**************************************************************************************

void I_InitScreen_e32() {}

//**************************************************************************************

void I_BlitScreenBmp_e32() {}

//**************************************************************************************

void I_StartWServEvents_e32() {}

//**************************************************************************************

void I_PollWServEvents_e32() {}

//**************************************************************************************

void I_ClearWindow_e32() {}

//**************************************************************************************

static unsigned short buffer1[MAX_SCREENHEIGHT * MAX_SCREENWIDTH];

unsigned short *I_GetBackBuffer() {
    return &buffer1[0];
}

//**************************************************************************************

unsigned short *I_GetFrontBuffer() {
    return &buffer1[0];
}

//**************************************************************************************

static ST7789_t *sobj;
void I_CreateWindow_e32() {
  spi_init(spi_default, 50000000);
  gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
  gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
  gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
  // gpio_set_function(1, GPIO_FUNC_SPI);

  // CS 19
  gpio_init(19);
  gpio_set_dir(19, GPIO_OUT);

  gpio_init(20);
  gpio_set_dir(20, GPIO_OUT);
  gpio_init(21);
  gpio_set_dir(21, GPIO_OUT);
  gpio_init(22);
  gpio_set_dir(22, GPIO_OUT);
  spi_set_format(spi0, 8, SPI_CPOL_1, SPI_CPHA_0, SPI_MSB_FIRST);

  sobj = ST7789_create(spi0, 240, 240, 22, 21, 20);
  ST7789_init(sobj);
  ST7789_on(sobj);
}

//**************************************************************************************

void I_CreateBackBuffer_e32() { I_CreateWindow_e32(); }

//**************************************************************************************

static unsigned short st_buffer[MAX_SCREENWIDTH * 2];
static unsigned short pal_ram[256];

void I_FinishUpdate_e32(const byte *srcBuffer, const byte *pallete,
                        const unsigned int width, const unsigned int height) {
  for (int i = 0; i < 256; i++) {
    unsigned int r = *pallete++;
    unsigned int g = *pallete++;
    unsigned int b = *pallete++;

    uint16_t p = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xFF) >> 3);
    uint16_t t = (p >> 8) | (p << 8);
    pal_ram[i] = t;
  }

  for (int j = 0 ; j < height; j++) {
    unsigned short *st_ptr = &st_buffer[0];
    for (int i = 0; i < width * 2 ; i++) {
        *st_ptr++ = pal_ram[*srcBuffer++];
    }
     ST7789_blit_buffer(sobj, (char *) st_buffer, MAX_SCREENWIDTH*4, 0,
                     (240 - 160) / 2 + j, width*2, 1, 0);
  }
}

//**************************************************************************************

void I_SetPallete_e32(const byte *pallete) {
}

//**************************************************************************************

int I_GetVideoWidth_e32() { return 120; }

//**************************************************************************************

int I_GetVideoHeight_e32() { return 160; }

//**************************************************************************************

void I_ProcessKeyEvents() {}

//**************************************************************************************

#define MAX_MESSAGE_SIZE 1024

void I_Error(const char *error, ...) {
  char msg[MAX_MESSAGE_SIZE];

  va_list v;
  va_start(v, error);

  vsprintf(msg, error, v);

  va_end(v);

  printf("%s", msg);
}

//**************************************************************************************

void I_Quit_e32() {}

//**************************************************************************************

#endif