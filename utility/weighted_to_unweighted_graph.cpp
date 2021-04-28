/*
Copyright (c++) of 2020 by haicheng guo
topic: find the kmax of the truss
time: 11/21/2020
version: 9-5 truss parallel 未优化
*/
#include<fstream>
#include<iostream>
#include<vector>
#include<ctime>
#include<algorithm>



using namespace std;


inline char nc(){
    ///*
  static char buf[100000],*p1=buf,*p2=buf;
  if (p1==p2) { p2=(p1=buf)+fread(buf,1,100000,stdin); if (p1==p2) return EOF; }
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

void InFile(char** argv){

    int pre    = 999;//顶点的name是从1开始的
    int Index = 0;
	FILE* fp=freopen(argv[1],"r",stdin);

	ofstream out(argv[2]);
	//C:\\Users\\Omi\\Desktop\\ktruss-data\\s18.e16.rmat.edgelist.tsv
    while(1){
        int ver1 = 0,ver2 = 0,ver3 = 0;
        int t=read(ver2);
        if (t==0) break;
        t=read(ver1);t=read(ver3);
        out<<ver1<<" "<<ver2<<"\n";
    }
	fclose(stdin);
	out.close();


	return ;
}


int main(int argc, char **argv){
    InFile(argv);


    return 0;
}


