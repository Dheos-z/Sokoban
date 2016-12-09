#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include "constantes.h"
#include "victoire.h"


int victoire(SDL_Surface* ecran)
{
    int continuer = 1, valeur = 1;
    SDL_Event event;
    SDL_Surface *menuVictoire = NULL;
    SDL_Rect posMenuVictoire;


    menuVictoire = IMG_Load("victoire_morsay.png");
    posMenuVictoire.x = 0;
    posMenuVictoire.y = 0;


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
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0; // Retour au choix du niveaux
                break;


            // Niveau suivant
            case SDLK_KP0:

                break;

            case SDLK_0:

                break;


            // Choix du niveau
            case SDLK_KP1:
                continuer = 0;

                break;

            case SDLK_1:
                continuer = 0;

                break;


            // Menu principal
            case SDLK_KP2:
                continuer = 0;
                valeur = 0;

                break;

            case SDLK_2:
                continuer = 0;
                valeur = 0;

                break;
            }
            break;
        }


        // RENOUVELLEMENT DE L'ECRAN
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));


        // SURFACES A BLITTER
        SDL_BlitSurface(menuVictoire, NULL, ecran, &posMenuVictoire);

        // M‡J de l'Ècran
        SDL_Flip(ecran);
    }

    // LIBERATION DES SURFACES
    SDL_FreeSurface(menuVictoire);

    return valeur;
}
