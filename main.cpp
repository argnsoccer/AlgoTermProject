#define _USE_MATH_DEFINES

#include <iostream>
#include <stdio.h>
#include <fstream>
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
    fstream file;
    file.open("file.dat", ios::out | ios::in);

    FILE *pipe = popen("gnuplot -persist", "w");
    fprintf(pipe, "set terminal png\n");
    fprintf(pipe, "set output 'output.png'\n");
    fprintf(pipe, "set xlabel 'Average Degree'\n");
    fprintf(pipe, "set ylabel 'Running Time (s)'\n");
    // fprintf(pipe, "set xrange [0:1]\n");
    // fprintf(pipe, "set yrange [0:1]\n");

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
        //file << v->x << " " << v->y << endl;
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
                cur->edges.push_back(nextNode);//add to current node edge list
                //file << cur->x << " " << cur->y << " " << j << endl;//write to file
                cur->degree++;//increment degree
                nextNode->edges.push_back(cur);//add to the paired node edge list
                //file << nextNode->x << " " << nextNode->y << " " << nextI << endl;//write the rest of the edge to file for plotting
                nextNode->degree++;
                //file << endl;//getting ready for next edge
            }
            nextI++;
        }
    }
    
    // fprintf(pipe, "plot \
    //                 'file.dat' using 1:2       with lines lc rgb 'black' lw 0.0000001 notitle,\
    //                 'file.dat' using 1:2:(0.000001) with circles fill solid lc rgb 'black' notitle\n");

    // fprintf(pipe, "plot 'file.dat' using 1:2:(0.000001) with circles fill solid lc rgb 'black' notitle\n");
    

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
    // int degCount = 0;
    // for(int x = 0; x < max; x++)
    // {
    //     degCount = 0;
    //     for(int y = 0; y < verts.size(); y++)
    //     {
    //         if(verts.at(y)->degree == x)
    //         {
    //             degCount++;
    //         }
    //     }
    //     file << x << " " << degCount << endl;
    // }

    fprintf(pipe, "plot 'file.dat' using 1:2:(0.5) with circles fill solid lc rgb 'black' notitle,\
                   'file.dat' using 1:2 with lines lc rgb 'black lw .25; \n");
    time_t endTimer;
    time(&endTimer);

    cout << "Max degree: " << max << endl;
    cout << "Min degree: " << min << endl;
    double density = total/numVerts;
    cout << "Average degree: " << density << endl;
    cout << "Total edges: " << totalEdges << endl;
    double seconds = endTimer - startTimer;
    cout << "Time taken to generate graph, calculate max and min, and plot: " << seconds << " seconds" << endl;

    fclose(pipe);
 

    
    
    file.close();

    


}

