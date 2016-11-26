#ifndef JEU_H
#define JEU_H

#include "constantes.h"

int partie(SDL_Surface* ecran, char niveau[]);
void reset(char niveau[], SDL_Surface** perso, SDL_Rect* posPerso, SDL_Surface* caisse[], SDL_Rect posCaisse[], int *nbCaissesUtile, SDL_Surface* mur[], SDL_Rect posMur[], int *nbMursUtile, SDL_Surface* objectif[], SDL_Rect posObjectif[], int *nbObjectifsUtile, int carte[MAP_HEIGHT][MAP_WIDTH], int carteObjectifs[MAP_HEIGHT][MAP_WIDTH]);
void verifieDeplacement(SDL_Rect posPerso, int touche, int carte[MAP_HEIGHT][MAP_WIDTH]);
void deplaceEntite(int carte[MAP_HEIGHT][MAP_WIDTH], SDL_Rect* posPerso, SDL_Rect posCaisse[NB_CAISSES_MAX]);
void placeObjectifs(int carte[MAP_HEIGHT][MAP_WIDTH], int carteObjectifs[MAP_HEIGHT][MAP_WIDTH]);
int verifieVictoire(int carte[MAP_HEIGHT][MAP_WIDTH]);

#endif // JEU_H
