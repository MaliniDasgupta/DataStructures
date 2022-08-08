#include "graph.hpp"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <queue>
#include <stack>


Graph::Graph(const std::string& filename){
    std::ifstream inputFile {filename};
    if(inputFile.fail()){
        throw std::logic_error{"Failed to open file"};
    }
    std::string line;
    inputFile >> line;
    if(line == "Weighted"){
        weighted = 1;
    }
    else if(line == "Unweighted"){
        weighted = 0;
    }
    else{
        throw std::logic_error{"Invalid graph"};
    }

    inputFile >> line;
    if(line == "Directed"){
        directed = 1;
    }
    else if(line == "Undirected"){
        directed = 0;
    }
    else{
        throw std::logic_error{"Invalid graph"};
    }

    std::string format;
    inputFile >> format;
    inputFile >> numVertex;

    if(numVertex<1){
        throw std::logic_error{"Invalid graph"};
    }
    std::getline(inputFile,line);
    //format AdjMatrix or AdjList
    if(format == "AdjMatrix" || format == "AdjList"){
        std::vector<int> matrixLine; //vector for each line in the adjacency matrix
        for(int i=0;i<numVertex;i++){
            std::getline(inputFile,line);
            if(format == "AdjMatrix"){
                matrixLine = helperAdjMatrix(line,i);
            }
            else{
                matrixLine = helperAdjList(line);
            }
            graphData.push_back(matrixLine);

            //check for self loop
            if(graphData[i][i]!=0){
                throw std::logic_error{"Invalid graph"};
            }
        }
    }
    //format ListEdges
    else if(format == "ListEdges"){
        if(!weighted){
            std::vector<std::pair<unsigned, unsigned>> edges;
            while(std::getline(inputFile,line)){
                std::stringstream lineStream(line);
                unsigned first, second;
                lineStream >> first;
                lineStream >> second;
                std::pair<unsigned, unsigned> p1{first,second};
                edges.push_back(p1);
            }
            graphData = helperUnweightedListEdge(edges);
        }
        else{
            std::vector<std::tuple<unsigned, unsigned, int>> edges;
            while(std::getline(inputFile,line)){
                std::stringstream lineStream(line);
                lineStream << line;
                unsigned first, second;
                int weight;
                lineStream >> first;
                lineStream >> second;
                lineStream >> weight;
                std::tuple<unsigned, unsigned, int> t1{first, second, weight};
                edges.push_back(t1);
            }
            graphData = helperWeightedListEdge(edges);
        } 
    }
    //wrong format
    else{
        throw std::logic_error{"Invalid graph"};
    }
    
}

Graph::Graph(unsigned numVertices, const std::vector<std::pair<unsigned, unsigned>>& edges, bool isDirected){
    numVertex = numVertices;
    weighted = 0;
    directed = isDirected;  
    graphData = helperUnweightedListEdge(edges);
}

Graph::Graph(unsigned numVertices, const std::vector<std::tuple<unsigned, unsigned, int>>& edges, bool isDirected){
    numVertex = numVertices;
    weighted = 1;
    directed = isDirected;
    graphData = helperWeightedListEdge(edges);
}

bool Graph::isWeighted() const {
    return weighted;
}

bool Graph::isDirected() const {
    return directed;
}

unsigned Graph::getNumVertices() const {
    return numVertex;
}

unsigned Graph::getNumEdges() const {
    unsigned count = 0;
    for(int i=0;i<numVertex; i++){
        for(int j=0;j<numVertex;j++){
            if(graphData[i][j]!=0){
                count++;
            }
        }
    }
    if(directed){
        return count;
    }
    else{
        return (int)(count/2);
    }
    
}

std::vector<std::vector<int>> Graph::getAdjacencyMatrix() const {
    return graphData;

}

std::vector<std::vector<std::pair<unsigned, int>>> Graph::getAdjacencyList() const {
    std::vector<std::vector<std::pair<unsigned, int>>> returnVector;

    for(int i=0;i<numVertex;i++){
        std::vector<std::pair<unsigned, int>> adjListLine;
        for(int j=0;j<(int)numVertex;j++){
            if(graphData[i][j]!=0){
                std::pair<unsigned, int> p1{j, graphData[i][j]};
                adjListLine.push_back(p1);
            }
        }
        returnVector.push_back(adjListLine);
    }

    return returnVector;
}

std::vector<unsigned> Graph::getBFSOrdering(unsigned start) const{
    if((int)start>=numVertex){
        throw std::logic_error{"Invalid start node"};
    }
    std::vector<unsigned> returnVector;
    std::queue<unsigned> q;
    std::vector<bool> discovered;

    for(int i=0;i<numVertex;i++){
        discovered.push_back(0);
    }
    discovered[start] = 1;
    q.push(start);
    while(q.size()>0){
        unsigned currNode = q.front();
        q.pop();
        returnVector.push_back(currNode);
        for(unsigned i=0;i<(unsigned)numVertex;i++){
            if(graphData[currNode][i]!=0){
                if(!discovered[i]){
                    discovered[i] = 1;
                    q.push(i);
                }
            }
        }
    }

    return returnVector;
}

std::vector<unsigned> Graph::getDFSOrdering(unsigned start) const{
    if((int)start>=numVertex){
        throw std::logic_error{"Invalid start node"};
    }
    std::vector<unsigned> returnVector;
    std::stack<unsigned> s;
    std::vector<bool> discovered;

    for(int i=0;i<numVertex;i++){
        discovered.push_back(0);
    }
    discovered[start] = 1;
    s.push(start);
    while(s.size()>0){
        unsigned currNode = s.top();
        s.pop();
        returnVector.push_back(currNode);
        for(unsigned i=0;i<(unsigned)numVertex;i++){
            if(graphData[currNode][i]!=0){
                if(!discovered[i]){
                    discovered[i] = 1;
                    s.push(i);
                }
            }
        }
    }

    return returnVector;
}

std::vector<std::vector<bool>> Graph::getTransitiveClosure() const{
    std::vector<std::vector<bool>> returnVector;

    //initialize returnVector to all 0s
    for(int i=0;i<numVertex;i++){
        std::vector<bool> matrixLine;
        for(int j=0;j<numVertex;j++){
            matrixLine.push_back(0);
        }
        returnVector.push_back(matrixLine);
    }
    //for all neighbors, and self, returnValue will be 1
    for(int i=0;i<numVertex;i++){
        for(int j=0;j<numVertex;j++){
            if(i==j){
                returnVector[i][j] = 1;
            }
            else if(graphData[i][j]!=0){
                returnVector[i][j] = 1;
            }
        }
       
    }
    //now to check if there is still a path for non neighbors
    for(int i=0; i<numVertex;i++){
        for(int j=0;j<numVertex;j++){
            if(returnVector[i][j]==0){
                for(int k=0;k<numVertex;k++){
                    if(returnVector[i][k] && returnVector[k][j]){
                        returnVector[i][j] = 1;
                    }
                    else{
                        for(int l=0;l<numVertex;l++){
                            if(returnVector[i][l] && returnVector[l][k] && returnVector[k][j]){
                                returnVector[i][j] = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    return returnVector;
}


//helper function to format graphs in AdjMatrix format
std::vector<int> Graph::helperAdjMatrix(std::string line, int lineNo) const{
    std::vector<int> matrixLine;
    std::stringstream lineStream (line);
    if(directed){
        //if graph is unweighted
        if(line[0]=='T' || line[0]=='F'){
            char b;
            for(int j=0;j<numVertex;j++){
                lineStream >> b;
                if(b=='T'){
                    matrixLine.push_back(1);
                }
                else if(b=='F'){
                    matrixLine.push_back(0);
                }
            }
       }
        //if graph is weighted
        else{
            int b;
            for(int j=0;j<numVertex;j++){
                lineStream >> b;
                matrixLine.push_back(b);
            }
        }
        return matrixLine;
    }
    //for all AdjMatrix graphs that are undirected
    else{
        int k;
        for(k=lineNo-1;k>=0;k--){
            std::vector<int> prevLine = graphData[lineNo-(k+1)];
            matrixLine.push_back(prevLine[lineNo]);
        }
        //if graph is unweighted
        if(!weighted){
            char b;
            for(int j=lineNo;j<numVertex;j++){
                lineStream >> b;
                if(b=='T'){
                    matrixLine.push_back(1);
                }
                else if(b=='F'){
                    matrixLine.push_back(0);
                }
                    
             }

        }
        else{
            int b;
            for(int j=lineNo;j<numVertex;j++){
                lineStream >> b;
                matrixLine.push_back(b);
            }
        }
            
    return matrixLine;
    
    }
}

//helper function to format graphs in AdjList format 
std::vector<int> Graph::helperAdjList(std::string line) const{
    std::vector<int> matrixLine;
    std::stringstream lineStream (line);
    for(int i=0;i<numVertex;i++){
        matrixLine.push_back(0);
    }
    if(!weighted){
        int b;
        while(lineStream >> b){
            //checking for valid nodeID and for multi edge
            if(b>=0 && b<numVertex && matrixLine[b]==0){
                matrixLine[b] = 1;
            }
            else{
                throw std::logic_error{"Invalid graph"};  
            }

        }
        return matrixLine;
    }

    //for all AdjList graphs that are weighted
    else{
        int b;
        while(lineStream >> b){        
            int c;
            lineStream >> c;
            //checking for valid nodeID, multiedge, and invalid weight
            if(b>=0 && b<numVertex && matrixLine[b]==0 && c!=0){
                matrixLine[b] = c;
            }
            else{
                throw std::logic_error{"Invalid graph"};  
            } 
        }
        return matrixLine;
    }
}


//helper function for unweighted graph in ListEdges format
std::vector<std::vector<int>> Graph::helperUnweightedListEdge(const std::vector<std::pair<unsigned, unsigned>>& edges) const{
    std::vector<int> matrixLine;
    std::vector<std::vector<int>> returnMatrix;

    //initialize all values to 0
    for(int i=0;i<numVertex;i++){
        for(int j=0;j<numVertex;j++){
            matrixLine.push_back(0);
        }
        returnMatrix.push_back(matrixLine);
    }

    for(int i=0;i<(int)edges.size();i++){
        int first = (int)edges[i].first;
        int second = (int)edges[i].second;

        //checking for self loops
        if(first == second){
            throw std::logic_error{"Invalid graph"};
        }
        //checking for valid nodeID and multiedge
        if(first>=0 && second>=0 && first<numVertex && second<numVertex && returnMatrix[first][second]==0){
            returnMatrix[first][second] = 1;

            if(!directed){
                //checking for multiple edge
                if(returnMatrix[second][first]==0){
                    returnMatrix[second][first] = 1;
                }
                else{
                    throw std::logic_error{"Invalid graph"};
                }
            }
        }
        else{
            throw std::logic_error{"Invalid graph"};
        }
    }
    return returnMatrix;

}

//helper function for weighted graph in ListEdges format
std::vector<std::vector<int>> Graph::helperWeightedListEdge(const std::vector<std::tuple<unsigned, unsigned, int>>& edges) const{
    std::vector<int> matrixLine;
    std::vector<std::vector<int>> returnMatrix;

    //initalize all values to 0
    for(int i=0;i<numVertex;i++){
        for(int j=0;j<numVertex;j++){
            matrixLine.push_back(0);
        }
        returnMatrix.push_back(matrixLine);
    }

    for(int i=0;i<(int)edges.size();i++){
        std::tuple<unsigned, unsigned, int> a = edges[i];
        int first = (int)std::get<0>(a);
        int second = (int)std::get<1>(a);
        int weight = std::get<2>(a);

        //checking for self loop
        if(first==second){
            throw std::logic_error{"Invalid graph"};
        }
        //checking for valid nodeID, multi edge, and valid weight
        if(first>=0 && second>=0 && first<numVertex && second<numVertex && weight!=0 && returnMatrix[first][second]==0){
            returnMatrix[first][second] = weight;

            if(!directed){
                //checking for multiple edge
                if(returnMatrix[second][first]==0){
                    returnMatrix[second][first] = weight;
                }
                else{
                    throw std::logic_error{"Invalid graph"};
                }
            }
        }
        else{
            throw std::logic_error{"Invalid graph"};
        }
    }
    return returnMatrix;
}



