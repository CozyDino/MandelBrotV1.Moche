#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "SDL2/SDL.h"
#include "main.h"



int continuer = 1;
SDL_Event event;
SDL_Rect background;
int i = 7000;
double long echelle = 1;
double long focus_x = 0.01;
double long focus_y = 0.01;
float vitesse = 50.0;
int grille[800][600] = {0};
complexe point;
double long echelle_x = 200.0;
double long echelle_y = 150.0;
int calcul = 1;
int colormode = 0;
int mouseX1,mouseX2 = 0;
int mouseY1,mouseY2 = 0;
SDL_Point center;

int main(int argc, char** argv)
{
    background.x = 0;
    background.y = 0;
    background.w = 800;
    background.h = 600;
    point.img = 0;
    point.reel = 0;

     if (SDL_Init(SDL_INIT_VIDEO) != 0 )
    {
        fprintf(stdout,"Échec de l'initialisation de la SDL (%s)\n",SDL_GetError());
        return -1;
    }

    /* Création de la fenêtre */
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_CreateWindowAndRenderer(background.w, background.h, 0, &window, &renderer);
                // Background Colouring
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_RenderDrawRect(renderer,&background);
        SDL_RenderFillRect(renderer,&background);
        //FinBC
    while(continuer)
    {

        SDL_SetRenderDrawColor(renderer, 0,0,0,255);
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    continuer = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    printf("\n");
                    SDL_GetMouseState(&mouseX1,&mouseY1);
                    break;
                case SDL_MOUSEBUTTONUP:
                    SDL_GetMouseState(&mouseX2,&mouseY2);
                    printf("x1 :%d y1 : %d x2 : %d y2 : %d  \n",mouseX1,mouseY1,mouseX2, mouseY2);
                    float distance = sqrt(pow(mouseX2-mouseX1,2) + (pow(mouseY2-mouseY1,2)));
                    center.x = (mouseX2+mouseX1)/2;
                    center.y = (mouseY2+mouseY1)/2;
                    focus_x = ((center.x-400.0)/echelle_x)+focus_x;
                    focus_y = ((center.y-300.0)/echelle_y)+focus_y;
                    echelle_x = echelle_x / (distance/1000);
                    echelle_y = echelle_y / (distance/1000);
                    printf("%f" , distance);
                case SDL_KEYUP:
                    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
                    SDL_RenderDrawRect(renderer,&background);
                    SDL_RenderFillRect(renderer,&background);
                    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
                    calcul = 1;
                    switch(event.key.keysym.sym)
                    {
                    case SDLK_i:
                        focus_y = focus_y - (vitesse/echelle_y);
                        break;
                    case SDLK_k:
                        focus_y += vitesse/echelle_y;
                        break;
                    case SDLK_j:
                        focus_x -= vitesse/echelle_x;
                        break;
                    case SDLK_l:
                        focus_x += vitesse/echelle_x;
                        break;
                    case SDLK_2:
                        echelle_x *= 1.2;
                        echelle_y *= 1.2;
                        break;
                    case SDLK_1:
                        echelle_x /= 1.2;
                        echelle_y /= 1.2;
                        break;
                    case SDLK_3:
                        i -= 2000;
                        break;
                    case SDLK_4:
                        i += 2000;
                        break;
                    default:
                        break;
                    }
                    break;
                default:
                    break;
            }
        }

        if(calcul == 1)
        {
            for(int a = 0; a <= 800; a++)
            {
                for(int b = 0; b <= 600; b++)
                {
                    point.img = -((b-300.0)/echelle_y)-focus_y;
                    point.reel = ((a-400.0)/echelle_x)+focus_x;
                    grille[a][b] = mandelbrot(point, i);
                    if(grille[a][b] == -1)
                    {
                        dessinerpoint(a,b,renderer);
                    }
                    if(grille[a][b] != -1)
                    {
                        switch(colormode)
                        {
                            case 0:
                                SDL_SetRenderDrawColor(renderer, 50*log10(grille[a][b]),0,50*log10(grille[a][b]),50);
                                dessinerpoint(a,b,renderer);
                                SDL_SetRenderDrawColor(renderer, 0,0,0,255);
                                break;
                            case 1:
                                SDL_SetRenderDrawColor(renderer, 3*grille[a][b],3*grille[a][b],0,50);
                                dessinerpoint(a,b,renderer);
                                SDL_SetRenderDrawColor(renderer, 0,0,0,255);
                                break;
                            case 2:
                                SDL_SetRenderDrawColor(renderer, 0,3*grille[a][b],3*grille[a][b],50);
                                dessinerpoint(a,b,renderer);
                                SDL_SetRenderDrawColor(renderer, 0,0,0,255);
                                break;
                        }
                    }
                }
            }
            calcul = 0;
        }
        SDL_RenderPresent(renderer);
    }
        return 0;
}

void dessinerpoint(int x, int y,SDL_Renderer *renderer)
{
    SDL_RenderDrawPoint(renderer,x,y);
}

int mandelbrot(complexe nombre, int i)
{
    complexe temp;
    temp = nombre;
    for(int n = 0; n <= i; n++)
    {
        //temp = nombre;
        nombre = multipliercomplexe(nombre,nombre);
        nombre = ajoutercomplexe(temp,nombre);
        if(modulecomplexe(nombre) > 4)
        {
            return n;
        }
        if(i == n)
        {
            return -1;
        }
    }
    return 0;
}

void affichercomplexe(complexe Complex)
{
    if(Complex.img < 0)
    {
        printf("%lf - %lfi\n", Complex.reel, -Complex.img);
    }
    else
    {
        printf("%lf + %lfi\n", Complex.reel,Complex.img);
    }
}

float modulecomplexe(complexe nombre)
{
    return (pow(nombre.reel, 2) + pow(nombre.img, 2));
}

float argumentcomplexe(complexe nombre)
{
    return atan(nombre.img/nombre.reel);
}

complexe multipliercomplexe(complexe c1, complexe c2)
{
    complexe resultat;
    resultat.img = (c1.reel*c2.img) + (c1.img*c2.reel);
    resultat.reel = (c1.reel*c2.reel) - (c1.img*c2.img);
    return resultat;
}

complexe ajoutercomplexe(complexe c1, complexe c2)
{
    complexe resultat;
    resultat.img = c1.img + c2.img;
    resultat.reel = c1.reel + c2.reel;
    return resultat;
}

