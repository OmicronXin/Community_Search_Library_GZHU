# 广州大学方班网络安全综合实验-指导老师：王乐
# Community_Search_Library
A comprehensive library/toolbox for the community detection problem. 

# **api_core_truss.cpp**
Parallel Graph Truss Decomposition and k-core decomposition on undirected graph that output the subgraphs with maximal k.
The parallel k-core decomposition algorithm appears in [Parallel k-core Decomposition on Multicore Platforms](https://ieeexplore.ieee.org/document/7965211/citations?tabFilter=papers), Humayun Kabir and Kamesh Madduri, IPDPSW 2017. The parallel k-truss decomposition appears in [Parallel k-truss Decomposition on Multicore Systems](https://ieeexplore.ieee.org/document/8091052), Humayun Kabir and Kamesh Madduri, HiPC 2017. Both were used to produce the results in the papers. 


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

Two text files kmaxcore.txt and kmaxtruss.txt which contains maximal k-core and maximal k-truss respectively. 
The format of output files are consistent with the input file.

## Demo:

    $[username]$g++ api_core_truss.cpp -o api_truss -std=gnu++0x -O3 -openmp
    $[username]$./api_core_truss example.txt
    
    
# **densest_subgraph.cpp**

Find the subgraph with highest average degree.

## Compiling the program:
    
    $[username]$g++ densest_subgraph.cpp -o densest_subgraph -std=gnu++0x -O3 -openmp
    
## Input:
Same as `api_core_truss.cpp`.

## Execution:

The program can be executed using the following command:

    $[username]$ ./scores <subgraph-folder-path> <original-file-path>
    
## Output:

A text file densest_subgraph.txt, with the same format as the input file.

## Demo:

    $[username]$g++ densest_subgraph.cpp -o densest_subgraph -std=gnu++0x -O3 -openmp
    $[username]$./densest_subgraph example.txt
    
# **scores.cpp**

Print related statistics of the given subgraphs, which can be used to assess the quality of found community. 

## Compiling the program:
    
    $[username]$g++ scores.cpp -o scores -std=gnu++0x -O3 -openmp
    
## Input:

The input to the program is the folder containing the subgraphs kmaxtruss and kmaxcore, and a text file containing the original graph. 
Text file format should consist with format of `api_core_truss.cpp`.
    
## Execution:

The program can be executed using the following command:

    $[username]$ ./scores <subgraph-folder-path> <original-file-path>
    
## Output:

Print the related statistics of the given subgraphs to the console.

## Demo:


    $[username]$g++ scores.cpp -o scores -std=gnu++0x -O3 -openmp
    $[username]$./scores /subgraphs example.txt
    

# **extractDBLP.cpp**

Extract information from [DBLP.xml](https://dblp.uni-trier.de/xml/). DBLP is a network of scholars in the field of computer science. The network is a classic example of social network and has been studied by a lot of researchers. This program extracts relevent data from xml file.

## Compiling the program:
    
    $[username]$g++ extractDBLP.cpp -o extractDBLP -std=gnu++0x -O3 -openmp
    
## Input:
The program only support the data with identical format as [DBLP.xml](https://dblp.uni-trier.de/xml/).
Upon the execution of the program, three parameters will be enter sequentially:
 1.The last year of the data to be included
 2.Number of years to be included
 3.Minimum requirement of cooperated papers between two authors to be counted as "neighbors" in the graph

## Execution:

The program can be executed using the following command:

    $[username]$ ./extractDBLP dblp.xml
    
## Output:

6 text files:
  - dblp_authors: All the authors in the network.
  - dblp_papers：All the papers in the network.
  - dblp_au_paper_: paper-author relations
  - dblp_au_relation: author-author relations
  - dblp_authors_active: active authors
  - dblp_au_papernum: numbers of papers for each author
  
# To do:

- [x] Refine README.md
- [ ] Support more input file formats 
- [ ] Better graph visualization
- [ ] A more compact library
- [ ] Enrich the library with other functionalities

# Support:

    Please email Xintong Wang (omicronwang@gmail.com) and Haicheng Guo(mcd3team@gmail.com).
— 

Provided for academic use only. For non-academic/commercial use contact 
Xintong Wang (omicronwang@gmail.com)
