// Copyright Yixue Zhang jedzhang@bu.edu
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <set>
#include <algorithm>
#include <iomanip>
#include <cmath>

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
using std::find;
using std::cin;


typedef struct Node Data_Node;

struct result_node {
    string Name;
    int steps;
    double time;
    long visited;
};

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
    bool DFS_visited = false;
    int DFS_depth = std::numeric_limits<int>::max() -1;
    int BF_dist = std::numeric_limits<int>::max() - 1;
    int WF_height = std::numeric_limits<int>::max() - 1;
    int DK_weight = std::numeric_limits<int>::max() - 1;
    double A_G_cost = std::numeric_limits<double>::max() - 1;;
    double A_H_cost = std::numeric_limits<double>::max() - 1;;
    double A_F_cost = std::numeric_limits<double>::max() - 1;;
    struct Node* A_parent;
};

void creat_row(vector<Node*>* a, int i);
void create_node(Node* a, int i, int j);
vector<vector<Node*>> build_map();
vector<int> find_neightbors(vector<vector<Node*>>* map, vector<vector<int>>* openlist, vector<int> current_element);
void clear_wall(vector<vector<Node*>>* map, vector<int> current_open);
void build_maze(vector<vector<Node*>>* map);
void BFS_find_neighbor(vector<vector<Node*>>* map, vector<Node*>* openlist, vector<int> current_element);
bool BFS(vector<vector<Node*>>* map, vector<int> start, vector<int> end);
Node* DFS_find_neighbor(vector<vector<Node*>>* map, vector<Node*>* openlist, Node* current_node);
bool DFS(vector<vector<Node*>>* map, vector<int> start, vector<int> end);
void bellman_ford_pre(vector<vector<Node*>>* map, std::set<vector<int>>* edges);
void bellman_ford_relax(vector<vector<Node*>>* map, int from, int to);
void bellman_ford(vector<vector<Node*>>* map, std::set<vector<int>>* edges, vector<int> start);
int wave_front_find_min_neighbor(Node* temp);
void wave_front(vector<vector<Node*>>* map, vector<int> start);
bool check_element_in_vector(Node* element, vector<Node*> target_vector);
Node* DK_find_min_in_openlist (vector<Node*>* openlist);
void DK_find_neighbor (Node* current_node, vector<Node*>* openlist, vector<Node*>* closelist);
void dijkstra(vector<vector<Node*>>* map, vector<int> start, vector<int> end);
void A_star (vector<vector<Node*>>* map, vector<int> start, vector<int> end);
vector<Node*> A_find_neighbor (Node* current_node);
double A_compute_heuristic_distance(Node* current_node, Node* end_node);



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


vector<int> find_neightbors(vector<vector<Node*>>* map, vector<vector<int>>* openlist, vector<int> current_element) {
    vector<int> result;
    vector<vector<int>> possible_neighbor;
    vector<int> next;
    int num_open, rand_index;
    if (current_element[0] != 0) {
        if ((*map)[current_element[0] - 1][current_element[1]] ->visited == false){
            result.push_back(current_element[0] - 1);
            result.push_back(current_element[1]);
            result.push_back(current_element[0]);
            result.push_back(current_element[1]);
            result.push_back(8);
            possible_neighbor.push_back(result);
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
            possible_neighbor.push_back(result);
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
            possible_neighbor.push_back(result);
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
            possible_neighbor.push_back(result);
            result.clear();
        }
    }
    num_open = possible_neighbor.size();
    if (num_open == 0) {
        (*openlist).erase(openlist->begin(),openlist->begin()+1);
        return (*openlist)[0];
    }
    rand_index = ((double)rand() / double(RAND_MAX))*(num_open);
    next.push_back(possible_neighbor[rand_index][0]);
    next.push_back(possible_neighbor[rand_index][1]);
    next.push_back(current_element[0]);
    next.push_back(current_element[1]);
    next.push_back(possible_neighbor[rand_index][4]);
    possible_neighbor.erase(possible_neighbor.begin()+rand_index, possible_neighbor.begin()+rand_index+1);
    if (!possible_neighbor.empty()) {
        for (int i=0;i<possible_neighbor.size();i++) {
            result.clear();
            result.push_back(possible_neighbor[i][0]);
            result.push_back(possible_neighbor[i][1]);
            result.push_back(current_element[0]);
            result.push_back(current_element[1]);
            result.push_back(possible_neighbor[i][4]);
            openlist->insert(openlist->begin(), result);
        }
    }
    return next;
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
    vector<int> next;
    (*map)[x_start][y_start]->visited = true;
    current_element.push_back(x_start);
    current_element.push_back(y_start);
    next = find_neightbors(map, &openlist, current_element);
    clear_wall(map, next);
    while (1) {
        current_element[0] = next[0];
        current_element[1] = next[1];
        next = find_neightbors(map, &openlist, current_element);
        if (openlist.empty()) {
            break;
        }
        clear_wall(map, next);
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

bool BFS(vector<vector<Node*>>* map, vector<int> start, vector<int> end, long * visited) {
    vector<int> result;
    vector<Node*> openlist;
    vector<int> current;
    Node* current_node;
    current.push_back(start[0]);
    current.push_back(start[1]);
    (*map)[current[0]][current[1]]->dist_for_BFS = 0;
    (*map)[current[0]][current[1]]->BFS_inopen = true;
    (*visited) ++;
    BFS_find_neighbor(map, &openlist, current);
    while(1) {
        (*visited) ++;
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

Node* DFS_find_neighbor(vector<vector<Node*>>* map, vector<Node*>* openlist, Node* current_node) {
    vector<Node*> neighbor;
    if (current_node->left != nullptr && !current_node->left->DFS_visited) {
        if (find(openlist->begin(),openlist->end(), current_node->left) == openlist->end()) {
            neighbor.push_back(current_node->left);
            current_node->left->DFS_visited = true;
        }
    }
    if (current_node->right != nullptr && !current_node->right->DFS_visited) {
        if (find(openlist->begin(),openlist->end(), current_node->right) == openlist->end()){
            neighbor.push_back(current_node->right);
            current_node->right->DFS_visited = true;
        }
    }
    if (current_node->up != nullptr && !current_node->up->DFS_visited) {
        if (find(openlist->begin(),openlist->end(), current_node->up) == openlist->end()) {
            neighbor.push_back(current_node->up);
            current_node->up->DFS_visited = true;
        }
    }
    if (current_node->down != nullptr && !current_node->down->DFS_visited) {
        if (find(openlist->begin(),openlist->end(), current_node->down) == openlist->end()) {
            neighbor.push_back(current_node->down);
            current_node->down->DFS_visited = true;
        }
    }
    if (neighbor.empty()) {
        if (openlist->empty()) {
            current_node = nullptr;
            return current_node;
        }
        else{
            current_node->DFS_visited = true;
            current_node = (*openlist)[0];
            openlist->erase(openlist->begin(),openlist->begin()+1);
            return current_node;
        }
    }
    else {
        for (int i=0;i<neighbor.size();i++) {
            if (neighbor[i]->DFS_depth > current_node->DFS_depth+1) {
            neighbor[i]->prev_for_BFS = current_node;
            neighbor[i]->DFS_depth = current_node->DFS_depth + 1;
            openlist->insert(openlist->begin(), neighbor[i]);
            }
        }
        current_node = (*openlist)[0];
    }
    return current_node;
}

bool DFS(vector<vector<Node*>>* map, vector<int> start, vector<int> end, long* visited) {
    Node* current_node;
    vector<Node*> openlist;
    bool flag = false;
    current_node = (*map)[start[0]][start[1]];
    current_node->DFS_depth = 0;
    openlist.push_back(current_node);
    (*visited) ++;
    current_node = DFS_find_neighbor(map, &openlist, current_node);
    while (1) {
        (*visited) ++;
        current_node = DFS_find_neighbor(map, &openlist, current_node);
        if (current_node == nullptr) {
            return flag;
        }
        else {
            if (current_node->col == end[0] && current_node->row == end[1]) {
                flag = true;
            }
        }
    }
}

void bellman_ford_pre(vector<vector<Node*>>* map, std::set<vector<int>>* edges) {
    int temp1,temp2,temp3;
    vector<int> edge_idx;
    for (int i=0;i<matrix_size;i++) {
        for (int j=0;j<matrix_size;j++) {
            if ((*map)[i][j]->up != NULL) {
                temp1 = (*map)[i][j]->idx;
                temp2 = (*map)[i][j]->up->idx;
                if (temp1 < temp2) {
                    temp3 = temp2;
                    temp2 = temp1;
                    temp1 = temp3;
                }
                edge_idx.clear();
                edge_idx.push_back(temp2);
                edge_idx.push_back(temp1);
                (*edges).insert(edge_idx);
            }
            if ((*map)[i][j]->down != NULL) {
                temp1 = (*map)[i][j]->idx;
                temp2 = (*map)[i][j]->down->idx;
                if (temp1 < temp2) {
                    temp3 = temp2;
                    temp2 = temp1;
                    temp1 = temp3;
                }
                edge_idx.clear();
                edge_idx.push_back(temp2);
                edge_idx.push_back(temp1);
                (*edges).insert(edge_idx);
            }
            if ((*map)[i][j]->left != NULL) {
                temp1 = (*map)[i][j]->idx;
                temp2 = (*map)[i][j]->left->idx;
                if (temp1 < temp2) {
                    temp3 = temp2;
                    temp2 = temp1;
                    temp1 = temp3;
                }
                edge_idx.clear();
                edge_idx.push_back(temp2);
                edge_idx.push_back(temp1);
                (*edges).insert(edge_idx);
            }
            if ((*map)[i][j]->right != NULL) {
                temp1 = (*map)[i][j]->idx;
                temp2 = (*map)[i][j]->right->idx;
                if (temp1 < temp2) {
                    temp3 = temp2;
                    temp2 = temp1;
                    temp1 = temp3;
                }
                edge_idx.clear();
                edge_idx.push_back(temp2);
                edge_idx.push_back(temp1);
                (*edges).insert(edge_idx);
            }

        }
    }
}

void bellman_ford_relax(vector<vector<Node*>>* map, int from, int to) {
    int from_x, from_y, to_x, to_y;
    from_x = from / matrix_size;
    from_y = from % matrix_size;
    to_x = to / matrix_size;
    to_y = to % matrix_size;
    if (((*map)[from_x][from_y]->BF_dist) > ((*map)[to_x][to_y]->BF_dist + 1)) {
        (*map)[from_x][from_y]->BF_dist = (*map)[to_x][to_y]->BF_dist + 1;
    }
}

void bellman_ford(vector<vector<Node*>>* map, std::set<vector<int>>* edges, vector<int> start, long * visited) {
    int t1,t2;
    (*map)[start[0]][start[1]]->BF_dist = 0;
    (*visited)++;
    for (int i=0;i<(matrix_size)*(matrix_size)-1;i++) {
        for (auto edge : *edges) {
            (*visited)++;
            t1 = edge[0];
            t2 = edge[1];
            bellman_ford_relax(map, t1, t2);
            bellman_ford_relax(map, t2, t1);
        }
    }
}

int wave_front_find_min_neighbor(Node* temp) {
    int result;
    vector<int> possible_neighbor;
    if (temp->up != NULL) {
        possible_neighbor.push_back(temp->up->WF_height);
    }
    if (temp->down != NULL) {
        possible_neighbor.push_back(temp->down->WF_height);
    }
    if (temp->left != NULL) {
        possible_neighbor.push_back(temp->left->WF_height);
    }
    if (temp->right != NULL) {
        possible_neighbor.push_back(temp->right->WF_height);
    }
    result = *std::min_element(possible_neighbor.begin(),possible_neighbor.end());
    return result + 1;
}

void wave_front(vector<vector<Node*>>* map, vector<int> start, long * visited) {
    int counter;
    Node* temp;
    int new_height;
    (*visited)++;
    (*map)[start[0]][start[1]]->WF_height = 0;
    while (1) {
        counter = 0;
        for (int i=0;i<matrix_size;i++) {
            for (int j=0;j<matrix_size;j++) {
                (*visited)++;
                temp = (*map)[i][j];
                if (temp->WF_height == std::numeric_limits<int>::max() - 1) {
                    counter ++;
                    new_height = wave_front_find_min_neighbor(temp);
                    if (new_height < temp->WF_height) {
                        temp->WF_height = new_height;
                    }
                }
            }
        }
        if (counter == 0) {
            break;
        }
    }
}

bool check_element_in_vector(Node* element, vector<Node*> target_vector) {
    if (find(target_vector.begin(), target_vector.end(), element) == target_vector.end()) {
        return false;
    }
    else {
        return true;
    }
}

Node* DK_find_min_in_openlist (vector<Node*>* openlist) {
    int min_index;
    int min_value = std::numeric_limits<int>::max() - 1;
    Node* result;
    for (int i=0;i<(*openlist).size();i++) {
        if ((*openlist)[i]->DK_weight < min_value) {
            min_index = i;
            min_value = (*openlist)[i]->DK_weight;
        }
    }
    result = (*openlist)[min_index];
    (*openlist).erase(openlist->begin()+min_index, openlist->begin() + min_index + 1);
    return result;
}

void DK_find_neighbor (Node* current_node, vector<Node*>* openlist, vector<Node*>* closelist) {
    vector<Node*> possible_neighbor;
    vector<int> removelist;
    (*closelist).push_back(current_node);
    if (current_node->up != NULL) {
        possible_neighbor.push_back(current_node->up);
    }
    if (current_node->down != NULL) {
        possible_neighbor.push_back(current_node->down);
    }
    if (current_node->left != NULL) {
        possible_neighbor.push_back(current_node->left);
    }
    if (current_node->right != NULL) {
        possible_neighbor.push_back(current_node->right);
    }
    for (int i=0;i<possible_neighbor.size();i++) {
        if (check_element_in_vector(possible_neighbor[i], *closelist)) {
            removelist.push_back(i);
            continue;
        }
        if (check_element_in_vector(possible_neighbor[i], *openlist)) {
            removelist.push_back(i);
        }
    }
    if (!removelist.empty()) {
        std::reverse(removelist.begin(), removelist.end());
        for (int i=0;i<removelist.size();i++) {
            possible_neighbor.erase(possible_neighbor.begin()+removelist[i],possible_neighbor.begin()+removelist[i]+1);
        }
    }
    if (!possible_neighbor.empty()) {
        for (auto a : possible_neighbor) {
            a->DK_weight = current_node->DK_weight +1;
            openlist->push_back(a);
        }
    }
}

void dijkstra(vector<vector<Node*>>* map, vector<int> start, vector<int> end, long* visited) {
    vector<Node*> possible_neighbor;
    vector<Node*> openlist;
    vector<Node*> closelist;
    Node* current_node;
    (*map)[start[0]][start[1]]->DK_weight = 0;
    current_node = (*map)[start[0]][start[1]];
    (*visited)++;
    while (1) {
        (*visited)++;
        DK_find_neighbor(current_node, &openlist, &closelist);
        if (openlist.empty()) {
            break;
        }
        else {
            current_node = DK_find_min_in_openlist(&openlist);
            if (current_node == (*map)[end[0]][end[1]]) {
                break;
            }
        }
    }
}

double A_compute_heuristic_distance(Node* current_node, Node* end_node) {
    double result;
    result = pow((pow((current_node->row - end_node->row),2) + (pow((current_node->col - end_node->col),2))),0.5);
    return result;
}


Node* A_find_next (vector<Node*>* openlist) {
    vector<double> F_value;
    Node* result;
    int min_index;
    for (auto i : *openlist) {
        F_value.push_back(i->A_F_cost);
    }
    min_index = std::distance(F_value.begin(), find(F_value.begin(), F_value.end(), *std::min_element(F_value.begin(),F_value.end())));
    result = (*openlist)[min_index];
    (*openlist).erase((*openlist).begin() + min_index, (*openlist).begin() + min_index + 1);
    return result;
}

vector<Node*> A_find_neighbor (Node* current_node) {
    vector<Node*> result;
    if ((*current_node).up != NULL) {
        result.push_back((*current_node).up);
    }
    if ((*current_node).down != NULL) {
        result.push_back((*current_node).down);
    }
    if ((*current_node).left != NULL) {
        result.push_back((*current_node).left);
    }
    if ((*current_node).right != NULL) {
        result.push_back((*current_node).right);
    }
    return result;
}

void A_star (vector<vector<Node*>>* map, vector<int> start, vector<int> end, long* visited) {
    Node* current_node;
    Node* start_node;
    Node* end_node;
    vector<Node*> openlist;
    vector<Node*> closelist;
    vector<Node*> neighbors;
    vector<Node*> real_neighbors;
    double tentative_g_score;
    bool tentative_better;
    end_node = (*map)[end[0]][end[1]];
    current_node = (*map)[start[0]][start[1]];
    current_node->A_G_cost = 0;
    current_node->A_H_cost = A_compute_heuristic_distance(current_node, end_node);
    current_node->A_F_cost = current_node->A_H_cost;
    openlist.push_back(current_node);
    (*visited)++;
    while (!openlist.empty()) {
        (*visited)++;
        neighbors.clear();
        real_neighbors.clear();
        current_node = A_find_next(&openlist);
        if (current_node == end_node) {
            break;
        }
        closelist.push_back(current_node);
        neighbors = A_find_neighbor(current_node);
        for (int i=0;i<neighbors.size();i++) {
            if (find(closelist.begin(), closelist.end(), neighbors[i]) != closelist.end()) {
                continue;
            }
            tentative_g_score = current_node->A_G_cost + A_compute_heuristic_distance(neighbors[i], current_node);
            if (find(openlist.begin(), openlist.end(), neighbors[i]) == closelist.end()) {
                tentative_better = true;
            }
            else if (tentative_g_score < neighbors[i]->A_G_cost) {
                tentative_better = true;
            }
            else {
                tentative_better = false;
            }
            if (tentative_better) {
                neighbors[i]->A_parent = current_node;
                neighbors[i]->A_G_cost = tentative_g_score;
                neighbors[i]->A_H_cost = A_compute_heuristic_distance(neighbors[i], end_node);
                neighbors[i]->A_F_cost = neighbors[i]->A_H_cost + neighbors[i]->A_G_cost;
                openlist.push_back(neighbors[i]);
            }

        }

    }


}


int main() {
    // program start
    // declare variables
    srand(time(NULL));
    auto start_t = std::chrono::system_clock::now();
    vector<vector<Node*>> map;
    vector<vector<Node*>> map_for_BFS;
    vector<vector<Node*>> map_for_DFS;
    vector<vector<Node*>> map_for_BF;
    vector<vector<Node*>> map_for_WF;
    vector<vector<Node*>> map_for_DK;
    vector<vector<Node*>> map_for_A;
    vector<vector<int>> BFS_result;
    vector<vector<int>> DFS_result;
    vector<vector<int>> BF_result;
    vector<vector<int>> WF_result;
    vector<vector<int>> DK_result;
    vector<vector<int>> A_result;
    vector<result_node*> res_collection;
    std::set<vector<int>> edges;
    result_node BFS_res;
    result_node DFS_res;
    result_node BF_res;
    result_node WF_res;
    result_node DK_res;
    result_node A_res;
    vector<int> possible_neighbor;
    vector<int> start;
    vector<int> end;
    vector<int> temp_coord;
    vector<Node*> direction;
    long BFS_visited = 0;
    long DFS_visited = 0;
    long BF_visited = 0;
    long WF_visited = 0;
    long DK_visited = 0;
    long A_visitied = 0;
    Node* temp;
    int min_number_BF;
    int counter;
    int BFS_step;
    int DFS_step;
    // start coord
    start.push_back(x_start);
    start.push_back(y_start);
    // matrix size
    cout<<"Input an integer for maze size:" <<endl;
    cin>>matrix_size;
    while (matrix_size < 3) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout<<"Invalid input, try again"<<endl;
        cin>>matrix_size;

    }
//    matrix_size = 50;
    // end coord
    x_end = matrix_size - 1;
    y_end = matrix_size - 1;
    end.push_back(x_end);
    end.push_back(y_end);
    // build map
    map = build_map();
    build_maze(&map);
    auto end_t = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_t-start_t;
    std::cout << "Maze generated, elapsed time: " << elapsed_seconds.count()*1000 << "ms\n\n";
    map_for_BF = map;
    map_for_BFS = map;
    map_for_DFS = map;
    map_for_WF = map;
    map_for_DK = map;
    map_for_A = map;
    // BFS DFS Balmen-Ford Wavefront A* Dijkstra Greedy are to be tested

    cout<<"...................................................................\n"<<endl;

    // BFS test
    cout<<"BFS start...\n"<<endl;
    start_t = std::chrono::system_clock::now();
    if (BFS(&map_for_BFS, start, end, &BFS_visited)) {
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
        cout<<"BFS found path with distance of "<<BFS_step+1<<endl;
        cout<<"The pass is showed as follows:"<<endl;
        cout<<"Step:" <<counter<<"  current coord is:["<<start[0]<<","<<start[1]<<"]"<<endl;
        for (int i=BFS_result.size()-1;i>-1;i--) {
            counter ++;
            cout<<"Step:" <<counter<<"  current coord is:["<<BFS_result[i][0]<<","<<BFS_result[i][1]<<"]"<<endl;
        }
    }
    else { cout<<"No path found!"<<endl; }
    end_t = std::chrono::system_clock::now();
    elapsed_seconds = end_t-start_t;
    std::cout << "Breadth First Searched completed, elapsed time: " << elapsed_seconds.count()*1000 << "ms\n";
    BFS_res.Name = "BFS";
    BFS_res.steps = BFS_step + 1;
    BFS_res.time = elapsed_seconds.count()*1000;
    BFS_res.visited = BFS_visited;
    res_collection.push_back(&BFS_res);
    cout<<"...................................................................\n"<<endl;
    // DFS test
    temp_coord.clear();
    cout<<"DFS start...\n"<<endl;
    start_t = std::chrono::system_clock::now();
    DFS(&map_for_DFS, start, end, &DFS_visited);
    if ((map_for_DFS)[end[0]][end[1]] -> DFS_depth != std::numeric_limits<int>::max()) {
        temp = map_for_DFS[end[0]][end[1]];
        DFS_step = temp->dist_for_BFS;
        while (1){
            if (temp->prev_for_BFS == NULL) {
                break;
            }
            temp_coord.push_back(temp->col);
            temp_coord.push_back(temp->row);
            DFS_result.push_back(temp_coord);
            temp_coord.clear();
            temp = temp->prev_for_BFS;
        }
        counter = 0;
        cout<<"DFS found path with distance of "<<DFS_step+1<<endl;
        cout<<"The pass is showed as follows:"<<endl;
        cout<<"Step:" <<counter<<"  current coord is:["<<start[0]<<","<<start[1]<<"]"<<endl;
        for (int i=DFS_result.size()-1;i>-1;i--) {
            counter ++;
            cout<<"Step:" <<counter<<"  current coord is:["<<DFS_result[i][0]<<","<<DFS_result[i][1]<<"]"<<endl;
        }
    }
    else {
        cout<<"No path found!"<<endl;
    }
    end_t = std::chrono::system_clock::now();
    elapsed_seconds = end_t-start_t;
    std::cout << "Depth First Search completed, elapsed time: " << elapsed_seconds.count()*1000 << "ms\n";
    DFS_res.Name = "DFS";
    DFS_res.steps = DFS_step + 1;
    DFS_res.time = elapsed_seconds.count()*1000;
    DFS_res.visited = DFS_visited;
        res_collection.push_back(&DFS_res);
    cout<<"...................................................................\n"<<endl;
    // Bellman Ford test

    temp_coord.clear();
    cout<<"Bellman Ford start...\n"<<endl;
    start_t = std::chrono::system_clock::now();
    bellman_ford_pre(&map_for_BF, &edges);
    end_t = std::chrono::system_clock::now();
    elapsed_seconds = end_t-start_t;
    std::cout << "Bellman Ford Pre-process completed, elapsed time: " << elapsed_seconds.count()*1000 << "ms\n";
    start_t = std::chrono::system_clock::now();
    bellman_ford(&map_for_BF, &edges, start, &BF_visited);
    temp = map_for_BF[end[0]][end[0]];
    if (map_for_BF[end[0]][end[1]] -> BF_dist != std::numeric_limits<int>::max()-1) {
        while (1) {
            temp_coord.clear();
            temp_coord.push_back(temp->row);
            temp_coord.push_back(temp->col);
            BF_result.push_back(temp_coord);
            if (temp_coord == start) {
                break;
            }
            possible_neighbor.clear();
            direction.clear();
            if (temp->up != NULL) {
                possible_neighbor.push_back(temp->up->BF_dist);
                direction.push_back(temp->up);
            }
            if (temp->down != NULL) {
                possible_neighbor.push_back(temp->down->BF_dist);
                direction.push_back(temp->down);
            }
            if (temp->left != NULL) {
                possible_neighbor.push_back(temp->left->BF_dist);
                direction.push_back(temp->left);
            }
            if (temp->right != NULL) {
                possible_neighbor.push_back(temp->right->BF_dist);
                direction.push_back(temp->right);
            }
            min_number_BF = *std::min_element(possible_neighbor.begin(),possible_neighbor.end());
            min_number_BF = std::distance(possible_neighbor.begin(), find(possible_neighbor.begin(), possible_neighbor.end(), min_number_BF));
            temp = direction[min_number_BF];
        }
        counter = 0;
        cout<<"Ballman Ford found path with distance of "<<BF_result.size()<<endl;
        cout<<"The pass is showed as follows:"<<endl;
        cout<<"Step:" <<counter<<"  current coord is:["<<start[0]<<","<<start[1]<<"]"<<endl;
        for (int i=DFS_result.size()-1;i>-1;i--) {
            counter ++;
            cout<<"Step:" <<counter<<"  current coord is:["<<DFS_result[i][0]<<","<<DFS_result[i][1]<<"]"<<endl;
        }
    }
    else {
        cout<<"No path found!"<<endl;
    }
    end_t = std::chrono::system_clock::now();
    elapsed_seconds = end_t-start_t;
    std::cout << "Bellman Ford Search completed, elapsed time: " << elapsed_seconds.count()*1000 << "ms\n";
    BF_res.Name = "Bellman Ford";
    BF_res.steps = BF_result.size();
    BF_res.time = elapsed_seconds.count()*1000;
    BF_res.visited = BF_visited;
        res_collection.push_back(&BF_res);
    cout<<"...................................................................\n"<<endl;
    // Wavefront test

    temp_coord.clear();
    cout<<"Lazy_Wavefront start...\n"<<endl;
    start_t = std::chrono::system_clock::now();
    wave_front(&map_for_WF,start, &WF_visited);
    if (map_for_WF[end[0]][end[1]]->WF_height != std::numeric_limits<int>::max() - 1) {
        temp = map_for_WF[end[1]][end[1]];
        while (1) {
            temp_coord.clear();
            temp_coord.push_back(temp->row);
            temp_coord.push_back(temp->col);
            WF_result.push_back(temp_coord);
            if (temp_coord == start) {
                break;
            }
            possible_neighbor.clear();
            direction.clear();
            if (temp->up != NULL) {
                possible_neighbor.push_back(temp->up->WF_height);
                direction.push_back(temp->up);
            }
            if (temp->down != NULL) {
                possible_neighbor.push_back(temp->down->WF_height);
                direction.push_back(temp->down);
            }
            if (temp->left != NULL) {
                possible_neighbor.push_back(temp->left->WF_height);
                direction.push_back(temp->left);
            }
            if (temp->right != NULL) {
                possible_neighbor.push_back(temp->right->WF_height);
                direction.push_back(temp->right);
            }
            min_number_BF = *std::min_element(possible_neighbor.begin(),possible_neighbor.end());
            min_number_BF = std::distance(possible_neighbor.begin(), find(possible_neighbor.begin(), possible_neighbor.end(), min_number_BF));
            temp = direction[min_number_BF];
        }
        counter = 0;
        cout<<"Lazy_Wavefront found path with distance of "<<WF_result.size()<<endl;
        cout<<"The pass is showed as follows:"<<endl;
//        cout<<"Step:" <<counter<<"  current coord is:["<<start[0]<<","<<start[1]<<"]"<<endl;
        for (int i=WF_result.size()-1;i>-1;i--) {
            cout<<"Step:" <<counter<<"  current coord is:["<<WF_result[i][0]<<","<<WF_result[i][1]<<"]"<<endl;
            counter ++;
        }
    }
    else {
        cout<<"No path found!"<<endl;
    }
    end_t = std::chrono::system_clock::now();
    elapsed_seconds = end_t-start_t;
    std::cout << "Lazy_Wavefront Search completed elapsed time: " << elapsed_seconds.count()*1000 << "ms\n";
    WF_res.Name = "Lazy_Wavefront";
    WF_res.steps = WF_result.size();
    WF_res.time = elapsed_seconds.count()*1000;
    WF_res.visited = WF_visited;
    res_collection.push_back(&WF_res);
    cout<<"...................................................................\n"<<endl;

    // Dijkstra test
    cout<<"Dijkstra start...\n"<<endl;

    start_t = std::chrono::system_clock::now();
    dijkstra(&map_for_DK, start, end, &DK_visited);
    if (map_for_DK[end[0]][end[1]]->DK_weight != std::numeric_limits<int>::max() - 1) {
        temp = map_for_DK[end[0]][end[0]];
        while (1) {
            temp_coord.clear();
            temp_coord.push_back(temp->row);
            temp_coord.push_back(temp->col);
            DK_result.push_back(temp_coord);
            if (temp_coord == start) {
                break;
            }
            possible_neighbor.clear();
            direction.clear();
            if (temp->up != NULL) {
                possible_neighbor.push_back(temp->up->DK_weight);
                direction.push_back(temp->up);
            }
            if (temp->down != NULL) {
                possible_neighbor.push_back(temp->down->DK_weight);
                direction.push_back(temp->down);
            }
            if (temp->left != NULL) {
                possible_neighbor.push_back(temp->left->DK_weight);
                direction.push_back(temp->left);
            }
            if (temp->right != NULL) {
                possible_neighbor.push_back(temp->right->DK_weight);
                direction.push_back(temp->right);
            }
            min_number_BF = *std::min_element(possible_neighbor.begin(),possible_neighbor.end());
            min_number_BF = std::distance(possible_neighbor.begin(), find(possible_neighbor.begin(), possible_neighbor.end(), min_number_BF));
            temp = direction[min_number_BF];
        }
        counter = 0;
        cout<<"Dijkstra found path with distance of "<<DK_result.size()<<endl;
        cout<<"The pass is showed as follows:"<<endl;
        cout<<"Step:" <<counter<<"  current coord is:["<<start[0]<<","<<start[1]<<"]"<<endl;
        for (int i=DK_result.size()-1;i> -1;i--) {
            counter ++;
            cout<<"Step:" <<counter<<"  current coord is:["<<DK_result[i][0]<<","<<DK_result[i][1]<<"]"<<endl;
        }
    }
    else {
                cout<<"No path found!"<<endl;
    }
    end_t = std::chrono::system_clock::now();
    elapsed_seconds = end_t-start_t;
    std::cout << "Dijkstra Search completed, elapsed time: " << elapsed_seconds.count()*1000 << "ms\n";
    DK_res.Name = "Dijkstra";
    DK_res.steps = DK_result.size();
    DK_res.time = elapsed_seconds.count()*1000;
    DK_res.visited = DK_visited;
        res_collection.push_back(&DK_res);
    cout<<"...................................................................\n"<<endl;

    // A* test
    cout<<"A* start...\n"<<endl;
    start_t = std::chrono::system_clock::now();
    A_star(&map_for_A, start, end, &A_visitied);
    if (map_for_A[end[0]][end[1]]->A_parent != NULL) {
        temp = map_for_A[end[0]][end[1]];
        while (1) {
            temp_coord.clear();
            temp_coord.push_back(temp->row);
            temp_coord.push_back(temp->col);
            A_result.push_back(temp_coord);
            if (temp_coord == start) {
                break;
            }
            temp = temp->A_parent;
        }
        counter = 0;
        cout<<"A* found path with distance of "<<A_result.size()<<endl;
        cout<<"The pass is showed as follows:"<<endl;
//        cout<<"Step:" <<counter<<"  current coord is:["<<start[0]<<","<<start[1]<<"]"<<endl;
        for (int i=A_result.size()-1;i> -1;i--) {
            counter ++;
            cout<<"Step:" <<counter<<"  current coord is:["<<DK_result[i][0]<<","<<DK_result[i][1]<<"]"<<endl;
        }
    }

    else {
        cout<<"No path found!"<<endl;
    }
    end_t = std::chrono::system_clock::now();
    elapsed_seconds = end_t-start_t;
    std::cout << "A* Search completed, elapsed time: " << elapsed_seconds.count()*1000 << "ms\n";
    A_res.Name = "A-star";
    A_res.steps = A_result.size();
    A_res.time = elapsed_seconds.count()*1000;
    A_res.visited = A_visitied;
    res_collection.push_back(&A_res);
    cout<<"...................................................................\n"<<endl;
    cout<<"Demo summary"<<endl;
    cout << std::left << std::setw(15) << "Algorithm" << std::left << std::setw(15) << "Steps";
    cout << std::left << std::setw(15) << "visited nodes"<< std::left << std::setw(15) << "Time(ms)" << endl;
    for (int i = 0; i < res_collection.size(); ++i) {
        cout << std::left << std::setw(15) << res_collection[i]->Name;
        cout << std::left << std::setw(15) << res_collection[i]->steps;
        cout << std::left << std::setw(15) << res_collection[i]->visited;
        cout << std::dec << std::setw(15) << res_collection[i]->time<<endl;
    }
    return 0;
}
