#include<iostream>
#include<vector>
#include<ctime>
#include<algorithm>
#include<omp.h>
#include<assert.h>
#include<cstring>
#include<fcntl.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fstream>
#include<unordered_map>

using namespace std;

#define num_of_thread 8

struct G_ds{
    unsigned *h;
    unsigned *e;
    unsigned sz;
    unsigned es;
};

struct Edge{
    unsigned u;
    unsigned v;
};

vector<Edge> ID2ends;

const unsigned memory_node=10000000;
const unsigned memory_edge=3000000000;
const unsigned memory_node_id=100000000;


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

inline void InFile(G_ds &G,char* path){
    unsigned pre    = 999;//顶点的name是从1开始的
    unsigned ptr_node = 0;
    unsigned ptr_edge = 0;
	FILE* fp=freopen(path,"r",stdin);

	G.e =(unsigned *)malloc(memory_edge*sizeof(unsigned ));
    G.h =(unsigned*)malloc(memory_node*sizeof(unsigned));

    unsigned  *ver_to_ID =(unsigned *)malloc(memory_node_id*sizeof(unsigned ));

    while(1){
        unsigned ver1 = 0,ver2 = 0;
        int t=read(ver1);
        if (t==0) break;
        t=read(ver2);

        if(pre  != ver1){
            G.h[ptr_node] = ptr_edge;
            ver_to_ID[ver1] = ptr_node++;
            G.e[ptr_edge++] = ver2;
            pre = ver1;
        }else{
            G.e[ptr_edge++] = ver2;
        }

    }//while

        G.sz = ptr_node;
        G.es = ptr_edge;
        G.h[ptr_node] = ptr_edge;

       /***  name to ID***/
    #pragma omp parallel for schedule(static) num_threads(num_of_thread)
    for (int i = 0; i < G.es; i++)  {
         G.e[i] = ver_to_ID[G.e[i]];
    }

//    for(unsigned i=0;i<G.sz;++i){
//            cout<< "i = " << i<<endl;
//        for(unsigned j=G.h[i];j<G.h[i+1];++j){
//            cout<<" "<<G.e[j];
//        }
//        cout<<endl;
//    }

    free(ver_to_ID);
	fclose(stdin);
}



inline void core_parallel(G_ds &g,vector<int>& deg,unsigned todo,unsigned n) {

    int level = 1;
    while (todo > 0){
    #pragma omp parallel num_threads(num_of_thread)
    {
        //unsigned thread_num = omp_get_num_threads();
        // Size of cache line
        const unsigned BUFFER_SIZE = n / num_of_thread + 1;
        unsigned buff[BUFFER_SIZE];
        unsigned index = 0;

        #pragma omp for schedule(static)
        for (long i = 0; i < n; i++) {

            if (deg[i] == level) {
                buff[index] = i;
                index++;
            }
        }

/******************************scan pro **********************/
        unsigned start = 0;
        while(index > start){

            unsigned v = buff[start];
            ++start;
            //Check the adj list of vertex v

            for (unsigned i=g.h[v];i<g.h[v+1];i++)
            {

               unsigned u =  g.e[i];
                if (deg[u] > level) {
                    int du =  __sync_fetch_and_sub(&deg[u], 1);
                    if(du <= level )  __sync_fetch_and_add(&deg[u],1);
                    if (du == (level+1)) {
                        buff[index] = u;
                        index++;
                    }
                }
            }//for-end
        }//while-end
        #pragma omp barrier
        __sync_fetch_and_sub(&todo,index);
    }//  #pragma omp parallel
    ++level;
    }//while-end
    return;
}

//ParK to compute k core decompsitions in parallel
inline void ParK(G_ds &g, vector<int> &deg){


    unsigned todo = g.sz;
    unsigned n = todo;
   #pragma omp parallel for schedule(static) num_threads(num_of_thread)
    for (long i = 0; i < n; i++)  {
        //deg[i] = g[i].size();
        deg[i] = g.h[i+1]-g.h[i];
    }
    core_parallel(g,deg,todo,n);

//    for(unsigned i=0;i<g.sz;++i){
//        cout<<deg[i]<<" ";
//    }
}



inline  void core2truss(G_ds &G,vector<int>& deg,int coreness,G_ds &G_truss,G_ds &G_truss_direct){

     unsigned * deg_small = new unsigned [G.sz]();

    unsigned num_ver = G.sz;

    unsigned *ver_to_ID_tr=(unsigned*)malloc(memory_node_id*sizeof(unsigned));

   unsigned number_edge=0,number_node=0;

    G_truss.e=(unsigned*)malloc(memory_edge*sizeof(unsigned));
    G_truss.h=(unsigned*)malloc(memory_node*sizeof(unsigned));


    for(unsigned i=0,Index = 0;i<num_ver;++i){   //构造索引
        unsigned deg_h = deg[i];
        if( deg_h >=coreness ){

            ver_to_ID_tr[i] = Index++;
            deg_small[number_node] = deg[i];
            G_truss.h[number_node++]=number_edge;

            for (unsigned itt=G.h[i];itt<G.h[i+1];itt++){
                unsigned name = G.e[itt];
                if(deg[name]>= coreness){
                    G_truss.e[number_edge++]= name;
                }
            }//for
        }//if
    }//for


  #pragma omp parallel for schedule(static) num_threads(num_of_thread)
   for(unsigned i=0;i<number_edge;++i){
    G_truss.e[i] = ver_to_ID_tr[G_truss.e[i]];
   }

    G_truss.sz=number_node;
    G_truss.es=number_edge;
    G_truss.h[number_node] = number_edge;

    free(ver_to_ID_tr);

    // create the adj-structure of directed-graph
    G_truss_direct.e=(unsigned*)malloc(memory_edge*sizeof(unsigned));
    G_truss_direct.h=(unsigned*)malloc(memory_node*sizeof(unsigned));

    unsigned number_edge_s = 0,number_node_s = 0;

    for(unsigned i=0;i<G_truss.sz;++i){

        G_truss_direct.h[number_node_s++] = number_edge_s;
        for(unsigned j=G_truss.h[i];j<G_truss.h[i+1];++j){
            unsigned node_adj = G_truss.e[j];
            if(deg_small[node_adj] > deg_small[i] || deg_small[node_adj] == deg_small[i] &&  node_adj > i){
                G_truss_direct.e[number_edge_s++] = node_adj;
            }//if
        }//for end
    }//for end

    G_truss_direct.h[number_node_s] = number_edge_s;
    G_truss_direct.es = number_edge_s;
    G_truss_direct.sz = number_node_s;

     free(deg_small);

    return ;
}

inline void calculateSupport_parallel(G_ds& G,unsigned * ID2sup,unsigned * edge2id) {//calculate the support number on edges and return the max sup

    unsigned ID=0;

    //#pragma omp parallel for
    for(unsigned i=0;i<G.sz;i++){//iterate through vertices, i is the id of the first vertex
        for( unsigned j=G.h[i];j<G.h[i+1];++j){

            Edge tempEdge;
            tempEdge.u=i;
            tempEdge.v= G.e[j];
            ID2ends.emplace_back(tempEdge);
            edge2id[j] = ID++;
        }
    }

    unsigned **sup = new unsigned*[num_of_thread]; //开辟行
    for (int i = 0; i < num_of_thread; i++)
        sup[i] = new unsigned[ID](); //开辟列

#pragma omp parallel num_threads(num_of_thread)
    {

    unsigned short * thash = new unsigned short[G.sz]();

    #pragma omp for schedule(static)
	for (unsigned i=0;i<G.sz;i++){//iterate through vertices, i is the id of the first vertex
        int thread_num = omp_get_thread_num();
        for(unsigned j=G.h[i];j<G.h[i+1];++j){

            unsigned node_adj = G.e[j];

            unsigned a_size = G.h[i+1]-G.h[i];
            unsigned b_size = G.h[node_adj+1]-G.h[node_adj];

            if( a_size==0 || b_size==0) {continue;}

            unsigned ptr_small,ptr_big;
            if(a_size>b_size){
                ptr_big = i;
                ptr_small = node_adj;
            }else{
                ptr_big = node_adj;
                ptr_small = i;
            }

            for(unsigned k= G.h[ptr_big],num = 1;k<G.h[ptr_big+1];++k){
                thash[G.e[k]] = num++ ;
            }

            for(unsigned k = G.h[ptr_small],num = 1;k<G.h[ptr_small+1];++k,++num){
                unsigned node_w = G.e[k];
                if(thash[node_w] != 0 ) {

                    unsigned e1 = edge2id[j];//i->node_adj
                    unsigned e2 = edge2id[ G.h[ptr_big] + thash[node_w] - 1] ;//ptr_big ->node_w
                    unsigned e3 = edge2id[ G.h[ptr_small] + num - 1];   //ptr_samll -> node_w

                   ++ sup[thread_num][e1];
                   ++ sup[thread_num][e2];
                   ++ sup[thread_num][e3];
                }
            } //for

           for(unsigned k = G.h[ptr_big];k<G.h[ptr_big+1];++k){
                thash[G.e[k]] = 0 ;
            }
        }//for j
	}//for i

    delete [] thash;

}//pragma end

for(int i = 0;i<num_of_thread;++i){
    #pragma omp parallel for schedule(static) num_threads(num_of_thread)
    for (unsigned j= 0; j < ID;j++)  {
            ID2sup[j]+= sup[i][j];
    }
}

    //释放开辟的资源
for (int i = 0; i < num_of_thread; i++)
        delete[] sup[i];
    delete[] sup;

//cout<<"ID = "<<ID<<endl;
//	  for(unsigned i=0;i<ID;++i)
//        cout<<ID2sup[i]<<"  ";

}

inline void PKT_scan(long n, unsigned * ID2sup, int level, unsigned *curr, long *currTail, bool *InCurr) {
    // Size of cache line

    const long BUFFER_SIZE = 2048;

    unsigned buff[BUFFER_SIZE];
    long index = 0;

#pragma omp for schedule(static)
    for(long i = 0; i < n; i++) {
        if( ID2sup[i] == level ) {
            buff[index++] = i;
            InCurr[i] = true;

            if(index >= BUFFER_SIZE) {
                long tempIdx = __sync_fetch_and_add(currTail, BUFFER_SIZE);

                for(long j = 0; j < BUFFER_SIZE; j++) {
                    curr[tempIdx+j] = buff[j];
                }
                index = 0;
            }
        }
    }

    if(index > 0) {
        long tempIdx = __sync_fetch_and_add(currTail, index);

        for(long j = 0; j < index; j++) {
            curr[tempIdx+j] = buff[j];
        }
    }

#pragma omp barrier
}

//Process a sublevel in a level using intersection based approach
inline void PKT_processSubLevel_intersection(G_ds &G_truss, G_ds &G_truss_direct,unsigned *curr, bool *InCurr, long currTail, unsigned*  ID2sup,
    int level, unsigned *next, bool *InNext, long *nextTail, bool *processed,unsigned * edge2id) {

    //Size of cache line

    const long BUFFER_SIZE = 2048;

    unsigned buff[BUFFER_SIZE];
    long index = 0;

#pragma omp for schedule(static)
    for (long i = 0; i < currTail; i++) {

        //process edge <u,v>
        unsigned e1 = curr[i];

        unsigned u=  ID2ends[e1].u;
        unsigned v=  ID2ends[e1].v;

       // for(vector<unsigned>::iterator it=comneigh[e1].begin();it != comneigh[e1].end();++it){

        unsigned u_size = G_truss.h[u+1] - G_truss.h[u];
        unsigned v_size = G_truss.h[v+1] - G_truss.h[v];

        bool * thash =  new bool [G_truss.sz]();
        unsigned u2v;

        if(u_size > v_size){
            for(unsigned i = G_truss.h[u];i<G_truss.h[u+1];++i){
                thash[G_truss.e[i]] = true;
            }
            u2v = v;
        }
        else{
            for(unsigned i = G_truss.h[v];i<G_truss.h[v+1];++i){
                thash[G_truss.e[i]] = true;
            }

            u2v = u;
        }


        for(unsigned j = G_truss.h[u2v];j<G_truss.h[u2v+1];j++){

            if(!thash[G_truss.e[j]])  { continue;}

            unsigned w = G_truss.e[j] ;

            unsigned te1 = u<w?u:w;
            unsigned te2 = u<w?w:u;
            unsigned e2 = edge2id[ lower_bound(G_truss_direct.e +  G_truss_direct.h[te1] ,G_truss_direct.e + G_truss_direct.h[te1+1] ,te2) - G_truss_direct.e];

            te1 = v<w?v:w;
            te2 = v<w?w:v;
            unsigned e3 = edge2id[ lower_bound(G_truss_direct.e +  G_truss_direct.h[te1] ,G_truss_direct.e + G_truss_direct.h[te1+1] ,te2) - G_truss_direct.e];

            if( processed[e2]  || processed[e3] )  continue;

                //Decrease support of both e2 and e3
                if( ID2sup[e2] > level && ID2sup[e3] > level) {

                    //Process e2
                    int supE2 = __sync_fetch_and_sub( &ID2sup[e2], 1);
                    if( supE2 == (level+1) ) {
                        buff[index++] = e2;
                        InNext[e2] = true;
                    }

                    if( supE2 <= level ) {
                        __sync_fetch_and_add(&ID2sup[e2],1);
                    }

                    if( index >= BUFFER_SIZE) {
                            long tempIdx = __sync_fetch_and_add(nextTail, BUFFER_SIZE);

                            for(long bufIdx = 0; bufIdx < BUFFER_SIZE; bufIdx++)
                                    next [tempIdx + bufIdx] = buff[bufIdx];
                            index = 0;
                    }

                    //Process e3
                    int supE3 = __sync_fetch_and_sub(&ID2sup[e3], 1);

                    if( supE3 == (level +1) ) {
                        buff[index++] = e3;
                        InNext[e3] = true;
                    }

                    if(supE3 <= level ) {
                        __sync_fetch_and_add(&ID2sup[e3],1);
                    }

                    if( index >= BUFFER_SIZE) {
                            long tempIdx = __sync_fetch_and_add(nextTail, BUFFER_SIZE);

                            for(long bufIdx = 0; bufIdx < BUFFER_SIZE; bufIdx++)
                                next [tempIdx + bufIdx] = buff[bufIdx];
                            index = 0;
                    }
		    } //Decrease support of both e2 and e3

		    else if(ID2sup[e2] > level ) {
                if( e1 < e3 && InCurr[e3] || !InCurr[e3]) {

                    int supE2 = __sync_fetch_and_sub(&ID2sup[e2], 1);
                    if( supE2 == (level+1) ) {
                            buff[index++] = e2;
                            InNext[e2] = true;
                     }

                    if( supE2 <= level ) {
                            __sync_fetch_and_add(&ID2sup[e2],1);
                    }

                    if( index >= BUFFER_SIZE) {
                            long tempIdx = __sync_fetch_and_add(nextTail, BUFFER_SIZE);

                            for(long bufIdx = 0; bufIdx < BUFFER_SIZE; bufIdx++)
                                next [tempIdx + bufIdx] = buff[bufIdx];
                            index = 0;
                    }
                }
		    }//if D2sup[e2] > level

		    else if(ID2sup[e3] > level) {
                //process e3 only if e1 < e2
                if( e1 < e2 && InCurr[e2]  || !InCurr[e2] ) {
                            int supE3 = __sync_fetch_and_sub(&ID2sup[e3], 1);

                            if( supE3 == (level +1) ) {
                                buff[index++] = e3;
                                InNext[e3] = true;
                            }

                            if(supE3 <= level ) {
                                __sync_fetch_and_add(&ID2sup[e3],1);
                            }

                            if( index >= BUFFER_SIZE) {
                                long tempIdx = __sync_fetch_and_add(nextTail, BUFFER_SIZE);

                                for(long bufIdx = 0; bufIdx < BUFFER_SIZE; bufIdx++)
                                    next [tempIdx + bufIdx] = buff[bufIdx];
                                index = 0;
                            }
                }
		    }//if ID2sup[e3] > level

        }//for comneigh
         delete[] thash;
    }//for currTail

    if (index > 0) {
        long tempIdx =  __sync_fetch_and_add(nextTail, index);;
        for (long bufIdx = 0; bufIdx < index; bufIdx++)
            next [tempIdx + bufIdx] = buff[bufIdx];
    }

#pragma omp barrier

#pragma omp for schedule(static)
    for (long i = 0; i < currTail; i++) {
        unsigned e = curr[i];

	processed[e] = true;
	InCurr[e] = false;
    }

#pragma omp barrier
}

inline void k_turss(G_ds &G_truss,G_ds &G_truss_direct, unsigned * ID2sup,unsigned &result0, unsigned &result1,unsigned * edge2id){

    unsigned n = G_truss_direct.es;

    bool *processed = (bool *)malloc( n * sizeof(bool) );

    long currTail = 0;
    long nextTail = 0;

    unsigned *curr = (unsigned *)malloc( n * sizeof(unsigned) );
    bool *InCurr = (bool *)malloc( n * sizeof(bool) );
    unsigned *next = (unsigned *)malloc( n * sizeof(unsigned) );
    bool *InNext = (bool *)malloc( n * sizeof(bool) );

    unsigned sum=0;
    unsigned level_result = 0;

#pragma omp parallel num_threads(num_of_thread)
{
    int tid = omp_get_thread_num();

    #pragma omp for schedule(static)
    for( unsigned e = 0; e < n; e++ ) {
        //Initialize processed array with false
        processed[ e ] = false;

        InCurr[e] = false;
        InNext[e] = false;
    }

    unsigned todo  = n;
    int level =0;

    while(todo >0){
        sum = todo;
        PKT_scan(n, ID2sup, level, curr, &currTail, InCurr);

    	while( currTail > 0 ) {
            todo = todo - currTail;

            PKT_processSubLevel_intersection(G_truss, G_truss_direct,curr, InCurr, currTail, ID2sup, level, next, InNext, &nextTail, processed,edge2id);
            if( tid == 0 ) {
                unsigned *tempCurr = curr;
                curr = next;
                next = tempCurr;
                bool *tempInCurr = InCurr;
                InCurr = InNext;
                InNext = tempInCurr;
                currTail = nextTail;
                nextTail = 0;
                level_result = level;
            }

    #pragma omp barrier
    }

	level = level + 1;
#pragma omp barrier
    }

}  //End of parallel region

    //Free memory
    free( next );
    free( InNext );
    free( curr );
    free( InCurr );
    free( processed );


    //unsigned level = *max_element(ID2sup,ID2sup + G_truss_direct*sizeof(unsigned) );

    result0 = level_result+2;
    result1 = sum;
}

void out_fun_core( G_ds &G,vector<int>& deg,int coreness){

    char filename[] = "kmaxcore.csv";
    ofstream fout(filename);

    int num_ver = G.sz ;

    for(unsigned i=0;i<num_ver;++i){
        if(deg[i] >= coreness){
            fout<<i;
            for(int j = G.h[i];j<G.h[i+1];++j){
                fout<<" "<<G.e[j];
            }
            fout<<"\n";
        }
    }

    fout.close();
    return ;
}


void out_fun_truss(unsigned * ID2sup, unsigned num,unsigned  result0){

    unordered_map<unsigned,vector<unsigned>> result;

    unsigned level = result0 - 2;
    for(unsigned i = 0;i< num;++i){
        if(ID2sup[i] ==  level){
            unsigned ver1 = ID2ends[i].u;
            unsigned ver2 = ID2ends[i].v;

            unordered_map<unsigned,vector<unsigned>> :: iterator iter1 =  result.find(ver1) ;
            if(iter1 == result.end()){
                vector<unsigned> temp;

                result. insert(pair<unsigned, vector<unsigned>> (ver1, temp));
                result.find(ver1)->second.emplace_back(ver2);

            }else{
                iter1->second.emplace_back(ver2);
            }

            unordered_map<unsigned,vector<unsigned>> :: iterator iter2 =  result.find(ver2) ;

            if(iter2 == result.end()){
                vector<unsigned> temp;

                result. insert(pair<unsigned, vector<unsigned>> (ver2, temp));
                result.find(ver2)->second.emplace_back(ver1);

            }else{
                iter2->second.emplace_back(ver1);
            }
        }
    }//for

    char filename[] = "kmaxtruss.csv";
    ofstream fout1(filename);

    for( unordered_map<unsigned,vector<unsigned>> :: iterator iter  = result.begin();iter != result.end();++iter){
        fout1<<iter->first;

        vector<unsigned> cur = iter->second;
        for(unsigned i=0;i<cur.size();++i){
            fout1<<" "<<cur[i];
        }

        fout1<<"\n";
    }

    fout1.close();

    return;
}


int main(int argc, char** argv){

double time0 =omp_get_wtime() ;

    G_ds G;
    InFile(G,argv[1]);

double time1 =omp_get_wtime() ;
cout<<"read time"<< time1 - time0 <<endl;
    vector<int> deg(G.sz,0);
    //vector<unsigned> bin = k_core(G,deg,vert);
    ParK(G,deg);

double time2 =omp_get_wtime() ;
cout<<"core time"<< time2 - time1 <<endl;

    unsigned k = *max_element(deg.begin(),deg.end());/////////////////////////

    out_fun_core(G,deg,k);

    G_ds G_truss;
    G_ds G_truss_direct;
    core2truss(G,deg,k,G_truss,G_truss_direct);

double time3 =omp_get_wtime() ;
cout<<"kmaxcore: "<<k<<endl;
cout<<"core2turss time"<< time3 - time2 <<endl;

    unsigned *  ID2sup = new unsigned [G_truss_direct.es]();

    unsigned *edge2id = (unsigned *)malloc( G_truss_direct.es * sizeof(unsigned) );
    calculateSupport_parallel(G_truss_direct,ID2sup,edge2id);/////////////////////

double time4 =omp_get_wtime() ;
cout<<"triangle time"<< time4 - time3 <<endl;

    unsigned result0,result1;
    k_turss(G_truss,G_truss_direct, ID2sup,result0,result1,edge2id);/////////////////

double time5=omp_get_wtime() ;
cout<<"truss time"<< time5 - time4 <<endl;
cout<<"middle result "<<"kmax_mid = "<<result0<<", Edges in kmax-truss = "<<result1<<"."<<endl;

    free(G_truss.e);
     free(G_truss.h);
    free(G_truss_direct.e);
    free(G_truss_direct.h);

    G_ds G_tr;
    G_ds G_tr_direct;
    core2truss(G,deg,result0-1,G_tr,G_tr_direct);

double time6 =omp_get_wtime() ;
cout<<"core2turss time"<< time6- time5 <<endl;

    ID2ends.clear();
    free(edge2id);
    edge2id = (unsigned *)malloc( G_tr_direct.es * sizeof(unsigned) );

    free(ID2sup);
    ID2sup = new unsigned[G_tr_direct.es]();

    calculateSupport_parallel(G_tr_direct,ID2sup,edge2id);

double time7 =omp_get_wtime() ;
cout<<"triangle time"<< time7 - time6 <<endl;
    k_turss(G_tr,G_tr_direct, ID2sup,result0,result1,edge2id);

    out_fun_truss(ID2sup,G_truss_direct.es,result0);
double time8 =omp_get_wtime() ;

    cout<<"kmax = "<<result0<<", Edges in kmax-truss = "<<result1<<"."<<endl;
cout<<"truss time"<< time8 - time7 <<endl;

cout<<"alg time "<< time8 - time1 <<endl;
cout<<"total time "<< time8 - time0 <<endl;


    return 0;
}
