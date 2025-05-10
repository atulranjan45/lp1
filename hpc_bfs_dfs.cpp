#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

// Edge class
class Edge {
public:
    int src, dest, wt;
    Edge(int s, int d, int w) {
        src = s;
        dest = d;
        wt = w;
    }
};

// Create undirected graph
void createGraph(vector<Edge> graph[], int n) {
    for (int i = 0; i < n; i++) {
        graph[i] = vector<Edge>();
    }

    // Sample undirected graph
    graph[0].push_back(Edge(0, 1, 1));
    graph[0].push_back(Edge(0, 2, 1));

    graph[1].push_back(Edge(1, 0, 1));
    graph[1].push_back(Edge(1, 3, 1));

    graph[2].push_back(Edge(2, 0, 1));
    graph[2].push_back(Edge(2, 4, 1));

    graph[3].push_back(Edge(3, 1, 1));
    graph[3].push_back(Edge(3, 4, 1));
    graph[3].push_back(Edge(3, 5, 1));

    graph[4].push_back(Edge(4, 2, 1));
    graph[4].push_back(Edge(4, 3, 1));
    graph[4].push_back(Edge(4, 5, 1));

    graph[5].push_back(Edge(5, 3, 1));
    graph[5].push_back(Edge(5, 4, 1));
    graph[5].push_back(Edge(5, 6, 1));

    graph[6].push_back(Edge(6, 5, 1));
}

// Sequential BFS
void bfs(vector<Edge> graph[], int n) {
    queue<int> q;
    bool vist[100] = {false};
    q.push(0);

    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        if (!vist[curr]) {
            vist[curr] = true;
            cout << curr << " ";

            for (Edge e : graph[curr]) {
                q.push(e.dest);
            }
        }
    }
}

// Parallel BFS using OpenMP
void par_bfs(vector<Edge> graph[], int n) {
    queue<int> q;
    bool vist[100] = {false};
    q.push(0);
    vist[0] = true;

    while (!q.empty()) {
        int size = q.size();
        vector<int> next_level;

        vector<int> curr_level;
        for (int i = 0; i < size; i++) {
            curr_level.push_back(q.front());
            q.pop();
        }

        #pragma omp parallel for shared(graph, vist, next_level)
        for (int i = 0; i < curr_level.size(); i++) {
            int curr = curr_level[i];
            cout << curr << " ";

            for (Edge e : graph[curr]) {
                int neighbor = e.dest;
                #pragma omp critical
                {
                    if (!vist[neighbor]) {
                        vist[neighbor] = true;
                        next_level.push_back(neighbor);
                    }
                }
            }
        }

        for (int node : next_level) {
            q.push(node);
        }
    }
}
// Sequential BFS:	One node processed at a time	
// Parallel BFS: All nodes at same level processed in parallel

// Sequential DFS
void dfs(vector<Edge> graph[], int curr, bool vist[]) {
    vist[curr] = true;
    cout << curr << " ";

    for (Edge e : graph[curr]) {
        if (!vist[e.dest]) {
            dfs(graph, e.dest, vist);
        }
    }
}

// Parallel DFS using OpenMP tasks
void par_dfs(vector<Edge> graph[], int curr, bool vist[]) {
    bool proceed = false;

    #pragma omp critical
    {
        if (!vist[curr]) {
            vist[curr] = true;
            cout << curr << " ";
            proceed = true;
        }
    }

    if (proceed) {
        #pragma omp parallel for
        for (int i = 0; i < graph[curr].size(); i++) {
            int neighbor = graph[curr][i].dest;
            #pragma omp task
            par_dfs(graph, neighbor, vist);
        }
    }
}

// Sequential DFS = 1 path explored deeply at a time
// Parallel DFS = All neighbors at each node can be explored concurrently using #pragma omp task

int main() {
    const int n = 7;
    vector<Edge> graph[n];

    createGraph(graph, n);

    cout << "\n===== HPC Application: Parallel BFS & DFS =====\n";

    cout << "\nSequential BFS: ";
    double start = omp_get_wtime();
    bfs(graph, n);
    double end = omp_get_wtime();
    cout << "\nTime: " << (end - start) << " sec\n";

    cout << "\nParallel BFS: ";
    start = omp_get_wtime();
    par_bfs(graph, n);
    end = omp_get_wtime();
    cout << "\nTime: " << (end - start) << " sec\n";

    bool vist1[100] = {false};
    cout << "\nSequential DFS: ";
    start = omp_get_wtime();
    dfs(graph, 0, vist1);
    end = omp_get_wtime();
    cout << "\nTime: " << (end - start) << " sec\n";

    bool vist2[100] = {false};
    cout << "\nParallel DFS: ";
    start = omp_get_wtime();
    #pragma omp parallel
    {
        #pragma omp single
        par_dfs(graph, 0, vist2);
    }
    end = omp_get_wtime();
    cout << "\nTime pri done: " << (end - start) << " sec\n";

    cout << "\n==============================================\n";
    return 0;
}
