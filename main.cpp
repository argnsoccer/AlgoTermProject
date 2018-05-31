#define _USE_MATH_DEFINES

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <algorithm>
#include <string>
#include <stack>
#include <set> 

using namespace std;

struct Node{
    double x;
    double y;
    vector <Node*> edges;
    int degree;
    int color;
    int id;

};

class ComparatorByX{
    public:
        bool operator() (Node* a, Node* b){
            return a->x < b->x;
        }
};


int main()
{
    fstream file;
    file.open("in.txt", ios::out | ios::in);

    srand((unsigned)time(NULL));

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
    int idCount = 0;
    vector<Node*> vertsCopy;
    vector <Node*> verts;
    for (int i = 0; i < numVerts; i++)//create random vertices
    {
        Node* v = new Node();
        Node* v2 = new Node();
        v->degree = 0;
        v2->degree = 0;
        v->x = ((double) rand() / (RAND_MAX + 1.0));
        v->y = ((double) rand() / (RAND_MAX + 1.0));
        v->color = __INT_MAX__;
        v2->x = ((double) rand() / (RAND_MAX + 1.0));
        v2->y = ((double) rand() / (RAND_MAX + 1.0));
        v2->color = __INT_MAX__;
        v->id = idCount;
        v2->id = idCount;
        idCount++;
        //file << v->x << " " << v->y << endl;
        verts.push_back(v);
        vertsCopy.push_back(v2);
    }

    sort(verts.begin(), verts.end(), ComparatorByX());//sort by x so we can sweep

    double distance = 0.0;
    double changeX = 0.0;
    double changeY = 0.0;
    double negChangeX = 0.0;

    
    for (int j = 0; j < verts.size()-1; j++)
    {
        
        Node * cur = verts.at(j);
        Node* cur2 = vertsCopy.at(j);
        Node * next = verts.at(j+1);
        Node * next2 = vertsCopy.at(j+1);

        changeX = next->x - cur->x;//x dist
        
        for(int x = j+1; x < verts.size()-1;x++)
        {  
            changeX = next->x - cur->x;
            if (changeX < r)
            {
                changeX *= changeX;
                changeY = next->y - cur->y;//y dist
                changeY *= changeY; 
                distance = sqrt(changeX + changeY);//euclid dist
                if(distance <= r)
                {
                    cur->edges.push_back(next);//add to cur edge list
                    cur2->edges.push_back(next2);
                    // file << cur->x << " " << cur->y << " " << j << endl;//write to file
                    cur->degree++;
                    cur2->degree++;
                    next->edges.push_back(cur);
                    next2->edges.push_back(cur2);
                    // file << next->x << " " << next->y << " " << x << endl;//write the rest of the edge to file for plotting
                    next->degree++;
                    next2->degree++;
                    // file << endl;//getting ready for next edge
                }
            }
            else
            {
                break;
            }
            next = verts.at(x+1);
            next2 = vertsCopy.at(x+1);
        }
    }





    FILE *pipe = popen("gnuplot -persist", "w");
    fprintf(pipe, "set terminal png\n");
    fprintf(pipe, "set output 'out.png'\n");
    fprintf(pipe, "set xlabel 'X'\n");
    fprintf(pipe, "set ylabel 'Y'\n");
    
    // //plot verts and edges
    // // fprintf(pipe, "plot \
    // //                 'file.dat' using 1:2       with lines lc rgb 'black' lw 0.0000001 notitle,\
    // //                 'file.dat' using 1:2:(0.000001) with circles fill solid lc rgb 'black' notitle\n");

    // //plot verts
    // fprintf(pipe, "plot 'in.txt' using 1:2:(0.000001) with circles fill solid lc rgb 'black' notitle\n");
    

    int total = verts.at(0)->degree;
    int min = __INT_MAX__;
    int max = 0;
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

    stack<Node*> SLVO;
    int min2 = min;
    while(!vertsCopy.empty())
    {
        for(int i = 0; i < vertsCopy.size(); i++)
        {
            if(vertsCopy.at(i)->degree <= min2)
            {
                SLVO.push(vertsCopy.at(i));
                vertsCopy.erase(vertsCopy.begin()+i);
                min2 = max;
                for(int j = 0; j < vertsCopy.size();j++)
                {
                    if(vertsCopy.at(j)->degree < min2){
                        min2 = vertsCopy.at(j)->degree;
                    }
                }
            }
        }
    }

    int maxDegWhenDeleted = 0;
    bool available[numVerts];
    for(int k = 0; k < numVerts; k++)
    {
        available[k] = 0;
    }
    int maxColor = 0;
    SLVO.top()->color = 0;
    SLVO.top()->edges.clear();
    vertsCopy.push_back(SLVO.top());
    SLVO.pop();
    while(!SLVO.empty())
    {
        maxDegWhenDeleted = SLVO.top()->degree;
        for(int i = 0; i < SLVO.top()->edges.size(); i++)
        {
            if(SLVO.top()->edges.at(i)->color != __INT_MAX__)
            {
                available[SLVO.top()->edges.at(i)->color] = 1;
            }
        }

        int color;
        for(color = 0; color < numVerts; color++)
        {
            if(available[color] == 0)
            {
                break;
            }
        }

        SLVO.top()->color = color; 
        if(color > maxColor)
        {
            maxColor = color;
        }

        for(int i = 0; i < SLVO.top()->edges.size(); i++)
        {
            if(SLVO.top()->edges.at(i)->color != __INT_MAX__)
            {
                available[SLVO.top()->edges.at(i)->color] = 0;
            }
        }
        // SLVO.top()->edges.clear();
        vertsCopy.push_back(SLVO.top());
        SLVO.pop();
        
        
    }

    vector<int> colorSize;
    vector<int> colorCopy;
    for(int r = 0; r <= maxColor; r++)
    {
        colorSize.push_back(0);
        colorCopy.push_back(0);
    }

    for(int y = 0; y < vertsCopy.size(); y++)
    {
        colorSize.at(vertsCopy.at(y)->color)++;
        colorCopy.at(vertsCopy.at(y)->color)++;
    }
    int clmax = 0;
    int clmax2 = 0;
    int clmax3 = 0;
    int clmax4 = 0;
    sort(colorCopy.begin(),colorCopy.end());
    clmax = colorCopy.back();
    clmax2 = colorCopy.end()[-2];
    clmax3 = colorCopy.end()[-3];
    clmax4 = colorCopy.end()[-4];
    // for(int g = 0; g < colorSize.size(); g++)
    // {
    //     if(colorSize.at(g) > clmax)
    //     {
    //         clmax = colorSize.at(g);
    //     }
    // }

    // for(int c = 0; c < maxColor; c++)
    // {
    //     file << c << " " << colorSize.at(c) << endl;
    // }

    int indy1 = 0;
    int indy2 = 0;
    int indy3 = 0;
    int indy4 = 0;
    
    for(int e = 0; e < maxColor; e++)
    {
        if(colorSize.at(e) == clmax)
        {
            indy1 = e;
        }
        else if(colorSize.at(e) == clmax2)
        {
            indy2 = e;
        }
        else if(colorSize.at(e) == clmax3)
        {
            indy3 = e;
        }
        else if(colorSize.at(e) == clmax4)
        {
            indy4 = e;
        }
    }
    vector<Node*> independentOne;
    vector<Node*> independentTwo;
    vector<Node*> independentThree;
    vector<Node*> independentFour;

    for(int a = 0; a < vertsCopy.size(); a++)
    {
        if(vertsCopy.at(a)->color == indy1)
        {
            independentOne.push_back(vertsCopy.at(a));
        }
        if(vertsCopy.at(a)->color == indy2)
        {
            independentTwo.push_back(vertsCopy.at(a));
        }
        if(vertsCopy.at(a)->color == indy3)
        {
            independentThree.push_back(vertsCopy.at(a));
        }
        if(vertsCopy.at(a)->color == indy4)
        {
            independentFour.push_back(vertsCopy.at(a));
        }
    }

    int edgeAmount1 = 0;
    int edgeAmount2 = 0;
    int edgeAmount3 = 0;
    int edgeAmount4 = 0;

    int sumEdges1 = 0;
    int sumEdges2 = 0;
    int sumEdges3 = 0;
    int sumEdges4 = 0;
    int sumEdges5 = 0;
    int sumEdges6 = 0;
    vector<Node*> bipartiteOne;
    vector<Node*> bipartiteTwo;
    vector<Node*> bipartiteThree;
    vector<Node*> bipartiteFour;
    vector<Node*> bipartiteFive;
    vector<Node*> bipartiteSix;
    set<int> b1N;//bipartiteOne neighbors, etc.
    set<int> b2N;
    set<int> b3N;
    set<int> b4N;
    set<int> b5N;
    set<int> b6N;
    
    for(int i = 0; i < independentOne.size(); i++)
    {
        edgeAmount1 += independentOne.at(i)->edges.size()/2;
        bipartiteOne.push_back(independentOne.at(i));
        bipartiteThree.push_back(independentOne.at(i));
        bipartiteFour.push_back(independentOne.at(i));
    }
    for(int i = 0; i < independentTwo.size(); i++)
    {
        edgeAmount2 += independentTwo.at(i)->edges.size()/2;
        bipartiteOne.push_back(independentTwo.at(i));
        bipartiteFive.push_back(independentTwo.at(i));
        bipartiteSix.push_back(independentTwo.at(i));
    }
    for(int i = 0; i < independentThree.size(); i++)
    {
        edgeAmount3 += independentThree.at(i)->edges.size()/2;
        bipartiteTwo.push_back(independentThree.at(i));
        bipartiteThree.push_back(independentThree.at(i));
        bipartiteFive.push_back(independentThree.at(i));

    }
    for(int i = 0; i < independentFour.size(); i++)
    {
        edgeAmount4 += independentFour.at(i)->edges.size()/2;
        bipartiteTwo.push_back(independentFour.at(i));
        bipartiteFour.push_back(independentFour.at(i));
        bipartiteSix.push_back(independentFour.at(i));
    }

    int county = 0;
    for(int i = 0; i < bipartiteOne.size(); i++)
    {
        for(int j = 0; j < bipartiteOne.at(i)->edges.size(); j++)
        {
            b1N.insert(bipartiteOne.at(i)->edges.at(j)->id);
        }
    }
    for(int i = 0; i < bipartiteTwo.size(); i++)
    {
        for(int j = 0; j < bipartiteTwo.at(i)->edges.size(); j++)
        {
            b2N.insert(bipartiteTwo.at(i)->edges.at(j)->id);
        }
    }
    for(int i = 0; i < bipartiteThree.size(); i++)
    {
        for(int j = 0; j < bipartiteThree.at(i)->edges.size(); j++)
        {
            b3N.insert(bipartiteThree.at(i)->edges.at(j)->id);
        }
    }
    for(int i = 0; i < bipartiteFour.size(); i++)
    {
        for(int j = 0; j < bipartiteFour.at(i)->edges.size(); j++)
        {
            b4N.insert(bipartiteFour.at(i)->edges.at(j)->id);
        }
    }
    for(int i = 0; i < bipartiteFive.size(); i++)
    {
        for(int j = 0; j < bipartiteFive.at(i)->edges.size(); j++)
        {
            b5N.insert(bipartiteFive.at(i)->edges.at(j)->id);
        }
    }
    for(int i = 0; i < bipartiteSix.size(); i++)
    {
        for(int j = 0; j < bipartiteSix.at(i)->edges.size(); j++)
        {
            b6N.insert(bipartiteSix.at(i)->edges.at(j)->id);
        }
    }

    double dom1 = 0.0;
    double dom2 = 0.0;
    double dom3 = 0.0;
    double dom4 = 0.0;
    double dom5 = 0.0;
    double dom6 = 0.0;

    int order1 = 0;
    int order2 = 0;
    int order3 = 0;
    int order4 = 0;
    int order5 = 0;
    int order6 = 0;

    order1 = bipartiteOne.size();
    order2 = bipartiteTwo.size();
    order3 = bipartiteThree.size();
    order4 = bipartiteFour.size();
    order5 = bipartiteFive.size();
    order6 = bipartiteSix.size();

    dom1 = b1N.size();
    dom2 = b2N.size();
    dom3 = b3N.size();
    dom4 = b4N.size();
    dom5 = b5N.size();
    dom6 = b6N.size();

    dom1 /= numVerts;
    dom2 /= numVerts;
    dom3 /= numVerts;
    dom4 /= numVerts;
    dom5 /= numVerts;
    dom6 /= numVerts;

    cout << "order1: " << order1 << endl;
    cout << "order2: " << order2 << endl;
    cout << "order3: " << order3 << endl;
    cout << "order4: " << order4 << endl;
    cout << "order5: " << order5 << endl;
    cout << "order6: " << order6 << endl;

    cout << "dom1: " << dom1 << endl;
    cout << "dom2: " << dom2 << endl;
    cout << "dom3: " << dom3 << endl;
    cout << "dom4: " << dom4 << endl;
    cout << "dom5: " << dom5 << endl;
    cout << "dom6: " << dom6 << endl;

    sumEdges1 = edgeAmount1+edgeAmount2;
    sumEdges2 = edgeAmount3+edgeAmount4;
    sumEdges3 = edgeAmount1+edgeAmount3;
    sumEdges4 = edgeAmount1+edgeAmount4;
    sumEdges5 = edgeAmount2+edgeAmount3;
    sumEdges6 = edgeAmount2+edgeAmount4;

    vector<int> compareEdges;
    vector<double> doms;

    doms.push_back(dom1);
    doms.push_back(dom2);
    doms.push_back(dom3);
    doms.push_back(dom4);
    doms.push_back(dom5);
    doms.push_back(dom6);

    compareEdges.push_back(sumEdges1);
    compareEdges.push_back(sumEdges2);
    compareEdges.push_back(sumEdges3);
    compareEdges.push_back(sumEdges4);
    compareEdges.push_back(sumEdges5);
    compareEdges.push_back(sumEdges6);

    for(int i = 0; i < compareEdges.size(); i++)
    {
        cout << "compareEdges" << i + 1 << ": " <<  compareEdges.at(i) << endl;
    }

    sort(doms.begin(),doms.end());

    cout << "Total dom for first bipartite: " << doms.back() << endl;
    cout << "Total dom for second bipartite: " << doms.end()[-2] << endl;

    cout << "colors1: " << indy1 << " & " << indy2 << endl;
    cout << "colors2: " << indy3 << " & " << indy4 << endl;
    cout << "colors3: " << indy1 << " & " << indy3 << endl;
    cout << "colors4: " << indy1 << " & " << indy4 << endl;
    cout << "colors5: " << indy2 << " & " << indy3 << endl;
    cout << "colors6: " << indy2 << " & " << indy4 << endl;

    // // for(int i = 0; i < bipartiteSix.size(); i++)
    // // {
    // //     for(int j = 0; j < bipartiteSix.at(i)->edges.size(); j++)
    // //     {
    // //         file << bipartiteSix.at(i)->x << " " << bipartiteSix.at(i)->y << " " << bipartiteSix.at(i)->id << endl;
    // //         file << bipartiteSix.at(i)->edges.at(j)->x << " " << bipartiteSix.at(i)->edges.at(j)->y << " " << bipartiteSix.at(i)->edges.at(j)->id << endl;
    // //         file << endl;

    // //     }
        
    // // }
    // // file.close();
    // // file.open("in2.txt", ios::out | ios::in);
    // for(int i = 0; i < bipartiteThree.size(); i++)
    // {
    //     for(int j = 0; j < bipartiteThree.at(i)->edges.size(); j++)
    //     {
    //         file << bipartiteThree.at(i)->x << " " << bipartiteThree.at(i)->y << " " << bipartiteThree.at(i)->id << endl;
    //         file << bipartiteThree.at(i)->edges.at(j)->x << " " << bipartiteThree.at(i)->edges.at(j)->y << " " << bipartiteThree.at(i)->edges.at(j)->id << endl;
    //         file << endl;

    //     }
        
    // }
    
    totalEdges = totalEdges/2; 

    fprintf(pipe, "set xrange [0:1]\n");
    fprintf(pipe, "set yrange [0:1]\n");
    
    // fprintf(pipe, "plot \
    //                 'in.txt' using 1:2       with lines lc rgb 'black' lw 0.0000000000000000000000000001 notitle,\
    //                 'in.txt' using 1:2:(0.0035) with circles fill solid lc rgb 'black' notitle\n");
    
    fprintf(pipe, "plot 'in.txt' using 1:2:(0.000005) with circles fill solid lc rgb 'black' notitle\n");
    file.close();

    // fprintf(pipe, "plot 'file.dat' using 1:2:(0.5) with circles fill solid lc rgb 'black' notitle,\
    //                'file.dat' using 1:2 with lines lc rgb 'black lw .25; \n");
    fclose(pipe);
    time_t endTimer;
    time(&endTimer);

    cout << "Max degree: " << max << endl;
    cout << "Min degree: " << min << endl;
    double density = total/numVerts;
    cout << "Average degree: " << density << endl;
    cout << "Total edges: " << totalEdges << endl;
    cout << "Number of colors: " << maxColor+1 << endl;
    cout << "Max color set size: " << colorSize.at(maxColor) << endl;
    cout << "Terminal Clique Order: " << clmax << endl;
    double seconds = endTimer - startTimer;
    cout << "Time taken to generate graph: " << seconds << " seconds" << endl;  


}

