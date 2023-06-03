

#include <limits.h>

typedef int  booleen;
#define faux 0
#define vrai 1
typedef char NomSom[20];  // nom d'un sommet
#define INFINI INT_MAX



typedef int* Matrice;

typedef struct {
  int      n;       // nombre de sommets
  int      nMax;    // nombre max de sommets
  booleen  value;   // graphe valué ou non
  NomSom*  nomS;    // noms des sommets
  Matrice  element; // existence d'un arc (i, j)
  Matrice  valeur;  // cout de l'arc (i, j)
  booleen* marque;  // sommet marqué (visité) ou non
} GrapheMat;

GrapheMat* creerGrapheMat     (int nMax, int value);
void detruireGraphe     (GrapheMat* graphe);
int  rang               (GrapheMat* graphe, NomSom nom);
void initMarque         (GrapheMat* graphe);
void ajouterUnSommet    (GrapheMat* graphe, NomSom nom);
void ajouterUnArc       (GrapheMat* graphe, NomSom somD, NomSom somA, int cout);
void ecrireGraphe       (GrapheMat* graphe);
void parcoursProfond    (GrapheMat* graphe);
void parcoursLargeur    (GrapheMat* graphe);
void floyd              (GrapheMat* graphe);



void parcoursLargeurEx  (GrapheMat* graphe);
void parcoursCoutUniforme       (GrapheMat* graphe );
void parcoursProfondAbord       (GrapheMat* graphe);
void parcoursProfendeurLimite   (GrapheMat* graphe);
void parcoursProfondeurIteratif (GrapheMat* graphe);

void AEtoile                    (GrapheMat* graphe );
void IDAEtoile                  (GrapheMat* graphe );
void parcoursPlusProcheVoisin   (GrapheMat* graphe);
void parcoursEscalade           (GrapheMat* graphe);

void parcoursRecuitSimule       (GrapheMat* graphe);

void perceptron();
void perceptronMulticouche();





