//Mustafa Shami     Due: May 3rd, 2022     CSCI3412-Algorithms
//Remember to make sure the cmake list is building the right file
//used "Node" and "Vertex" interchangeably in the comments

//STRUGGLED ON THIS ONE: WAS SO CLOSE :(

#include <iostream>
using namespace std;
#include<vector>

#define INFINITY 10000
#define MAX_VERTICES 1000


//Store list nodes
struct Node
{
    int vertexId;
    vector<Node*> adjacencyList; //tried storing the adjacency list in a linked list first which made things unnecessarily complicated
    vector<double> weights;
    int parent;
    double key;
    //Node* parent; //(BFS)parent will be used for BFS when we reverse path and follow parent nodes from the end destination vertext to the start vertex

};


// graph edge structure
struct Edge
{
    int source, destination;
    double weight;
};

class Graph {   //insert Edge at the end of adjacency list (helper function)
    void insertEdge(Node *source, Node *destination, double weight) {
        source->adjacencyList.push_back(destination);
        destination->adjacencyList.push_back(source);
    }

    int numNodes; //total number of nodes in the graph

public:
    // A array of pointers to Node to represent the adjacency list
    Node **head;



    double weights[MAX_VERTICES][MAX_VERTICES]; //to store all of the weights

    // Constructor
    Graph(vector<Edge> edges, int numEdges, int numNodes) {
        // allocate memory (array of pointers, to our nodes/vertices
        head = new Node *[numNodes]();
        this->numNodes = numNodes;

        //initialize head pointer for all the vertices (zero index)
        for (int i = 0; i < numNodes; i++) {
            head[i] = new Node();
            head[i]->vertexId = i + 1;
        }

        //initialize array with all zeros
        for (int i = 0; i < numNodes; i++) {
            for (int j = 0; j < numNodes; j++) {
                weights[i][j] = 0;
            }
        }
        //add a directed edge between two vertices
        for (int i = 0; i < numEdges; i++) {   //Give values/edges to the new node
            int source = edges[i].source;
            int destination = edges[i].destination;
            double weight = edges[i].weight;

            weights[source][destination] = weight;

            insertEdge(head[source - 1], head[destination - 1],
                       weight); //using graph helper function (-1 because zero indexed)
        }
    }

    void findMST()
    {
        //numNodes++; //to get rid of the zero index
        //parallel arrays for parent and key
        int parent[numNodes]; //stores the parent values of the nodes we visted
        double key[numNodes]; //help us determine which vertex to visit next based on weight edge
        bool inMST[numNodes];
        //Initialize arrays
        for(int i=0; i<numNodes; i++)
        {
            key[i] = INFINITY;
            inMST[i] = false;
        }

        //starting at first vertex so initialize key and parent
        key[0] = 0;
        parent[0] = -12345;

        for(int i=0; i<numNodes-1; i++) // Minimum Spanning Tree always has edges = number of Vertices - 1
        {
            int current = selectMinNode(key, inMST); //pick the next "safe" vertex

            inMST[current] = true; //add picked vertex to the list of visited(-1 cuz zero indexed)

            for(int j=0; j<numNodes; j++)
            {
                double thisWeight = weights[current][j]; //store in variable so we don't have to write this long shit out

                // check 3 conditions, 1. vertex is not visited 2.edge exists 3. update value if current weight less than key weight
                bool edgeExists = false;
                if(thisWeight != 0)
                {
                    edgeExists = true;
                }

                if(!inMST[j] && edgeExists && thisWeight < key[j])
                {
                    parent[j] = current;
                    key[j] = thisWeight;
                }
            }
        }

        cout << "A minimum weight spanning tree consists of:" << endl;
        for(int i=1; i<numNodes; i++)
        {
            cout << "Edge:" ;
            cout << "(" << parent[i]+1 << ", " << i+1 << ") of weight " << weights[i-1][parent[i-1]] << endl;
        }
        int sum;
        for(int i=0; i<numNodes;i++)
        {
            sum += key[i];
        }
        cout << "Total weight = " << sum-10000;
    }

    int selectMinNode(double key[], bool inMST[])
    {
        double min = 1234567;
        int index = 0;

        for(int i=0; i < numNodes; i++)
        {
            if(!inMST[i] && key[i] < min)
            {
                min = key[i];
                index = i;
            }
        }
        if(index > -1)
        {
            return index;
        }

        return 0;
    }

// FAIL 3
//    void findMST()
//    {
//         //numNodes++; //to get rid of the zero index
//        //parallel arrays for parent and key
//        int parent[numNodes]; //stores the parent values of the nodes we visted
//        double key[numNodes]; //help us determine which vertex to visit next based on weight edge
//        bool inMST[numNodes];
//        //Initialize arrays
//        for(int i=0; i<numNodes; i++)
//        {
//            key[i] = INFINITY;
//            inMST[i] = false;
//        }
//
//        //starting at first vertex so initialize key and parent
//        key[0] = 0;
//        parent[0] = -12345;
//
//        for(int i=0; i<numNodes-1; i++) // Minimum Spanning Tree always has edges = number of Vertices - 1
//        {
//            Node* current = selectMinNode(key, inMST); //pick the next "safe" vertex
//
//            int currentV = current->vertexId-1;
//
//            inMST[current->vertexId-1] = true; //add picked vertex to the list of visited(-1 cuz zero indexed)
//
//            for(int j=0; j<numNodes; j++)
//            {
//                double thisWeight = weights[current->vertexId-1][j]; //store in variable so we don't have to write this long shit out
//
//                // check 3 conditions, 1. vertex is not visited 2.edge exists 3. update value if current weight less than key weight
//                bool edgeExists = false;
//                if(thisWeight != 0)
//                {
//                    edgeExists = true;
//                }
//
//                if(!inMST[j] && edgeExists && thisWeight < key[j])
//                {
//                    parent[j] = current->vertexId-1;
//                    key[j] = thisWeight;
//                }
//            }
//        }
//
//        cout << "A minimum weight spanning tree consists of: \nEdge ";
//        for(int i=1; i<numNodes; i++)
//        {
//            cout << "(" << parent[i] << ", " << i << ") of weight " << weights[i][parent[i]] << endl;
//        }
//    }
//
//    int selectMinNode(double key[], bool inMST[])
//    {
//        double min = 123456;
//        int index = -1;
//
//        for(int i=0; i < numNodes; i++)
//        {
//            if(!inMST[i] && key[i] < min)
//            {
//                min = key[i];
//                index = i;
//            }
//        }
//        if(index > -1)
//        {
//            return index-1;
//        }
//
//        return 0;
//    }

//FAIL 1
//    void findMST() {
//
//        vector<bool> inMST(numNodes, false); //array that will keep track of nodes in the Minimum Spanning Tree
//        int parent;
//
//        Node *current = head[0];
//////        visited.clear();
//////        visited.push_back(current);
//        vector<int> value(numNodes, INFINITY); // used to store/compare weights
//
//        value[0] = 0;
////        current->parent = -1;
////        current->key = 0;
//
//        for (int i = 0; i < numNodes - 1; i++) //V-1 since MST always has V-1 edges
//        {
//            //select adjacent vertex to visit using GREEDY method (get the adjacent node that has the least weight)
//            //current = selectMinNode(current, inMST);
//            current = selectMinNode(value, inMST);
//            inMST[current->vertexId - 1] = true;
//
//            for (int j = 0; j < numNodes; j++) {
//                bool AdjacentCheck = isAdjacent(current, head[j]->vertexId) > -1;
//
//                if (!inMST[j] && AdjacentCheck > -1 && current->weights[AdjacentCheck] < value[j]) {
//                    value[j] = current->weights[AdjacentCheck];
//                    //head[j]->parent = current->vertexId;
//                }
//            }
//        }
//
//        for (int i = 0; i < numNodes; i++) {
//            cout << head[i]->vertexId << " -> " << head[i]->vertexId << endl;
//        }
//    }
//
//
//    Node *selectMinNode(vector<int> &value, vector<bool> &inMST) {
//        int min = 10000;
//        int minIndex = -1;
//        for (int i = 0; i < value.size(); i++) {
//            if (!inMST[i] && value[i] < min) {
//                min = value[i];
//                minIndex = i;
//            }
//        }
//        if (minIndex > -1) {
//            return head[minIndex];
//        }
//        return nullptr;
//    }
//
//    bool isAdjacent(Node *ptr, int potentialNeighbor) {
//        for (int i = 0; i < ptr->adjacencyList.size(); i++) {
//            if (ptr->adjacencyList[i]->vertexId - 1 == potentialNeighbor) {
//                return true;
//            }
//        }
//        return false;
//    }




//    bool inVisited(Node* ptr) //passed in the pointer to a vertex
//    {
//        for(int i=0; i < visited.size(); i++)
//        {
//            if (visited[i]->vertexId == ptr->vertexId) //check if we have visited this vertex before
//            {
//                return true;
//            }
//        }
//        return false;
//    }




    void printNeighborVertices(Node* ptr) // takes pointer to vertex as param and then prints the connected neighbors of that vertex
    {
        for(int i=0; i<ptr->adjacencyList.size(); i++)
        {
            cout << ptr->adjacencyList[i]->vertexId << " ";
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
    //freopen("/Users/mustafa/Desktop/SPRING 2022/Algorithms CSCI3412/Programming Assignments/Shami_Prog.Assignment 5/inputC.txt", "r", stdin);
    int numCases;
    cin >> numCases;
    int graphCounter = 1;

    while (numCases != 0) {

        int numVertices;
        cin >> numVertices;

        int numEdges;
        cin >> numEdges;
        int countEdge = numEdges-1;

        int source, destination;
        double weight;
        cin >> source;
        cin >> destination;
        cin >> weight;
        vector<Edge> edges; //vector that will hold Edge objects which we create from user input on the edges that are in the graph
        while (countEdge > 0) //while (countEdge >= 0) CHANGE TO THIS FOR IT TO WORK ON LOCAL MACHINE
        {
            Edge e = {source, destination, weight}; //create edge object
            edges.push_back(e); //store edge object

            cin >> source >> destination >> weight;
            countEdge--;
        }

        numEdges = edges.size();
        Graph graph(edges, numEdges, numVertices); //give values to create the graph object

        cout << "Graph #" << graphCounter << ":" << endl;
        //print adjacency list
        graph.printAdjListRepresentation();

        cout << endl;

        graph.findMST();

        cout << endl << endl;
        graphCounter++;
        numCases--;
    }
    return 0;
}