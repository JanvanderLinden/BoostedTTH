#include "BoostedTTH/BoostedAnalyzer/interface/SingleMuonControlSelection.hpp"

using namespace std;

SingleMuonControlSelection::SingleMuonControlSelection (){}
SingleMuonControlSelection::~SingleMuonControlSelection (){}

void SingleMuonControlSelection::InitCutflow(Cutflow& cutflow){

  cutflow.AddStep("In Single Muon Control Sample");

  initialized=true;
}

bool SingleMuonControlSelection::IsSelected(const InputCollections& input,Cutflow& cutflow){
  if(!initialized) cerr << "SingleMuonControlSelection not initialized" << endl;
  float Et=0;
  float pt_tmp=0;
  for(std::vector<pat::Jet>::const_iterator itJet = input.selectedJets.begin() ; itJet != input.selectedJets.end(); ++itJet){
    Et+=itJet->pt()*itJet->energy()/sqrt(itJet->energy()*itJet->energy()-itJet->mass()*itJet->mass());
    pt_tmp+=itJet->pt();
  }
  float mt=sqrt(Et*Et-pt_tmp*pt_tmp);

  if(input.selectedMuons.size()!=1 || input.selectedElectronsLoose.size()>0 || input.selectedTaus.size() >0) {
    return false;
  }
  if(mt>160){
    return false;
  }
  else{
    cutflow.EventSurvivedStep("In Single Muon Control Sample",input.weights.at("Weight"));
    return true;    
  }

}