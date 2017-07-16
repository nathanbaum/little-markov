#include <vector>
#include <string>

using namespace std;

class WordGraph{
private:
  vector <string> dictionary;       //vector of words that correspond to the relational graph (un-sorted)
  vector <vector <int>> graph;      //2d vector of ints: fundamental data structure for unidirectional relational graph
public:
  int add(string word);             //attempts to add a new word to the dictionary -- returns 0 if successful, 1 if word already exists
                                    //if add success, update graph with 0s in all relevant slots (n,0),(0,n),(n,1),(1,n),(n,2)...
  vector <int> search(string word); //returns the row in graph corresponding to word, or a vector<int> with first value of -1 if search was unsuccessful
  int link(string w1, string w2);   //increments the int at w1,w2 -- returns 0 if successful, 1 if one or both words are not found
  int importGraph(string g);        //imports new graph from string using delimeter colon (:) for rows and semicolon (;) for columns
                                    //example:                        2  4  8
                                    //   "2:4:5;4:0:4;8:4:5;"   ->    4  0  4
                                    //                                5  4  5
  string exportGraph();             //returns string formatted like above
  int importDictionary(string d);   //imports new dictionary from string using delimeter space ( )
                                    //example: "cat dog , fish . mouse"
  string exportDictionary();        //returns string formatted like above
};
