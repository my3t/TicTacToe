#include "ui.h"
//set up variables dor window and renderer which is what we draw to
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
//Loading fonts with different sizes
TTF_Font *pcsenior24_f;
TTF_Font *pcsenior18_f;
TTF_Font *arcade24_f;

//setting colours that are reusable
SDL_Color blue = {0, 155, 255};
SDL_Color indigo = {42, 52, 146};
SDL_Color red = {239, 68, 35};
SDL_Color orange = {255, 149, 38};
SDL_Color green = {79, 175, 68};
SDL_Color black = {0, 0, 0};
SDL_Color white = {255, 255, 255};
SDL_Color cyan = {80, 200, 200};
SDL_Color btnblue ={0, 100, 200};
SDL_Color deepblue = {42, 52, 146};
SDL_Color transparent = {0, 0, 0, 0};

int initialiseFonts()
{
	pcsenior24_f = TTF_OpenFont(PCSENIOR_FONT, 24);
	if (pcsenior24_f == NULL)
    {
        fprintf(stderr, "TTF_OpenFont Errors: %s\n", TTF_GetError());
        return ERROR;
    }
	pcsenior18_f = TTF_OpenFont(PCSENIOR_FONT, 18);
	if (pcsenior18_f == NULL)
    {
        fprintf(stderr, "TTF_OpenFont Errors: %s\n", TTF_GetError());
        return ERROR;
    }
	arcade24_f = TTF_OpenFont(ARCADE_FONT, 24);
	if (arcade24_f == NULL)
    {
        fprintf(stderr, "TTF_OpenFont Errors: %s\n", TTF_GetError());
        return ERROR;
    }

	return SUCCESS;
}
//set the background image for all the windows
int setBackgroundImage(const char *imagePath)
{
    // load image into surface
    SDL_Surface *backgroundSurface = IMG_Load(imagePath);
    if (backgroundSurface == NULL)
    {
        // error validation
        printf("IMG_Load Error: %s\n", IMG_GetError());
        return ERROR;
    }

    SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    if (backgroundTexture == NULL)
    {
        printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        SDL_FreeSurface(backgroundSurface);
        return ERROR;
    }

    // Create source and destination rectangles
    SDL_Rect srcRect = {0, 0, 1920, 900};
    SDL_Rect destRect = {0, 0, 1950, 700};

    // Render the background image using the original size
    SDL_RenderCopy(renderer, backgroundTexture, &srcRect, &destRect);

    SDL_FreeSurface(backgroundSurface);
    SDL_DestroyTexture(backgroundTexture);

    return SUCCESS;
}

void drawTextInput(char *label, char *var)
{
    clearScreen();
    renderAnchoredText(
        label,
        pcsenior18_f,
        SCREEN_WIDTH / 2,
        SCREEN_HEIGHT / 2 + 50,
        white);
    renderAnchoredText(
        var,
        pcsenior24_f,
        SCREEN_WIDTH / 2,
        SCREEN_HEIGHT / 2 - 50,
        white);
    SDL_RenderPresent(renderer);
}

void renderText(char *text, TTF_Font *font, int x, int y, SDL_Color color)
{
    //setting up font style for a specific text in the design
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect pos;
    surface = TTF_RenderText_Solid(font, text, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    pos.w = surface->w;
    pos.h = surface->h;
    pos.x = x;
    pos.y = y;
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &pos);
}

void renderAnchoredText(char *text, TTF_Font *font, int x, int y, SDL_Color color)
{   //setting up font style for a specific text in the design
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect pos;
    surface = TTF_RenderText_Solid(font, text, color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    pos.w = surface->w;
    pos.h = surface->h;
    pos.x = x - surface->w / 2;
    pos.y = y - surface->h / 2;
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &pos);
}

void clearScreen()
{
    //clear the screen and set the background colour
    //free up resources
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
    if (setBackgroundImage(BACKDROP_IMAGE) == ERROR)
    {
        printf("Image not loading");
    }
}

void clearSDL()
{   //Free up resources
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}