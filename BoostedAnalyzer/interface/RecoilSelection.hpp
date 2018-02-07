#ifndef RecoilSelection_HPP
#define RecoilSelection_HPP

#include <vector>

#include "BoostedTTH/BoostedAnalyzer/interface/Selection.hpp"

class RecoilSelection: public Selection{
  
  public:
    
    RecoilSelection(const edm::ParameterSet& iConfig);
    RecoilSelection(double minRecoil=-1, double maxRecoil=999999);
    ~RecoilSelection();
    void InitCutflow(Cutflow& cutflow);
    bool IsSelected(const InputCollections& input,Cutflow& cutflow);


  private:
    double minRecoil;
    double maxRecoil;
    std::string selectionName;

};

#endif
