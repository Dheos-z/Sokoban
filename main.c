/*
AVANCEMENT :

Reste à faire :

- Effaceur de niveau
- D'autres écrans de victoire
- Concevoir d'autres niveaux
- BONUS : Luigi et autres personnages
*/

#include <SDL/SDL.h>
#include <stdlib.h>
#include <stdio.h>
// #include <SDL/SDL_image.h>
#include "jeu.h"
#include "constantes.h"
#include "choixNiveau.h"

int main(int argc, char* argv[])
{
    SDL_Surface *ecran = NULL, *menu = NULL;
    SDL_Event event;
    int continuer = 1;
    SDL_Rect posMenu;



    SDL_Init(SDL_INIT_VIDEO); // Initialise la SDL en mode vidéo

    ecran = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Mario Sokoban", NULL);


    // Chargement du menu principal

    menu = SDL_LoadBMP("menu.bmp");
    posMenu.x = (WIDTH-408)/2;
    posMenu.y = (HEIGHT-408)/2;


    // MENU PRINCIPAL

    while(continuer)
    {
        SDL_WaitEvent(&event); // Active les événements en mode bloquant

        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0; // Lorsque l'utilisateur clique sur la croix, le prg s'arrête
            break;


        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0; // La touche ECHAP permet de quitter le jeu
                break;

            case SDLK_KP1:
                continuer = choixNiveau(ecran);
                break;

            case SDLK_1:
                continuer = choixNiveau(ecran);
                break;

            case SDLK_KP2:
                continuer = menuEditeur(ecran);
                break;

            case SDLK_2:
                continuer = menuEditeur(ecran);
                break;
            }
            break;
        }


        // RENOUVELLEMENT DE L'ECRAN
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));


        // SURFACES A BLITTER
        SDL_BlitSurface(menu, NULL, ecran, &posMenu);

        // MàJ de l'écran
        SDL_Flip(ecran);

    }

    // LIBERATION DES SURFACES
    SDL_FreeSurface(menu);

    SDL_Quit(); // Libère la mémoire occupée par ecran

    return EXIT_SUCCESS;
}
