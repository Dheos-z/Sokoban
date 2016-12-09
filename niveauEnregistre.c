#include <SDL/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL_image.h>
#include "constantes.h"
#include "niveauEnregistre.h"


int niveauEnregistre(SDL_Surface *ecran)
{
    int continuer = 1, valeur = 1;
    SDL_Event event;
    SDL_Surface *menu = NULL;
    SDL_Rect posMenu;


    menu = IMG_Load("level_saved.png");
    posMenu.x = 0;
    posMenu.y = 0;


    while(continuer)
    {
        SDL_WaitEvent(&event);


        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0; // Lorsque l'utilisateur clique sur la croix, le programme s'arrÍte
            valeur = -1;
            break;


        case SDL_KEYDOWN:
            continuer = 0;
            valeur = 0;
            break;
        }


        // RENOUVELLEMENT DE L'ECRAN
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));


        // SURFACES A BLITTER
        SDL_BlitSurface(menu, NULL, ecran, &posMenu);

        // M‡J de l'Ècran
        SDL_Flip(ecran);
    }

    // LIBERATION DES SURFACES
    SDL_FreeSurface(menu);

    return valeur;
}
