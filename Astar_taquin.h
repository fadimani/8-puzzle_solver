#include <algorithm>
#include <stdio.h>
#include <vector>
#include <cstring>
#include "liregraphe.h"




vector<int> fatherOf(GrapheMat* graphe, NomSom s);
vector<vector<int>> getNeighbours(GrapheMat* graphe,NomSom& state);
void addNeighbours (GrapheMat* graphe, vector<vector<int>> neighbors);
int getHscore(NomSom state,NomSom EndState);
int getHscore(vector<int> state, vector<int> EndState);
void linkNeighbours (GrapheMat* graphe, NomSom state, vector<vector<int>> neighbors );
int getGscore(GrapheMat* graphe,vector<int> node);
int getGscore(GrapheMat* graphe,NomSom node);
void developState(GrapheMat* graphe,NomSom& state );
void drawState(NomSom& state);
void drawState(vector<int> state);
void isFather(GrapheMat* graphe , NomSom d, NomSom s);
bool testSolution(vector<int> state);
void reconstructPath(GrapheMat* graphe,vector<int> start, vector<int> but);


bool A_star (NomSom starter,int profondeur);

void IDA_star (NomSom starter,int profondeur);