#pragma once

typedef struct {
  uint32_t pixels[1280*720];
  uint32_t frame_number;
} canvas_display_t;

typedef struct {
  volatile uint32_t rendered_framenumber;
  volatile uint32_t presented_framenumber;
} canvas_framenumbers_t;

extern canvas_display_t *canvas_display;
extern canvas_framenumbers_t *canvas_framenumbers;
