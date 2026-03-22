#include "graphe.h"


station *new_station(int id, char *name){
    station *new=malloc(sizeof(station));
    if(!new) return NULL;

    new->id=id;
    new->nom = malloc(strlen(name) + 1);
    if (!new->nom) {
        free(new);
        return NULL;
    }
    strcpy(new->nom,name);
    new->suivant_st=NULL;

    return new;
}

void free_station(station *st){
    free(st->nom);
    free(st);
}

list_station *new_list_station(){
    list_station *new=malloc(sizeof(list_station));
    if(!new) return NULL;

    new->taille=0;
    new->tete=NULL;

    return new;
}

void free_list_station(list_station *list){
    station *tmp=list->tete;
    while(list->tete){
         tmp=list->tete;
         list->tete=tmp->suivant_st;
         free_station(tmp);
    }
    free(list);
}

void ajout_sommet_list_station(int id,char *nom,list_station *list){
        if(!nom) return ;
        station *new=new_station(id,nom);
        if(!new) return;
        if(!list->tete){
            list->tete=new;
            }
        else{
            new->suivant_st=list->tete;
            list->tete=new;
        }
        list->taille++;
}


sommet *new_sommet(int id, char *name,char *ligne ){
    if(!name || !ligne) return NULL;

    sommet *new=malloc(sizeof(sommet));
    if(!new) return NULL;
     
    new->ligne=malloc(strlen(ligne)+1);
    if(!new->ligne) return NULL;

    strcpy(new->ligne,ligne);
    
   
    new->parada=new_station(id,name);
   
    if(!new->parada){
        free(new);
        return NULL;
    }
   
    new->suivant=NULL;
    
    return new;

}

void free_sommet(sommet *s){
     free(s->ligne);
     free_station(s->parada);
     free(s);
}


arete *new_arete(int des, int cout){
    arete *new=malloc(sizeof(arete));
    if(!new) return NULL;

    new->cout=cout;
    new->id_destination=des;
    new->next=NULL;

    return new;
}

void free_arete(arete *ar){
    free(ar);
}

graph *new_graph(){
    graph *new=malloc(sizeof(graph));
    if(!new) return NULL;

    new->list_arete=NULL;
    new->list_sommet=NULL;
    new->nb_sommet=0;

    return new;
}

void free_graph(graph *graph){

    if(!graph) return;
    

    for(int i=0;i<graph->nb_sommet;i++){  
        sommet *tmp=graph->list_sommet;
        graph->list_sommet=graph->list_sommet->suivant;

        free_sommet(tmp);

        while ((graph->list_arete)[i])
        {
            arete *tmpr=(graph->list_arete)[i];
            (graph->list_arete)[i]=(graph->list_arete)[i]->next;
            free(tmpr);
        }
        
    }
    free(graph->list_arete);
    free(graph);
}

void ajouter_sommet(graph *g,int id,char *name,char *ligne){
     
    if(!g || !name || !ligne) return;

    sommet *s=new_sommet(id,name,ligne);
    if(!s) return;

    if(g->list_sommet ==NULL){
        g->list_sommet=s;
    }else{
        sommet *current=g->list_sommet;
        while(current->suivant){
            current=current->suivant;
        }
        current->suivant=s;
    }

    // reallouer le tableau 
    int old_n=g->nb_sommet;
    int nv_n=old_n+1;

    arete **new_liste_arete= realloc(g->list_arete,nv_n*sizeof(arete*));
    if(!new_liste_arete){
        perror("realloc new_liste_arete");
        
        return;
    }

    //les aretes pour ce nouveau sommet initialement vide
    g->list_arete=new_liste_arete;
    g->list_arete[old_n]=NULL;
    g->nb_sommet=nv_n;
}

void ajouter_arete(arete **tete,int des,int cout){
    arete *new=new_arete(des,cout);
    if(!new) return;

    new->next=*tete;
    *tete=new;

}

list_station* lire_station(char *filename){

    list_station *list=new_list_station();

     FILE * f=fopen(filename,"r");
     if (f==NULL){
         perror("Erreur ouverture fichier station");
         //free_list_station(list);
         return NULL ;
    }
    char nom_station[256];
    fgets(nom_station,sizeof(nom_station),f);/*ignorer la premiere ligne*/
   
    while(fgets(nom_station,sizeof(nom_station),f))
    {
     char *t=strtok(nom_station,",");
     if(!t) continue;
    t=strtok(NULL,",\n"); //le \n juste pour l'ingorer à la fin
     if(!t) continue;
     int id= atoi(t);
     ajout_sommet_list_station(id,nom_station,list) ;   //ligne sera mise à jour avec le fichier aretes!
    }
     fclose(f);
     return list;
}
char *find_name(list_station *list, int id ){
    if(!list || !list->tete ) return NULL;
    station *tmp=list->tete;
    while (tmp)
    {
        if(tmp->id==id){
            char *s=malloc(strlen(tmp->nom)+1);
             strcpy(s,tmp->nom);
             return s;
         }

         tmp=tmp->suivant_st;
    }
    return NULL;
}



int exist_sommet(graph *g,int id, char *ligne){
    
     if(!g || !ligne){
        perror("ligne de sommet ");
        exit(1);
     } 

    sommet *tmp=g->list_sommet;
    int index=0;
    while(tmp){
         if(tmp->parada->id==id && (strcmp(ligne,tmp->ligne)==0)){
            return index;
         }
         tmp=tmp->suivant;
         index++;
    }
    return -1;
}

graph *lire_aretes(char *filename,list_station *list_st){
     if(!list_st){
        perror("lire aretes (list_station =NULL)");
        return NULL;
     } 
     
     graph *g=new_graph();
     if(!g){
        perror("mallor graph");
        exit(1);
     }

     FILE * f=fopen(filename,"r");
     if (f==NULL){
         perror("Erreur ouverture fichier station");
         return NULL;
    }
    char ligne[256];
     fgets(ligne,sizeof(ligne),f);/*ignorer l'entete*/

     while(fgets(ligne,sizeof(ligne),f)){
        char *t=strtok(ligne,",");
        if(!t) continue;
        int src=atoi(t);

        t=strtok(NULL,",");
        if(!t) continue;
        int dest=atoi(t);

        t=strtok(NULL,",\n");
        if(!t) continue;
        char ligne_metro[8];
        strncpy(ligne_metro, t, 7);
        ligne_metro[7] = '\0';
        
        int index_src=exist_sommet(g,src,ligne_metro);
        int index_des=exist_sommet(g,dest,ligne_metro);
        
        if(index_des==-1){
            char *name=find_name(list_st,dest);
           
            if(!name) {
             printf("Erreur : station id %d non trouvée\n", dest);
             continue;
            }
            
            ajouter_sommet(g,dest,name,ligne_metro);
            free(name);//
            index_des=g->nb_sommet-1;
            }
        
        if(index_src==-1){
            char *name=find_name(list_st,src);
            if(!name) {
            printf("Erreur : station id %d non trouvée\n", dest);
            free(name);
            continue;
}

            ajouter_sommet(g,src,name,ligne_metro);
            free(name);
            ajouter_arete(&(g->list_arete[g->nb_sommet-1]),index_des,1);
        }else{
            ajouter_arete(&(g->list_arete[index_src]),index_des,1);
        }
        
        }
      
     sommet *s1=g->list_sommet;
     for(int i=0;i<g->nb_sommet;i++){
        sommet *s2=s1->suivant;
        for(int j=i+1;j<g->nb_sommet;j++){
            if(s1->parada->id==s2->parada->id){
                ajouter_arete(&(g->list_arete[i]),j,5);
                ajouter_arete(&(g->list_arete[j]),i,5);
            }
            s2=s2->suivant;
        }
        s1=s1->suivant;
     }
        
     fclose(f);
     return g;
}


void afficher_stations(list_station *list) {
    printf(YELLOW BOLD "\n📍 LISTE DES STATIONS\n" RESET);
    printf("--------------------------------------------------\n");

    station *s = list->tete;
    int i = 1;
    while (s) {
        
        
        printf(VIOLET "📍 %-20s " RESET "(ID: %d)\n", s->nom, s->id);

        s = s->suivant_st;
        i++;
    }

    printf("--------------------------------------------------\n");
}


int min_distance(int *dist, int *visited, int taille) {
    int min = INT_MAX;
    int index = -1;

    for (int i = 0; i < taille; i++) {
        if (!visited[i] && dist[i] < min) {
            min = dist[i];
            index = i;
        }
    }
    return index;
}



int find_index_sommet(graph *g, char *nom) {
    if (!nom || !g || !g->list_sommet) return -1;

    sommet *s = g->list_sommet;
    int index = 0;

    while (s) {
        if (s->parada && s->parada->nom && strcmp(s->parada->nom, nom) == 0)
            return index;
        s = s->suivant;
        index++;
    }
    return -1;
}

void afficher_chemin(int *prev, int src, int dest, graph *g) {

    int path[g->nb_sommet];
    int taille = 0;

    for (int v = dest; v != -1; v = prev[v]) {
        path[taille++] = v;
        if (v == src) break;
    }

    printf(BLUE BOLD "\n🧭 Itinéraire trouvé :\n" RESET);

    for (int i = taille - 1; i >= 0; i--) {
        sommet *s = g->list_sommet;
        for (int j = 0; j < path[i]; j++)
            s = s->suivant;

        printf(CYAN "🚉 %-20s " RESET "Ligne %s\n",
               s->parada->nom, s->ligne);

        if (i > 0)
            printf("   ⬇️\n");
    }
}

void dijkstra(graph *g, int src, int dest) {

    int *dist   = malloc(g->nb_sommet * sizeof(int));
    int *prev   = malloc(g->nb_sommet * sizeof(int));
    int *visited = malloc(g->nb_sommet * sizeof(int));
    if(!dist || !prev || !visited){
        perror("malloc tab");
        free(dist);
        free(prev);
        free(visited);
        exit(1);
    }

    for (int i = 0; i < g->nb_sommet; i++) {
        dist[i] = INT_MAX;
        prev[i] = -1;
        visited[i] = 0;
    }

    dist[src] = 0;

    for (int j = 0; j < g->nb_sommet; j++) {

        int dist_min = min_distance(dist, visited, g->nb_sommet);
        if (dist_min == -1) break;

        visited[dist_min] = 1;

        arete *ar = g->list_arete[dist_min];
        while (ar) {
            int index_adj = ar->id_destination;

            if (!visited[index_adj] && dist[dist_min] != INT_MAX &&
                dist[dist_min] + ar->cout < dist[index_adj]) {

                dist[index_adj] = dist[dist_min] + ar->cout;
                prev[index_adj] = dist_min;
            }

            ar = ar->next;
        }
    }

    
    afficher_trajet(prev,src,dest,g);


    free(dist);
    free(prev);
    free(visited);
}




void afficher_trajet(int *prev, int src, int dest, graph *g) {
    int chemin[g->nb_sommet];
    int taille = 0;

    for (int v = dest; v != -1; v = prev[v]) {
        chemin[taille++] = v;
        if (v == src) break;
    }

    printf("\n\033[1;35m===== ITINÉRAIRE =====\033[0m\n\n");

    int temps_total = 0;
    for (int i = taille - 1; i > 0; i--) {
        sommet *s_act = g->list_sommet;
        sommet *s_next = g->list_sommet;
        for (int j = 0; j < chemin[i]; j++) s_act = s_act->suivant;
        for (int j = 0; j < chemin[i-1]; j++) s_next = s_next->suivant;
    
        printf("\033[35;1m🚉 %-20s\033[0m\n", s_act->parada->nom);
        printf("│\n");

        
        int temps = 1;
        if (strcmp(s_act->ligne, s_next->ligne) != 0) {
            printf("│  \033[1;33m🔄 Metro %s → Metro %s (5 min, correspondance)\033[0m\n",
                    s_act->ligne, s_next->ligne);
            temps = 5;
        } else {
            printf("│  \033[1;32mMetro %s (%d min)\033[0m\n", s_act->ligne, temps);
        }

        printf("│\n");
        temps_total += temps;
    }


    sommet *last = g->list_sommet;
    for (int j = 0; j < chemin[0]; j++) last = last->suivant;
    printf("\033[35;1m🚉 %-20s\033[0m\n\n", last->parada->nom);

    printf("\033[1;36m ⏱️ Temps total estimé : %d min\033[0m\n\n", temps_total);
}

void afficher_logo_rarp() {
    printf("\n\033[35;1m");
    printf("██████╗  █████╗ ████████╗██████╗ \n");
    printf("██╔══██╗██╔══██╗╚══██╔══╝██╔══██╗\n");
    printf("██████╔╝███████║   ██║   ██████╔╝\n");
    printf("██╔══██╗██╔══██║   ██║   ██╔═══╝ \n");
    printf("██║  ██║██║  ██║   ██║   ██║     \n");
    printf("╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝   ╚═╝     \n");
    printf("\n   🚇 Régie Autonome des Transports Parisiens 🚇\n");
    printf("\033[0m\n");
}
