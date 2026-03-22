#ifndef GRAPHE_H
#define GRAPHE_H


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <limits.h>
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"
#define VIOLET "\033[35;1m" 


typedef struct station{
    int id;
    char *nom;
    struct station *suivant_st;
}station;

typedef struct list_station
{
    station *tete;
    int taille;
}list_station;

typedef struct sommet
{
    station *parada;
    char* ligne;   //il faut le changer pour un char ex="7bis"
    struct sommet *suivant;
}sommet;

typedef struct arete
{
    int cout;
    int id_destination;
    struct arete *next;
}arete;

typedef struct graph
{
    int nb_sommet ;
    sommet *list_sommet;
    arete **list_arete;
}graph;

station *new_station(int id, char *name);
list_station *new_list_station();
sommet *new_sommet(int id, char *name,char* ligne );
arete *new_arete(int des, int cout);
graph *new_graph();

void ajout_sommet_list_station(int id,char *nom,list_station *list);
void ajouter_sommet(graph *g,int id,char *name,char *ligne);
void afficher_stations(list_station *list);
void ajouter_arete(arete **tete,int des,int cout);

list_station *lire_station(char * filename);
graph *lire_aretes(char *filename,list_station *list_st);
char* find_name(list_station *list, int id);
int find_index_sommet(graph *g, char *nom);
int min_distance(int *dist, int *visite, int n) ;
void dijkstra(graph *g, int src, int dest) ;

void afficher_chemin(int *prev, int src,int dest, graph *g);
void afficher_trajet(int *prev, int src, int dest, graph *g);
void afficher_logo_rarp();

void free_station(station *st);
void free_list_station(list_station *list);
void free_sommet(sommet *s);
void free_arete(arete *ar);
void free_graph(graph *graph);




#endif