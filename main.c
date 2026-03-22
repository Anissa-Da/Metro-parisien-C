#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphe.h"


int main(){
    afficher_logo_rarp();
    graph *g;
    list_station *list=lire_station("Metro Paris Data - Stations.csv");
    g=lire_aretes("Metro Paris Data - Aretes.csv",list);
    

    int choix;
    char src[256];
    char dest[256];
    
    do{
    
    printf("\n\033[1;35m================ METRO PARISIEN ================\033[0m\n");
         printf("\033[1;36m1️⃣  Afficher toutes les stations\033[0m\n");
         printf("\033[1;36m2️⃣  Calculer mon itinéraire\033[0m\n");
         printf("\033[1;36m0️⃣  Quitter\033[0m\n");
         printf("\033[1;33mVotre choix : \033[0m");
        
    scanf("%d",&choix);

    switch(choix){
        case 1: afficher_stations(list); break;
        case 2:
            printf("Votre position (nom de station de départ) : ");
            getchar();
            fgets(src,sizeof(src),stdin);
            src[strcspn(src,"\n")]=0;

            printf("Votre destination (nom de station d'arrivee): ");
            fgets(dest,sizeof(dest),stdin);
            dest[strcspn(dest,"\n")]=0;

            int src_i = find_index_sommet(g,src);
            int dest_i = find_index_sommet(g,dest);
            if(src_i==-1 || dest_i==-1){
                printf("\033[31mStation source ou destination introuvable !\033[0m\n");
                break;
            }
            dijkstra(g,src_i,dest_i);
            break;
        case 0: printf("\033[35;1m👋 À bientôt !\033[0m\n"); break;
        default: printf("\033[31mVeuillez entrer un nombre entre 0 et 2 !\033[0m\n");
    }

} while(choix!=0);

    
        free_graph(g);
        g=NULL;
                
        free_list_station(list);
        list=NULL;
    return 0;
    
    

}