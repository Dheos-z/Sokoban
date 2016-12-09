#include <SDL/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL_image.h>
#include "constantes.h"
#include "editeur.h"
#include "choixNiveau.h"
#include "niveauEnregistre.h"


int editeur(SDL_Surface *ecran)
{
    int continuer = 1, valeur = 1, nbDeLvl = 0, carte[MAP_HEIGHT][MAP_WIDTH] = {0}, entite = MUR, nbCaisse = 0, nbMur = 0, nbObjectif = 0, i = 0, imageCurseur = 0;
    char fichierEdition[T_MAX] = "niveauX.txt";
    SDL_Event event;
    SDL_Surface *perso = NULL, *curseur = NULL, *caisse[MAP_HEIGHT*MAP_WIDTH] = {NULL}, *objectif[MAP_HEIGHT*MAP_WIDTH] = {NULL}, *mur[MAP_HEIGHT*MAP_WIDTH] = {NULL};
    SDL_Rect posPerso, posCurseur, posCaisse[MAP_HEIGHT*MAP_WIDTH], posObjectif[MAP_HEIGHT*MAP_WIDTH], posMur[MAP_HEIGHT*MAP_WIDTH];


    // DETERMINATION DU NOMBRE DE NIVEAUX DISPONIBLES
    nbDeLvl = niveauxDispo();


    // DETERMINATION DU NIVEAU A EDITER
    fichierEdition[6] = '0' + nbDeLvl;


    // MISE EN PLACE GRAPHIQUE DU NIVEAU
    lectureNiveau(fichierEdition, &perso, &posPerso, caisse, posCaisse, &nbCaisse, mur, posMur, &nbMur, objectif, posObjectif, &nbObjectif, carte);


    // Initialisation du curseur
    curseur = IMG_Load("mur.jpg");
    imageCurseur = MUR;


    ///////////////////////////////// BOUCLE PRINCIPALE /////////////////////////////////
    while(continuer)
    {
        SDL_WaitEvent(&event); // Active les ÈvÈnements en mode bloquant

        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            valeur = -1;
            break;


        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0;
                break;

                // Choisir perso
            case SDLK_KP0:
                curseur = IMG_Load("mario_bas.gif");
                imageCurseur = PERSO;
                break;

            case SDLK_0:
                curseur = IMG_Load("mario_bas.gif");
                imageCurseur = PERSO;
                break;

                // Choisir caisse
            case SDLK_KP1:
                curseur = IMG_Load("caisse.jpg");
                imageCurseur = CAISSE;
                break;

            case SDLK_1:
                curseur = IMG_Load("caisse.jpg");
                imageCurseur = CAISSE;
                break;

                // Choisir objectif
            case SDLK_KP2:
                curseur = IMG_Load("objectif.png");
                imageCurseur = OBJECTIF;
                break;

            case SDLK_2:
                curseur = IMG_Load("objectif.png");
                imageCurseur = OBJECTIF;
                break;

                // Choisir mur
            case SDLK_KP3:
                curseur = IMG_Load("mur.jpg");
                imageCurseur = MUR;
                break;

            case SDLK_3:
                curseur = IMG_Load("mur.jpg");
                imageCurseur = MUR;
                break;

            // Sauvegarde du niveau
            case SDLK_s:
                niveauComplet(nbDeLvl);
                valeur = niveauEnregistre(ecran);
                continuer = 0;
                break;
            }
            break;


        case SDL_MOUSEMOTION:
            posCurseur.x = ((event.motion.x)/T_ICONE) * T_ICONE;
            posCurseur.y = ((event.motion.y)/T_ICONE) * T_ICONE;
            break;


        case SDL_MOUSEBUTTONUP:
            switch(event.button.button)
            {
            // Placement de l'entitÈ
            case SDL_BUTTON_LEFT:
                if(carte[posCurseur.y/T_ICONE][posCurseur.x/T_ICONE] == VIDE)
                {
                    switch(imageCurseur)
                    {
                    case PERSO:
                        placePerso(carte, posCurseur, curseur, &perso, &posPerso);
                        break;

                    case CAISSE:
                        placeEntite(carte, posCurseur, CAISSE, curseur, caisse, posCaisse, &nbCaisse);
                        break;

                    case OBJECTIF:
                        placeEntite(carte, posCurseur, OBJECTIF, curseur, objectif, posObjectif, &nbObjectif);
                        break;

                    case MUR:
                        placeEntite(carte, posCurseur, MUR, curseur, mur, posMur, &nbMur);
                        break;
                    }
                }
                break;

            // Suppression de l'entitÈ
            case SDL_BUTTON_RIGHT:
                if(carte[posCurseur.y/T_ICONE][posCurseur.x/T_ICONE] != VIDE)
                {
                    switch(carte[posCurseur.y/T_ICONE][posCurseur.x/T_ICONE])
                    {
                    case PERSO:
                        break;

                    case CAISSE:
                        supprimeEntite(carte, posCurseur, caisse, posCaisse, &nbCaisse);
                        break;

                    case OBJECTIF:
                        supprimeEntite(carte, posCurseur, objectif, posObjectif, &nbObjectif);
                        break;

                    case MUR:
                        supprimeEntite(carte, posCurseur, mur, posMur, &nbMur);
                        break;
                    }
                }
                break;
            }
            break;
        }

        // RENOUVELLEMENT DE L'ECRAN
        SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));


        // BLIT DES SURFACES
        for(i = 0; i < nbMur; i++)
        {
            SDL_BlitSurface(mur[i], NULL, ecran, &posMur[i]);
        }

        for(i = 0; i < nbCaisse; i++)
        {
            SDL_BlitSurface(caisse[i], NULL, ecran, &posCaisse[i]);
        }

        for(i = 0; i < nbObjectif; i++)
        {
            SDL_BlitSurface(objectif[i], NULL, ecran, &posObjectif[i]);
        }

        SDL_BlitSurface(perso, NULL, ecran, &posPerso);
        SDL_BlitSurface(curseur, NULL, ecran, &posCurseur);

        // MAJ DE L'ECRAN
        SDL_Flip(ecran);
    }


    ////////////////// FIN DE LA BOUCLE DE JEU //////////////////


    // MISE A JOUR DU NIVEAU DANS LE FICHIER (AFIN DE POUVOIR CONTINUER L'EDITION PLUS TARD)
    majNiveau(fichierEdition, carte);


    // LIBERATION DES SURFACES
    if(perso != NULL) // Perso
    {
        SDL_FreeSurface(perso);
    }

    SDL_FreeSurface(curseur); // Curseur

    for(i = 0; i < nbMur+100; i++) // Murs
    {
        SDL_FreeSurface(mur[i]);
    }

    for(i = 0; i < nbCaisse; i++) // Caisses
    {
        SDL_FreeSurface(caisse[i]);
    }

    for(i = 0; i < nbObjectif; i++) // Objectifs
    {
        SDL_FreeSurface(objectif[i]);
    }

    return valeur;
}


//////////////////////////////// MENU EDITEUR ////////////////////////////////

int menuEditeur(SDL_Surface* ecran)
{
    int continuer = 1, valeur = 1;
    SDL_Event event;
    SDL_Surface *menu = NULL;
    SDL_Rect posMenu;

    menu = IMG_Load("editeur.png");
    posMenu.x = 0;
    posMenu.y = 0;

    while(continuer)
    {
        SDL_WaitEvent(&event);


        switch(event.type)
        {
        case SDL_QUIT:
            continuer = 0;
            valeur = 0;
            break;


        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                continuer = 0;
                break;

            case SDLK_KP_ENTER:
                continuer = editeur(ecran);
                break;
            }
            break;
        }


        // SI L'UTILISATEUR A CLIQUE SUR LA CROIX
        if(continuer == -1)
        {
            continuer = 0;
            valeur = 0;
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

// _______________________________________________________________________________________________

void lectureNiveau(char niveau[], SDL_Surface** perso, SDL_Rect* posPerso, SDL_Surface* caisse[], SDL_Rect posCaisse[], int *nbCaisse, SDL_Surface* mur[], SDL_Rect posMur[], int *nbMur, SDL_Surface* objectif[], SDL_Rect posObjectif[], int *nbObjectif, int carte[MAP_HEIGHT][MAP_WIDTH])
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
        }
        fclose(fichier);
    }



    // RESET DE LA CARTE 2D

    for(i = 0; i < MAP_HEIGHT; i++)
    {
        for(j = 0; j < MAP_WIDTH; j++)
        {
            carte[i][j] = VIDE;
        }
    }


    // DEFINITION DES SPRITES ET DES POSITIONS DE CHAQUE SURFACE

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

    *nbMur = numMur;
    *nbObjectif = numObjectif;
    *nbCaisse = numCaisse;

    return;
}

// _______________________________________________________________________________________________

void placeEntite(int carte[MAP_WIDTH][MAP_HEIGHT], SDL_Rect posCurseur, int imageCurseur, SDL_Surface *curseur, SDL_Surface *entite[], SDL_Rect posEntite[], int *nbEntite)
{
    carte[(posCurseur.y)/T_ICONE][(posCurseur.x)/T_ICONE] = imageCurseur;

    entite[(*nbEntite)] = curseur;
    posEntite[(*nbEntite)].x = posCurseur.x;
    posEntite[(*nbEntite)].y = posCurseur.y;

    (*nbEntite)++; // TRES IMPORTANT !!!!!! METTRE LES PARENTHESES SINON CA MARCHE PAS

    return;
}

// _______________________________________________________________________________________________

void placePerso(int carte[MAP_WIDTH][MAP_HEIGHT], SDL_Rect posCurseur, SDL_Surface *curseur, SDL_Surface **perso, SDL_Rect *posPerso)
{
    int i = 0, j = 0;

    for(i = 0; i < MAP_HEIGHT; i++)
    {
        for(j = 0; j < MAP_WIDTH; j++)
        {
            if(carte[i][j] == PERSO)
            {
                carte[i][j] = VIDE;
            }
        }
    }

    carte[(posCurseur.y)/T_ICONE][(posCurseur.x)/T_ICONE] = PERSO;

    *perso = IMG_Load("mario_bas.gif");
    posPerso->x = posCurseur.x;
    posPerso->y = posCurseur.y;

    return;
}

// _______________________________________________________________________________________________

void supprimeEntite(int carte[MAP_WIDTH][MAP_HEIGHT], SDL_Rect posCurseur, SDL_Surface *entite[], SDL_Rect posEntite[], int *nbEntite)
{
    int i = 0, j = 0, cherche = 1;

    while(i < *nbEntite && cherche)
    {
        if(posCurseur.x == posEntite[i].x && posCurseur.y == posEntite[i].y)
        {
            cherche = 0; // On ne cherche plus : on a trouvÈ l'entitÈ qu'il faut supprimer

            carte[posCurseur.y/T_ICONE][posCurseur.x/T_ICONE] = VIDE;
            (*nbEntite)--; // Il y a une entitÈ en moins

            for(j = i; j < *nbEntite; j++) // Ecrasement des positions de l'entitÈ ‡ supprimer
            {
                posEntite[j].x = posEntite[j+1].x;
                posEntite[j].y = posEntite[j+1].y;
            }
        }

        i++;
    }

    return;
}

// _______________________________________________________________________________________________

void majNiveau(char fichierNiveau[], int carte[MAP_HEIGHT][MAP_WIDTH]) // Enregistre le niveau pour qu'il puisse Ítre continuÈ plus tard
{
    int i = 0, j = 0;
    FILE *fichier = NULL;

    fichier = fopen(fichierNiveau, "w");

    if(fichier != NULL) // On peut Ècrire dans le fichier
    {
        for(i = 0; i < MAP_HEIGHT; i++)
        {
            for(j = 0; j < MAP_WIDTH; j++)
            {
                fputc('0' + carte[i][j], fichier);
            }
            fputc('\n', fichier);
        }

        fclose(fichier);
    }

    return;
}

// _______________________________________________________________________________________________

void niveauComplet(int nbDeLvl) // Indique que le niveau est complet
{
    FILE *fichier = NULL;

    fichier = fopen("fichier_lvl.txt", "a");

    if(fichier != NULL)
    {
        fputc('0' + nbDeLvl, fichier);
    }

    return;
}
