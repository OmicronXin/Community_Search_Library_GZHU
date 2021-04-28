/*
Copyright (c++) of 2020 by Xintong Wang
topic: find the kmax of the truss
time: 2021/1/11
*/
#include<fstream>
#include<iostream>
#include<vector>
#include<ctime>
#include<algorithm>
#include<omp.h>
#include<assert.h>
#include<mutex>
#include<unordered_set>
#include<unordered_map>
#include<string>
using namespace std;

#define num_of_thread 2
typedef vector<vector<unsigned> > G_ds;

typedef struct {
    unsigned u;
    unsigned v;
}Edge;

typedef struct {
    G_ds g;//stored as IDs not names
    unsigned n;
    unsigned m;
    unsigned b; //# of boundary edges
    unsigned triangles;
    unsigned triplets;
    unordered_map<unsigned,unsigned> ID2name,name2ID;
}graph;

inline char nc(){
    ///*
  static char buf[1000000],*p1=buf,*p2=buf;
  if (p1==p2) { p2=(p1=buf)+fread(buf,1,1000000,stdin); if (p1==p2) return EOF; }
  return *p1++;
    //*/return getchar();
}

inline int read(unsigned &x){
  char c=nc();unsigned b=1;
  if (c=='\377') return 0;
  for (;!(c>='0' && c<='9');c=nc()) if (c=='-') b=-1;   //为啥不能去掉
  for (x=0;c>='0' && c<='9';x=x*10+c-'0',c=nc()); x*=b;
  return 1;
}

inline void InFile(graph &G,const char* path){
    unsigned pre    = 999;//顶点的name是从1开始的
    unsigned Index = 0;
    unsigned m=0;
	FILE* fp=freopen(path,"r",stdin);
    while(1){
        unsigned ver1 = 0,ver2 = 0;
        int t=read(ver1);
        if (t==0) break;
        t=read(ver2);

        if(ver1 != pre){
            vector<unsigned> edge;
            edge.emplace_back(ver2);
            G.name2ID[ver1]=Index;
            G.ID2name[Index++]=ver1;
            G.g.emplace_back(edge);
            pre = ver1;
        }
        else{
            G.g[Index-1].emplace_back(ver2);
        }
        ++m;
    }
	fclose(stdin);
#pragma omp parallel for
	for(int i=0;i<G.g.size();i++){
        for(int j=0;j<G.g[i].size();j++){
            G.g[i][j]=G.name2ID[G.g[i][j]];
        }
	}
	G.m=m;
	G.n=G.g.size();

}

void calculate_num_of_boundary_edges(graph& original,graph& subgraph){
    unsigned* b_temp=new unsigned[100]();

    unsigned b=0;
//#pragma omp parallel for
    for(int i=0;i<subgraph.g.size();i++){
        unsigned u=subgraph.ID2name[i];
        unsigned u_ID_original=original.name2ID[u];
        b_temp[omp_get_thread_num()]+=original.g[u_ID_original].size()-subgraph.g[i].size();

	}
    for(int i=0;i<omp_get_num_threads();i++){
        b+=b_temp[i];
	}
	subgraph.b=b;
}

void calculate_num_of_triangles_parallel(graph& g) {
    unsigned* num_of_tri_private=new unsigned[omp_get_num_threads()];
    for(int i=0;i<omp_get_num_threads();i++) num_of_tri_private[i]=0;
    //#pragma omp parallel for
	for (unsigned i=0;i<g.n;i++){
	    for(unsigned j=0;j<g.g[i].size();j++){
            for(unsigned k=0;k<g.g[g.g[i][j]].size();k++){
                unsigned v=g.g[g.g[i][j]][k];
                if(find(g.g[i].begin(),g.g[i].end(),v)!=g.g[i].end()){
                    num_of_tri_private[omp_get_thread_num()]++;
                }
            }
	    }
	}
	unsigned sum=0;
	for (int i=0;i<omp_get_num_threads();i++) {
            sum+=num_of_tri_private[i];
    }
	g.triangles=sum/6;


}


void print_graph(graph G){
    for(int i=0;i<G.g.size();i++){
        for(int j=0;j<G.g[i].size();j++){
            printf("%d\t%d\n",G.ID2name[i],G.ID2name[G.g[i][j]]);
        }
	}
}

void calculate_num_of_triplets(graph& g){
    unsigned pairs=0;
    for (unsigned i=0;i<g.n;i++){
        pairs+=g.g[i].size()*(g.g[i].size()-1)/2;
    }
    g.triplets=pairs/2;
}

void calculate_scores(graph& g,graph& original){
    calculate_num_of_boundary_edges(original,g);
    calculate_num_of_triangles_parallel(g);
    calculate_num_of_triplets(g);
    printf("Num of edges=%d\nNum of Vertices=%d\nAverage degree=%f\nInternal Density=%f\nCut ratio=%f\nConductance=%f\nClustering coefficient=%f\n", g.m,g.n,
    g.m/(float)g.n,g.m/(float)(g.n*(g.n-1)),1-g.b/(float)(g.n*(original.n-g.n)),1-g.b/(float)(g.m+g.b),g.triangles/(float)g.triplets);

}

void calculate_scores_original(graph& g){
    calculate_num_of_triangles_parallel(g);
    calculate_num_of_triplets(g);
    printf("Num of edges=%d\nNum of Vertices=%d\nAverage degree=%f\nInternal Density=%f\nClustering coefficient=%f\n", g.m,g.n,g.m/(float)g.n,g.m/(float)(g.n*(g.n-1)),g.triangles/(float)g.triplets);

}
int main(int argc, char** argv){
    string folder_path=argv[2];
    string truss_path,densest_subgraph_path,maximal_clique_path,kmaxcore_path;
    densest_subgraph_path=folder_path+"/densest_subgraph.txt";
    truss_path=folder_path+"/kmaxtruss.txt";
    maximal_clique_path=folder_path+"/maximal_clique.txt";
    kmaxcore_path=folder_path+"/kmaxcore.txt";
    printf("Dataset=%s\n",folder_path.c_str());
    graph original,truss,densest_subgraph,maximal_clique,kmaxcore;
    InFile(original,argv[1]);
    InFile(densest_subgraph,densest_subgraph_path.c_str());
    //InFile(maximal_clique,maximal_clique_path.c_str());
    InFile(truss,truss_path.c_str());
    InFile(kmaxcore,kmaxcore_path.c_str());
    printf("--------Original Graph--------\n");
//    printf("m=%d,n=%d, m/n=%f\n",original.m,original.n,original.m/(float)original.n);
    calculate_scores_original(original);
    printf("--------Kmax Truss--------\n");
    calculate_scores(truss,original);
    printf("--------Densest Subgraph--------\n");
    calculate_scores(densest_subgraph,original);
    //printf("--------Maximal Clique--------\n");
    //calculate_scores(maximal_clique,original);
    printf("--------Kmax Core--------\n");
    calculate_scores(kmaxcore,original);
    return 0;
}


