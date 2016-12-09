#include <SDL/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL/SDL_image.h>
#include "jeu.h"
#include "constantes.h"
#include "choixNiveau.h"

int choixNiveau(SDL_Surface* ecran)
{
    int continuer = 1, i = 0, j = 0, valeur = 1, nbDeLvl = 0;
    char fichierImage[T_MAX] = "niveauX.png";
    SDL_Surface *niveau[10], *menuJouer = NULL;
    SDL_Rect posNiveau[10], posMenuJouer;
    SDL_Event event;


    // DETERMINATION DU NOMBRE DE NIVEAUX DISPONIBLES
    nbDeLvl = niveauxDispo();


    // INITIALISATION DES SURFACES
    initSurfacesLvl(niveau, posNiveau); // Initialisation des 10 niveaux avec l'image "vide.png"

    for(i = 0; i < nbDeLvl; i++)
    {
        fichierImage[6] = '0' + i; // On va modifier la chaine de caractère pour que le programme détecte le nom fichier correspondant à chaque niveau
        niveau[i] = IMG_Load(fichierImage);
    }

    menuJouer = IMG_Load("menu_jouer.png");

    posMenuJouer.x = 0;
    posMenuJouer.y = 0;



    ////// CHOIX DU NIVEAU //////

    while(continuer)
    {
        SDL_WaitEvent(&event);


        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0; // Lorsque l'utilisateur clique sur la croix, le programme s'arrête
            valeur = 0;
            break;


        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0; // La touche ECHAP permet de revenir au menu principal
                break;


            case SDLK_KP0:
                if(nbDeLvl > 0)
                {
                    continuer = partie(ecran, "niveau0.txt");
                }

                break;

            case SDLK_0:
                if(nbDeLvl > 0)
                {
                    continuer = partie(ecran, "niveau0.txt");
                }

                break;


            case SDLK_KP1:
                if(nbDeLvl > 1)
                {
                    continuer = partie(ecran, "niveau1.txt");
                }

                break;

            case SDLK_1:
                if(nbDeLvl > 1)
                {
                    continuer = partie(ecran, "niveau1.txt");
                }

                break;


            case SDLK_KP2:
                if(nbDeLvl > 2)
                {
                    continuer = partie(ecran, "niveau2.txt");
                }

                break;

            case SDLK_2:
                if(nbDeLvl > 2)
                {
                    continuer = partie(ecran, "niveau2.txt");
                }

                break;


            case SDLK_KP3:
                if(nbDeLvl > 3)
                {
                    continuer = partie(ecran, "niveau3.txt");
                }

                break;

            case SDLK_3:
                if(nbDeLvl > 3)
                {
                    continuer = partie(ecran, "niveau3.txt");
                }

                break;


            case SDLK_KP4:
                if(nbDeLvl > 4)
                {
                    continuer = partie(ecran, "niveau4.txt");
                }

                break;

            case SDLK_4:
                if(nbDeLvl > 4)
                {
                    continuer = partie(ecran, "niveau4.txt");
                }

                break;


            case SDLK_KP5:
                if(nbDeLvl > 5)
                {
                    continuer = partie(ecran, "niveau5.txt");
                }

                break;

            case SDLK_5:
                if(nbDeLvl > 5)
                {
                    continuer = partie(ecran, "niveau5.txt");
                }

                break;


            case SDLK_KP6:
                if(nbDeLvl > 6)
                {
                    continuer = partie(ecran, "niveau6.txt");
                }

                break;

            case SDLK_6:
                if(nbDeLvl > 6)
                {
                    continuer = partie(ecran, "niveau6.txt");
                }

                break;


            case SDLK_KP7:
                if(nbDeLvl > 7)
                {
                    continuer = partie(ecran, "niveau7.txt");
                }

                break;

            case SDLK_7:
                if(nbDeLvl > 7)
                {
                    continuer = partie(ecran, "niveau7.txt");
                }

                break;


            case SDLK_KP8:
                if(nbDeLvl > 8)
                {
                    continuer = partie(ecran, "niveau8.txt");
                }

                break;

            case SDLK_8:
                if(nbDeLvl > 8)
                {
                    continuer = partie(ecran, "niveau8.txt");
                }

                break;


            case SDLK_KP9:
                if(nbDeLvl > 9)
                {
                    continuer = partie(ecran, "niveau9.txt");
                }

                break;

            case SDLK_9:
                if(nbDeLvl > 9)
                {
                    continuer = partie(ecran, "niveau9.txt");
                }

                break;
            }
            break;
        }


        // Si l'utilisateur a cliqué sur la croix
        if(continuer == -1)
        {
            continuer = 0;
            valeur = 0;
        }


        // RENOUVELLEMENT DE L'ECRAN
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));


        // SURFACES A BLITTER
        SDL_BlitSurface(menuJouer, NULL, ecran, &posMenuJouer);

        for(i = 0; i < 10; i++)
        {
            SDL_BlitSurface(niveau[i], NULL, ecran, &posNiveau[i]);
        }

        // MàJ de l'écran
        SDL_Flip(ecran);
    }

    // LIBERATION DES SURFACES
    SDL_FreeSurface(menuJouer);

    for(i = 0; i < 10; i++)
    {
        SDL_FreeSurface(niveau[i]);
    }

    return valeur;
}




void initSurfacesLvl(SDL_Surface* niveau[], SDL_Rect posNiveau[])
{
    int i = 0, j = 0, posX = 2*T_ICONE, posY = 7*T_ICONE;

    for(i = 0; i < 2; i++)
    {
        for(j = 0; j < 5; j++)
        {
            niveau[5*i + j] = IMG_Load("vide.png");

            posNiveau[5*i + j].x = posX + 2*j*T_ICONE;
            posNiveau[5*i + j].y = posY + 3*i*T_ICONE;
        }
    }
}



int niveauxDispo()
{
    FILE *fichier = NULL;
    int nbDeLvl = 0;
    char fichierNiveau[T_MAX] = "";


    fichier = fopen("fichier_lvl.txt", "r");

    if(fichier == NULL)
    {
        exit(EXIT_FAILURE);
    }
    else
    {
        fgets(fichierNiveau, 10, fichier); // 10 est le nombre de caractères max pouvant être lus
        nbDeLvl = (int)strlen(fichierNiveau); // Le nombre de niveaux disponibles est stocké dans nbDeLvl

        fclose(fichier);
    }

    return nbDeLvl;
}
