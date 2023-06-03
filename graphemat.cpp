
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphemat.h"
#include "liste.h"
#include <time.h>
#include <math.h>

#define SOMMET_INITIAL 0

typedef char NomS[50];
static int noeudsVisite=0;
static int* cout = (int*)malloc(sizeof(int)*100) ;
static booleen trouveS = faux;
booleen  grapheDetruit = vrai;
static booleen check=faux;
static NomS* chemin = (NomS*) malloc((sizeof(NomS))*100);


static float coutTotal =0;
static int nbEltTab=0;
static float valeur=0;



void initMarque (GrapheMat* graphe)
{
    for (int i=0; i<graphe->nMax; i++)
        graphe->marque [i] = faux;
}

GrapheMat* creerGrapheMat (int nMax, int value)
{
    // allocation de graphe
    GrapheMat* graphe  = (GrapheMat*) malloc (sizeof (GrapheMat));
    graphe->n       = 0;
    graphe->nMax    = nMax;
    graphe->value   = value; // value : vrai si le graphe est valué
    graphe->nomS    = (NomSom*)  malloc (sizeof(NomSom)  *nMax);
    graphe->marque  = (booleen*) malloc (sizeof(booleen) *nMax);
    graphe->element = (int*)     malloc (sizeof(int)*nMax*nMax);
    graphe->valeur  = (int*)     malloc (sizeof(int)*nMax*nMax);
    // initialisation par défaut
    for (int i=0; i<nMax; i++)
    {
        for (int j=0; j<nMax; j++)
        {
            graphe->element [i*nMax+j] = faux;
            graphe->valeur  [i*nMax+j] = INFINI;
        }
    }
    initMarque (graphe);
    grapheDetruit = faux;
    return graphe;
}


// désallocation d'un graphe
void detruireGraphe (GrapheMat* graphe)
{
    free (graphe->nomS);
    free (graphe->marque);
    free (graphe->element);
    free (graphe->valeur);
    free (graphe);
    grapheDetruit = vrai;
}
//? rang sees if a node name is already in the graph. if it exists, it returns its index. if not, it returns -1
int rang (GrapheMat* graphe, NomSom nom)
{
    int      i      = 0;
    booleen  trouve = faux;
    while ( (i<graphe->n) && !trouve)
    {
        trouve = strcmp (graphe->nomS [i], nom) == 0;
        if (!trouve)
            i++;
    }
    return trouve ? i : -1;
}

void ajouterUnSommet (GrapheMat* graphe, NomSom nom)
{
    if (rang (graphe, nom) == -1)
    {
        if (graphe->n < graphe->nMax)
        {
            strcpy (graphe->nomS [graphe->n++], nom);
        }
        else
        {
            printf ("\nNombre de sommets > %d\n", graphe->nMax);
        }
    }
    else
    {
//        printf ("\n%s already defined\n", nom);
    }
}

void ajouterUnArc (GrapheMat* graphe, NomSom somD, NomSom somA, int cout)
{
    int nMax = graphe->nMax;
    int rd = rang (graphe, somD);
    int rg = rang (graphe, somA);
    graphe->element [rd*nMax+rg] = vrai;
    graphe->valeur  [rd*nMax+rg] = cout;
}

void ecrireGraphe (GrapheMat* graphe)
{
    if (grapheDetruit == vrai)
    {
        printf ("graph doesn't exist or has been destroyed\n");
    }
    else
    {
        int nMax = graphe->nMax;
        for (int i=0; i<graphe->n; i++)
            printf ("%s ", graphe->nomS[i]);
        printf (";\n");
        for (int i=0; i<graphe->n; i++)
        {
            printf ("\n%s : ", graphe->nomS[i]);
            for (int j=0; j<graphe->n; j++)
            {
                if (graphe->element [i*nMax+j] == vrai)
                {
                    printf ("%s ", graphe->nomS[j]) ;
                    if (graphe->value)
                    {
                        printf (" (%d) ", graphe->valeur [i*nMax+j] );
                    }
                }
            }
            printf (";");
        }
    }
}
//!############################################################################################
static void profondeur (GrapheMat* graphe, int numSommet)
{
    int nMax = graphe->nMax;

    graphe->marque [numSommet] = vrai;
    printf ("%s\n", graphe->nomS [numSommet]);

    for (int i=0; i<graphe->n; i++)
    {
        if ( (graphe->element [numSommet*nMax+i] == vrai)
                && !graphe->marque [i] )
        {
            profondeur (graphe, i);
        }
    }
}


void parcoursProfond (GrapheMat* graphe)
{
    initMarque (graphe);
    for (int i=0; i<graphe->n; i++)
    {
        if (!graphe->marque [i])  //printf("i>> %d",i);
        {
            profondeur (graphe, i);
        }
    }
}
//!############################################################################################
static void largeur (GrapheMat* graphe, int numSommet)
{
    int nMax = graphe->nMax;
    if(graphe->marque [numSommet]!=vrai)
    {
        graphe->marque [numSommet] = vrai;
        printf ("%s\n", graphe->nomS [numSommet]);
    }
    int tab[10];
    for(int i=0; i<10; i++)
    {
        tab[i]=-1;
    }

    for (int i=0,j=0; i<graphe->n; i++)
    {
        if ( (graphe->element [numSommet*nMax+i] == vrai)
                && !graphe->marque [i] )
        {
            tab[j]=i;
            j++;

        }
    }
    for(int i=0; tab[i]!=-1; i++)
    {

        printf("%s\n",graphe->nomS[tab[i]]);
        graphe->marque [tab[i]] = vrai;
    }

    if(tab[0]!=-1)
        largeur(graphe,tab[0]);
    for(int i=0; i<10; i++)
    {
        tab[i]=-1;
    }
}

void parcoursLargeur(GrapheMat* graphe)
{

    initMarque (graphe);
    for (int i=0; i<graphe->n; i++)
    {
        if (!graphe->marque [i])
        {

            largeur (graphe, i);
        }
    }
}
//!############################################################################################
static int largeurEx (GrapheMat* graphe, Liste* li, int numSommet,char but[])
{
    int nMax = graphe->nMax;
    NomSom* extraite=NULL;
    insererEnFinDeListe (li,graphe->nomS[numSommet]);
    graphe->marque [numSommet]=true;
    while (!listeVide (li) )
    {
        extraite = (NomSom*) extraireEnTeteDeListe (li);
        noeudsVisite++;
        if(strcmp(*extraite,but)==0)
        {
            printf("  ->  %s (Noeud but) \n",but);
            return 1;
        }
        numSommet = rang (graphe,*extraite);
        printf("->%s",*extraite);

        for (int i=0; i<graphe->nMax; i++)
        {
            if ( (graphe->element [numSommet*nMax+i] == vrai)
                    && !graphe->marque [i] )
            {
                insererEnFinDeListe(li,graphe->nomS[i]);
                graphe->marque[i] = vrai;
            }
        }
    }
    if(numSommet==graphe->n)
    {
        printf("\nLe noeud but %s est INTROUVABLE. ",but);
        return 0;
    }
}


void parcoursLargeurEx(GrapheMat* graphe)
{    printf("*******Parcours en Largeur******* : \n\n");

    clock_t begin, end;
    double tempsExec;
    initMarque(graphe);
    Liste* li = creerListe();
    char str[20];

    printf("Entrez le sommet but :  ");
    scanf("%s", str);
    printf("******* Chemin du le parcours ******* :   \n\n");
    begin = clock();
    int trouve=0;
    for (int i=0; i<graphe->n; i++)
    {
        if(!graphe->marque[i])
        {
            trouve=largeurEx(graphe,li,i,str);
        }
        if(trouve==1)
            break;
    }
    end = clock();
    tempsExec = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n Le temps d'exécution :  %f",tempsExec);
    printf("\n Le nombre de Noeuds visites : %d",noeudsVisite);
    noeudsVisite=0;
}
//!############################################################################################
static void coutUniforme (GrapheMat* graphe, Liste* li, int numSommet, int but)
{
    for(int j=0; j<100; j++)
    {
        strcpy (*(chemin+j),"");
    }
    strcpy (*(chemin+numSommet),graphe->nomS[numSommet]);
    int nMax = graphe->nMax;
    Element* extraite=NULL;
    insererEnFinDeListe (li,graphe->nomS[numSommet],cout+numSommet);
    graphe->marque [numSommet]=vrai;
    while (!listeVide (li)  )
    {
        extraite = (Element*) extraireEnTeteDeListe(li);
        noeudsVisite++;
        numSommet = rang (graphe,(char*)extraite);
        if(numSommet==but)
        {
            trouveS=vrai;
            return;
        }
        for (int i=0; i<graphe->n; i++)
        {
            if ( (graphe->element [numSommet*nMax+i] == vrai) && !graphe->marque [i] )
            {
                strcat (*(chemin+i),*(chemin+numSommet));
                strcat (*(chemin+i),"->");
                strcat (*(chemin+i),graphe->nomS[i]);
                *(cout+i)=graphe->valeur[numSommet*nMax+i]+*(cout+numSommet );

                insererEnOrdre(li,graphe->nomS[i],cout+i);
                graphe->marque[i] = vrai;

            }
            else
            {
                booleen trouvee = chercherUnObjetBis(li,graphe->nomS[i]);
                if((graphe->element [numSommet*nMax+i] == vrai)&& trouvee &&*(cout+i)>graphe->valeur[numSommet*nMax+i]+*(cout+numSommet ))
                {
                    *(cout+i)=graphe->valeur[numSommet*nMax+i]+*(cout+numSommet );
                    strcpy (*(chemin+i),"");
                    strcat (*(chemin+i),*(chemin+numSommet));
                    strcat (*(chemin+i),"->");
                    strcat (*(chemin+i),graphe->nomS[i]);
                }
            }
        }
    }
}
void parcoursCoutUniforme (GrapheMat* graphe )
{
    clock_t begin, end;
    double tempsExec;
    int but;
    printf("Entrez le nombre du sommet but  : ");
    scanf("%d", &but);
    Liste* li=creerListe(1);
    initMarque (graphe);
    for(int j=0; j<graphe->n; j++)
    {
        *(cout+j)=0;
    }
    begin = clock();
    for (int i=0; i<graphe->n; i++)
    {
        if (!graphe->marque [i])
        {
            *(cout+i)=0;
            coutUniforme (graphe, li,i,but);
        }
        break;
    }
    if(trouveS)
    {
        printf("\n ****** Le chemin du parcours  ******  :  ");
        printf("%s\n",*(chemin+but));
        printf("\nLe cout du  chemin :  %d ",*(cout+but));
        trouveS=faux;
    }
    else
    {
        printf("\nLE Noeud EST INTROUVABLE");
    }
    end = clock();
    tempsExec = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nLe temps pris :  %f",tempsExec);
    printf("\nNoeuds visites : %d",noeudsVisite);
    noeudsVisite=0;
}
//!############################################################################################
static void profondeurDabord (GrapheMat* graphe, int numSommet,char but[])
{
    int nMax = graphe->nMax;

    graphe->marque [numSommet] = vrai;              // marking our starting point as visited
    if(strcmp(graphe->nomS[numSommet],but)==0)      // checking if the starting point is the but
    {
        check=vrai;
        printf("%s -> BUT ",but);
        return ;
    }
    printf ("%s\n", graphe->nomS [numSommet]);

    for (int i=0; i<graphe->n; i++)
    {
        if ( (graphe->element [numSommet*nMax+i] == vrai) && !graphe->marque [i] )
        {
            if (check!=vrai) profondeurDabord (graphe, i,but);
        }
    }
}
void parcoursProfondAbord (GrapheMat* graphe)
{
    initMarque (graphe);
    char str[20];

    printf("Entrez le sommet but :  ");
    scanf("%s", str);
    printf("\n****** Le chemin du parcours ****** : \n");

    for (int i=0; i<graphe->n; i++)
    {
        if (!graphe->marque [i])
        {
            profondeurDabord (graphe, i,str);
            if(check== vrai)
            {
               break;
            }

        }
    }
    if(check!=vrai)
    {
        printf("\n Noeuds INTROUVABLE");
    }

    check=faux;
}
//!############################################################################################
// Profondeur Limite

static booleen trouve=false;

static void profondeurLimite (GrapheMat* graphe,int k, char but[], int limite)
{

    if(!trouveS)
    {
        noeudsVisite++;
        int nMax = graphe->nMax;
        if(strcmp(graphe->nomS[k],but) == 0)
        {
            printf(" %s  ",graphe->nomS[k]);
            trouveS = vrai;
            return;
        }
        else if(limite <= 0)
        {
            printf("%s  ",graphe->nomS[k]);
            graphe->marque[k] = vrai;
            return;
        }
        else
        {
            if(limite>0)
            {
                printf("%s  ",graphe->nomS[k]);
                graphe->marque[k] = vrai;
                for(int i = 0; i < graphe->n; i++)
                {

                    if ( (graphe->element [k*nMax+i] == vrai) && !graphe->marque [i] && !trouveS )
                    {

                        profondeurLimite (graphe,i, but,limite-1);
                    }
                }
            }
        }
    }
}


void parcoursProfendeurLimite(GrapheMat* graphe)
{
    clock_t begin, end;
    double tempsExec;
    initMarque(graphe);
    Liste* li = creerListe();
    char str[20];

    printf("Entrer le sommet but :  ");
    scanf("%s", str);
    int limite;
    printf("Entrer la limite (niveau) :   ");
    scanf("%d",&limite);
    printf(" \n******* Parcours Profondeur Limite ******* :  \n \n");
    begin = clock();
    //int trouve=0;
    for (int i=0; i<graphe->n; i++)
    {
        if(!graphe->marque[i])
            profondeurLimite(graphe,i,str,limite);

        break;
    }
    if(trouveS==faux)
        printf("\nLe Noeud est INTROUVABLE\n");
    else
    {
        end = clock();
        tempsExec = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("\n\n Le temps d'execution :  %f",tempsExec);
        printf("\n Le nombre de Noeuds visites : %d",noeudsVisite);
    }
    noeudsVisite=0;
    trouveS=faux;
}



static void profondeurIteratif (GrapheMat* graphe, int numSommet,int numNiveau,char but[])
{
    if(!trouveS)
    {
        noeudsVisite++;
        if(strcmp(graphe->nomS[numSommet],but)==0)
        {
            printf("%s -> But ",but);
            trouveS=true;
        }
        else if(numNiveau==0)
        {
            printf ("-> %s -> ", graphe->nomS [numSommet]);
            graphe->marque [numSommet] = vrai;
        }
        else
        {
            if (numNiveau>0)
            {
                int nMax = graphe->nMax;
                graphe->marque [numSommet] = vrai;
                printf ("-> %s ", graphe->nomS [numSommet]);
                for (int i=0; i<graphe->n; i++)
                {
                    if ( (graphe->element [numSommet*nMax+i] == vrai)
                            && !graphe->marque [i] )
                    {
                        profondeurIteratif(graphe, i, numNiveau-1,but);
                    }
                }
            }
        }
    }
}

void parcoursProfondeurIteratif(GrapheMat* graphe)
{
    clock_t begin, end;
    double tempsExec;
    char str[20];
    int niveau,numNiveau=0;
    printf("Entrer le sommet but  : ");
    scanf("%s", str);
    printf("Entrer la limite (niveau) : ");
    scanf("%d", &niveau);
    initMarque (graphe);
    printf("Chemin du parcous  :  \n\n");
    begin = clock();
    for ( numNiveau=0; numNiveau<=niveau; numNiveau++)
    {
        if (trouveS)
            break;
        printf("Limite %d : ",numNiveau);
        initMarque (graphe);
        for (int i=0; i<graphe->n; i++)
        {
            if(!graphe->marque[i])
                profondeurIteratif(graphe,i,numNiveau,str);
            break;
        }
        printf("\n");
    }
    if(!trouveS)
    {
        trouveS=false;
        printf("\nLe noeud %s est INTROUVABLE",str);
    }
    else
    {
        printf("\n***** Le noeud est trouve dans la limite \" %d \" *****\n\n",numNiveau-1);
    }
    end = clock();
    tempsExec = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n Le temps d'execution :  %f",tempsExec);
    printf("\n Les noeuds visites : %d",noeudsVisite);
    noeudsVisite=0;
    trouveS=false;
}


//!############################################################################################
static void A (GrapheMat* graphe, Liste* li, int numSommet, char but[])
{
    int* fn=(int*)malloc(sizeof(int)*graphe->n);
    for(int j=0; j<graphe->n; j++)
    {
        *(fn+j)=0;
    }

//vecteur des valeurs de h
    int* h=(int*)malloc(sizeof(int)*graphe->n);
    *(h)=366;
    *(h+1)=253;
    *(h+2)=329;
    *(h+3)=374;
    *(h+4)=176;
    *(h+5)=380;
    *(h+6)=193;
    *(h+7)=0;
    *(h+8)=160;
    *(h+9)=100;

    for(int j=0; j<100; j++)
    {
        strcpy (*(chemin+j),"");
    }
    strcpy (*(chemin+numSommet),graphe->nomS[numSommet]);   // copying the start of the chemin, root of the search
    int nMax = graphe->nMax;
    Element* extraite=NULL;
    *(fn+numSommet)=*(cout+numSommet)+h[numSommet];
    insererEnFinDeListe (li,graphe->nomS[numSommet],fn+numSommet);
    graphe->marque [numSommet]=vrai;
    while (!listeVide (li)  )
    {
        extraite = (Element*) extraireEnTeteDeListe(li);

        noeudsVisite++;
        numSommet = rang (graphe,(char*)extraite);
        printf("(%s,%d)\t", graphe->nomS[numSommet],fn[numSommet]);
        if(strcmp(graphe->nomS[numSommet],but)==0)
        {
            trouveS=vrai;
            return;
        }
        for (int i=0; i<graphe->n; i++)
        {
            if ( (graphe->element [numSommet*nMax+i] == vrai) && !graphe->marque [i] )
            {
                strcat (*(chemin+i),*(chemin+numSommet));
                strcat (*(chemin+i),"->");
                strcat (*(chemin+i),graphe->nomS[i]);
                *(cout+i)=graphe->valeur[numSommet*nMax+i]+*(cout+numSommet );
                *(fn+i)=*(cout+i)+h[i];
                insererEnOrdre(li,graphe->nomS[i],fn+i);
                graphe->marque[i] = vrai;

            }
            else
            {
                booleen trouvee = chercherUnObjetBis(li,graphe->nomS[i]);
                if((graphe->element [numSommet*nMax+i] == vrai)&& trouvee && *(fn+i)>graphe->valeur[numSommet*nMax+i]+*(cout+numSommet)+*(h+i))
                {

                    *(cout+i)=graphe->valeur[numSommet*nMax+i]+*(cout+numSommet );
                    *(fn+i)=*(cout+i)+h[i];
                    strcpy (*(chemin+i),"");
                    strcat (*(chemin+i),*(chemin+numSommet));
                    strcat (*(chemin+i),"->");
                    strcat (*(chemin+i),graphe->nomS[i]);
                    booleen flag=extraireUnObjet(li,graphe->nomS[i]);
                    insererEnOrdre(li,graphe->nomS[i],fn+i);

                }
            }
        }
    }
}
void AEtoile (GrapheMat* graphe )
{
    char but[20];
    printf("Enter the target node : ");
    scanf("%s", but);
    int num=rang(graphe,but);
    Liste* li=creerListe(1);
    initMarque (graphe);
    for(int j=0; j<graphe->n; j++)
    {
        *(cout+j)=0;                    // cout is an int table above
    }
    for (int i=0; i<graphe->n; i++)
    {
        if (!graphe->marque [i])
        {
            *(cout+i)=0;
            A(graphe, li,i,but);
        }
        break;
    }
    if(trouveS)
    {
        printf("\n\n ********* Le chemin trouve vers le noeud  %s *********  :",graphe->nomS[num]);
        printf("%s\n",*(chemin+num));
        printf("Le cout du  chemin :  %d ",*(cout+num));
        trouveS=faux;
    }
    else
    {
        printf("\nLE Noeud EST INTROUVABLE");
    }
    printf("\nNoeuds visites : %d",noeudsVisite);
    noeudsVisite=0;
}
//!############################################################################################

static void ecrireEtape (Matrice a, Matrice p, int k, int ns, int nMax)
{
    printf ("Passage par le sommet nb %d\n", k);

    for (int i=0; i<ns; i++)
    {
        for (int j=0; j<ns; j++)
        {
            if (a [i*nMax+j]==INFINI)
            {
                printf (" %3s", "*");

            }
            else
            {
                printf (" %3d", a [i*nMax+j]);
            }
        }
        printf ("%6s", " ");
        for (int j=0; j<ns; j++)
        {
            printf ("%3d", p [i*nMax+j]);
        }
        printf ("\n");
    }
    printf ("\n");
}


static void initFloyd (GrapheMat* graphe, Matrice* a, Matrice* p)
{
    int nMax = graphe->nMax;

    Matrice ta = (int*) malloc (sizeof(int)*nMax*nMax);
    Matrice tp = (int*) malloc (sizeof(int)*nMax*nMax);


    for (int i=0; i<graphe->n; i++)
    {
        for (int j=0; j<graphe->n; j++)
        {
            if(i==j)
            {
                ta [i*nMax+j]=0;
                tp [i*nMax+j] = i;
            }
            else
            {
                ta [i*nMax+j] = graphe->valeur [i*nMax+j];
                tp [i*nMax+j] = i;
            }
        }
    }
    *a = ta;
    *p = tp;
}

void floyd (GrapheMat* graphe)
{
    Matrice a, p;
    int n = graphe->n;
    int nMax = graphe->nMax;

    initFloyd (graphe, &a, &p);


    for (int k=0; k<n; k++)
    {
        for (int i=0; i<n; i++)
        {
            for (int j=0; j<n; j++)
            {
                if ( (a [i*nMax+k] != INFINI) &&
                        (a [k*nMax+j] != INFINI) &&
                        (a [i*nMax+k] + a [k*nMax+j] < a [i*nMax+j]) )
                {
                    a [i*nMax+j] = a [i*nMax+k] + a [k*nMax+j];
                    p [i*nMax+j] = p [k*nMax+j];
                }
            }
        }
        ecrireEtape (a, p, k, n, nMax);
    }
}


int const depart=0;

static void plusProcheVoisin (GrapheMat* graphe, int numSommet,int tab[])
{
    int nMax = graphe->nMax;
    if(nbEltTab!=graphe->n)
    {
        valeur=100000;
        tab[nbEltTab]=numSommet;
        nbEltTab++;
        graphe->marque [numSommet]=true;
        for (int i=0; i<graphe->n; i++)
        {
            if ( (graphe->element [numSommet*nMax+i] == vrai)
                    && !graphe->marque [i] )
            {
                if (valeur>graphe->valeur [numSommet*nMax+i])
                {
                    valeur=graphe->valeur [numSommet*nMax+i];
                    tab[nbEltTab]=i;
                }
            }
        }
        if(valeur!=100000)
            coutTotal+=valeur;
        plusProcheVoisin(graphe,tab[nbEltTab],tab);
    }
    else
    {
        coutTotal+=graphe->valeur [tab[nbEltTab-1]*nMax+depart];
        tab[nbEltTab]=depart;
    }
}
//!############################################################################################

float coutTrajet(GrapheMat* graphe,int tab[])
{
    float valeurTemp=0;
    int nMax=graphe->nMax;
    for(int i=0; i<(graphe->n); i++)
    {
        valeurTemp+= graphe->valeur[tab[i]*nMax+tab[i+1]];      // value of travel between node i and i+1
    }
    return valeurTemp;
}

static void inverserTableau(int t[],int debut,int fin)
{
    int temp;

    for(int i=debut; i<(fin+debut+1)/2; i++)
    {
        temp=t[i];
        t[i]=t[(fin+1+debut)-i-1];
        t[(fin+1+debut)-i-1]=temp;
    }

}


static void Escalade(GrapheMat* graphe, int numSommet, int tab[])
{
    int tabTemporaire[(graphe->n)+1];               // +1 so last element repeats
    int tabBut[(graphe->n)+1];
    valeur=0;

    for(int k=0; k<(graphe->n)+1; k++)
    {
        tabTemporaire[k]=tab[k];
    }
    for(int i=1; i<graphe->n; i++)
    {

        for(int j=i+1; j<graphe->n; j++)
        {
            if(i==1&&j==(graphe->n)-1)
            {
                continue;
            }
            printf(" Invereser %d %d\t",i,j);
            inverserTableau(tab,i,j);

            for(int k=0; k<(graphe->n)+1; k++)
            {

                printf("%d",tab[k]);
            }

            printf("   %f",coutTrajet(graphe,tab));
            printf("\n");


            if(coutTrajet(graphe,tabTemporaire)>coutTrajet(graphe,tab))
            {
                valeur=coutTrajet(graphe,tab);
                 for(int k=0; k<(graphe->n)+1; k++)
                    {
                        tabTemporaire[k]=tab[k];
                        tabBut[k]=tab[k];
                    }
            }
            else
            {
                valeur=coutTrajet(graphe,tabTemporaire);
                for(int n=0; n<(graphe->n)+1; n++)
                {
                    tabBut[n]=tabTemporaire[n];
                }
            }

            for(int k=0; k<(graphe->n)+1; k++)
            {
                tab[k]=tabBut[k];
            }
        }
    }
    for(int n=0; n<(graphe->n)+1; n++)
    {
        tab[n]=tabBut[n];
    }
}

void parcoursEscalade(GrapheMat* graphe)
{
//    clock_t begin, end;
//    double time_spent;
    int tab[] = {0,3,2,4,1,0};          // le trajet initiale (par plus proche voisin)
    initMarque(graphe);
//    begin = clock();
    for (int i=0; i<graphe->n; i++)
    {
        if(!graphe->marque[i])
        {

            float cout=coutTrajet(graphe,tab);
            printf("***** le trajet de depart ***** :    ");
            for(int k=0; k<(graphe->n)+1; k++)
            {
                printf("  A%d  ",tab[k]);
            }
            printf("\n   le cout : %d   \n",(int)coutTrajet(graphe,tab));
            printf("\n");
            Escalade(graphe,i,tab);
        }
        break;
    }
//    end = clock();
//    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
//    printf("\n ******* Le temps pris par le parcours d'escalade ******* :  %f \n",time_spent);
    printf("\n ******* Le trajet du parcours d'escalade ******* :  \n\n");
    for(int k=0; k<(graphe->n)+1; k++)
    {
        printf(" %d ",tab[k]);
        if (k==(graphe->n)) continue;
        printf("->");
    }
    printf("\n\n Le meilleur cout d'escalade : %d \n ",(int)valeur);
    valeur=0;
    nbEltTab=0;
    coutTotal=0;
}
//!############################################################################################


//// percoptron

float const M=0.1;
float const THETA=0.2;
int const NBRENTREE=4;
int const NBRPOIDS=2;
void modifierPoids(float w[],int d[],int x[],int e[NBRPOIDS][NBRENTREE],int i)
{
    for(int j=0; j<NBRPOIDS; j++)
    {
        w[j]=w[j]+M*((d[i]-x[i])*e[j][i]);

    }
}

int calculerSortie(float w[],int e[NBRPOIDS][NBRENTREE],int i)
{
    float resultat=0;
    int resultatTemporaire;
    for(int j=0; j<NBRPOIDS; j++)
    {
        resultat+=w[j]*e[j][i];
    }
    resultat=resultat-THETA;

    if(resultat>0)
    {
        resultatTemporaire=1;
    }
    else
        resultatTemporaire=0;

    return resultatTemporaire;
}



void perceptron()
{
    float w[NBRPOIDS]= {0.3,-0.1};
    int e[NBRPOIDS][NBRENTREE]= {{0,0,1,1},{0,1,0,1}};
    int d[NBRENTREE]= {0,0,0,1};
    int x[NBRENTREE];
    booleen fini=false;
    int nbrOK=0;
    while(!fini)
    {
        nbrOK=0;
        printf("e1  e2  d      w1          w2     x    w1Final   w2Final   \n");
        for (int i=0; i<NBRENTREE; i++)
        {
            x[i]=calculerSortie(w,e,i);
            printf("%d   %d   %d   %f   %f   %d   ",e[0][i],e[1][i],d[i],w[0],w[1],x[i]);
            if(x[i]!=d[i])
            {
                modifierPoids(w,d,x,e,i);
            }
            else
                nbrOK++;
            printf("%f  %f",w[0],w[1]);
            if(nbrOK==4)
                fini=true;
            printf("\n");
        }
        printf("\n");
        printf("\n");
        printf("\n");
    }
}

///perceptron Multicouche



int const NBRENTREE_MULTIPLE=2;
int const NBR_ACTIVATION=7;

float sigmoide(float x)
{
    return 1/(1+exp(-x));
}


void perceptronMulticouche()
{
    float w[NBR_ACTIVATION][NBR_ACTIVATION+1]= {{0,0,0,0,0,0,0,0},
        {0,0,0,0.5,-1,0,0,0},
        {0,0,0,1.5,-2,0,0,0},
        {0,0,0,0,0,1,-1,0},
        {0,0,0,0,0,3,-4,0},
        {0,0,0,0,0,0,0,1},
        {0,0,0,0,0,0,0,-3}
    };
    float temp=0.0;
    float alpha=0.1;
    float delta[NBR_ACTIVATION+1]= {0,0,0,0,0,0,0,0};
    float in[NBR_ACTIVATION+1]= {0,0,0,0,0,0,0,0};

    float a[NBR_ACTIVATION+1]= {0,0,0,0,0,0,0,0};
    float x[NBRENTREE_MULTIPLE+1]= {0,2,-1};
    printf(" Propagation Avant \n\n");
    do{
        for(int i=0; i<3; i++)
        {
            a[i]=x[i];
        }

        for( int j=3; j<=NBR_ACTIVATION; j++)
        {
            for( int i=0; i<NBR_ACTIVATION; i++)
            {
                in[j]+=w[i][j]*a[i];
            }
            a[j]=sigmoide(in[j]);
        }
        for(int j=1; j<NBR_ACTIVATION+1; j++)
        {
            printf("\ta%d  ====  %f\n",j,a[j]);
        }
        delta[NBR_ACTIVATION]=1-a[NBR_ACTIVATION];
        printf("\n\n");
        printf(" Retropropagation \n\n");
        printf("\tDelta7  ====  %f\n",delta[NBR_ACTIVATION]);
        for(int i=NBR_ACTIVATION-1; i>2; i--)
        {
            for(int j=1; j<NBR_ACTIVATION+1; j++)
            {
                temp+=w[i][j]*delta[j];

            }
            delta[i]=sigmoide(in[i])*(1-sigmoide(in[i]))*temp;
            printf("\tDelta%d  ====  %f\n",i,delta[i]);
            temp=0;
        }
        printf("\n\n");
        for(int i=1; i<NBR_ACTIVATION; i++)
        {
            for(int j=1; j<NBR_ACTIVATION+1; j++)
            {
                if(w[i][j]!=0)
                {
                    w[i][j]=w[i][j]+alpha*a[i]*delta[j];
                }
            }
        }
        for(int i=1; i<NBR_ACTIVATION; i++)
        {
            for(int j=1; j<NBR_ACTIVATION+1; j++)
            {
                if(w[i][j]!=0)
                {
                    printf("\tW%d%d  ====  %f\n",i,j,w[i][j]);
                }
            }
        }
        printf("\n\n");
        if((delta[NBR_ACTIVATION]>0.1))printf("Propagation Avant \n\n");
    }while(delta[NBR_ACTIVATION]>0.1);
}

