
#include "CommonFunction.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "MainObject.h"
#include "TextObject.h"
#include "ThreatsObject.h"
#include "ExplosionObject.h"
#include "BossObject.h"
#include "Savegame.h"
#include "Mouse.h"

SDL_Texture* gBackgroundTexture = NULL;
SDL_Texture* gBackgroundTexture1 = NULL;
SDL_Texture* gstart = NULL;
SDL_Texture* texture1 = NULL;
TTF_Font* font_score = NULL;
TTF_Font* font_pause = NULL;
TTF_Font* font_start = NULL;

int level = 0;

int coins = 0; 
int init2 = Mix_Init(0); 

int quack_time = 0; 

typedef std::vector<std::string> stringvec;

// doc cac file trong thu muc

void read_directory(const std::string& name, stringvec& v)
{
    DIR* dirp = opendir(name.c_str());
    struct dirent* dp;
    while ((dp = readdir(dirp)) != NULL) {
        v.push_back(dp->d_name);
    }
    closedir(dirp);
}

bool InitData()
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);

    if (ret < 0) {
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (g_window == NULL)
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL)
            success = false;
        else
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                success = false;
        }
    }

    if (TTF_Init() == -1)
    {
        success = false;
    }
    font_score = TTF_OpenFont("font//SuperMario256.ttf", 30);

    if (font_score == NULL)
    {
        success = false;
    }
    font_pause = TTF_OpenFont("font//SuperMario256.ttf", 50);
    if (font_pause == NULL) {
        success = false;
    }
    font_start = TTF_OpenFont("font//SuperMario256.ttf", 24);
    if (font_start == NULL)
    {
        success = false;
    }
}


void close()
{
    
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    SDL_DestroyTexture(gBackgroundTexture);
    SDL_DestroyTexture(gBackgroundTexture1);
    gBackgroundTexture = NULL;
    gBackgroundTexture1 = NULL;
    font_pause = NULL;
    font_score = NULL;
    font_start = NULL;

    gstart = NULL;
    gBackgroundTexture = NULL;
    gBackgroundTexture1 = NULL;

    IMG_Quit();
    SDL_Quit();

}

std::vector<ThreatsObject*> makethreatslist(const int& level)
{
    std::vector<ThreatsObject*> list_threats;

    ThreatsObject* dynamioc_threats = new ThreatsObject[10];
    for (int i = 0; i < 10; i++)
    {
        ThreatsObject* p_threat = (dynamioc_threats + i);
        if (p_threat != NULL)
        {
            p_threat->set_input_left(1);
            p_threat->LoadImg("img//threat//threat_left.png", g_screen);

            p_threat->set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT);

            p_threat->set_x_pos(500 + i * 1000);
            p_threat->set_y_pos(200);
            p_threat->get_val(10 + level * 2);

            int pos1 = p_threat->get_x_pos() - 60;
            int pos2 = p_threat->get_x_pos() + 60;
            p_threat->set_animationpos(pos1, pos2);
            p_threat->set_clips();
            p_threat->set_blood(1);
            p_threat->set_maxblood(1);
            p_threat->set_level(level);
            p_threat->set_threatval(level*2 + 1);
            list_threats.push_back(p_threat); 
        }
    }

    ThreatsObject* threats_ob = new ThreatsObject[20];

    for (int i = 0; i < 5; i++)
    {
        ThreatsObject* p_threat = &threats_ob[i];

        if (p_threat != NULL)
        {
            p_threat->LoadImg("img//threat_level1.png", g_screen);

            p_threat->set_x_pos(i*1100 + 1000);
            p_threat->set_y_pos(250);
            p_threat->set_type_move(ThreatsObject::STATIC_THREAT);
            p_threat->set_input_left(0);

            BulletObject* p_bullet = new BulletObject();
            p_threat->InitBullet(p_bullet, g_screen);
            p_threat->set_clips();
            list_threats.push_back(p_threat);
            p_threat->set_blood(level * 2);
            p_threat->set_maxblood(level * 2);
            p_threat->set_threatval(level*2 + 1);
            p_threat->set_level(level);

        }

    }
    return list_threats;

}

void renderBackground(int x, int y) {
    SDL_Rect backgroundRect = { x, y, BACKGROUND_WIDTH, BACKGROUND_HEIGHT };
    SDL_RenderCopy(g_screen, gBackgroundTexture, NULL, &backgroundRect);
}
void renderBackground1(int x, int y) {
    SDL_Rect backgroundRect = { x, y, BACKGROUND_WIDTH, BACKGROUND_HEIGHT };
    SDL_RenderCopy(g_screen, gBackgroundTexture1, NULL, &backgroundRect);
}
void renderStart(SDL_Texture* newt) {
    SDL_Rect backgroundRect = { 1280/2 - 150, 640/2-251, 300, 502 };
    SDL_RenderCopy(g_screen, newt , NULL, &backgroundRect);
}

void renderheart(const int& lift, BaseObject& heart)
{
    for (int i = 0; i < lift; i++)
    {
        heart.SetRect(10 + i * 22, 10);
        heart.Render(g_screen);
    }
}
void get_text_and_rect(SDL_Renderer* renderer, int x, int y, const char* text,
    TTF_Font* font, SDL_Texture** texture, SDL_Rect* rect) {
    int text_width;
    int text_height;
    SDL_Surface* surface;
    SDL_Color textColor = { 255, 255, 255, 0 };

    surface = TTF_RenderText_Solid(font, text, textColor);
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
}

int main(int argc, char* argv[])
{

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
    Mix_Volume(-1, MIX_MAX_VOLUME * 2);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
    
    Mix_Chunk* sound = Mix_LoadWAV("sound//coins_sound.wav");
    if (!sound) {
        std::cout << "sound error: " << Mix_GetError() ;
    }
    Mix_Chunk* death_sound = Mix_LoadWAV("sound//death_sound.mp3");
    if (!death_sound) {
        std::cout << "sound error: " << Mix_GetError();
    }
    Mix_Chunk* boom_sound = Mix_LoadWAV("sound//boom.wav");
    if (!boom_sound) {
        std::cout << "sound error: " << Mix_GetError();
    }
    Mix_Chunk* throw_shuriken = Mix_LoadWAV("sound//throw.mp3");
    if (!throw_shuriken) {
        std::cout << "sound error: " << Mix_GetError();
    }
    Mix_Chunk* jump = Mix_LoadWAV("sound//jump_sound.mp3");
    if (!jump) {
        std::cout << "sound error: " << Mix_GetError();
    }
    
    
    Mix_Music* music = Mix_LoadMUS("sound//background_music.mp3");
    if (!music)
    {
        std::cout << "music error: " << Mix_GetError();
    }


    ImpTimer fps_time;

    if (InitData() == false)
        return -1;

    //
    GameMap game_map, game_map1, game_map2;

    game_map.LoadMap("map//map01.txt");
    game_map.LoadTiles(g_screen);

    game_map1.LoadMap("map//map02.txt");
    game_map1.LoadTiles(g_screen);

    game_map2.LoadMap("map//map03.txt");
    game_map2.LoadTiles(g_screen);
    
    
    MainObject p_player;
    p_player.LoadImg("img//player sprite//player_right.png", g_screen);
    p_player.set_clips();

    std::vector<ThreatsObject*> threats_list = makethreatslist(1);

    
    ExplosionObject exp_threat;
    bool tRet = exp_threat.LoadImg("img//exp3.png", g_screen);
    if (!tRet)
        return -1;
    exp_threat.set_clip();

    ExplosionObject blood_pl;
    bool tRet1 = blood_pl.LoadImg("img//blood.png", g_screen);
    if (!tRet1)
        return -1;
    blood_pl.set_clip();

    ExplosionObject light;
    bool tRet2 = light.LoadImg("img//lightning.png", g_screen);
    if (!tRet2)
        return -1;
    light.set_clip();

    ExplosionObject heart;
    bool t2 = heart.LoadImg("img//heart.png", g_screen);
    if (!t2)
        return -1;
    heart.set_clip();

    ExplosionObject heal;
    bool tret3 = heal.LoadImg("img//heal.png", g_screen);
    if (!tret3)
        return -1;
    heal.set_clip();
  


    bool is_quit = false;
    bool is_pause = false;
   
    TextObject score;
    score.SetColor(TextObject::WHITE_TEXT);

    TextObject start_button[3];
    start_button[0].SetColor(TextObject::RED_TEXT);
    start_button[1].SetColor(TextObject::RED_TEXT);
    start_button[2].SetColor(TextObject::RED_TEXT);
    start_button[0].SetText("Start");
    start_button[1].SetText("Load");
    start_button[2].SetText("Quit");

    TextObject pause[3];
    pause[0].SetColor(TextObject::RED_TEXT);
    pause[1].SetColor(TextObject::RED_TEXT);
    pause[2].SetColor(TextObject::RED_TEXT);
    pause[0].SetText("Continue");
    pause[1].SetText("Save");
    pause[2].SetText("Quit");

    TextObject Level;
    Level.SetColor(TextObject::RED_TEXT);

    Mix_PlayMusic(music, -1);
   
    int diem = 0;
    int backgroundX = -1280;
    int backgroundX1 = -1280;

    SDL_Surface* backgroundSurface = IMG_Load("img//background.png");
    if (backgroundSurface == NULL) {
        std::cerr << "Failed to load background image! SDL_image Error: " << IMG_GetError() << std::endl;
        return -1;
    }

    SDL_Surface* backgroundSurface1 = IMG_Load("img//background_clouds.png");
    if (backgroundSurface1 == NULL) {
        std::cerr << "Failed to load background image! SDL_image1 Error: " << IMG_GetError() << std::endl;
        return -1;
    }

    SDL_Surface* startSurface = IMG_Load("img//menu.png");
    if (startSurface == NULL) {
        std::cerr << "Failed to load start image! SDL_image Error: " << IMG_GetError() << std::endl;
        return -1;
    }

    gBackgroundTexture = SDL_CreateTextureFromSurface(g_screen, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);

    gBackgroundTexture1 = SDL_CreateTextureFromSurface(g_screen, backgroundSurface1);
    SDL_FreeSurface(backgroundSurface1);
    
    gstart = SDL_CreateTextureFromSurface(g_screen, startSurface);

    SDL_Texture* new_textture = NULL;
    SDL_SetColorKey(startSurface, SDL_TRUE, SDL_MapRGB(startSurface->format, color_key_r, color_key_g, color_key_b));
    new_textture = SDL_CreateTextureFromSurface(g_screen, startSurface);
    SDL_FreeSurface(startSurface);
    
    // Menu
    bool start = true;
 
    //save game
    Savegame save;
    Savegame save1;

    // vong lap game
    bool is_blood = false;

    int LEVEL = 1;

    Map map_data = game_map.getMap();
    Map map_data1 = game_map1.getMap();
    Map map_data2 = game_map2.getMap();
    int time_throw = 0;
    SDL_ShowCursor(false);

    BaseObject mouse;
    mouse.LoadImg("img//mouse.png", g_screen);
    bool jump_times = false;
    Uint32 last_time = 0;
    Uint32 val_time;

    while (start && !is_quit) {
        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT)
            {
                start = false;
                is_quit = true;
            }
           // p_player.HandelInputAction(g_event, g_screen);
        }
        

        SDL_Rect backgroundRect = { 0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT };
        //  SDL_SetTextureBlendMode(gBackgroundTexture, SDL_BLENDMODE_MOD);
        SDL_RenderCopy(g_screen, gBackgroundTexture, NULL, &backgroundRect);

        renderStart(new_textture);

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);



        if (mouseX >= 1280 / 2 - 40 && mouseX <= 1280 / 2 - 40 + 90 && mouseY >= 360 && mouseY <= 397) {
            start_button[0].SetColor(TextObject::GRAY_TEXT);
            if (g_event.type == SDL_MOUSEBUTTONDOWN) {
                if (g_event.button.button = SDL_BUTTON_RIGHT)
                {
                    start = false;
                    SDL_Delay(500);
                }
            }
        }
        else {
            start_button[0].SetColor(TextObject::RED_TEXT);
        }
        start_button[0].LoadFromRenderText(font_start, g_screen);
        start_button[0].RenderText(g_screen, 1280 / 2 - 44, 362);




        if (mouseX >= 1280 / 2 - 40 && mouseX <= 1280 / 2 - 40 + 90 && mouseY >= 360 + 50 && mouseY <= 397 + 50) {
            start_button[1].SetColor(TextObject::GRAY_TEXT);
            if (g_event.type == SDL_MOUSEBUTTONDOWN) {
                if (g_event.button.button = SDL_BUTTON_RIGHT) {
                    stringvec v;
                    read_directory("save_file", v);
                    v.erase(v.begin());
                    v.erase(v.begin());
                    std::string h = "save_file//" + v[0];

                    save1.read(h);
                    save1.get_level(LEVEL);
                    save1.get(map_data, map_data2, p_player);
                    p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
                    threats_list.erase(threats_list.begin(), threats_list.end());
                    save1.get_threats(threats_list, g_screen);
                    save1.close();

                    start = false;
                    SDL_Delay(100);
                }
            }
        }
        else {
            start_button[1].SetColor(TextObject::RED_TEXT);
        }

        start_button[1].LoadFromRenderText(font_start, g_screen);
        start_button[1].RenderText(g_screen, 1280 / 2 - 40 + 5, 360 + 58);

        if (mouseX >= 1280 / 2 - 40 && mouseX <= 1280 / 2 - 40 + 90 && mouseY >= 360 + 120 && mouseY <= 397 + 120) {
            start_button[2].SetColor(TextObject::GRAY_TEXT);
            if (g_event.type == SDL_MOUSEBUTTONDOWN) {
                if (g_event.button.button = SDL_BUTTON_RIGHT)
                {
                    is_quit = true;
                }
            }
        }
        else {
            start_button[2].SetColor(TextObject::RED_TEXT);
        }

        start_button[2].LoadFromRenderText(font_start, g_screen);
        start_button[2].RenderText(g_screen, 1280 / 2 - 40 + 4, 360 + 118);


        int x, y;
        SDL_GetMouseState(&x, &y);
        mouse.SetRect(x, y);
        mouse.Render(g_screen);

        SDL_RenderPresent(g_screen);
        SDL_Delay(10);

    }
    
    while (!is_quit)
    {

        fps_time.start();

        if (p_player.get_isnewlevel())
        {
            last_time = SDL_GetTicks();
            game_map.LoadMap("map//map01.txt");
            game_map1.LoadMap("map//map02.txt");
            game_map2.LoadMap("map//map03.txt");

            map_data = game_map.getMap();
            map_data1 = game_map1.getMap();
            map_data2 = game_map2.getMap();

            LEVEL++;
            threats_list = makethreatslist(LEVEL);

        }
   //     std::cout << p_player.GetRect().x << " " << p_player.GetRect().y << std::endl << ;
        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT)
            {
                is_quit = true;
            }
            p_player.HandelInputAction(g_event, g_screen);
        }
        

        if (g_event.type == SDL_KEYDOWN && g_event.key.keysym.sym == SDLK_ESCAPE) {
            is_pause = true;
            
        }
        while (is_pause && !is_quit)
        {   
            while (SDL_PollEvent(&g_event) != 0)
            {
                if (g_event.type == SDL_QUIT)
                {
                    is_quit = true;
                }
             //  p_player.HandelInputAction(g_event, g_screen);

               if (g_event.type == SDL_KEYDOWN ) {
                   if (g_event.key.keysym.sym == SDLK_RETURN)
                       is_pause = false;

               }
               
            }
            
            SDL_SetRenderDrawColor(g_screen, 210, 210, 210, 210);
            SDL_RenderClear(g_screen);

        // Vẽ hình nền
            renderBackground(backgroundX, 0);

            SDL_SetRenderDrawColor(g_screen, 210, 210, 210, 210);
            SDL_RenderClear(g_screen);

            SDL_Rect backgroundRect = { 0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT };
            SDL_RenderCopy(g_screen, gBackgroundTexture, NULL, &backgroundRect);

            renderStart(new_textture);

            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);





            if (mouseX >= 1280 / 2 - 40 && mouseX <= 1280 / 2 - 40 + 90 && mouseY >= 360 && mouseY <= 397) {
                pause[0].SetColor(TextObject::GRAY_TEXT);
                if (g_event.type == SDL_MOUSEBUTTONDOWN) {
                    if (g_event.button.button = SDL_BUTTON_RIGHT)
                    {
                        is_pause = false;
                        SDL_Delay(500);
                    }
                }
            }
            else {
                pause[0].SetColor(TextObject::RED_TEXT);
            }

            pause[0].LoadFromRenderText(font_start, g_screen);
            pause[0].RenderText(g_screen, 1280 / 2 - 60, 362);

            if (mouseX >= 1280 / 2 - 40 && mouseX <= 1280 / 2 - 40 + 90 && mouseY >= 360 + 50 && mouseY <= 397 + 50) {
                pause[1].SetColor(TextObject::GRAY_TEXT);
                if (g_event.type == SDL_MOUSEBUTTONDOWN) {
                    if (g_event.button.button = SDL_BUTTON_RIGHT) {
                        bool op = save.open("save_file//save_game1.txt");
                        if (!op)
                            return -1;
                        save.save_level(LEVEL);
                        save.savemap(map_data, map_data2);
                        save.saveplayer(p_player);
                        save.savethreat(threats_list);
                        save.close();
                        is_pause = false;
                        SDL_Delay(500);
                    }
                }
            }
            else {
                pause[1].SetColor(TextObject::RED_TEXT);
            }
            pause[1].LoadFromRenderText(font_start, g_screen);
            pause[1].RenderText(g_screen, 1280 / 2 - 40, 360 + 56);

            if (mouseX >= 1280 / 2 - 40 && mouseX <= 1280 / 2 - 40 + 90 && mouseY >= 360 + 120 && mouseY <= 397 + 120) {
                pause[2].SetColor(TextObject::GRAY_TEXT);
                if (g_event.type == SDL_MOUSEBUTTONDOWN) {
                    if (g_event.button.button = SDL_BUTTON_RIGHT)
                    {
                        is_quit = true;
                    }
                }
            }
            else {
                pause[2].SetColor(TextObject::RED_TEXT);
            }
            pause[2].LoadFromRenderText(font_start, g_screen);
            pause[2].RenderText(g_screen, 1280 / 2 - 38, 360 + 118);


            int x, y;
            SDL_GetMouseState(&x, &y);
            mouse.SetRect(x, y);
            mouse.Render(g_screen);

            SDL_RenderPresent(g_screen);
            SDL_Delay(10);
        }

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);

        if (p_player.return_actions() == 2) { // Nếu nhân vật di chuyển sang phải

            backgroundX -= 2;
            backgroundX1 -= 1;// Di chuyển hình nền sang trái
            if (backgroundX <= -1280*2) {
                backgroundX = -1280 - 2;// Reset vị trí nếu hình nền cuộn hết
            }
            if (backgroundX1 <= -1280 * 2) {
                backgroundX1 = -1280 - 1;
            }
        }
        else if (p_player.return_actions() == 1) { // Nếu nhân vật di chuyển sang trái
            backgroundX += 2;
            backgroundX1 += 1;// Di chuyển hình nền sang phải
            if (backgroundX >= 0) {
                backgroundX = -1280 + 2;// Reset vị trí nếu hình nền cuộn hết
            }
            if (backgroundX1 >= 0) {
                backgroundX1 = -1280 + 1;
            }

        }
        

        // Xóa màn hình
        SDL_RenderClear(g_screen);

        // Vẽ hình nền
        renderBackground(backgroundX, 0);
        renderBackground1(backgroundX1, 0);

        //if (p_player.get_xpos() >= 390 * 64) {

        //    int t = 200;
        //    SDL_RenderPresent(g_screen);
        //    // Vẽ hình nền
        //    renderBackground(backgroundX, 0);
        //    renderBackground1(backgroundX1, 0);
        //    while (t--)
        //    {
        //        SDL_Delay(10);
        //    }
        //    p_player.set_xpos(0);
        //    
        //}

        // Do player
        

        p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
        
        p_player.DoPlayer(map_data, map_data2);
        
        map_data1.start_x_ = map_data.start_x_;
        map_data2.start_x_ = map_data.start_x_;
        map_data1.start_y_ = map_data.start_y_;
        map_data2.start_y_ = map_data.start_y_;

        game_map.SetMap(map_data);
        game_map.Load("img//gate.png", g_screen);
        game_map1.SetMap(map_data1);
        game_map2.SetMap(map_data2);

       game_map.render_gate(map_data, g_screen);

        game_map1.DrawMap(g_screen);
        game_map2.DrawMap(g_screen);

        p_player.Show(g_screen);
        p_player.Show_blood(g_screen);
        p_player.HandleBullet(g_screen, map_data);

        if (p_player.get_isthrow()) {
            if (time_throw == 0) {
                time_throw = 1;
                Mix_PlayChannel(-1, throw_shuriken, 0);
            }
        }
        else
            time_throw = 0;
        
        if (p_player.get_jump() == true) {
            if (!jump_times) {
                jump_times = true;
                Mix_PlayChannel(-1, jump, 0);
            }
        }
        else {
            jump_times = false;
        }
        game_map.DrawMap(g_screen);
        
        renderheart(p_player.get_lift(), heart);

        std::string str_level = "Level: ";
        str_level += std::to_string(LEVEL);
        Level.SetText(str_level);
        Level.LoadFromRenderText(font_score, g_screen);
        Level.RenderText(g_screen, 1280 / 2 - 50, 20);
        
        std::string str_time = "Time: ";
        Uint32 time_val = SDL_GetTicks() / 1000 - last_time;
        val_time = 300 - time_val;
        score.SetText(str_time + std::to_string(val_time));
        score.LoadFromRenderText(font_score, g_screen);
        score.RenderText(g_screen, 1280 - 200, 20);


        if (p_player.return_coins() > coins) {
            coins = p_player.return_coins();

            if (p_player.get_blood() < 10) {
                p_player.set_blood(p_player.get_blood() + 1);
                Mix_PlayChannel(-1, sound, 0);

                for (int ex = 0; ex < 8; ex++) {
                    // show blood
                    int x_pos = p_player.GetRect().x - 20;
                    int y_pos = p_player.GetRect().y - 10;

                    heal.set_frame(ex);
                    heal.SetRect(x_pos, y_pos);
                    heal.Show(g_screen);
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    mouse.SetRect(x, y);
                    mouse.Render(g_screen);
                    SDL_RenderPresent(g_screen);
                    SDL_Delay(5);
                }
            }
        }
        

        

        for (int i = 0; i < threats_list.size(); i++)
        {
            ThreatsObject* p_threat = threats_list.at(i);
            if (p_threat != NULL)
            {
                p_threat->SetMapxy(map_data.start_x_, map_data.start_y_);
                p_threat->ImpMoveType(g_screen);
                p_threat->DoPlayer(map_data);
               
                SDL_Rect player = p_player.GetRectFrame();
                if (p_threat->get_type_move() == 0)
                {
                    p_threat->tracking_player(map_data, p_player.get_xpos(), p_player.get_ypos());
                }
                if (p_player.get_xpos() < 1280 / 2) {
                    p_threat->return_player_val(0);
                }
                else {
                    p_threat->return_player_val(p_player.return_xval());
                }
                p_threat->GetPlayerRect(player);
                p_threat->makeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
                p_threat->Show(g_screen);
                if (p_threat->get_blood() > 0) {
                    p_threat->Show_blood(g_screen);
                }

                SDL_Rect rect_player = p_player.GetRectFrame();

                rect_player.x += 20;
                rect_player.y += 10;
                rect_player.w -= 40;
                rect_player.h -= 10;
                bool bCol1 = false;

                std::vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();

                for (int jj = 0; jj < tBullet_list.size(); jj++)
                {
                    BulletObject* pt_bullet = tBullet_list.at(jj);
                    if (pt_bullet)
                    {
                        SDL_Rect bRect;
                        bRect.x = pt_bullet->GetRect().x ;
                        bRect.y = pt_bullet->GetRect().y ;
                        bRect.w = pt_bullet->get_width_frame();
                        bRect.h = pt_bullet->get_height_frame();

                        bCol1 = SDLCommonFunction::CheckCollision(bRect, rect_player);

                        if (bCol1) {

                            pt_bullet->SetRect(-50, -50);

                            if (p_player.get_blood() > 3) {
                                p_player.set_blood(p_player.get_blood() - 3);
                                bCol1 = false;
                            }

                            for(int ex = 0; ex < 8; ex++){
                                // show blood
                                int x_pos = p_player.GetRect().x;
                                int y_pos = p_player.GetRect().y;

                                blood_pl.set_frame(ex);
                                blood_pl.SetRect(x_pos, y_pos);
                                blood_pl.Show(g_screen);
                                int x, y;
                                SDL_GetMouseState(&x, &y);
                                mouse.SetRect(x, y);
                                mouse.Render(g_screen);
                                SDL_RenderPresent(g_screen);
                                SDL_Delay(5);

                            }
                        }

                        if (p_player.checkcollision(bRect, map_data))
                        {
                            pt_bullet->SetRect(-50, -50);
                        }
                    }
                }

                SDL_Rect rect_threat = p_threat->GetRectFrame();
                bool bCol2 = SDLCommonFunction::CheckCollision(rect_threat, rect_player);
                
                if (bCol2 || bCol1)
                {
                    
                    p_player.set_blood(10);
                    if (p_player.GetRect().x < 300) {
                        p_player.set_xpos(0);
                    }
                    else {
                        p_player.set_xpos(p_player.GetRect().x - 256);
                    }
                    p_player.set_ypos(250);
                    if (p_player.get_lift() > 1) {
                        Mix_PlayChannel(-1, death_sound, 0);
                    }
                    SDL_Delay(1000);
                    p_player.set_lift( p_player.get_lift() - 1 );
                    continue;
                }
            }
        }

        if (p_player.return_isdie()) {
            if (p_player.get_lift() > 1) {
                Mix_PlayChannel(-1, death_sound, 0);
            }
            p_player.set_lift(p_player.get_lift() - 1);
            SDL_Delay(1000);
            continue;
        }
        if (p_player.get_lift() <= 0) {
            Mix_PlayChannel(-1, death_sound, 0);
            SDL_ShowCursor(true);
            
            SDL_Delay(500);
            if (MessageBox(NULL, L"GAME OVER", L"G_O", MB_OK | MB_ICONSTOP) == IDOK)
            {
                close();
                SDL_Quit;
                return 0;
            }
        }

        int frame_exp_width = exp_threat.get_frame_width();
        int frame_exp_height = exp_threat.get_frame_height();

        std::vector<BulletObject*> bullet_arr = p_player.get_bullet_list();

        for (int r = 0; r < bullet_arr.size(); r++)
        {
            BulletObject* p_bullet = bullet_arr.at(r);
            if (p_bullet != NULL)
            {
                for (int t = 0; t < threats_list.size(); t++)
                {
                    ThreatsObject* obj_threat = threats_list.at(t);
                    if (obj_threat != NULL)
                    {
                        SDL_Rect tRect;
                        tRect.x = obj_threat->GetRect().x;
                        tRect.y = obj_threat->GetRect().y;
                        tRect.w = obj_threat->get_width_frame();
                        tRect.h = obj_threat->get_height_frame();

                        SDL_Rect bRect;
                        bRect.x = p_bullet->GetRect().x + 15;
                        bRect.y = p_bullet->GetRect().y + 15;
                        bRect.w = p_bullet->get_width_frame() - 20;
                        bRect.h = p_bullet->get_height_frame() - 20;

                        bool bCol = SDLCommonFunction::CheckCollision(tRect, bRect);

                        if (bCol)
                        {
                            int x_pos = p_bullet->GetRect().x - frame_exp_width / 2;
                            int y_pos = p_bullet->GetRect().y - frame_exp_height / 2;
                            
                            for (int ex = 0; ex < 8; ex++) {

                                exp_threat.set_frame(ex);
                                exp_threat.SetRect(x_pos, y_pos);
                                exp_threat.Show(g_screen);
                                int x, y;
                                SDL_GetMouseState(&x, &y);
                                mouse.SetRect(x, y);
                                mouse.Render(g_screen);
                                SDL_RenderPresent(g_screen);
                                SDL_Delay(8);
                                if (ex == NUM_FRAME_EXP - 1) {
                                    Mix_PlayChannel(-1, boom_sound, 0);

                                    if (obj_threat->get_blood() > 1) {
                                        p_player.RemoveBullet(r);
                                        obj_threat->set_blood(obj_threat->get_blood() - 1);
                                    }
                                    else {
                                        p_player.RemoveBullet(r);
                                        if (obj_threat->get_type_move() == ThreatsObject::STATIC_THREAT) {
                                            map_data2.tile[((int)obj_threat->get_y_pos()) / 64][((int)obj_threat->get_x_pos() + 64) / 64] = 70;
                                            map_data2.tile[((int)obj_threat->get_y_pos()) / 64][((int)obj_threat->get_x_pos()) / 64] = 70;
                                        }

                                        else
                                            map_data2.tile[((int)obj_threat->get_y_pos()) / 64][((int)obj_threat->get_x_pos() + 64) / 64] = 70;

                                        obj_threat->Free();
                                        threats_list.erase(threats_list.begin() + t);
                                    }
                                }
                            }

                        }

                    }
                }
            }
        }

       

        //check lightning
        if (((p_player.GetRect().x >= 240 + backgroundX1 && p_player.GetRect().x <= 485 + backgroundX1) || (p_player.GetRect().x >= 240 + backgroundX1 + 1280 && p_player.GetRect().x <= 485 + 1280 + backgroundX1) || (p_player.GetRect().x >= 240 + 1280 * 2 + backgroundX1 && p_player.GetRect().x <= 485 + 1280 * 2 + backgroundX1)) && (p_player.GetRect().y <= 150))
        {
            for (int ex = 0; ex < 8; ex++)
            {
                int x_pos = p_player.GetRect().x;
                int y_pos = p_player.GetRect().y;

                light.set_frame(ex);
                light.SetRect(x_pos + 10, y_pos - 100);
                light.Show(g_screen);
                int x, y;
                SDL_GetMouseState(&x, &y);
                mouse.SetRect(x, y);
                mouse.Render(g_screen);
                SDL_RenderPresent(g_screen);
                SDL_Delay(5);
            }
            p_player.set_ypos(280);
            p_player.set_yval(0);
            p_player.set_xpos(p_player.get_xpos() - 64 * 2);
            Mix_PlayChannel(-1, death_sound, 0);

            SDL_Delay(1000);
        }

        int x, y;
        SDL_GetMouseState(&x, &y);
        mouse.SetRect(x, y);
        mouse.Render(g_screen);

        SDL_RenderPresent(g_screen);

        int real_imp_time = fps_time.get_ticks();
        int time_one_frame = 1000 / FRAME_PER_SECOND; // time_ ms

        if (real_imp_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_imp_time;
            SDL_Delay(delay_time);
        }
    }
    
    close();

    return 0;
}



