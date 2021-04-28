# Community_Search_Library
A comprehensive library/toolbox for the community detection problem. 

# api_core_truss.cpp
Shared-memory Graph Truss Decomposition and k-core decomposition on undirected graph that output the subgraphs with maximal k.
The PKT k-core decomposition algorithm appears in [Parallel k-core Decomposition on Multicore Platforms](https://ieeexplore.ieee.org/document/7965211/citations?tabFilter=papers), Humayun Kabir and Kamesh Madduri, IPDPSW 2017, and was used to produce the results in the paper. 


## Compiling the program:
    
    $[username]$g++ api_core_truss.cpp -o api_core_truss -std=gnu++0x -O3 -openmp
    
## Input:
The input to the program is a text file. The file should contain an undirected graph and vertex index should 
start from 0. Next M lines contain the M edges. Two ends of the edge are separated by space. An example graph 
is given in [example.txt](https://github.com/OmicronXin/Community_Search_Library_GZHU/files/6389103/example.txt). Each unordered pair of vertices has been stored twice. i.e. <u> <v> and <v> <u>.
    
## Execution:

    The program can be executed using the following command:
    $[username]$ ./api_core_truss <file-path>
    
## Output:

    Two text files kmaxcore.txt and kmaxtruss.txt whcih contains maximal k-core and maximal k-truss respectively. 
    The format of output files are consistent with the input file.

## Demo:

    $[username]$g++ api_core_truss.cpp -o api_truss -std=gnu++0x -O3 -openmp
    $[username]$./api_core_truss example.txt
    
# scores.cpp

    Print related statistics of the given subgraphs, which can be used to assess the quality of found community. 

## Compiling the program:
    
    $[username]$g++ scores.cpp -o scores -std=gnu++0x -O3 -openmp
    
## Input:

    The input to the program is the folder containing the subgraphs kmaxtruss and kmaxcore, and a text file containing the original graph. 
    The file should contain an undirected graph and vertex index should start from 0. Next M lines contain the M edges. 
    Two ends of the edge are separated by space. An example graph is given in [example.txt](https://github.com/OmicronXin/Community_Search_Library_GZHU/files/6389103/example.txt). Each unordered pair of 
    vertices has been stored twice. i.e. <u> <v> and <v> <u>. 
    
## Execution:

    The program can be executed using the following command:
    $[username]$ ./scores <subgraph-folder-path> <original-file-path>
    
## Output:

    Print the related statistics of the given subgraphs to the console.

## Demo:


    $[username]$g++ scores.cpp -o scores -std=gnu++0x -O3 -openmp
    $[username]$./scores /subgraphs example.txt
    
    
    
# Support:

    Please email Xintong Wang (omicronwang@gmail.com) and Haicheng Guo(mcd3team@gmail.com).
— 

Provided for academic use only. For non-academic/commercial use contact 
Xintong Wang (omicronwang@gmail.com)
