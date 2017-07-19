#include <time.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
  time_t timer;
  int startTime = time(&timer);
  int lineTotal = 100;
  int curLine = 0;
  while(curLine!=lineTotal){
    curLine = time(&timer) - startTime;
    cout << "\r[";
    for(int i=0; i<int (double (curLine)/double (lineTotal) * 100); i++){
      cout << "|";
    }
    for(int i=0; i< 100 - int (double (curLine)/double (lineTotal) * 100); i++){
      cout << " ";
    }
    cout << "]" << int (double (curLine)/double (lineTotal) * 100)<< "%";
  }
}
