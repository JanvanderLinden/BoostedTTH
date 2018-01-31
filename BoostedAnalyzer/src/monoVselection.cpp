#include "BoostedTTH/BoostedAnalyzer/interface/monoVselection.hpp"

using namespace std;

monoVselection::monoVselection (const edm::ParameterSet& iConfig): monoVselection(iConfig.getParameter<double>("maxpt"), iConfig.getParameter<double>("maxeta"), iConfig.getParameter<double>("minPrunedMass"), iConfig.getParameter<double>("maxPrunedMass"), iConfig.getParameter<double>("tau21")) {}
monoVselection::monoVselection (double maxpt_, double maxeta_, double minPrunedMass_, double maxPrunedMass_, double tau21_): maxpt(maxpt_), maxeta(maxeta_), minPrunedMass(minPrunedMass_), maxPrunedMass(maxPrunedMass_), tau21(tau21_) {}
monoVselection::~monoVselection () {}

void monoVselection::InitCutflow(Cutflow& cutflow) {

  cutflow.AddStep("not monoVtagged");

  initialized = true;
}

bool monoVselection::IsSelected(const InputCollections& input, Cutflow& cutflow) {
  if (!initialized) cerr << "monoVselection not initialized" << endl;
  double tau1 = -1;
  double tau2 = -1;

  pat::Jet *leadingJet = new pat::Jet;
  leadingJet = input.AK8Jets.at(0).clone();

  tau1 = leadingJet->userFloat("NjettinessAK8:tau1");
  tau2 = leadingJet->userFloat("NjettinessAK8:tau2");

  double leadingJet_PrunedMass = leadingJet->userFloat("ak8PFJetsCHSPrunedMass");
  double tau21 = tau2 / tau1;

  bool monoVtag = false;
  
  if (leadingJet->pt() > 250 && abs(leadingJet->eta()) < 2.4 && leadingJet_PrunedMass > 65 && leadingJet_PrunedMass < 105 && tau21 < 0.6 ) {
    monoVtag = true;
  }

  if (monoVtag) {
    return false;
  }
  else {
    cutflow.EventSurvivedStep("not monoVtagged", input.weights.at("Weight"));
    return true;
  }

}