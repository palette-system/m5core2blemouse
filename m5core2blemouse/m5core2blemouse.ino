// 参考にしたソース
// https://github.com/Katsushun89/m5core2_draw_by_touch/blob/master/m5core2_draw_by_touch.ino
// https://github.com/T-vK/ESP32-BLE-Mouse

#include <M5Core2.h>
#include <stdio.h>
#include <BleMouse.h>

BleMouse bleMouse;
int last_x = 0;
int last_y = 0;
int last_t = 0;
int start_t = 0;

void setup() {
  M5.begin(true, true, true, true);
  M5.Lcd.fillScreen(WHITE);
  bleMouse.begin();
}

void loop() {
  int now_x, now_y, now_t;
  TouchPoint_t pos= M5.Touch.getPressPoint();

  now_t = millis();
  if(pos.y > 1 && pos.x > 1){ // タッチがある
    now_x = pos.x;
    now_y = pos.y;
    if ((now_t - last_t) < 100) {
        // 前のタッチから100ミリ秒以内なら移動
        bleMouse.move((now_x - last_x),(now_y - last_y), 0);
    } else {
        // 前のタッチから100ミリ以上経っていればタッチ開始
        start_t = now_t;
    }
    // 今回のタッチ位置を保持
    last_x = now_x;
    last_y = now_y;
    last_t = now_t;

  } else if (start_t) {
    // 100ミリ秒タッチが無ければタッチ終了
    if ((now_t - last_t) > 100) {
      // タップしていた時間が120ミリ秒以下ならクリック
      if ((last_t - start_t) < 120) {
        bleMouse.click(MOUSE_LEFT);
      }
      start_t = 0;
    }
  }

  delay(10);
}
