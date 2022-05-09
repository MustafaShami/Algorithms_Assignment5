//Mustafa Shami     Due: May 3rd, 2022     CSCI3412-Algorithms
//Remember to make sure the cmake list is building the right file
//used "Node" and "Vertex" interchangeably in the comments
#include <iostream>
using namespace std;
#include<vector>
#include<queue>
#include<stack>

//Store list nodes
struct Node
{
    int value;
    vector<Node*> adjacencyList; //tried storing the adjacency list in a linked list first which made things unnecessarily complicated
    Node* parent; //(BFS)parent will be used for BFS when we reverse path and follow parent nodes from the end destination vertext to the start vertex

    int discoverTime;
    int finishTime;
    int color; // 0=white, 1=grey, 2=black
};

// graph edge structure
struct Edge
{
    int source, destination;
};

class Graph
{   //insert Edge at the end of adjacency list (helper function)
    void insertEdge(Node* source, Node* destination)
    {
        source->adjacencyList.push_back(destination);
    }

    int numNodes; //total number of nodes in the graph

public:
    // A array of pointers to Node to represent the adjacency list
    Node **head;

    vector<Node*> visited; //(BFS) this vector will store the vertices that we have already visited during

    // Constructor
    Graph(vector<Edge> edges, int numEdges, int numNodes)
    {
        // allocate memory (array of pointers, to our nodes/vertices
        head = new Node*[numNodes]();
        this->numNodes = numNodes;

        //initialize head pointer for all the vertices (zero index)
        for(int i=0; i < numNodes; i++)
        {
            head[i] = new Node();
            head[i]->value = i+1;
        }

        //add a directed edge between two vertices
        for (int i=0; i < numEdges; i++)
        {   //Give values/edges to the new node
            int source = edges[i].source;
            int destination = edges[i].destination;

            //so a directed edge goes from source to destination so we are going to plug in those values to the insert function
            insertEdge(head[source-1], head[destination-1]); //using graph helper function (-1 because zero indexed)
            //insertEdge(head[destination-1], head[source-1]); //if we wanted to make an undirected graph we would just add this line which adds the opposite direction for each edge
        }
    }

    void transposeGraph(vector<Edge> & edges, int V) //basically the same as the constructor except we are going to insert the opposite direction edges
    {
        // allocate memory (array of pointers, to our nodes/vertices
        head = new Node*[V]();

        //initialize head pointer for all the vertices (zero index)
        for(int i=0; i < V; i++)
        {
            head[i] = new Node();
            head[i]->value = i+1;
        }

        //add a directed edge between two vertices
        for (int i=0; i < edges.size(); i++)
        {   //Give values/edges to the new node
            int source = edges[i].destination; //storing opposite value
            int destination = edges[i].source; //storing opposite value
            //insert edges but their source/dest values are flipped which is how we will get our transposed graph
            insertEdge(head[source-1], head[destination-1]);
        }
    }

    void DFS(Graph &G, int V, stack<Node*> & finishedVertices)
    {
        //stack<Node*> finishedVertices;
        bool saveOrder = true;

        int time;
        for (int i = 0; i < V; i++) //initialize vertices to white(undiscovered) and w/ no parent
        {
            head[i]->color = 0;
            head[i]->parent = nullptr;
        }
        time = 0;

        for(int i=0; i < V; i++)
        {
            if(head[i]->color == 0)
            {
                DFSVisit(G, head[i], time, finishedVertices, saveOrder);
            }
        }

        //cout << finishedVertices.size(); shows in order the vertices that the DFS algorithm visited as it ran
        //printFinishedVerticesList();
    }

    void DFSVisit(Graph &G, Node* vertex, int &time, stack<Node*> & finishedVertices, bool addToStack)
    {
        if(!addToStack && !inVisited(vertex))
        {
            cout << vertex->value << " ";
        }

        time++;
        vertex->discoverTime = time;
        vertex->color = 1; // vertex is discovered (grey)

        for(int i=0; i<vertex->adjacencyList.size(); i++)
        {
            Node* neighbor = vertex->adjacencyList[i];
            if(neighbor->color == 0)
            {
                neighbor->parent = vertex;
                DFSVisit(G, neighbor, time, finishedVertices, addToStack);
            }
        }
        vertex->color = 2; //vertex is finished (black)
        visited.push_back(vertex);
        time++;
        vertex->finishTime = time;

        if(addToStack)
        {
            finishedVertices.push(vertex);
        }
    }

    void DFS_SpecifcOrder(Graph &G, int V, stack<Node*> & finishedVertices)
    {
        bool saveOrder = false;

        int time;
        for (int i = 0; i < V; i++) //initialize vertices to white(undiscovered) and w/ no parent
        {
            head[i]->color = 0;
            head[i]->parent = nullptr;
        }
        time = 0;

        cout << "G contains following strongly connected components:" << endl;
        int countComponents = 0;
        while(!finishedVertices.empty())
        {
            int i = finishedVertices.top()->value - 1; //-1 cuz zero indexed
            if(head[i]->color == 0) //now we are going to run the recursive DFS algorithm
            {
                countComponents++;
                cout << "Component " << countComponents << ": ";
                DFSVisit(G, head[i], time, finishedVertices, saveOrder);
                cout << endl;
            }
            finishedVertices.pop(); //remove the vertex we just visited from the stack
        }

    }

    bool inVisited(Node* ptr) //passed in the pointer to a vertex
    {
        for(int i=0; i < visited.size(); i++)
        {
            if (visited[i]->value == ptr->value) //check if we have visited this vertex before
            {
                return true;
            }
        }
        return false;
    }

    void printNeighborVertices(Node* ptr) // takes pointer to vertex as param and then prints the connected neighbors of that vertex
    {
        for(int i=0; i<ptr->adjacencyList.size(); i++)
        {
            cout << ptr->adjacencyList[i]->value << " ";
        }
        cout << endl;
    }

    void printAdjListRepresentation() //Prints the adjacency list (split into two functions)
    {
        for (int i = 0; i < this->numNodes; i++) {
            cout << i+1 << ": "; //print the current vertex/node
            printNeighborVertices(head[i]); // print the neighboring vertices/nodes of the current one
        }
    }


    // Destructor to deallocate memory
    ~Graph()
    {
        delete[] head;
    }
};

int main() {
    //Below line is to take input from input.txt instead of having to type each command
    //freopen("/Users/mustafa/Desktop/SPRING 2022/Algorithms CSCI3412/Programming Assignments/Shami_Prog.Assignment 5/inputB.txt", "r", stdin);
    int numCases;
    cin >> numCases;
    int graphCounter = 1;

    while (numCases != 0) {

        int numVertices;
        cin >> numVertices;

        int source, destination;
        cin >> source;
        cin >> destination;
        vector<Edge> edges; //vector that will hold Edge objects which we create from user input on the edges that are in the graph
        while (source != 0 && destination != 0)
        {
            Edge e = {source, destination}; //create edge object
            edges.push_back(e); //store edge object

            cin >> source >> destination;
        }

        int numEdges = edges.size();
        Graph graph(edges, numEdges, numVertices); //give values to create the graph object

        cout << "Graph #" << graphCounter << ":" << endl;
        //print adjacency list
        graph.printAdjListRepresentation();

        cout << endl;

        // Stack that will hold the order of the finished vertices (in decreasing order of finised times i.e last finished to first finished)
        stack<Node*> finishedVertices;
        //RUN DFS on Graph
        graph.DFS(graph,numVertices, finishedVertices);

        //Transpose the graph
        Graph transposedG(edges, numEdges, numVertices);//creating a new graph
        transposedG.transposeGraph(edges, numVertices);//transposing our new graph

        //Run DFS on the graph in the decreasing order we found from DFS on original graph
        transposedG.DFS_SpecifcOrder(transposedG, numVertices, finishedVertices);

        //transposedG.printAdjListRepresentation(); //test to see if transposed correctly

        cout << endl << endl;
        graphCounter++;
        numCases--;
    }
    return 0;
}


