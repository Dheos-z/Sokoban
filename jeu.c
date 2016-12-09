#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdlib.h>
#include <stdio.h>
#include "constantes.h"


int partie(SDL_Surface* ecran, char niveau[])
{
    int continuer = 1, valeur = 1, i = 0, j = 0, carte[MAP_HEIGHT][MAP_WIDTH] = {0}, nbCaissesUtile = 0, nbMursUtile = 0, nbObjectifsUtile = 0, numCaisse = 0, carteObjectifs[MAP_HEIGHT][MAP_WIDTH] = {0};
    SDL_Event event;
    SDL_Surface *mario = NULL, *caisse[NB_CAISSES_MAX] = {0}, *objectif[NB_CAISSES_MAX] = {0}, *mur[MAP_HEIGHT*MAP_WIDTH] = {0};
    SDL_Rect posMario, posCaisse[NB_CAISSES_MAX], posObjectif[NB_CAISSES_MAX], posMur[MAP_HEIGHT*MAP_WIDTH];


    // MISE EN PLACE GRAPHIQUE

    reset(niveau, &mario, &posMario, caisse, posCaisse, &nbCaissesUtile, mur, posMur, &nbMursUtile, objectif, posObjectif, &nbObjectifsUtile, carte, carteObjectifs);


    //////////// DEROULEMENT DU JEU ///////////////

    SDL_EnableKeyRepeat(100, 100);
    // Active les événements qd on enfonce une touche : le 1er para est le délai avant d'activer la fonctionnalité, le 2e para est le délai entre chaque événement après activation

    while(continuer)
    {
        SDL_WaitEvent(&event); // Active les événements en mode bloquant

        switch(event.type)
        {
        case SDL_QUIT:
            valeur = -1;
            continuer = 0; // Lorsque l'utilisateur clique sur la croix, le prg s'arrête
            break;


        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0; // La touche ECHAP permet de quitter le jeu
                break;

            case SDLK_UP:
                mario = IMG_Load("mario_haut.gif");

                verifieDeplacement(posMario, SDLK_UP, carte);

                break;

            case SDLK_DOWN:
                mario = IMG_Load("mario_bas.gif");

                verifieDeplacement(posMario, SDLK_DOWN, carte);

                break;

            case SDLK_LEFT:
                mario = IMG_Load("mario_gauche.gif");

                verifieDeplacement(posMario, SDLK_LEFT, carte);

                break;

            case SDLK_RIGHT:
                mario = IMG_Load("mario_droite.gif");

                verifieDeplacement(posMario, SDLK_RIGHT, carte);

                break;

            case SDLK_r: // Reset de la map

                reset(niveau, &mario, &posMario, caisse, posCaisse, &nbCaissesUtile, mur, posMur, &nbMursUtile, objectif, posObjectif, &nbObjectifsUtile, carte, carteObjectifs);

                break;
            }
            break;
        }


        // REPLACEMENT DES OBJECTIFS SUR LA CARTE + PLACEMENT DES ENTITES POUVANT ETRE DEPLACEES

        placeObjectifs(carte, carteObjectifs);
        deplaceEntite(carte, &posMario, posCaisse);



        // VERIFICATION DU PLACEMENT DE CHAQUE CAISSE

        numCaisse = 0;

        for(i = 0; i < MAP_HEIGHT; i++)
        {
            for(j = 0; j < MAP_WIDTH; j++)
            {
                if(carte[i][j] == CAISSE)
                {
                    caisse[numCaisse] = IMG_Load("caisse.jpg");
                    numCaisse++;
                }
                else if(carte[i][j] == CAISSE_OK)
                {
                    caisse[numCaisse] = IMG_Load("caisse_ok.jpg");
                    numCaisse++;
                }
            }
        }


        // RENOUVELLEMENT DE L'ECRAN

        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));


        // SURFACES A BLITTER

        for(i = 0; i < nbMursUtile; i++) // Murs
        {
            SDL_BlitSurface(mur[i], NULL, ecran, &posMur[i]);
        }

        for(i = 0; i < nbObjectifsUtile; i++) // Objectifs
        {
            SDL_BlitSurface(objectif[i], NULL, ecran, &posObjectif[i]);
        }

        for(i = 0; i < nbCaissesUtile; i++) // Caisses
        {
            SDL_BlitSurface(caisse[i], NULL, ecran, &posCaisse[i]);
        }

        SDL_BlitSurface(mario, NULL, ecran, &posMario); // Perso

        // MàJ de l'écran
        SDL_Flip(ecran);


        // VERIFICATION VICTOIRE

        if(verifieVictoire(carte))
        {
            valeur = victoire(ecran);
            continuer = 0;
        }
    }


    // FIN DU JEU : LIBERATION DES SURFACES

    SDL_FreeSurface(mario);

    for(i = 0; i < nbMursUtile; i++)
    {
        SDL_FreeSurface(mur[i]);
    }

    for(i = 0; i < nbCaissesUtile; i++)
    {
        SDL_FreeSurface(caisse[i]);
    }

    for(i = 0; i < nbObjectifsUtile; i++)
    {
        SDL_FreeSurface(objectif[i]);
    }

    return valeur;
}



///////////////////////////////////////////////// FONCTIONS //////////////////////////////////////////////////////////


void reset(char niveau[], SDL_Surface** perso, SDL_Rect* posPerso, SDL_Surface* caisse[], SDL_Rect posCaisse[], int *nbCaissesUtile, SDL_Surface* mur[], SDL_Rect posMur[], int *nbMursUtile, SDL_Surface* objectif[], SDL_Rect posObjectif[], int *nbObjectifsUtile, int carte[MAP_HEIGHT][MAP_WIDTH], int carteObjectifs[MAP_HEIGHT][MAP_WIDTH])
{
    int i = 0, j = 0, numCaisse = 0, numMur = 0, numObjectif = 0;
    char ligneFichier[MAP_HEIGHT][MAP_WIDTH] = {0};
    FILE* fichier = NULL;


    // RECUPERATION DU CONTENU DU NIVEAU DANS UN TABLEAU
    fichier = fopen(niveau, "r");

    if(fichier == NULL)
    {
        fprintf(stderr, "Erreur : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    else
    {
        for(i = 0; i < MAP_HEIGHT; i++)
        {
            for(j = 0; j < MAP_WIDTH; j++)
            {
                ligneFichier[i][j] = fgetc(fichier);
            }
            fgetc(fichier);
            fgetc(fichier);
        }
        fclose(fichier);
    }



    // RESET DE LA CARTE 2D

    for(i = 0; i < MAP_HEIGHT; i++)
    {
        for(j = 0; j < MAP_WIDTH; j++)
        {
            carte[i][j] = VIDE;
            carteObjectifs[i][j] = VIDE;
        }
    }


    // DEFINITION DES SPRITES ET DES POSITIONS DE CHAQUE SURFACE

    *nbMursUtile = 0;

    for(i = 0; i < MAP_HEIGHT; i++)
    {
        for(j = 0; j < MAP_WIDTH; j++)
        {
            if(ligneFichier[i][j] == '1')
            {
                carte[i][j] = CAISSE;
                caisse[numCaisse] = IMG_Load("caisse.jpg");
                posCaisse[numCaisse].x = j*T_ICONE;
                posCaisse[numCaisse].y = i*T_ICONE;
                numCaisse++;
            }
            else if(ligneFichier[i][j] == '3')
            {
                carte[i][j] = MUR;
                mur[numMur] = IMG_Load("mur.jpg");
                posMur[numMur].x = j*T_ICONE;
                posMur[numMur].y = i*T_ICONE;
                numMur++;
            }
            else if(ligneFichier[i][j] == '2')
            {
                carte[i][j] = OBJECTIF;
                carteObjectifs[i][j] = OBJECTIF;

                objectif[numObjectif] = IMG_Load("objectif.png");
                posObjectif[numObjectif].x = j*T_ICONE;
                posObjectif[numObjectif].y = i*T_ICONE;
                numObjectif++;
            }
            else if(ligneFichier[i][j] == '9')
            {
                carte[i][j] = PERSO;
                *perso = IMG_Load("mario_bas.gif");
                posPerso->x = j*T_ICONE;
                posPerso->y = i*T_ICONE;
            }
        }
    }

    *nbMursUtile = numMur;
    *nbCaissesUtile = numCaisse;
    *nbObjectifsUtile = numObjectif;

    // A SAVOIR : QUAND ON MANIPULE DES POINTEURS DE STRUCTURES DANS UNE FONCTION, ON PEUT MODIFIER LES SOUS-VARIABLES DE 2 MANIERES:
    // SOIT (*posPerso).x = a;
    // SOIT posPerso->x = a;


    return;
}


void verifieDeplacement(SDL_Rect posPerso, int touche, int carte[MAP_HEIGHT][MAP_WIDTH])
{
    switch(touche)
    {
    case SDLK_UP:

        if(carte[posPerso.y/T_ICONE - 1][posPerso.x/T_ICONE] != MUR && posPerso.y > 0) // Si le perso n'a pas un mur ou le bord de la map devant lui
        {
            if(carte[posPerso.y/T_ICONE - 1][posPerso.x/T_ICONE] == CAISSE || carte[posPerso.y/T_ICONE - 1][posPerso.x/T_ICONE] == CAISSE_OK) // Si il a une caisse devant lui
            {
                if(carte[posPerso.y/T_ICONE - 2][posPerso.x/T_ICONE] != CAISSE && carte[posPerso.y/T_ICONE - 2][posPerso.x/T_ICONE] != CAISSE_OK && carte[posPerso.y/T_ICONE - 2][posPerso.x/T_ICONE] != MUR && posPerso.y/T_ICONE > 1) // Et que derrière cette caisse se trouvent ni caisse ni mur ni bord de la map
                {
                    if(carte[posPerso.y/T_ICONE - 2][posPerso.x/T_ICONE] == OBJECTIF) // Et que la case derrière cette caisse est un objectif
                    {
                        carte[posPerso.y/T_ICONE - 2][posPerso.x/T_ICONE] = CAISSE_OK; // Alors la caisse est poussée sur un objectif
                    }
                    else
                    {
                        carte[posPerso.y/T_ICONE - 2][posPerso.x/T_ICONE] = CAISSE; // Sinon la caisse est poussée
                    }

                    carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE] = VIDE; // Alors le perso peut avancer
                    carte[posPerso.y/T_ICONE - 1][posPerso.x/T_ICONE] = PERSO;
                }
            }
            else // Si il n'a ni mur ni caisse devant lui, c'est sûr qu'il peut avancer
            {
                carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE] = VIDE; // Alors le perso peut avancer
                carte[posPerso.y/T_ICONE - 1][posPerso.x/T_ICONE] = PERSO;
            }
        }

        break;

    case SDLK_DOWN:
        if(carte[posPerso.y/T_ICONE + 1][posPerso.x/T_ICONE] != MUR && posPerso.y < HEIGHT) // Si le perso n'a pas un mur ou le bord de la map devant lui
        {
            if(carte[posPerso.y/T_ICONE + 1][posPerso.x/T_ICONE] == CAISSE || carte[posPerso.y/T_ICONE + 1][posPerso.x/T_ICONE] == CAISSE_OK) // Si il a une caisse devant lui
            {
                if(carte[posPerso.y/T_ICONE + 2][posPerso.x/T_ICONE] != CAISSE && carte[posPerso.y/T_ICONE + 2][posPerso.x/T_ICONE] != CAISSE_OK && carte[posPerso.y/T_ICONE + 2][posPerso.x/T_ICONE] != MUR && posPerso.y/T_ICONE < MAP_HEIGHT-2) // Et que derrière cette caisse se trouvent ni caisse ni mur ni bord de la map
                {
                    if(carte[posPerso.y/T_ICONE + 2][posPerso.x/T_ICONE] == OBJECTIF) // Et que la case derrière cette caisse est un objectif
                    {
                        carte[posPerso.y/T_ICONE + 2][posPerso.x/T_ICONE] = CAISSE_OK; // Alors la caisse est poussée sur un objectif
                    }
                    else
                    {
                        carte[posPerso.y/T_ICONE + 2][posPerso.x/T_ICONE] = CAISSE; // Sinon la caisse est poussée
                    }

                    carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE] = VIDE; // Alors le perso peut avancer
                    carte[posPerso.y/T_ICONE + 1][posPerso.x/T_ICONE] = PERSO;
                }
            }
            else // Si il n'a ni mur ni caisse devant lui, c'est sûr qu'il peut avancer
            {
                carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE] = VIDE; // Alors le perso peut avancer
                carte[posPerso.y/T_ICONE + 1][posPerso.x/T_ICONE] = PERSO;
            }
        }

        break;

    case SDLK_LEFT:


        if(carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE - 1] != MUR && posPerso.x > 0) // Si le perso n'a pas un mur ou le bord de la map devant lui
        {
            if(carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE - 1] == CAISSE || carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE - 1] == CAISSE_OK) // Si il a une caisse devant lui
            {
                if(carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE - 2] != CAISSE && carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE - 2] != CAISSE_OK && carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE - 2] != MUR && posPerso.x/T_ICONE > 1) // Et que derrière cette caisse se trouvent ni caisse ni mur ni bord de la map
                {
                    if(carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE - 2] == OBJECTIF) // Et que la case derrière cette caisse est un objectif
                    {
                        carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE - 2] = CAISSE_OK; // Alors la caisse est poussée sur un objectif
                    }
                    else
                    {
                        carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE - 2] = CAISSE; // Sinon la caisse est poussée
                    }

                    carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE] = VIDE; // Alors le perso peut avancer
                    carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE - 1] = PERSO;
                }
            }
            else // Si il n'a ni mur ni caisse devant lui, c'est sûr qu'il peut avancer
            {
                carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE] = VIDE; // Alors le perso peut avancer
                carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE - 1] = PERSO;
            }
        }

        break;

    case SDLK_RIGHT:

        if(carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE + 1] != MUR && posPerso.x + T_MARIO < WIDTH) // Si le perso n'a pas un mur ou le bord de la map devant lui
        {
            if(carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE + 1] == CAISSE || carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE + 1] == CAISSE_OK) // Si il a une caisse devant lui
            {
                if(carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE + 2] != CAISSE && carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE + 2] != CAISSE_OK && carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE + 2] != MUR && posPerso.x/T_ICONE < MAP_WIDTH-2) // Et que derrière cette caisse se trouvent ni caisse ni mur ni bord de la map
                {
                    if(carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE + 2] == OBJECTIF) // Et que la case derrière cette caisse est un objectif
                    {
                        carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE + 2] = CAISSE_OK; // Alors la caisse est poussée sur un objectif
                    }
                    else
                    {
                        carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE + 2] = CAISSE; // Sinon la caisse est poussée
                    }

                    carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE] = VIDE; // Alors le perso peut avancer
                    carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE + 1] = PERSO;
                }
            }
            else // Si il n'a ni mur ni caisse devant lui, c'est sûr qu'il peut avancer
            {
                carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE] = VIDE; // Alors le perso peut avancer
                carte[posPerso.y/T_ICONE][posPerso.x/T_ICONE + 1] = PERSO;
            }
        }

        break;
    }

    return;
}


void deplaceEntite(int carte[MAP_HEIGHT][MAP_WIDTH], SDL_Rect* posPerso, SDL_Rect posCaisse[NB_CAISSES_MAX])
{
    int i = 0, j = 0, numCaisse = 0;

    for(i = 0; i < MAP_HEIGHT; i++)
    {
        for(j = 0; j < MAP_WIDTH; j++)
        {
            if(carte[i][j] == CAISSE || carte[i][j] == CAISSE_OK)
            {
                posCaisse[numCaisse].x = j*T_ICONE;
                posCaisse[numCaisse].y = i*T_ICONE;
                numCaisse++;
            }
            else if(carte[i][j] == PERSO)
            {
                posPerso->x = j*T_ICONE;
                posPerso->y = i*T_ICONE;
            }
        }
    }

    return;
}


void placeObjectifs(int carte[MAP_HEIGHT][MAP_WIDTH], int carteObjectifs[MAP_HEIGHT][MAP_WIDTH])
{
    int i = 0, j = 0;

    for(i = 0; i < MAP_HEIGHT; i++)
    {
        for(j = 0; j < MAP_WIDTH; j++)
        {
            if(carteObjectifs[i][j] == OBJECTIF && carte[i][j] != PERSO && carte[i][j] != CAISSE_OK && carte[i][j] != OBJECTIF)
            {
                carte[i][j] = OBJECTIF;
            }
        }
    }

    return;
}


int verifieVictoire(int carte[MAP_HEIGHT][MAP_WIDTH]) // Renvoie 1 si gagné, 0 sinon
{
    int i = 0, j = 0, caisse = 1;

    while(caisse && i < MAP_HEIGHT)
    {
        for(j = 0; j < MAP_WIDTH; j++)
        {
            if(carte[i][j] == CAISSE)
            {
                caisse = 0;
            }
        }
        i++;
    }
    return caisse;
}
