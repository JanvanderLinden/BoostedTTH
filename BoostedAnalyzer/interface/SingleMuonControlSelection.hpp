#ifndef SingleMuonControlSelection_HPP
#define SingleMuonControlSelection_HPP

#include <vector>

#include "BoostedTTH/BoostedAnalyzer/interface/Selection.hpp"

class SingleMuonControlSelection: public Selection{
  
  public:
    
    SingleMuonControlSelection(const edm::ParameterSet& iConfig);
    SingleMuonControlSelection(double max_mt_=-1);
    ~SingleMuonControlSelection();
    void InitCutflow(Cutflow& cutflow);
    bool IsSelected(const InputCollections& input,Cutflow& cutflow);


  private:
  	double max_mt;

};

#endif