#include "BoostedTTH/BoostedAnalyzer/interface/monoVProcessor.hpp"

using namespace std;

monoVProcessor::monoVProcessor() {}
monoVProcessor::~monoVProcessor() {}


void monoVProcessor::Init(const InputCollections& input, VariableContainer& vars) {

  vars.InitVar( "N_Jets_Ak8", "I" );

  vars.InitVars( "Jet_Pt_Ak8", "N_Jets_Ak8" );
  vars.InitVars( "PrunedMass_Ak8", "N_Jets_Ak8" );

  vars.InitVars( "tau1_Ak8", "N_Jets_Ak8" );
  vars.InitVars( "tau2_Ak8", "N_Jets_Ak8" );
  vars.InitVars( "tau3_Ak8", "N_Jets_Ak8" );

  vars.InitVars( "Jet_Eta_Ak8", "N_Jets_Ak8" );

  vars.InitVars( "puppi_softdrop_subjet_Pt", "N_Jets_Ak8" );


  initialized = true;
}

void monoVProcessor::Process(const InputCollections& input, VariableContainer& vars) {
  if (!initialized) cerr << "tree processor not initialized" << endl;
  vars.FillVar( "N_Jets_Ak8", input.AK8Jets.size());



  for (std::vector<pat::Jet>::const_iterator itJet = input.AK8Jets.begin() ; itJet != input.AK8Jets.end(); ++itJet) {
    int iJet = itJet - input.AK8Jets.begin();
    cout << "Filling AK8Jet " << iJet << endl;

    TLorentzVector puppi_softdrop, puppi_softdrop_subjet;
    auto const & sdSubjetsPuppi = itJet->subjets("SoftDropPuppi");
    for ( auto const & it : sdSubjetsPuppi ) {
      puppi_softdrop_subjet.SetPtEtaPhiM(it->correctedP4(0).pt(), it->correctedP4(0).eta(), it->correctedP4(0).phi(), it->correctedP4(0).mass());
      puppi_softdrop += puppi_softdrop_subjet;
    }

    vars.FillVars( "puppi_softdrop_subjet_Pt", iJet, puppi_softdrop_subjet.Pt());
    

    vars.FillVars( "Jet_Pt_Ak8", iJet, itJet->pt());

    vars.FillVars( "PrunedMass_Ak8", iJet, itJet->userFloat("ak8PFJetsCHSPrunedMass"));



    if ( itJet->hasUserFloat("NjettinessAK8:tau1") ) {
      vars.FillVars( "tau1_Ak8", iJet, itJet->userFloat("NjettinessAK8:tau1"));
    }
    else cout << " Ak8Jet doesnt have tau1" << endl;


    if ( itJet->hasUserFloat("NjettinessAK8:tau2") ) {
      vars.FillVars( "tau2_Ak8", iJet, itJet->userFloat("NjettinessAK8:tau2"));
    }
    else cout << " AK8Jet doesnt have tau2" << endl;


    if ( itJet->hasUserFloat("NjettinessAK8:tau3") ) {
      vars.FillVars( "tau3_Ak8", iJet, itJet->userFloat("NjettinessAK8:tau3"));
    }
    else cout << " AK8Jet doesnt have tau3" << endl;

    vars.FillVars( "Jet_Eta_Ak8", iJet, itJet->eta() );
  }






}
