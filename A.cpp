//Mustafa Shami     Due: May 3rd, 2022     CSCI3412-Algorithms
//Remember to make sure the cmake list is building the right file
//used "Node" and "Vertex" interchangeably in the comments
#include <iostream>
using namespace std;
#include<vector>
#include<queue>

//Store list nodes
struct Node
{
    int value;
    vector<Node*> adjacencyList; //tried storing the adjacency list in a linked list first which made things unnecessarily complicated
    Node* parent; //(BFS)parent will be used for BFS when we reverse path and follow parent nodes from the end destination vertext to the start vertex
};



// graph edge structure
struct Edge
{
    int source, destination;
};

class Graph
{   //insert Edge at the end of adjacency list (helper function)
    Node* insertEdge(Node* source, Node* destination)
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

        //add edges between two vertices
        for (int i=0; i < numEdges; i++)
        {   //Give values/edges to the new node
            int source = edges[i].source;
            int destination = edges[i].destination;

            //So undirected graph mean it can go both ways, so we are going to add both directions when adding edge to make
            //    and UNDIRECTED graph
            insertEdge(head[source-1], head[destination-1]); //using helper function (-1 because zero indexed)
            insertEdge(head[destination-1], head[source-1]); //if we wanted to make a directed graph we would just remove this line which adds the opposite direction to an edge
        }
    }

    //determine if we visited a node (BFS)
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

    void BFS(Node* start, Node* destination) //passed two Node pointers, start and end value for our search
    {
        visited.clear(); //empties vector list of visited vertices
        visited.push_back(start); //Include the start vertex into the list of visited

        queue<Node*> Q; //tells us what vertex to visit next
        Q.push(start); //add start vertex to queue
        bool destFound = false;

        start->parent = nullptr; //the start node in our search does not have a parent

        while(!Q.empty()) //keeps running while there are places that need to be visited
        {
            Node* current = Q.front(); //set current node to the next node from the queue
            Q.pop();//remove from the queue, node currently being visited

            if(destination->value == current->value) //if we found destination
            {
                destFound = true;
                break;
            }
            for(int i=0; i<current->adjacencyList.size(); i++) //loops through the current nodes neighbors
            {
                Node* neighbor = current->adjacencyList[i];
                if(!inVisited(neighbor)) //if not previosuly visited this node then mark as visited and add the queue of next visit
                { //looking current node's neighbor
                    visited.push_back(neighbor);
                    neighbor->parent = current; //the neighbor nodes' parent node will be the node that we are currently on
                    Q.push(neighbor); //add neighbor into the queue of vertices that we have yet to visit
                }
            }
        }
        if(destFound)
        {
            //print distance and store path
            vector<int> path;
            Node* currentNode = destination; //starting from the end and working our way to the start inputting the path into a vector
            path.push_back(destination->value);
            while(currentNode->parent != nullptr) //nullptr means we have reached the start node
            { //so now reversing through the path and visiting each parent node to construct the path into a vector
                path.insert(path.begin(), currentNode->parent->value); //path.begin = insert at the front of the vector
                currentNode = currentNode->parent;
            }
            cout << "The distance from " << start->value << " to " << destination->value << " is " << path.size()-1 << "." << endl; //-1 cuz its counting nodes instead of hops
            printShortestPath(path); //print path
        }
        else
        {
            cout << "The distance from " << start->value << " to " << destination->value << " is " << "infinity." << endl;
            cout << "No path from " << start->value << " to " << destination->value << " exists" << endl;
        }
    }

    void printShortestPath(vector<int> p)
    {
        cout << "A shortest path from " << p.front() << " to " << p.back() << " is: ";
        for(int i=0; i<p.size(); i++)
        {
            if(i==p.size()-1) // to fix the annoying problem of having and extra -> at the end
            {
                cout << p[i] << endl;
            }
            else
            {
                cout << p[i] << " -> ";
            }
        }
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
    freopen("/Users/mustafa/Desktop/SPRING 2022/Algorithms CSCI3412/Programming Assignments/Shami_Prog.Assignment 5/inputA.txt", "r", stdin);

    int numCases;
    cin >> numCases;
    int graphCounter = 1;

    while (numCases != 0) {
        cout << "Graph #" << graphCounter << ":" << endl;

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

        //print adjacency list
        graph.printAdjListRepresentation();

        cout << endl;

        //BFS
        cin >> source >> destination;
        while (source != 0 && destination != 0)
        {
            graph.BFS(graph.head[source-1], graph.head[destination-1]);
            cout << endl;
            cin >> source >> destination;
        }
        graphCounter++;
        numCases--;
// TESTING BFS
//        graph.BFS(graph.head[0], graph.head[4]);// BFS from 1 to 5 (zero index)
//        graph.BFS(graph.head[2], graph.head[5]);// BFS from 3 to 6 (zero index)
//        graph.BFS(graph.head[3], graph.head[3]);// BFS from 4 to 4 (zero index)
    }
    return 0;
}
