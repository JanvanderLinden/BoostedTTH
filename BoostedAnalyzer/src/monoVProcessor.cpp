#include "BoostedTTH/BoostedAnalyzer/interface/monoVProcessor.hpp"

using namespace std;

monoVProcessor::monoVProcessor() {}
monoVProcessor::~monoVProcessor() {}


void monoVProcessor::Init(const InputCollections& input, VariableContainer& vars) {

  vars.InitVar( "N_Jets_Ak8Chs", "I" );

  vars.InitVars( "Jet_Pt_Ak8Chs", "N_Jets_Ak8Chs" );
  vars.InitVars( "PrunedMass_Ak8Chs", "N_Jets_Ak8Chs" );
  vars.InitVars( "tau1_Ak8Chs", "N_Jets_Ak8Chs" );
  vars.InitVars( "tau2_Ak8Chs", "N_Jets_Ak8Chs" );
  vars.InitVars( "tau3_Ak8Chs", "N_Jets_Ak8Chs" );

  vars.InitVars( "Jet_Eta_Ak8Chs", "N_Jets_Ak8Chs" );

  initialized = true;
}

void monoVProcessor::Process(const InputCollections& input, VariableContainer& vars) {
  if (!initialized) cerr << "tree processor not initialized" << endl;
  vars.FillVar( "N_Jets_Ak8Chs", input.AK8PFCHSSoftDropPackedJets.size());

  // for (std::vector<pat::Jet>::const_iterator itJet = input.ak8jetsChsSoftDrop.begin() ; itJet != input.ak8jetsChsSoftDrop.end(); ++itJet) {
    // int iJet = itJet - input.ak8jetsChsSoftDrop.begin();
  //   cout << "Filling Ak8ChsJet" << iJet << endl;

  //   vars.FillVars( "Jet_Pt_Ak8Chs", iJet, itJet->pt());

  //   vars.FillVars( "PrunedMass_Ak8Chs", iJet, itJet->userFloat("ak8PFJetsCHSPrunedMass"));

  //   if ( itJet->hasUserFloat("ak8PFJetsCHSPrunedMass") ) {
  //     cout << "Jet has ak8PFJetsCHSPrunedMass" << endl;
  //     vars.FillVars( "PrunedMass_Ak8Chs", iJet, itJet->userFloat("ak8PFJetsCHSPrunedMass"));
  //     cout << "filling PrunedMass_Ak8Chs" << endl;
  //   }
  //   else cout << " Ak8ChsJet doesnt have ak8PFJetsCHSPrunedMass" << endl;


  //   if ( itJet->hasUserFloat("NjettinessAK8:tau1") ) {
  //     vars.FillVars( "tau1_Ak8Chs", iJet, itJet->userFloat("NjettinessAK8:tau1"));
  //   }
  //   else cout << " Ak8ChsJet doesnt have tau1" << endl;


  //   if ( itJet->hasUserFloat("NjettinessAK8:tau2") ) {
  //     vars.FillVars( "tau2_Ak8Chs", iJet, itJet->userFloat("NjettinessAK8:tau1"));
  //   }
  //   else cout << " Ak8ChsJet doesnt have tau2" << endl;


  //   if ( itJet->hasUserFloat("NjettinessAK8:tau3") ) {
  //     vars.FillVars( "tau3_Ak8Chs", iJet, itJet->userFloat("NjettinessAK8:tau1"));
  //   }
  //   else cout << " Ak8ChsJet doesnt have tau3" << endl;

  //   vars.FillVars( "Jet_Eta_Ak8Chs", iJet, itJet->eta() );


  // }

}
