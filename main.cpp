#define _USE_MATH_DEFINES

#include <iostream>

#include <stdlib.h>
#include <vector>
#include <math.h>
 
using namespace std;

struct Node{
    double x;
    double y;
    vector <Node*> edges;
};


int main()
{
    int numVerts = 0;
    cout << "Input number of vertices: ";
    cin >> numVerts;

    int avgDeg = 0;
    cout << "Input average degree of graph: ";
    cin >> avgDeg;

    // int topoChoice = 0;
    // cout << "Input Topology (1 for square): ";
    // cin >> topoChoice;

    double r = 0.0;
    r = sqrt(avgDeg/(numVerts * M_PI));

    cout << "r = " << r << endl;
    
    vector <Node*> verts;
    for (int i = 0; i < numVerts; i++)//create random vertices
    {
        Node* v = new Node();
        v->x = ((double) rand() / (RAND_MAX));
        v->y = ((double) rand() / (RAND_MAX));
        verts.push_back(v);
    }
    double distance = 0.0;
    double changeX = 0.0;
    double changeY = 0.0;
    for (int j = 0; j < verts.size(); j++)
    {
        Node * cur = verts.at(j);
        Node * nextNode = verts.at(j+1);
        while(j+1 < verts.size())//bounds check on verts in list
        {
            changeX = cur->x - nextNode->x;//distance in x
            changeY = cur->y - nextNode->y;//distance in y
            changeX *= changeX;//squared
            changeY *= changeY;
            distance = sqrt(changeX + changeY);//find distance
            if(distance <= r)
            {
                cur->edges.push_back(nextNode);
                nextNode->edges.push_back(cur);
            }
        }
    }

}

