#include "BoostedTTH/BoostedAnalyzer/interface/DMControlSelection.hpp"

using namespace std;

// DMControlSelection::DMControlSelection (const edm::ParameterSet& iConfig): DMControlSelection(iConfig.getParameter<std::string>("channel"),  iConfig.getParameter<edm:ParameterSet>("SingleMuon")) {}
// DMControlSelection::DMControlSelection (std::string channel_, edm::Parameters parameters_): channel(channel_), Parameters(Parameters_) {}
DMControlSelection::DMControlSelection(const edm::ParameterSet& iConfig)
{
  channel = iConfig.getParameter<std::string>("channel");
  cout << channel << " in DMControlSelection Constructor " << endl;

  if (channel == "SingleMuon") {
    const edm::ParameterSet params = iConfig.getParameter<edm::ParameterSet>("SingleMuon");
    max_mt = params.getParameter<double>("max_mt");
  }
  if (channel == "SingleElectron") {
    const edm::ParameterSet params = iConfig.getParameter<edm::ParameterSet>("SingleElectron");
    min_met = params.getParameter<double>("min_met");
  }
  if (channel == "DoubleMuon") {
    const edm::ParameterSet params = iConfig.getParameter<edm::ParameterSet>("DoubleMuon");
    min_pt = params.getParameter<double>("min_pt");
    min_mass_ll = params.getParameter<double>("min_mass_ll");
    max_mass_ll = params.getParameter<double>("max_mass_ll");
  }
  if (channel == "DoubleElectron") {
    const edm::ParameterSet params = iConfig.getParameter<edm::ParameterSet>("DoubleElectron");
    min_pt = params.getParameter<double>("min_pt");
    min_mass_ll = params.getParameter<double>("min_mass_ll");
    max_mass_ll = params.getParameter<double>("max_mass_ll");
  }
  if (channel == "Photon") {
    const edm::ParameterSet params = iConfig.getParameter<edm::ParameterSet>("Photon");
    min_pt = params.getParameter<double>("min_pt");
  }

}


DMControlSelection::~DMControlSelection () {}


void DMControlSelection::InitCutflow(Cutflow& cutflow) {
  if (channel == "signal") {
    cutflow.AddStep("signal Lepton selection");
  }
  if (channel == "SingleMuon") {
    cutflow.AddStep("SingleMuon Control selection");
  }
  if (channel == "SingleElectron") {
    cutflow.AddStep("SingleElectron Control selection");
  }
  if (channel == "DoubleMuon") {
    cutflow.AddStep("DoubleMuon Control selection");
  }
  if (channel == "DoubleElectron") {
    cutflow.AddStep("DoubleElectron Control selection");
  }
  if (channel == "Photon") {
    cutflow.AddStep("Photon+Jets Control selection");
  }

  initialized = true;
}



bool DMControlSelection::IsSelected(const InputCollections & input, Cutflow & cutflow) {
  if (!initialized) cerr << "DMControlSelection not initialized" << endl;
  // cout << channel << endl;

  if (channel == "signal") {
    if (input.selectedElectronsLoose.size() > 0 || input.selectedMuonsLoose.size() > 0 || input.selectedTaus.size() > 0 || input.selectedPhotonsLoose.size() > 0) {
      return false;
    }
    else {
      cutflow.EventSurvivedStep("signal Lepton selection", input.weights.at("Weight"));
      return true;
    }
  }


  if (channel == "SingleMuon") {
    float Et = 0;
    float pt_tmp = 0;

    // cout << max_mt << endl;
    for (std::vector<pat::Jet>::const_iterator itJet = input.selectedJets.begin() ; itJet != input.selectedJets.end(); ++itJet) {
      Et += itJet->pt() * itJet->energy() / sqrt(itJet->energy() * itJet->energy() - itJet->mass() * itJet->mass());
      pt_tmp += itJet->pt();
    }
    float mt = sqrt(Et * Et - pt_tmp * pt_tmp);

    if (input.selectedMuons.size() != 1 || input.selectedElectronsLoose.size() > 0 || input.selectedTaus.size() > 0 || input.selectedPhotonsLoose.size() > 0) {
      return false;
    }
    if (mt > max_mt) {
      return false;
    }
    else {
      cutflow.EventSurvivedStep("SingleMuon Control selection", input.weights.at("Weight"));
      return true;
    }
  }

  if (channel == "SingleElectron") {
    // cout << min_met << endl;
    double met = input.correctedMET.corPt(pat::MET::Type1XY);
    // cout << "Event MET: " << met << endl;
    if (input.selectedMuonsLoose.size() > 0 || input.selectedElectronsLoose.size() != 1 || input.selectedTaus.size() > 0 || input.selectedPhotonsLoose.size() > 0) {
      return false;
    }
    if (met < min_met) {
      return false;
    }
    else {
      cutflow.EventSurvivedStep("SingleElectron Control selection", input.weights.at("Weight"));
      return true;
    }
  }

  if (channel == "DoubleMuon") {
    if (input.selectedMuonsLoose.size() != 2 || input.selectedElectronsLoose.size() >  0 || input.selectedTaus.size() > 0 || input.selectedPhotonsLoose.size() > 0) {
      return false;
    }
    // cout << "== 2 loose Muons check" << endl;
    if (input.selectedMuonsLoose.at(0).charge()*input.selectedMuonsLoose.at(1).charge() > 1) {
      return false;
    }
    // cout << "opposite charge check" << endl;
    if (input.selectedMuons.size() < 1 ) {
      return false;
    }
    // cout << "min 1 tight Muon" << endl;
    for (std::vector<pat::Muon>::const_iterator itMuon = input.selectedMuons.begin() ; itMuon != input.selectedMuons.end(); ++itMuon) {
      if (itMuon->pt() < min_pt) return false;
    }
    // cout << "min_pt check"  << endl;
    math::XYZTLorentzVector vec;
    vec = input.selectedMuonsLoose.at(0).p4() + input.selectedMuonsLoose.at(1).p4();
    double mass_mumu = vec.M();
    // cout << "mass_mumu " << mass_mumu << endl;
    if (mass_mumu < min_mass_ll || mass_mumu > max_mass_ll) {
      return false;
    }
    else {
      cutflow.EventSurvivedStep("DoubleMuon Control selection", input.weights.at("Weight"));
      return true;
    }
  }

  if (channel == "DoubleElectron") {
    cout << input.selectedElectronsLoose.size() << endl;

    if (input.selectedMuonsLoose.size() > 0 || input.selectedElectronsLoose.size()  != 2 || input.selectedTaus.size() > 0 || input.selectedPhotonsLoose.size() > 0) {
      return false;
    }
    cout << "== 2 loose Electrons check" << endl;
    if (input.selectedElectronsLoose.at(0).charge()*input.selectedElectronsLoose.at(1).charge() > 1) {
      return false;
    }
    cout << "opposite charge check" << endl;
    if (input.selectedElectrons.size() < 1 ) {
      return false;
    }
    cout << "min 1 tight Electron" << endl;
    for (std::vector<pat::Electron>::const_iterator itElec = input.selectedElectrons.begin() ; itElec != input.selectedElectrons.end(); ++itElec) {
      if (itElec->pt() < min_pt) return false;
    }
    cout << "min_pt(" << min_pt << ") check"  << endl;
    math::XYZTLorentzVector vec;
    vec = input.selectedElectronsLoose.at(0).p4() + input.selectedElectronsLoose.at(1).p4();
    double mass_elel = vec.M();
    cout << "mass_elel " << mass_elel << endl;
    if (mass_elel < min_mass_ll || mass_elel > max_mass_ll) {
      return false;
    }
    else {
      cutflow.EventSurvivedStep("DoubleElectron Control selection", input.weights.at("Weight"));
      return true;
    }
  }

  if (channel == "Photon") {
    if (input.selectedElectronsLoose.size() > 0 || input.selectedMuonsLoose.size() > 0 || input.selectedTaus.size() > 0 || input.selectedPhotonsLoose.size() != 1) {
      return false;
    }
    cout << min_pt << endl;
    if ( input.selectedPhotons.size() != 1) {
      return false;
    }
    cout << input.selectedPhotons.at(0).pt() << endl;
    if (input.selectedPhotons.at(0).pt() < min_pt) {
      return false;
    }

    else {
      cutflow.EventSurvivedStep("Photon+Jets Control selection", input.weights.at("Weight"));
      return true;
    }
  }


  else {
    throw cms::Exception("BadSelection")
        << "channel '" << channel << "' of DMControlSelection does not exist!\n"
        << "Please fix BoostedAnalyzer.channel parameter\n"
        << "Values are 'signal', 'SingleMuon', 'SingleElectron', 'DoubleMuon', 'DoubleElectron', 'Photon' ";
    return false;
  }
  return true;

}