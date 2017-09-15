#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

using namespace std;

class WordGraph{
private:
  vector <string> dictionary;                                                   //vector of words that correspond to the relational graph (sorted)
  vector <vector <int>> graph;                                                  //2d vector of ints: fundamental data structure for unidirectional relational graph
  int current;
public:
  int add(string word);                                                         //attempts to add a new word to the dictionary -- returns 0 if successful, 1 if word already exists
  int search(string word);                                                      //returns the index in dictionary corresponding to word, or -1 if search was unsuccessful
  int search(string word, int lo, int hi);                                      //overloaded search function, with lower and upper bound for the dictionary search
  int link(string w1, string w2);                                               //pushes the index of w2 to the end of w1
  int import(string f);                                                       //imports new WordGraph from string using delimeter comma (,) for elements of each row (see format below)
                                                                                //example:
                                                                                //   dHello\n                 Hello goodbye thankyou
                                                                                //   dgoodbye\n
                                                                                //   dthankyou\n       ->       0  1  4
                                                                                //   g0,1,4\n                   4  5  0
                                                                                //   g4,5,0\n                   5  1  0
                                                                                //   g5,1,0\n
  string toString();                                                            //returns string formatted like above
  string getCurrent();                                                          //returns the string at dictionary[current]
  int setCurrent(string s);                                                     //searches dictionary for s, if it finds it sets current to index in dictionary, if not adds it to the dictionary and sets current
  int updateCurrent();                                                          //makes a step based on the probabilities and current -- returns 0 if success or 1 if current is null or there are no other links
  const vector <string> getDictionary();
  vector <int> getLinks(string w);
};
