#ifndef WORDLE_GAME_H
#define WORDLE_GAME_H
#include "gfx/gfx.h"
#include "torn.h"

typedef struct Wordle_Dictionary Wordle_Dictionary;
struct Wordle_Dictionary {
  char **words;
  int length;
};
Wordle_Dictionary wordle_open_new_dictionary(char *file);

typedef struct Wordle_Game Wordle_Game;
struct Wordle_Game {
  OS_App *app_ptr;
  GFX_Renderer *render_ptr;

  GFX_Rect *grid;
  V2I grid_vector_position;
  u8 grid_word_text[5];
  u8 full_string[40];
  U_Clock clock;
  V2F pos;
  u8 *word_answer;
  b32 do_shake;

  r32 is_over;
  r32 win;

  Wordle_Dictionary wd;

  GFX_Text line_texts[8];
};

Wordle_Game wordle_game_create(GFX_Renderer *renderer, OS_App *app);

void wordle_game_update(Wordle_Game *game);

void wordle_game_clean(Wordle_Game *game_ptr);

#endif /* WORDLE_GAME_H */