#include "WordGraph.hpp"

int WordGraph::add(string word){
  if(search(word)[0]==-1){                    //if a search for the word comes up empty
    for(int i=0; i<dictionary.size(); i++){   //then we add another row to every column
      graph[i].push_back(0);
    }
    dictionary.push_back(word);               //now we add the word to the dictionary
    vector <int> w;                           //make a new temporary vector for that word's collumn
    for(int i=0; i<dictionary.size(); i++){   //and fill it with zeros
      w.push_back(0);
    }
    graph.push_back(w);                       //now add that collumn to the graph
    return 0;                                 //return 0 indicating success
  }
  return 1;                                   //if the search found something, return 1, indicating failure
}

vector <int> WordGraph::search(string word){
  for(int i=0; i<dictionary.size(); i++){     //search dictionary
    if(dictionary[i].compare(word)==0){       //if it finds a match
      return graph[i];                        //return the column in graph corresponding to word
    }
  }
  vector<int> n = {-1};                       //n stands for null because I'm afraid of keywords
}

int WordGraph::link(string w1, string w2){
  int iw1=-1;                                 //iw1 stands for index of word 1
  int iw2=-1;                                 //I think you can guess what iw2 stands for
  for(int i=0; i<dictionary.size(); i++){     //search dictionary
    if(dictionary[i].compare(w1)==0){         //if it finds a match
      iw1 = i;                                //set iw to index where we found it
      break;                                  //no need to finish the traversal
    }
  }
  if(iw1==-1) return 1;                       //if iw was never overwritten, w wasn't found and link failure
  for(int i=0; i<dictionary.size(); i++){     //search dictionary
    if(dictionary[i].compare(w2)==0){         //if it finds a match
      iw2 = i;                                //set iw to index where we found it
      break;                                  //no need to finish the traversal
    }
  }
  if(iw2==-1) return 1;                       //if iw was never overwritten, w wasn't found and link failure
  graph[iw1][iw2]++;                          //increment that spot on graph by one to link w1 to w2
  return 0;
}

int WordGraph::importGraph(string g){         //all the ways g can be deformed:
                                              //unequal lengths between semicolons
                                              //number of collumns and rows not equal
                                              //invalid characters
  int graphSize = -1;
  for(int i=0; i<g.size(); i++){
    if(graphSize==-1 && g[i]==';') graphSize=(i+1)/2
  }
}
