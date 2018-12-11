// Copyright Yixue Zhang jedzhang@bu.edu
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <chrono>

// defalut parameters
int x_start = 0;
int y_start = 0;
int x_end = 0;
int y_end = 0;
int matrix_size = 50;


using std::vector;
using std::cout;
using std::endl;
using std::string;
using std::rand;


typedef struct Node Data_Node;

struct Node {
    int row;
    int col;
    int idx;
    struct Node *up;
    struct Node *down;
    struct Node *left;
    struct Node *right;
    bool visited = false;
    struct Node* prev_for_BFS;
    int dist_for_BFS;
    bool BFS_inopen = false;
};


void create_node(Node* a, int i, int j){
    // Node init
    a->row = i;
    a->col = j;
    a->idx = i*matrix_size + j;
}

void creat_row(vector<Node*>* a, int i) {
    // row init
    for (int j=0;j<matrix_size;j++) {
        Node * temp_node = new Node;
        create_node(temp_node, i ,j);
        a->push_back(temp_node);
    }

}

vector<vector<Node*>> build_map() {
    vector<vector<Node*>> map;
    for (int i=0;i<matrix_size;i++) {
        vector<Node*> *temp_row = new vector<Node*>;
        creat_row(temp_row, i);
        map.push_back(*temp_row);
    }
    return map;
}


void find_neightbors(vector<vector<Node*>>* map, vector<vector<int>>* openlist, vector<int> current_element) {
    vector<int> result;
    if (current_element[0] != 0) {
        if ((*map)[current_element[0] - 1][current_element[1]] ->visited == false){
            result.push_back(current_element[0] - 1);
            result.push_back(current_element[1]);
            result.push_back(current_element[0]);
            result.push_back(current_element[1]);
            result.push_back(8);
            openlist->push_back(result);
            result.clear();
        }
    }
    if (current_element[0] != matrix_size - 1) {
        if ((*map)[current_element[0] + 1][current_element[1]]->visited == false){
            result.push_back(current_element[0] + 1);
            result.push_back(current_element[1]);
            result.push_back(current_element[0]);
            result.push_back(current_element[1]);
            result.push_back(2);
            openlist->push_back(result);
            result.clear();
        }
    }
    if (current_element[1] != 0) {
        if ((*map)[current_element[0]][current_element[1] - 1]->visited == false){
            result.push_back(current_element[0]);
            result.push_back(current_element[1] - 1);
            result.push_back(current_element[0]);
            result.push_back(current_element[1]);
            result.push_back(4);
            openlist->push_back(result);
            result.clear();
        }
    }
    if (current_element[1] != matrix_size - 1) {
        if ((*map)[current_element[0]][current_element[1] + 1]->visited == false){
            result.push_back(current_element[0]);
            result.push_back(current_element[1] + 1);
            result.push_back(current_element[0]);
            result.push_back(current_element[1]);
            result.push_back(6);
            openlist->push_back(result);
            result.clear();
        }
    }
}

void clear_wall(vector<vector<Node*>>* map, vector<int> current_open) {
    if ((*map)[current_open[0]][current_open[1]]->visited) {
    }
    else {
        switch(current_open[4]) {
            case 8: (*map)[current_open[0]][current_open[1]]->down= (*map)[current_open[2]][current_open[3]];
                    (*map)[current_open[2]][current_open[3]]->up = (*map)[current_open[0]][current_open[1]];
                break;
            case 2: (*map)[current_open[0]][current_open[1]]->up = (*map)[current_open[2]][current_open[3]];
                    (*map)[current_open[2]][current_open[3]]->down = (*map)[current_open[0]][current_open[1]];
                break;
            case 4: (*map)[current_open[0]][current_open[1]]->right = (*map)[current_open[2]][current_open[3]];
                    (*map)[current_open[2]][current_open[3]]->left = (*map)[current_open[0]][current_open[1]];
                break;
            case 6: (*map)[current_open[0]][current_open[1]]->left = (*map)[current_open[2]][current_open[3]];
                    (*map)[current_open[2]][current_open[3]]->right = (*map)[current_open[0]][current_open[1]];
                break;
        }
    }
}

void build_maze(vector<vector<Node*>>* map) {
    vector<vector<int>> openlist;
    vector<int> current_element;
    vector<int> prev_element;
    vector<int> current_open;
    srand(time(NULL));
    (*map)[x_start][y_start]->visited = true;
    int num_open;
    int rand_index;
    current_element.push_back(x_start);
    current_element.push_back(y_start);
    find_neightbors(map, &openlist, current_element);
    while (!openlist.empty()) {
        num_open = openlist.size();

        rand_index = rand()%(num_open);
        cout<<num_open<<" "<<rand_index<<endl;
        current_open = openlist[rand_index];
        current_element[0] = current_open[0];
        current_element[1] = current_open[1];
        openlist.erase(openlist.begin()+ rand_index,openlist.begin()+rand_index+1);
        find_neightbors(map, &openlist, current_element);
        clear_wall(map, current_open);
        (*map)[current_element[0]][current_element[1]]->visited = true;
    }
}

void BFS_find_neighbor(vector<vector<Node*>>* map, vector<Node*>* openlist, vector<int> current_element) {
        vector<int> result;
        vector<int> prev_coord;
        prev_coord.push_back(current_element[0]);
        prev_coord.push_back(current_element[1]);
    if (current_element[0] != 0) {
        if ((*map)[current_element[0]][current_element[1]]->up != NULL && (*map)[current_element[0]-1][current_element[1]]->BFS_inopen != true){
            result.push_back(current_element[0] - 1);
            result.push_back(current_element[1]);
            (*map)[result[0]][result[1]] ->BFS_inopen = true;
            (*map)[result[0]][result[1]] ->dist_for_BFS = (*map)[current_element[0]][current_element[1]] ->dist_for_BFS + 1;
            (*map)[result[0]][result[1]] ->prev_for_BFS = (*map)[prev_coord[0]][prev_coord[1]];
            openlist->push_back((*map)[result[0]][result[1]]);
            result.clear();
        }
    }
    if (current_element[0] != matrix_size - 1) {
        if ((*map)[current_element[0]][current_element[1]]->down != NULL && (*map)[current_element[0]+1][current_element[1]]->BFS_inopen != true){
            result.push_back(current_element[0] + 1);
            result.push_back(current_element[1]);
            (*map)[result[0]][result[1]] ->BFS_inopen = true;
            (*map)[result[0]][result[1]] ->dist_for_BFS = (*map)[current_element[0]][current_element[1]] ->dist_for_BFS + 1;
            (*map)[result[0]][result[1]] ->prev_for_BFS = (*map)[prev_coord[0]][prev_coord[1]];
            openlist->push_back((*map)[result[0]][result[1]]);
            result.clear();
        }
    }
    if (current_element[1] != 0) {
        if ((*map)[current_element[0]][current_element[1]]->left != NULL && (*map)[current_element[0]][current_element[1] - 1]->BFS_inopen != true){
            result.push_back(current_element[0]);
            result.push_back(current_element[1] - 1);
            (*map)[result[0]][result[1]] ->BFS_inopen = true;
            (*map)[result[0]][result[1]] ->dist_for_BFS = (*map)[current_element[0]][current_element[1]] ->dist_for_BFS + 1;
            (*map)[result[0]][result[1]] ->prev_for_BFS = (*map)[prev_coord[0]][prev_coord[1]];
            openlist->push_back((*map)[result[0]][result[1]]);
            result.clear();
        }
    }
    if (current_element[1] != matrix_size - 1) {
        if ((*map)[current_element[0]][current_element[1]]->right != NULL && (*map)[current_element[0]][current_element[1] + 1]->BFS_inopen != true){
            result.push_back(current_element[0]);
            result.push_back(current_element[1] + 1);
            (*map)[result[0]][result[1]] ->BFS_inopen = true;
            (*map)[result[0]][result[1]] ->dist_for_BFS = (*map)[current_element[0]][current_element[1]] ->dist_for_BFS + 1;
            (*map)[result[0]][result[1]] ->prev_for_BFS = (*map)[prev_coord[0]][prev_coord[1]];
            openlist->push_back((*map)[result[0]][result[1]]);
            result.clear();
        }
    }
}

bool BFS(vector<vector<Node*>>* map, vector<int> start, vector<int> end) {
    vector<int> result;
    vector<Node*> openlist;
    vector<int> current;
    Node* current_node;
    current.push_back(start[0]);
    current.push_back(start[1]);
    (*map)[current[0]][current[1]]->dist_for_BFS = 0;
    (*map)[current[0]][current[1]]->BFS_inopen = true;
    BFS_find_neighbor(map, &openlist, current);
    while(1) {
        current_node = openlist[0];
        if (!openlist.empty()){
            openlist.erase(openlist.begin(), openlist.begin()+1);
        }
        else{
            return false;
        }
        current.clear();
        current.push_back(current_node->row);
        current.push_back(current_node->col);
        if (current[0] == end[0]) {
            if (current[1] == end[1]) {
                return true;
            }
        }
        BFS_find_neighbor(map, &openlist, current);
    }
}


int main() {
    // program start
    // declare variables
    auto start_t = std::chrono::system_clock::now();
    vector<vector<Node*>> map;
    vector<vector<Node*>> map_for_BFS;
    vector<int> start;
    vector<int> end;
    vector<vector<int>> BFS_result;
    vector<int> temp_coord;
    Node* temp;
    int counter;
    int BFS_step;
    // start coord
    start.push_back(x_start);
    start.push_back(y_start);
    // matrix size
    matrix_size = 5;
    // end coord
    x_end = matrix_size - 1;
    y_end = 0;
    end.push_back(x_end);
    end.push_back(y_end);
    // build map
    map = build_map();
    build_maze(&map);
    auto end_t = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_t-start_t;
    std::cout << "Maze generated, elapsed time: " << elapsed_seconds.count()*1000 << "ms\n\n";
    // BFS DFS Balmen-Ford Wavefront A* Dijkstra Greedy are to be tested


    // BFS test
    cout<<"BFS start\n"<<endl;
    map_for_BFS = map;
    start_t = std::chrono::system_clock::now();
    if (BFS(&map_for_BFS, start, end)) {
        temp = map_for_BFS[end[0]][end[1]];
        BFS_step = temp->dist_for_BFS;
        while (1){
            if (temp->prev_for_BFS == NULL) {
                break;
            }
            temp_coord.push_back(temp->col);
            temp_coord.push_back(temp->row);
            BFS_result.push_back(temp_coord);
            temp_coord.clear();
            temp = temp->prev_for_BFS;
        }
        counter = 0;
        cout<<"BFS found path with distance of "<<BFS_step<<endl;
        cout<<"The pass is showed as follows:"<<endl;
        cout<<"Step:" <<counter<<"  current coord is:["<<start[0]<<","<<start[1]<<"]"<<endl;
        for (int i=BFS_result.size()-1;i>-1;i--) {
            counter ++;
            cout<<"Step:" <<counter<<"  current coord is:["<<BFS_result[i][0]<<","<<BFS_result[i][1]<<"]"<<endl;
        }
    }
    else {
        cout<<"No path found!";
    }
    end_t = std::chrono::system_clock::now();
    elapsed_seconds = end_t-start_t;
    std::cout << "Breadth First Searched completed, elapsed time: " << elapsed_seconds.count()*1000 << "ms\n";

}