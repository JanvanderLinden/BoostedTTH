#ifndef SingleMuonControlSelection_HPP
#define SingleMuonControlSelection_HPP

#include <vector>

#include "BoostedTTH/BoostedAnalyzer/interface/Selection.hpp"

class SingleMuonControlSelection: public Selection{
  
  public:
    
    SingleMuonControlSelection();
    ~SingleMuonControlSelection();
    void InitCutflow(Cutflow& cutflow);
    bool IsSelected(const InputCollections& input,Cutflow& cutflow);


  private:

};

#endif