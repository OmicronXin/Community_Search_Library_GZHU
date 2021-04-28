# Community_Search_Library
A comprehensive library/toolbox for the community detection problem. 

# api_core_truss.cpp
Shared-memory Graph Truss Decomposition and k-core decomposition on undirected graph that output the subgraphs with maximal k.
The PKT k-core decomposition algorithm appears in [Parallel k-core Decomposition on Multicore Platforms](https://ieeexplore.ieee.org/document/7965211/citations?tabFilter=papers), Humayun Kabir and Kamesh Madduri, IPDPSW 2017, and was used to produce the results in the paper. 


## Compiling the program:
    
    $[username]$g++ api_core_truss.cpp -o api_core_truss -std=gnu++0x -O3 -openmp
    
## Input:

    The input to the program is a text file. The file should contain an undirected graph and vertex index should 
    start from 0. Next M lines contain the M edges. An example graph is given in example.txt. Each unordered pair of 
    
## Execution:

    The program can be executed using the following command:
    $[username]$ ./api_core_truss <file-path>
    
## Output:

    Two text files kmaxcore.txt and kmaxtruss.txt whcih contains maximal k-core and maximal k-truss respectively. 
    The format of output files are consistent with the input file.

## Demo:

    $[username]$g++ api_core_truss.cpp -o api_truss -std=gnu++0x -O3 -openmp
    $[username]$./api_core_truss example.txt
    
## Support:

    Please email Xintong Wang (omicronwang@gmail.com) and Haicheng Guo(mcd3team@gmail.com).
— 

Provided for academic use only. For non-academic/commercial use contact 
Xintong Wang (omicronwang@gmail.com)
