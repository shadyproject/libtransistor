#include<libtransistor/nx.h>
#include<libtransistor/canvas_display.h>
#include<malloc.h>
#include<string.h>

uint8_t draw_thread_stack[0x80000];

int frame_number = 0;

void draw_thread_impl(void* arg) {
  dbg_printf("started thread");
  svcSleepThread(1000000000);

  typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
  } pixel_t;
  
  for(int i = 0; i < 200; i++) {
    while(canvas_framenumbers->rendered_framenumber != canvas_framenumbers->presented_framenumber) {
      svcSleepThread(16666666); // 1/60 second
    }
    for(int x = 0; x < 1280; x++) {
      for(int y = 0; y < 720; y++) {
        pixel_t *pixel = (pixel_t*) (canvas_display->pixels + (y*1280) + x);
        pixel->r = 0;
        pixel->g = 0;
        pixel->b = 127;
        pixel->a = 255;
      }
    }

    int pos = frame_number;
    
    for(int x = pos; x < pos + 80; x++) {
      for(int y = pos; y < pos + 80; y++) {
        pixel_t *pixel = (pixel_t*) (canvas_display->pixels + (y*1280) + x);
        pixel->r = 255;
        pixel->g = 255;
        pixel->b = 255;
      }
    }
    
    frame_number++;
    canvas_framenumbers->rendered_framenumber++;
  }
  svcExitThread();
}

#define ASSERT_OK(label, expr) if((r = expr) != RESULT_OK) {            \
    dbg_printf("assertion failed at %s:%d: result 0x%x is not OK", __FILE__, __LINE__, r); \
    goto label;                                                         \
  }

int main() {
  svcSleepThread(100000000);

  result_t r;
  
  uint32_t priority;
  ASSERT_OK(fail, svcGetThreadPriority(&priority, 0xFFFF8000));
  
  thread_h thread;
  ASSERT_OK(fail, svcCreateThread(&thread, draw_thread_impl, 0, draw_thread_stack, priority, -2));

  ASSERT_OK(fail_thread, svcStartThread(thread));

  svcSleepThread(100000000);
  
  return 0;

 fail_thread:
  svcCloseHandle(thread);
 fail:
  return 1;
}
