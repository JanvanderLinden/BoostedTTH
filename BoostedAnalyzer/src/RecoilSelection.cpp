#include "BoostedTTH/BoostedAnalyzer/interface/RecoilSelection.hpp"

using namespace std;

RecoilSelection::RecoilSelection (const edm::ParameterSet& iConfig): RecoilSelection(iConfig.getParameter<double>("minRecoil"), iConfig.getParameter<double>("maxRecoil"))
{}
RecoilSelection::RecoilSelection (double minRecoil_, double maxRecoil_): minRecoil(minRecoil_), maxRecoil(maxRecoil_)
{}
RecoilSelection::~RecoilSelection () {}

void RecoilSelection::InitCutflow(Cutflow& cutflow) {
    selectionName = "Recoil > ";
    selectionName += std::to_string(minRecoil);
    selectionName += " and Recoil < ";
    selectionName += std::to_string(maxRecoil);
    cutflow.AddStep(selectionName);

    initialized = true;
}

bool RecoilSelection::IsSelected(const InputCollections& input, Cutflow& cutflow) {
    if (!initialized) cerr << "RecoilSelection not initialized" << endl;
    double met = input.correctedMET.corPt(pat::MET::Type1XY);
    for (std::vector<pat::Electron>::const_iterator itElec = input.selectedElectronsLoose.begin() ; itElec != input.selectedElectronsLoose.end(); ++itElec) {
        met -= itElec->pt();
    }
    for (std::vector<pat::Muon>::const_iterator itMuon = input.selectedMuonsLoose.begin() ; itMuon != input.selectedMuonsLoose.end(); ++itMuon) {
        met -= itMuon->pt();
    }
    for (std::vector<pat::Tau>::const_iterator itTau = input.selectedTaus.begin() ; itTau != input.selectedTaus.end(); ++itTau) {
        met -= itTau->pt();
    }
    for (std::vector<pat::Photon>::const_iterator itPhoton = input.selectedPhotonsLoose.begin() ; itPhoton != input.selectedPhotonsLoose.end(); ++itPhoton) {
        met -= itPhoton->pt();
    }
    double Recoil = met;
    cout << "Recoil="<< Recoil << endl;

    if (Recoil > maxRecoil || Recoil < minRecoil) {
        return false;
    }
    else {
        cutflow.EventSurvivedStep(selectionName , input.weights.at("Weight"));
        return true;
    }
}


