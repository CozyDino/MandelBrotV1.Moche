#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


typedef struct
{
    long double reel;
    long double img;
} complexe;

struct complexe;
void affichercomplexe();
float modulecomplexe();
float argumentcomplexe();
complexe multipliercomplexe();
complexe ajoutercomplexe();
void dessinerpoint();
void dessinerdroite();
int mandelbrot();


#endif // MAIN_H_INCLUDED
