#include <iostream>
#include <vector>
#include <queue>          // std::priority_queue
#include <iomanip>
#include "IO.h"
#include "HASH.h"
#include <fstream>
#include<ctime>
#include<algorithm>
#include<omp.h>
#include<assert.h>
#include "khash.h"
#include<mutex>
#include<unordered_map>
#include<unordered_set>
#define num_of_thread 2
mutex mtx;
typedef struct {
    unsigned u;
    unsigned v;
}Edge;

KHASH_MAP_INIT_INT(32,unsigned)

typedef vector<vector<unsigned >>  G_ds;
typedef khash_t(32) NeighborsWithHeads;
typedef vector<khash_t(32)*> G_tr_ds;
typedef vector<vector<unsigned>> CommonNeigh;
khash_t(32) *name2id = kh_init(32);
int name2id_ret;
//int MAXDEG=0;
vector<vector<unsigned>> comneigh;

bool IF_WEIGHTED_DATASET=0;
khiter_t k;


inline char nc(){
    ///*
  static char buf[1000000],*p1=buf,*p2=buf;
  if (p1==p2) { p2=(p1=buf)+fread(buf,1,1000000,stdin); if (p1==p2) return EOF; }
  return *p1++;
    //*/return getchar();
}

inline int read(int &x){
  char c=nc();int b=1;
  if (c=='\377') return 0;
  for (;!(c>='0' && c<='9');c=nc()) if (c=='\377') return 0;   //为啥不能去掉
  for (x=0;c>='0' && c<='9';x=x*10+c-'0',c=nc()); x*=b;
  return 1;
}

//inline void InFile(G_ds &G,char* path){
//    unsigned pre    = 999;//顶点的name是从1开始的
//    unsigned Index = 0;
//	FILE* fp=freopen(path,"r",stdin);
//    while(1){
//        unsigned ver1 = 0,ver2 = 0,ver3 = 0;
//        int t=read(ver2);
//        if (t==0) break;
//        t=read(ver1);t=read(ver3);
//
////        int t=read(ver1);
////        if (t==0) break;
////        t=read(ver2);
//
//        if(ver1 != pre){
//            vector<unsigned> edge;
//            edge.emplace_back(ver2);
//            k = kh_put(32,name2id,ver1,&name2id_ret);
//            kh_value(name2id,k) = Index++;
//            G.emplace_back(edge);
//            pre = ver1;
//        }
//        else{
//            G[Index-1].emplace_back(ver2);
//        }
//    }
//	fclose(stdin);
//}

using namespace std;

/*
Compile: g++ CharikarPeel.cpp -o CharikarPeel -std=gnu++0x -O3
Demo: ./CharikarPeel.exe < toy.txt > toy.log
Charalampos E. Tsourakakis, babis@seas.harvard.edu

*/


// Charalampos E. Tsourakakis, babis@seas.harvard.edu
const unsigned MAXV = 10000000; //10^7 vertices, change it to whatever you like
const unsigned MAXE = 100000000; //10^8 edges
const int QUERYBUFFER = 6000000;



unsigned E, V;
int eList[MAXE+1][2]; // we load all edges on memoer
int degrees[MAXV+1];
int NumTriangles=0;

// remove multiple edges, self-loops
void SimplifyGraph()
{
  int E1 = 1;
  int multipleEdges=0;
  HASH::init();
  for(int i = 1; i <= E; ++i) {
    //printf("Testing edge (%d,%d)\n", eList[i][0], eList[i][1]);
    if(1 <= eList[i][0] && eList[i][0] <= V &&
         1 <= eList[i][1] && eList[i][1] <= V &&
         eList[i][0] != eList[i][1] &&
         HASH::insert(eList[i][0], eList[i][1])) {
      eList[E1][0] = eList[i][0];
      eList[E1][1] = eList[i][1];
	  degrees[eList[i][0]]++;
	  degrees[eList[i][1]]++;
      E1++;
    }
	else
		multipleEdges++; //printf("has appeared before\n");
  }
  E = E1-1;
  printf("number of edges in simple graph: %d\n", E);
  printf("number of multipleEdges: %d\n",multipleEdges);
}


unordered_map<int,int> ID2name,name2ID;
void GraphIn(const char* path) {
  int u, v,junk;
  int index=1;
  FILE* fp=freopen(path,"r",stdin);
  while(1){
    if(IF_WEIGHTED_DATASET==0){
        int t=read(u);
        t=read(v);
        if (t==0) break;
    }
    else{
        int t=read(v);
        t=read(u);
        t=read(junk);
        if (t==0) break;
    }

    if(ID2name.find(u)==ID2name.end()){
        ID2name[index]=u;
        name2ID[u]=index++;
        ++V;
    }
    ++E;

  }
  fclose(stdin);
  fp=freopen(path,"r",stdin);
  E/=2;
//  IO::init(path);
//  IO::read(V);
//  IO::read(E);

  printf("Number of vertices and edges (%d,%d)\n",V,E);
  for(int i = 1; i <= E;)
  {

    read(u);
    read(v);
    //store ID of vertices
    u=name2ID[u];
    v=name2ID[v];
    if(u>v) continue;
//	printf("(%d,%d)\n",u,v);
    eList[i][0] = u;
    eList[i][1] = v;
    degrees[u]++;
    degrees[v]++;
    i++;
  }
  fclose(stdin);
}

void PrintEdgeList()
{
   for(int i=1; i<=E; i++)
   {
        printf("(%d,%d)\n",eList[i][0],eList[i][1]);
   }
}

void PrintDegrees()
{
   printf("***************************\n");
   printf("Vertex id \t Degree\n");
   for(int i = 1; i <= V; i++)
      printf("%d \t %d\n",i, degrees[i]);
   printf("***************************\n");
}



//int MaximumDegree()
//{
//   for(int i = 1; i <= V; i++)
//    if(MAXDEG < degrees[i] )
//    	MAXDEG=degrees[i];
//  //printf("maximum degree is %d\n", MAXDEG);
//}


void graphinStdio() {
  scanf("%d%d", &V, &E);
  for(int i = 0; i < E; ++i) {
    scanf("%d%d", &eList[i][0], &eList[i][1]);
  }
}

void ElapsedTime(double startTime)
{
   printf("Elapsed time to read input: %f\n", (clock()-startTime)/CLOCKS_PER_SEC );
}

void ElapsedTime(double startTime, char* s)
{
   printf("Elapsed time for %s: %f\n", s, (clock()-startTime)/CLOCKS_PER_SEC );
}
///////////////adjacency list
int eStart[MAXV];
int mynext[MAXE];

const int NOEDGE = -1;

void BuildGraph() {
  for(int i = 1; i <= V; ++i) {
    eStart[i] = NOEDGE;
  }
  for(int i = 1; i <= E; ++i) {
    mynext[i] = eStart[eList[i][0]];
    eStart[eList[i][0]] = i;
  }
}




vector<int> AdjList[MAXV];

void BuildAdjList()
{
  for(int i = 1; i<=E; i++)
  {
       AdjList[ eList[i][0] ].push_back( eList[i][1] );
	   AdjList[ eList[i][1] ].push_back( eList[i][0] );
  }
}

// This function deletes a vertex from the graph that has the minimum degree

int permutation[MAXV+1]; //permutation[1] through permutation[V] contains the desired perm

double  EDGEDENSITY = -1;
int CharikarSize=-1;
double CharikarFe = -1;
double CharikarSizeFraction=-1.0;

double AvgDegreeVals[MAXV+1];
double AvgDegreeOptVals[MAXV+1];

int OPTIND;
vector<int> subgraph;
priority_queue< pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>> > q;
void PQCharikarPeel()
{

	EDGEDENSITY= 2.0*E/V;
	CharikarFe = (double) 2*E/(V*(V-1));
	CharikarSize=V;
	CharikarSizeFraction=1.0;
	double numedges=(double)E;
	double numvertices=(double)V;
	for (int i = 1; i <= V; ++i)
        q.push(make_pair(degrees[i], i));

	int c = q.top().second;
	int counter = 0;
	while (!q.empty())
	{
		int c = q.top().second;
        q.pop();
         if (degrees[c] < 0 )// == -1)
		{
            continue;
        }
		else
		{
			AvgDegreeOptVals[counter] = EDGEDENSITY;
			AvgDegreeVals[counter] = 2.0*numedges / (double) numvertices;
			permutation[counter++]=c;
		}
        numedges -= degrees[c];
        --numvertices;
        degrees[c] = -1;
		if (numvertices > 0) {
            double cur = 2.0*numedges / (double) numvertices;
            if (cur >= EDGEDENSITY ) {
                EDGEDENSITY = cur;
				CharikarFe = EDGEDENSITY/(numvertices-1);
				CharikarSize = numvertices;
				CharikarSizeFraction = numvertices/V;
                subgraph.clear();
				OPTIND = counter;
            }
        }
        subgraph.push_back(c);

		for (int i=0; i < AdjList[c].size();i++)
		{
			int w = AdjList[c][i];
			if( degrees[w]>0)
			{
				q.push(make_pair(--degrees[w], w));
			}
		}
		if( counter == V)
		    break;
	}


}



void PrintAdjList()
{
	for(int j = 1; j<=V; j++)
	{
	    cout<< "Neighbors of "<<j<<" [";
		for (int i=0; i<AdjList[j].size();i++)
		{
		cout << AdjList[j][i]<< " ";
		}
        cout<< "]" <<endl;

	}
}




void PrintPermutationStatistics()
{
   int breakline = 0;
   cout<<"Peeling permutation order"<<endl;
   cout<<"[";
   for(int i=0; i < V; i++)
   {
		cout<<" "<< permutation[i];
		if( ++breakline > 25 ) // entries per line
		{
		        cout<<endl;
				breakline = 0;
		}
   }
   cout<<"]"<<endl;
   /*
	breakline = 0;
   cout<<"Avg Degree values"<<endl;
   cout<<"[";
   for(int i=0; i < V; i++)
   {
		cout<<" "<< AvgDegreeVals[i];
		if( ++breakline > 25 ) // entries per line
		{
		        cout<<endl;
				breakline = 0;
		}
   }
   cout<<"]"<<endl;*/

  breakline = 0;
   cout<<"Optimal avg degree values"<<endl;
   cout<<"[";
   for(int i=0; i < V; i++)
   {
		cout<<" "<< AvgDegreeOptVals[i];
		if( ++breakline > 25 ) // entries per line
		{
		        cout<<endl;
				breakline = 0;
		}
   }
   cout<<"]"<<endl;

}



void printOptSubgraph()
{
   cout<<"[";
   for(int i=OPTIND; i < V; i++)
   {
		cout<<" "<< permutation[i];
   }
   cout<<"]"<<endl;

}


int main(int argc, char **argv)
{
  if(argc>2) IF_WEIGHTED_DATASET=1;
  double startTime = clock();
  GraphIn(argv[1]);
  ElapsedTime(startTime);
//  SimplifyGraph(); // besides making G simple, we compute the degree sequence here
//  PrintEdgeList();
  BuildGraph();
  BuildAdjList();
//  MaximumDegree(); // what is the max degree?
  startTime = clock();
  PQCharikarPeel();
  cout<<"********** CHARIKAR PEELING ***************"<<endl;
  ElapsedTime(startTime,"Peel Charikar");
  cout<<"Charikar's Results"<<endl;
  cout<<"n:"<<V<<endl;
  cout<<"m:"<<E<<endl;
  cout<<"Avg. degree 2m/n:"<<setprecision(3) << fixed <<(double)2.0*E/V<<endl;
  cout<<"Optimal Avg. Degree:"  <<setprecision(3) << fixed <<EDGEDENSITY<<endl;
  cout<<"Size:"  <<setprecision(3) << fixed <<CharikarSize<<endl;
  cout<<"Size/|V|:"  <<setprecision(3) << fixed <<CharikarSizeFraction<<endl;
  cout<<"fe:"  <<setprecision(3) << fixed <<CharikarFe<<endl;
  cout<<"*******************************************"<<endl;

  bool subgraphflag = false; //false
  if( subgraphflag )
  {
      cout<<"********** Optimal Subgraph *********************"<<endl;
      printOptSubgraph();
	  cout<<"*************************************************"<<endl;
  }

  bool statsflag = false; //false
  if( statsflag )
  {

	  cout<<"********** Permutation Statistics *********************"<<endl;
	  PrintPermutationStatistics();
	  cout<<"*************************************************"<<endl;
  }


//  cout<<"**********  Subgraph *********************"<<endl;
//  for(int i=1;i<subgraph.size();++i) printf("%d ",ID2name[subgraph[i]]);
//  cout<<endl;
//  cout<<"*************************************************"<<endl;


  vector<vector<int > > sub_adj(V+1);
  unordered_set<int> sub_vertices;
  for(int i=1;i<subgraph.size();++i) sub_vertices.insert(subgraph[i]);
  for(int i=1;i<=E;i++){
    if(sub_vertices.find(eList[i][0])!=sub_vertices.end()&&sub_vertices.find(eList[i][1])!=sub_vertices.end()){
        sub_adj[eList[i][0]].push_back(eList[i][1]);
        sub_adj[eList[i][1]].push_back(eList[i][0]);
    }
  }
  int deg_sum=0;
  for(int i=1;i<=V;i++) deg_sum+=sub_adj[i].size();
  printf("Number of edges in subgraph=%d\n",deg_sum);


  ofstream out("densest_subgraph.txt");

    for(int u=1;u<=V;++u) {
        for (auto v:sub_adj[u]){
//            cout<<ID2name[u]<<" "<<ID2name[v]<<endl;
            out<<ID2name[u]<<" "<<ID2name[v]<<endl;
        }
    }



    out.close();

//  printf("deg_sum/size=%f\n",deg_sum/CharikarSize);


  /* Alternative version, slower than then one with priority queues, kept it for sake of completeness
  startTime = clock();
  CharikarPeel();
  cout<<"********** CHARIKAR PEELING ***************"<<endl;
  ElapsedTime(startTime,"Peel Charikar");
  cout<<"Charikar's Results"<<endl;
  cout<<"n:"<<V<<endl;
  cout<<"m:"<<E<<endl;
  cout<<"Avg. degree 2m/n:"<<(double)2.0*E/V<<endl;
  cout<<"Optimal Avg. Degree:"  <<EDGEDENSITY<<endl;
  cout<<"Size:"  <<CharikarSize<<endl;
  cout<<"fe:"  <<CharikarFe<<endl;
  cout<<"*******************************************"<<endl;
  */
  return 0;
}

