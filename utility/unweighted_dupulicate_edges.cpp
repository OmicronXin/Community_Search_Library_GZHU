//argv[1]:input file path          argv[2]:output file path
//dupulicate all edges (won't produce each ordered edge more than once) input:<ui,vi> output:<ui,vi>,<vi,ui>            


#define _CRT_SECURE_NO_WARNINGS
#include<fstream>
#include<iostream>
#include<vector>
#include<ctime>
#include<algorithm>
#include<map>


using namespace std;
map<int, vector<int> > G;

inline char nc() {
    ///*
    static char buf[100000], * p1 = buf, * p2 = buf;
    if (p1 == p2) { p2 = (p1 = buf) + fread(buf, 1, 100000, stdin); if (p1 == p2) return EOF; }
    return *p1++;
    //*/return getchar();
}

inline int read(int& x) {
    char c = nc(); int b = 1;
    if (c == '\377') return 0;
    for (; !(c >= '0' && c <= '9'); c = nc()) if (c == '\377') return 0;   //为啥不能去掉
    for (x = 0; c >= '0' && c <= '9'; x = x * 10 + c - '0', c = nc()); x *= b;
    return 1;
}

void InFile(char** argv) {

    int pre = 999;//顶点的name是从1开始的
    int Index = 0;
    FILE* fp = freopen(argv[1], "r", stdin);
    int n = 0;
    ofstream out(argv[2]);
    while (1) {
        int ver1 = 0, ver2 = 0;
        int t = read(ver1);
        if (t == 0) break;
        t = read(ver2);

        if (G.find(ver1) == G.end()) {
            vector<int> temp1;
            G[ver1] = temp1;
        }
        if (G.find(ver2) == G.end()) {
            vector<int> temp2;
            G[ver2] = temp2;
        }

    }
    fclose(stdin);

    fp = freopen(argv[1], "r", stdin);
    //C:\\Users\\Omicron\\Dropbox\\Study\\GZHU\\cpp\\datasets\\directed1.txt
    while (1) {
        int ver1 = 0, ver2 = 0;
        int t = read(ver1);
        if (t == 0) break;
        t = read(ver2);

        if (find(G[ver1].begin(), G[ver1].end(), ver2) == G[ver1].end()) G[ver1].push_back(ver2);
        if (find(G[ver2].begin(), G[ver2].end(), ver1) == G[ver2].end()) G[ver2].push_back(ver1);
    }
    fclose(stdin);


    for (map<int, vector<int> >::iterator it = G.begin(); it != G.end(); it++) {
        for (int i = 0; i < it->second.size(); i++) {

            out << it->first << " " << it->second[i] << "\n";
        }
    }
    out.close();


    return;
}


int main(int argc, char** argv) {
    InFile(argv);


    return 0;
}


