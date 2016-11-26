#ifndef EDITEUR_H
#define EDITEUR_H

int editeur(SDL_Surface *ecran);
int menuEditeur(SDL_Surface* ecran);
void lectureNiveau(char niveau[], SDL_Surface** perso, SDL_Rect* posPerso, SDL_Surface* caisse[], SDL_Rect posCaisse[], int *nbCaisse, SDL_Surface* mur[], SDL_Rect posMur[], int *nbMur, SDL_Surface* objectif[], SDL_Rect posObjectif[], int *nbObjectif, int carte[MAP_HEIGHT][MAP_WIDTH]);
void placeEntite(int carte[MAP_WIDTH][MAP_HEIGHT], SDL_Rect posCurseur, int imageCurseur, SDL_Surface *curseur, SDL_Surface *entite[], SDL_Rect posEntite[], int *nbEntite);
void placePerso(int carte[MAP_WIDTH][MAP_HEIGHT], SDL_Rect posCurseur, SDL_Surface *curseur, SDL_Surface **perso, SDL_Rect *posPerso);
void supprimeEntite(int carte[MAP_WIDTH][MAP_HEIGHT], SDL_Rect posCurseur, SDL_Surface *entite[], SDL_Rect posEntite[], int *nbEntite);
void majNiveau(char fichierNiveau[], int carte[MAP_HEIGHT][MAP_WIDTH]);
void niveauComplet(int nbDeLvl);

#endif // EDITEUR_H
