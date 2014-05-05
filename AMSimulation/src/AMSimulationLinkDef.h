#include <memory>
#include <vector>
#include <map>

#ifdef __MAKECINT__
//#pragma link off all globals;
//#pragma link off all classes;
//#pragma link off all functions;
//#pragma link C++ nestedclasses;
//#pragma link C++ class std::vector<unsigned>+;
//#pragma link C++ class std::vector<float>+;
#pragma link C++ class std::vector<std::vector<int> >+;
#pragma link C++ class std::vector<std::vector<bool> >+;
#pragma link C++ class std::map<unsigned, std::vector<unsigned> >+;
#endif