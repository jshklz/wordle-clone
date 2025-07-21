#include "wordle_game.h"
#include "base/base.h"
#include "base/settings.h"
#include "gfx/gfx.h"
#include "gfx/gfx_2d.h"
#include "memory/arena.h"
#include "os/os.h"
#include "utils/utils.h"
#include "ui/ui_console.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
static UI_Console console; 
#define PRNT(a)   UI_ShowConsole(&console);  UI_ConsolePrintLn(&console,a);


#define MAX_WORDS 20000

Wordle_Dictionary wordle_open_new_dictionary(char* file)
{
    Wordle_Dictionary wd = {0};
    wd.words = malloc(MAX_WORDS * sizeof(char*)); 

    if (wd.words == NULL) {
        PRNT("error: could not allocate memory for words!");
        exit(1);
    }

    FILE* f_ptr = fopen(file, "r");
    if (f_ptr == NULL) {
        PRNT("error: file could not be read!");
        free(wd.words);  // Clean up
        exit(1);
    }

    size_t len = 0;
    char* line = NULL;
    ssize_t read;
    int line_no = 0;

    while ((read = getline(&line, &len, f_ptr)) != -1)
    {
        // Strip trailing newline, if any
        if (read > 0 && line[read - 1] == '\n') {
            line[--read] = '\0';
        }

        char* text = MEM_ArenaAlloc(MEM_GetDefaultArena(), read + 1);
        if (!text) {
            PRNT("error: arena allocation failed!");
            break;
        }

        memcpy(text, line, read);
        text[read] = '\0';

        wd.words[line_no++] = text;

        if (line_no >= MAX_WORDS) break;
    }

    wd.length = line_no;
    free(line);
    fclose(f_ptr);
    return wd;
}

Wordle_Game wordle_game_create(GFX_Renderer* renderer, OS_App* app)
{
    Wordle_Game wg = {0};
    Wordle_Dictionary wd = {0};

    wg.app_ptr = app;
    wg.render_ptr = renderer;
    GFX_Font the_font = GFX_CreateFont("SpaceMono-Bold.ttf", 50);
    for (int i = 0 ; i < 8; i++)
    {
        wg.line_texts[i] = GFX_CreateText("     ", v2(50,0), the_font);
          wg.line_texts[i].text = MEM_ArenaAlloc(MEM_GetDefaultArena(), 50);

    }
    wg.grid = MEM_ArenaAlloc(MEM_GetDefaultArena(), sizeof(GFX_Rect) * 30);
    console = UI_CreateConsole(app, GFX_CreateFont("SpaceMono-Bold.ttf", 25));
    r32 grid_w = 50, grid_h = 50;
    r32 grid_x = ( (app->size.w - grid_w * 5 ) / 2) , grid_y = 100;
    wg.pos = v2(grid_x, grid_y);
    for (i32 j = 0; j < 6; j++)
    {
        for (i32 i = 0; i < 5; i++){
         
            wg.grid[(j * 5) + i] = GFX_CreateRect(v2(grid_w,grid_h), v2(grid_x+ (grid_w+5)*i , grid_y));
            GFX_SetRectColor(& wg.grid[(j * 5) + i], v4(33,33,33,255));
        }
        grid_y += grid_h +5;
     }

    
     for (int i = 0; i < 8; i++){
    wg.line_texts[i].pos.x = (grid_x +wg.line_texts[i].font.size/ 2) - 10;
    wg.line_texts[i].h_spacing = (grid_w - wg.line_texts[i].font.size + 70 )/2;    
wg.line_texts[i].pos.y = (100+(the_font.size-15)) + i * (grid_h + 5) + (grid_h - the_font.size) / 2;
     }
    U_ClockStart(&wg.clock, app);
   // UI_ShowConsole(&console);

    
       

    GFX_SetClearColor(v4(18,18,19,255));
     wd = wordle_open_new_dictionary("words.txt");
     wg.wd = wd;
     // Get Random Word from dictionary:
     srand(time(0));
     wg.word_answer = wd.words[rand() % wd.length - 1];
   
    return wg;

}

void wordle_game_update(Wordle_Game* game)
{

    
    GFX_Clear(game->render_ptr);
    for (i32 i = 0; i < 30; i++)
    {
        GFX_DrawRect(&game->grid[i], game->render_ptr);
    }
    for (char i = 'a'; i <= 'z'; i++)
    {
        if (  OS_GetKeyState(i).clicked && game->grid_vector_position.x < 5   )
        {
            u32 id = (game->grid_vector_position.y * 5) + game->grid_vector_position.x;
            game->grid_word_text[game->grid_vector_position.x++] += (toupper(i));
          
            GFX_SetRectColor(&game->grid[id], v4(58, 58,60,255));
            if(&game->line_texts[game->grid_vector_position.y] != 0)
             memcpy(game->line_texts[game->grid_vector_position.y].text, (game->grid_word_text), 6);
            
         }
        
    }

    if (OS_GetKeyState(9).clicked)
    {
        if (console.visiblity) {
        UI_HideConsole(&console); } else UI_ShowConsole(&console);
    }
    if (  OS_GetKeyState(13).clicked && game->grid_vector_position.x >= 5)
    {

        // Compare if the word they provided is a valid word.
        b32 is_it_real_word = 0;

        for (int i = 0; i < game->wd.length; i++)
        {
            is_it_real_word |= U_Compare(game->grid_word_text, game->wd.words[i]);
        }
        if (U_Compare(game->grid_word_text, game->word_answer)) game->win = 1;
        
        if (is_it_real_word){
          

           char answer_used[5] = {0}; 

for (int i = 0; i < 5; i++) {
    if (game->grid_word_text[i] == game->word_answer[i]) {
        GFX_SetRectColor(&game->grid[(game->grid_vector_position.y * 5) + i], v4(97, 140, 85, 255));
        answer_used[i] = 1;
    }
}

for (int i = 0; i < 5; i++) {
    if (game->grid_word_text[i] == game->word_answer[i]) continue; 

    for (int j = 0; j < 5; j++) {
        if (!answer_used[j] && game->grid_word_text[i] == game->word_answer[j]) {
            GFX_SetRectColor(&game->grid[(game->grid_vector_position.y * 5) + i], v4(177, 160, 76, 255));
            answer_used[j] = 1;
            break;
        }
    }
}
  for (int i = 0; i < 5; i++) game->grid_word_text[i] = 0;

            game->grid_vector_position.x = 0;
            game->grid_vector_position.y += 1;
        } else {
           game->do_shake = 1;
           U_ClockRestart(&game->clock, game->app_ptr);
          
        }
           

            

          
 
    }

    
if (game->do_shake || game->is_over)
{

 for (int i = 0; i < 5; i++)
{
    r32 x = U_ClockGetElapsedTime(&game->clock, game->app_ptr);

    r32 base_x = game->pos.x + (50+5)*i;
    if (x >= 2)   
    {
        
        game->grid[game->grid_vector_position.y * 5 + i].pos.x = base_x;
        game->do_shake = 0;
    }
    else
    {
         
        r32 shake_x = (5.0f * powf(2.71828f, -x) * sinf(20.0f * x));
        
      
        game->grid[game->grid_vector_position.y * 5 + i].pos.x = base_x + shake_x;
    }
}
}

    if (OS_GetKeyState(8).clicked && game->grid_vector_position.x > 0)
    {
        
        game->grid_vector_position.x -= 1;
        u32 id = (game->grid_vector_position.y * 5) + game->grid_vector_position.x;
        game->grid_word_text[game->grid_vector_position.x] = '\0';
        game->line_texts[game->grid_vector_position.y].text[game->grid_vector_position.x] = '\0';
        GFX_SetRectColor(&game->grid[id], v4(33,33,33,255));
 
    }


    if (game->win && !game->is_over)
    {
        U_ClockRestart(&game->clock, game->app_ptr);
        PRNT("The word was: \n")
        PRNT(game->word_answer);
        PRNT("You won!");
        PRNT("Thank you for playing my clone game!");
        PRNT("Github: https://github.com/jshklz");
        PRNT("Linkedin: https://linkedin.com/in/jshklz");
        PRNT("Email: jshokar4@uwo.ca");
        game->is_over = 1;
    }
    if (game->grid_vector_position.y == 6 && !game->is_over && !game->win) { 
        UI_ShowConsole(&console);
        PRNT("The word was: \n")
        PRNT(game->word_answer);
        PRNT("No avaliable chances left. Retry by restarting the game.");
        PRNT("Thank you for playing my clone game!");
        PRNT("Github: https://github.com/jshklz");
        PRNT("Linkedin: https://linkedin.com/in/jshklz");
        PRNT("Email: jshokar4@uwo.ca");
        game->is_over = 1;
    }
    for (int i = 0; i < game->grid_vector_position.y+1; i++)
    GFX_DrawText((&game->line_texts[i]  ), game->render_ptr);

    UI_DrawConsole(&console, game->render_ptr);
    UI_UpdateConsole(&console, game->render_ptr, game->app_ptr);

}

void wordle_game_clean(Wordle_Game* game_ptr)
{
    return;
}

