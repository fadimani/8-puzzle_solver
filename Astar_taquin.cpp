#include <algorithm>
#include <stdio.h>
#include <vector>
#include <cstring>
#include "graphemat.h"
#include "liregraphe.h"


using namespace std;
NomSom ETAT__BUT = "123894765";
vector<int> ETAT__BUT_vec{1,2,3,8,9,4,7,6,5};
vector<int> G_scores{0};
int developed_nodes = 0;



typedef GrapheMat Graphe;


vector<int> fatherOf(GrapheMat* graphe, NomSom s) {

    int idDad = 0;
    int idSon = rang(graphe, s);

    int nMax = graphe->nMax;

    for (int i = 0; i < graphe->n; ++i) {
        if (graphe->element[i * nMax + idSon] == vrai) {
            idDad = i;
            break;
        }
    }

    vector<char> nums(begin(graphe->nomS[idDad]), end(graphe->nomS[idDad]));
    vector<int> daddy;
    for (int i = 0; i < 9; i++) {
        int three_int = nums[i] - '0';
        daddy.push_back(three_int);
    }
    return daddy;
}


vector<vector<int>> getNeighbours(GrapheMat* graphe,NomSom& state){

    vector<char> nums(begin(state), end(state));
    vector<int> numsINT;
    for (int i = 0; i < 9; i++) {
        int three_int = nums[i] - '0';
        numsINT.push_back(three_int);
    }

    int blank_pos = -1;
    // Find the position of the blank space (the number 0)
    for (int i = 0; i < 9; i++) {
        if (numsINT[i]==9) {
            blank_pos = i;
            break;
        }
    }

    vector<vector<int>> neighbors;
    // Try to move the blank space up
    if (blank_pos >= 3) {
        vector<int> neighbor_state = numsINT;
        swap(neighbor_state[blank_pos], neighbor_state[blank_pos - 3]);
        if (neighbor_state!= fatherOf(graphe,state)) { neighbors.push_back(neighbor_state); }
    }
    // Try to move the blank space down
    if (blank_pos <= 5) {
        vector<int> neighbor_state = numsINT;
        swap(neighbor_state[blank_pos], neighbor_state[blank_pos + 3]);
        if (neighbor_state!=fatherOf(graphe,state)) { neighbors.push_back(neighbor_state); }
    }
    // Try to move the blank space left
    if (blank_pos % 3 > 0) {
        vector<int> neighbor_state = numsINT;
        swap(neighbor_state[blank_pos], neighbor_state[blank_pos - 1]);
        if (neighbor_state!=fatherOf(graphe,state)) { neighbors.push_back(neighbor_state); }
    }
    // Try to move the blank space right
    if (blank_pos % 3 < 2) {
        vector<int> neighbor_state = numsINT;
        swap(neighbor_state[blank_pos], neighbor_state[blank_pos + 1]);
        if (neighbor_state!=fatherOf(graphe,state)) { neighbors.push_back(neighbor_state); }
    }


    return neighbors;
}


void addNeighbours (GrapheMat* graphe, vector<vector<int>> neighbors){

    for (auto & neighbor : neighbors) {
        char arr[10] ;              //// remember that our NomSoms are of 10 not 9, cus we need that last ending character \0
        std::transform(begin(neighbor), end(neighbor), begin(arr), [](int i) { return '0' + i; });

        ajouterUnSommet(graphe,arr);
    }
}
int getHscore(NomSom state,NomSom EndState){
    int H=8;
    for (int i = 0; i <9; ++i) {
        if (state[i]!=9){
            if (state[i]==EndState[i]){
                H--;
            }
        }

    }
    return H;
}
int getHscore(vector<int> state, vector<int> EndState){
    int H=8;
    for (int i = 0; i <9; ++i) {
        if (state[i]!=9) {
            if (state[i] == EndState[i]) {
                H--;
            }
        }
    }
    return H;
}


void linkNeighbours (GrapheMat* graphe, NomSom state, vector<vector<int>> neighbors ){
    int cout = 0;
    int g_score =0;
    for (int i = 0; i < neighbors.size(); ++i) {

        int id_father = rang(graphe,state);


        g_score = G_scores[id_father]+1;
        G_scores.push_back(g_score);

        char arr[10] ;              //// remember that our NomSoms are of 10 not 9, cus we need that last ending character \0
        std::transform(begin(neighbors[i]), end(neighbors[i]), begin(arr), [](int i) { return '0' + i; });

        cout = getHscore(arr,ETAT__BUT) + g_score;
        ajouterUnArc (graphe, state, arr, cout);
    }

}
int getGscore(GrapheMat* graphe,vector<int> node){

    char almostNomSome[10] ;
    std::transform(begin(node), end(node), begin(almostNomSome), [](int i) { return '0' + i; });
    NomSom current_nomSome;
    std::fill_n(current_nomSome, 20, 0);
    ::strcpy(current_nomSome,almostNomSome);

    int idNode = rang(graphe,current_nomSome);
    return G_scores[idNode];
}
int getGscore(GrapheMat* graphe,NomSom node){

    int idNode = rang(graphe, node);
    return G_scores[idNode];
}


void developState(GrapheMat* graphe,NomSom& state ){
    developed_nodes++;
    vector<vector<int>> neighbours = getNeighbours(graphe,state);

    addNeighbours(graphe, neighbours);

    linkNeighbours(graphe,state,neighbours);
}

void drawState(NomSom& state){
    vector<char> nums(begin(state), end(state));
    vector<int> numsINT;
    for (int i = 0; i < 9; i++) {
        int three_int = nums[i] - '0';
        numsINT.push_back(three_int);
    }
    for (int j = 0; j < numsINT.size(); ++j) {
        if ((j+1)%3==0){
            ::printf("%d\n",numsINT[j]);
        }
        else{
            ::printf("%d\t",numsINT[j]);
        }
    }
}
void drawState(vector<int> state){
    for (int j = 0; j < state.size(); ++j) {
        if ((j+1)%3==0){
            ::printf("%d\n",state[j]);
        }
        else{
            ::printf("%d\t",state[j]);
        }
    }
}
void isFather(GrapheMat* graphe , NomSom d, NomSom s){


    int idDad = rang(graphe,d);
    int idSon = rang(graphe,s);

    int nMax = graphe->nMax;

    ::printf("id dad: %d      id son: %d \n",idDad,idSon);

    if (graphe->element [idDad*nMax+idSon] == vrai){
        ::printf("you are the father");
    }

}

bool testSolution(vector<int> state){
    return (state == ETAT__BUT_vec);
}


void reconstructPath(GrapheMat* graphe,vector<int> start, vector<int> but){
    vector<vector<int>> path{but};
    vector<int> current = but;
    while ( current!=start){

        char almostNomSome[10] ;
        std::transform(begin(current), end(current), begin(almostNomSome), [](int i) { return '0' + i; });
        NomSom current_nomSome;
        ::strcpy(current_nomSome,almostNomSome);

        vector<int> father = fatherOf(graphe,current_nomSome);
        path.push_back(father);

        current.clear();
        current = father;
    }
    reverse(path.begin(), path.end());
    for (int i = 0; i < path.size(); ++i) {
        ::printf("--------%d--------\n",i+1);
        drawState(path[i]);
        ::printf("-----------------\n",i+1);

    }
}


bool cmp(const pair<vector<int>,int>& a, const pair<vector<int>,int>& b){
    return (a.second + getHscore(a.first,ETAT__BUT_vec)) < (b.second + getHscore(b.first,ETAT__BUT_vec));
}



bool A_star (NomSom starter,int profondeur){

    //! initialisation
    bool found = false;
    Graphe* graphe;
    graphe = creerGrapheMat (1000, 1);

//    NomSom etatInitial = "813945276";
    NomSom etatInitial;
    strncpy(etatInitial, starter, 20);
//    NomSom etatInitial = "283164795";


    vector<char> nums(begin(etatInitial), end(etatInitial));
    vector<int> etatInitial_vec;
    for (int i = 0; i < 9; i++) {
        int three_int = nums[i] - '0';
        etatInitial_vec.push_back(three_int);
    }



//    vector<int> etatInitial_vec {8,1,3,9,4,5,2,7,6};
//    vector<int> etatInitial_vec {2,8,3,1,6,4,7,9,5};

    ajouterUnSommet(graphe,etatInitial);


    initMarque(graphe);



    vector<pair<vector<int>,int>> frontier{{etatInitial_vec, getGscore(graphe,etatInitial_vec)}};
    vector<vector<int>> frontier_node{etatInitial_vec};

    //! beginning of loop
    while(true){

        //? exit condition: frontier is empty
        if (frontier.empty()) {
            ::printf("\nnot found. profondeur:%d       nodes searched:%d\n",profondeur,developed_nodes);
            found = false;
            break;
        }

        //? sorting the frontier by F-score
        std::sort(frontier.begin(), frontier.end(),cmp);

        //? popping the next element to be developped
        vector<int> node = frontier[0].first;

        //? second exit condition: if the popped node is the solution
        if (testSolution(node)) {
            ::printf("\nsolution:\n");
            reconstructPath(graphe,etatInitial_vec,node);
            ::printf("nodes we had to develop: %d\n",developed_nodes);
//            ecrireGraphe(graphe);
            found = true;
            break;
        }


        //? mark node as explored
        char almostNomSome[10] ;
        std::transform(begin(frontier[0].first), end(frontier[0].first), begin(almostNomSome), [](int i) { return '0' + i; });
        NomSom real_nomSome;
        std::fill_n(real_nomSome, 20, 0);
        ::strcpy(real_nomSome,almostNomSome);

        int idNode = rang(graphe,real_nomSome);

        graphe->marque[idNode]=1;



        //? erasing the popped element from the frontier
        frontier.erase(frontier.begin());


        //! limiting the depth of nodes we'll develop
        if (getGscore(graphe,real_nomSome)<=profondeur){

            //! developping the selected node = {generating neighbours, adding them to the graph and calculating their values}
            developState(graphe,real_nomSome);

            vector<vector<int>> children= getNeighbours(graphe,real_nomSome);


            //? looping through all neighbours
            for (int x = 0; x < children.size(); ++x) {

                //? if node not in the frontier and not explored, add it to the frontier
                if (graphe->marque[idNode+x+1]== 0 && !(std::find(frontier.begin(), frontier.end(), make_pair(children[x], getGscore(graphe,children[x]))) != frontier.end())  ){
                    frontier.push_back(make_pair(children[x], getGscore(graphe,children[x])));
                    frontier_node.push_back(children[x]);
                }
                    //? if node already in the frontier
                else if (std::find(frontier_node.begin(), frontier_node.end(), children[x]) != frontier_node.end() ){

                    vector<vector<int>>::iterator it;
                    it = std::find(frontier_node.begin(), frontier_node.end(), children[x]);
                    //? if node already in the frontier and it's F-score is lower, replace its old value with this new lower value
                    if (getGscore(graphe,children[x])<frontier[it - frontier_node.begin()].second){
                        frontier[it - frontier_node.begin()]= make_pair(children[x],getGscore(graphe,children[x]));
                    }
                }

            }

        }



    }
    G_scores.clear();
    G_scores.push_back(0);
    developed_nodes = 0;
    detruireGraphe(graphe);
    return found;
}


void IDA_star (NomSom starter,int profondeur){
    for (int p = 0; p < 100; ++p) {
        if (A_star(starter,p)) break;
    }
}


