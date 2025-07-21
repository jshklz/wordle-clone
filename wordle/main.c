#include "gfx/gfx_2d.h"
#include "ui/ui_console.h"
#include "wordle_game.h"
#include <assert.h>
#include <torn.h>

i32 main(void) {
  TORN_Init();

  /////////////////////////////////////////////////////////////////
  OS_App *app = OS_ConstructApp("Wordle", v2(900, 550), OS_AppDefault,
                                OS_AppStyleDefault);
  assert(app && "Failed to create window");
  /////////////////////////////////////////////////////////////////

  GFX_Renderer *renderer = GFX_CreateRenderer(app);
  assert(renderer && "Failed to create renderer");

  Wordle_Game wg = wordle_game_create(renderer, app);

  while (app->running) {
    OS_PollEvents(app);

    wordle_game_update(&wg);

    GFX_Present(renderer);
  }

  wordle_game_clean(&wg);
  TORN_Destroy();
  return 0;
}
