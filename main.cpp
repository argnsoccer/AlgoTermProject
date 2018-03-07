#define _USE_MATH_DEFINES

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
 
using namespace std;

struct Node{
    double x;
    double y;
    vector <Node*> edges;
    int degree;

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
    
    time_t startTimer;
    time(&startTimer);

    vector <Node*> verts;
    for (int i = 0; i < numVerts; i++)//create random vertices
    {
        Node* v = new Node();
        v->degree = 0;
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
        int nextI = j+1;

        while(nextI < verts.size())//bounds check on verts in list
        {
            Node * nextNode = verts.at(nextI);
            changeX = cur->x - nextNode->x;//distance in x
            changeY = cur->y - nextNode->y;//distance in y
            changeX *= changeX;//squared
            changeY *= changeY;
            distance = sqrt(changeX + changeY);//find distance
            if(distance <= r)
            {
                cur->edges.push_back(nextNode);
                cur->degree++;
                nextNode->edges.push_back(cur);
                nextNode->degree++;
            }
            nextI++;
        }
    }
    time_t endTimer;
    time(&endTimer);

    double seconds = endTimer - startTimer;
    cout << "Time taken to generate vertices and edges: " << seconds << " seconds" << endl;

    int total = verts.at(0)->degree;
    int min = verts.at(0)->degree;
    int max = verts.at(0)->degree;
    int totalEdges = verts.at(0)->edges.size();
    for (int a = 1; a < verts.size(); a++)
    {
        total += verts.at(a)->degree;
        totalEdges += verts.at(a)->edges.size();
        if(verts.at(a)->degree < min)
        {
            min = verts.at(a)->degree;
        }
        if(verts.at(a)->degree > max)
        {
            max = verts.at(a)->degree;
        } 

    }
    cout << "Max degree: " << max << endl;
    cout << "Min degree: " << min << endl;
    double density = total/numVerts;
    cout << "Average degree: " << density << endl;
    cout << "Total edges: " << totalEdges << endl;



}

