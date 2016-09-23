// reading a text file
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int modID_list_cleaner() {
  unsigned long aux;
  ifstream myfile ("data/modID_list.txt");

  ofstream fout;
  fout.open ("data/modID_list.h");

  if (myfile.is_open())
  {
    int i=0;
    while ( !myfile.eof() )
    {
      myfile >> aux;
      myfile >> aux;
      fout << aux << "," << endl;
      i++;
    }
    myfile.close();
  }
  else cout << "Unable to open file";

  fout.close();

  return 0;
}
