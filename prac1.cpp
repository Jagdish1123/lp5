#include<bits/stdc++.h>
#include<omp.h>
using namespace std;

class Graph{
    int v;
    vector<vector<int>> adjMatrix;
public:
    Graph(int v){
        this->v = v;
        adjMatrix.resize(v);
    }
    void addEdge(int u,int v){
        adjMatrix[u].push_back(v);
        adjMatrix[v].push_back(u);
    }
    void dfs(int node){
        vector<int> visited(v,0);
        dfsUtil(node,visited);
    }
    void dfsParallel(int node){
        vector<int> visited(v,0);
        visited[node] = 1; // remember this
        #pragma omp parallel
        {
            #pragma omp single
            {
                #pragma omp task shared(visited)
                dfsParallelUtil(node,visited);
            }
        }
    }
    void bfs(int node){
        vector<int> visited(v,0);
        bfsUtil(node,visited);
    }
    void bfsParallel(int node){
        vector<int> visited(v,0);
        bfsParallelUtil(node,visited);
    }
private:
    void dfsUtil(int node,vector<int> &visited){
        visited[node] = 1;
        for(auto nbr : adjMatrix[node]){
            if(!visited[nbr]){
                visited[nbr] = true;
                dfsUtil(nbr,visited);
            }
        }
    }
    void dfsParallelUtil(int node,vector<int> &visited){
        for(auto nbr : adjMatrix[node]){
            if(!visited[nbr]){
                bool swapn_task = false;

                #pragma omp critical
                {
                    if(!visited[nbr]){
                    visited[nbr] = true;
                    swapn_task = true;
                    }
                }
                if(swapn_task){
                    #pragma omp task shared(visited)
                    dfsParallelUtil(nbr,visited);

                }
            }
        }
        #pragma omp taskwait
    }
    void bfsUtil(int node,vector<int> &visited){
        queue<int> q;
        q.push(node);
        visited[node] = true;
        while (!q.empty())
        {
            auto front = q.front();
            q.pop();

            for(auto nbr : adjMatrix[front]){
                if(!visited[nbr]){
                    q.push(nbr);
                    visited[nbr] = true;
                }
            }
        }
        
    }
    void bfsParallelUtil(int node,vector<int> &visited){
        vector<int> current;
        current.push_back(node);
        visited[node] = true;

        while(!current.empty()){
            vector<int> next;
            #pragma omp parallel for
            for(int i = 0; i < current.size(); i++){
                int node = current[i];

                for(auto nbr : adjMatrix[node]){
                    if(!visited[nbr]){
                        #pragma omp critical
                        {
                            if(!visited[nbr]){
                                visited[nbr] = true;
                                next.push_back(nbr);
                            }
                        }
                    }
                }
            }
        
            current = next;
        }
    }
};

int main(){
    // int v = 7000;
    // Graph *obj = new Graph(v);
    // srand(42);
    // for(int i = 0; i < v; i++){
    //     for(int j = 0; j < 20; j++){
    //         int nbr = rand() % v;
    //         if(nbr != i){
    //             obj->addEdge(i,nbr);
    //         }
    //     }
    // }
    int v;
    int e;
    cout<<"Enter the number of vertices and edges: ";
    cin>>v>>e;
    Graph *obj = new Graph(v);
    cout<<"Enter the edges: \n";
    for(int i = 0; i < e; i++){
        int u,v;
        cin>>u>>v;
        obj->addEdge(u,v);
    }
    int start;
    cout<<"Enter the starting node: ";
    cin>>start;

    double t1,t2;

    t1 = omp_get_wtime();
    obj->bfsParallel(start);
    t2 = omp_get_wtime();
    cout << "Time required for parallel BFS : " << t2 - t1 << " seconds " << endl;

    t1 = omp_get_wtime();
    obj->dfsParallel(start);
    t2 = omp_get_wtime();
    cout << "Time required for parallel DFS : " << t2 - t1 << " seconds " << endl;

    t1 = omp_get_wtime();
    obj->bfs(start);
    t2 = omp_get_wtime();
    cout << "Time required for sequential BFS : " << t2 - t1 << " seconds " << endl;

    t1 = omp_get_wtime();
    obj->dfs(start);
    t2 = omp_get_wtime();
    cout << "Time required for sequential DFS : " << t2 - t1 << " seconds " << endl;


    return 0;
}
