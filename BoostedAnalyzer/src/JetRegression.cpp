#include "BoostedTTH/BoostedAnalyzer/interface/JetRegression.hpp"
using namespace std;



//PUBLIC
JetRegression::JetRegression(){
  //Read weightfile and stuff
  //cout << "JetRegression Constructor" << endl;
  name = "BDTG";
  weightfile = "/afs/desy.de/user/h/hmildner/public/regression_weights/weights_ttbar_quark/TMVARegression_BDTG.weights.xml";
  //treevars (17, (float)0.0);
  for (int i = 0; i < 17; i++){treevars.push_back(0);}
  //cout << treevars.size() << endl;
  //TMVA Stuff
  //cout << "reader def" << endl;
  reader = new TMVA::Reader();
  //reader->AddVariable();
 
  reader->AddVariable("Jet_pt",&treevars[0]); //y
  reader->AddVariable("Jet_corr",&treevars[1]); //y
  reader->AddVariable("rho",&treevars[2]); //y
  reader->AddVariable("Jet_eta",&treevars[3]); //y
  reader->AddVariable("Jet_mt",&treevars[4]); //y
  reader->AddVariable("Jet_leadTrackPt",&treevars[5]); //y
  reader->AddVariable("Jet_leptonPtRel",&treevars[6]); //y
  reader->AddVariable("Jet_leptonPt",&treevars[7]); //y
  reader->AddVariable("Jet_leptonDeltaR",&treevars[8]); //y
  reader->AddVariable("Jet_neHEF",&treevars[9]); //y
  reader->AddVariable("Jet_neEmEF",&treevars[10]); //y
  reader->AddVariable("Jet_chMult",&treevars[11]); //y
  reader->AddVariable("Jet_vtxPt",&treevars[12]); //y
  reader->AddVariable("Jet_vtxMass",&treevars[13]); //y
  reader->AddVariable("Jet_vtx3dL",&treevars[14]); //y
  reader->AddVariable("Jet_vtxNtrk",&treevars[15]); //y
  reader->AddVariable("Jet_vtx3deL",&treevars[16]); //y
 
  //cout << "Vor BookMVA" << endl;
  reader->BookMVA(name,weightfile);
  //cout << "Nach BookMVA" << endl;
  
  //Set Cuts
  epTCut = 5.0;
  eetaCut = 2.5;
  edxyCut = 0.5;
  edzCut = 1.0;

  mupTCut = 3.0;
  muetaCut = 2.4;
  mudxyCut = 0.5;
  mudzCut = 1.0;

  deltaR2Max = 0.4*0.4;
}

/*
void JetRegression::SetUpRegression(){
  //Set Up Class variables -> getInclusiveLeptons, matchLeptonswithJets

}*/

void JetRegression::evaluateRegression (const edm::Event& iEvent,
					const edm::EDGetTokenT< edm::View<pat::Electron> >& electronToken,
					const edm::EDGetTokenT< std::vector<pat::Muon> >& muonToken,
					const edm::EDGetTokenT <double>& rhoToken,
					const edm::EDGetTokenT< std::vector<pat::Jet> >& jetToken,
					const std::vector<pat::Jet>& Jets){

  //cout << "Evaluate Regression"  << endl;
  //Evaluate Regression
  edm::Handle< edm::View<pat::Electron> > h_electrons;
  edm::Handle< vector<pat::Muon> > h_muons;
  edm::Handle< vector<pat::Jet> > h_pfjets;
  edm::Handle<double> h_rho;
  
  iEvent.getByToken(electronToken, h_electrons);
  iEvent.getByToken(muonToken, h_muons);
  iEvent.getByToken(jetToken,h_pfjets);
  iEvent.getByToken(rhoToken,h_rho);

  //Set up stuff: set back vectors for inclusive leptons -> setinclusiveLeptons() -> matchLeptonswithJet()

  vector<pat::Electron> electrons;
  for (size_t i = 0; i < h_electrons->size(); ++i){
    electrons.push_back(h_electrons->at(i));
  }

  vector<pat::Muon> const &muons = *h_muons;
  vector<pat::Jet> const &pfjets = *h_pfjets;
  
  //Use alle pfjets for the lepton-jet-matching
  matchLeptonswithJets(electrons, muons, pfjets);
  //cout << "Laufe über Jets" << endl;
  //Toss stuff in Tree to evaluate
  //cout << *h_rho << endl;
  //cout << &treevars[2] << endl;
  //cout << treevars[2] << endl;
  treevars[2] = (float)(*h_rho);
  //cout << Jets.size() << endl;
  for (size_t i = 0; i<Jets.size(); i++) {
    if (Jets.at(i).hadronFlavour() == 5){
      cout << "Jet: " << i << "at " << &(Jets.at(i)) << " " << Jets.at(i).pt() << " "<< Jets.at(i).eta() << " " << Jets.at(i).mt() << " " << Jets.at(i).hadronFlavour()<< " " << Jets.at(i).charge() <<  endl;
      cout << "jet pT: " <<  Jets.at(i).pt() << endl;
      treevars[0] = Jets.at(i).pt();
      treevars[1] = Jets.at(i).pt() * Jets.at(i).jecFactor("Uncorrected");
      //    treevars[2] = Jets.at(i).jecFactor("Uncorrected");
      treevars[3] = Jets.at(i).eta();
      treevars[4] = Jets.at(i).mt();
      treevars[5] = leadingTrackpT(Jets.at(i));
      //cout << "LeptonJet zeug" << endl;
      if (!(ElectronJetPairs.empty()) || !(MuonJetPairs.empty())) {	
	vector<pat::Electron*> JetElectrons; 
	vector<pat::Muon*> JetMuons;
	for(map<pat::Electron*, const pat::Jet*>::iterator eit= ElectronJetPairs.begin(); eit != ElectronJetPairs.end(); eit++) {
	  cout << "e- " << eit->first << "| j " << eit->second << endl;
	  //	  cout << "e- " << eit->first << "| j " << eit->second << " " << (eit->second)->pt() << " "<< (eit->second)->eta() << " " << (eit->second)->mt() << " " << (eit->second)->hadronFlavour() << " " << (eit->second)->charge() << " " <<  endl;
	  if( eit->second == &(Jets.at(i))  ){
	    JetElectrons.push_back(eit->first);
	  }
	}
	//cout << MuonJetPairs.size() << endl;
	for(map<pat::Muon*, const pat::Jet*>::iterator mit= MuonJetPairs.begin(); mit != MuonJetPairs.end(); mit++) {
	  cout << "m- " << mit->first <<  "| j " << mit->second << endl;
	  //	  cout << "m- " << mit->first <<  endl;/*"| j " << mit->second << " " << (mit->second)->pt() << " "<< (mit->second)->eta() << " " << (mit->second)->mt() << " " << (mit->second)->hadronFlavour() << " " << (mit->second)->charge() << " " <<*/
	  
	  if( mit->second == &(Jets.at(i))  ){
	    JetMuons.push_back(mit->first);
	    }
	}  
	if(JetElectrons.size() > 0 || JetMuons.size() > 0){
	  if(JetElectrons.size() > 0){
	    treevars[6] = BoostedUtils::GetTLorentzVector(JetElectrons.at(0)->p4()).Perp(TVector3(Jets.at(i).p4().x(),Jets.at(i).p4().y(),Jets.at(i).p4().z()));
	    treevars[7] = JetElectrons.at(0)->pt();
	    treevars[8] = BoostedUtils::DeltaR(JetElectrons.at(0)->p4(), Jets.at(i).p4());
	  }
	  else{
	    treevars[6] = BoostedUtils::GetTLorentzVector(JetMuons.at(0)->p4()).Perp(TVector3(Jets.at(i).p4().x(),Jets.at(i).p4().y(),Jets.at(i).p4().z()));
	    treevars[7] = JetMuons.at(0)->pt();
	    treevars[8] = BoostedUtils::DeltaR(JetMuons.at(0)->p4(), Jets.at(i).p4());
	  }

	}
      }
      else {
	treevars[6] = -99;
	treevars[7] = -99;
	treevars[8] = -99;
      }

      treevars[9] = Jets.at(i).chargedEmEnergyFraction();
      treevars[10] = Jets.at(i).chargedHadronEnergyFraction();
      treevars[11] = Jets.at(i).neutralHadronEnergyFraction();
      treevars[12] = Jets.at(i).neutralEmEnergyFraction();
      treevars[13] = Jets.at(i).chargedMultiplicity();
      treevars[14] = sqrt(Jets.at(i).userFloat("vtxPx")*Jets.at(i).userFloat("vtxPx") + Jets.at(i).userFloat("vtxPy")*Jets.at(i).userFloat("vtxPy"));
      treevars[15] = Jets.at(i).userFloat("vtxMass");
      treevars[16] = Jets.at(i).userFloat("vtx3DVal");
      treevars[17] = Jets.at(i).userFloat("vtxNtracks");
      treevars[18] = Jets.at(i).userFloat("vtx3DSig");
      
      for(int l = 0; l < 17; l++) { cout <<  treevars[l] << " "; }
      cout << endl;
      
      float regressionresult = (reader->EvaluateRegression(name)).at(0);
      //write result of evaluation in Jet as userFloat("")
      cout << "New pT: " << regressionresult << endl;
    }
  }

  //Clear Members
  inclusiveElectrons.clear();
  inclusiveMuons.clear();
  ElectronJetPairs.clear();
  MuonJetPairs.clear();
  
}




//PRIVATE

void JetRegression::setInclusiveLeptons(const vector<pat::Electron>& electrons, 
					const vector<pat::Muon>& muons){
  //cout << "setInclusiveLeptons" << endl;
  float dxy;
  float dz;
  //Electrons
  //cout << "Electrons "<< electrons.size()  << endl;
  for(size_t i = 0; i<electrons.size(); i++) {
    if(electrons[i].gsfTrack().isNonnull()){
      dxy = electrons[i].gsfTrack()->dxy();
      dz = electrons[i].gsfTrack()->dz();
      if(electrons[i].pt() > epTCut && electrons[i].eta() < eetaCut && abs(dxy) < edxyCut && abs(dz) < edzCut){
	inclusiveElectrons.push_back(electrons[i]);
      }
    }
  }
  //Muons
  //cout << "Mouns " << muons.size() << endl;
  for (size_t i = 0; i<muons.size(); i++) {
    if(muons[i].track().isNonnull()){
      dxy = muons[i].track()->dxy();
      dz = muons[i].track()->dz();
      if(muons[i].pt() > mupTCut && muons[i].eta() < muetaCut && abs(dxy) < mudxyCut && abs(dz) < mudzCut){
	inclusiveMuons.push_back(muons[i]);
      }
    }
  }
  //cout << "nach Muon" << endl;
}

void JetRegression::matchLeptonswithJets(const vector<pat::Electron>& electrons, 
					 const vector<pat::Muon>& muons,
					 const vector<pat::Jet>& jets){
  //cout << "matchLeptonswithJet" << endl;
  setInclusiveLeptons(electrons, muons);
  //Match Electrons with Jets  
  for (size_t i = 0; i < inclusiveElectrons.size(); i++) {
    float deltaR2Min = 10000000;
    int jmatch = -1;
    for(size_t j = 0; j < jets.size(); j++) {
      float dRlj = BoostedUtils::DeltaR(inclusiveElectrons[i].p4(), jets[j].p4());
      float dR2lj = dRlj*dRlj;
      if (dR2lj < deltaR2Min){
	deltaR2Min = dR2lj;
	jmatch = j;
      }
    }
    if (deltaR2Min < deltaR2Max && jmatch != -1){
      ElectronJetPairs[&(inclusiveElectrons[i])] = &(jets[jmatch]);
    }
    else {
      ElectronJetPairs[&(inclusiveElectrons[i])] = 0;
    }
  }
  //Match Muons with Jets
  for (size_t i = 0; i < inclusiveMuons.size(); i++) {
    float deltaR2Min = 10000000;
    int jmatch = -1;
    for(size_t j = 0; j < jets.size(); j++) {
      float dRlj = BoostedUtils::DeltaR(inclusiveMuons[i].p4(), jets[j].p4());
      float dR2lj = dRlj*dRlj;
      if (dR2lj < deltaR2Min){
	deltaR2Min = dR2lj;
	jmatch = j;
      }
    }
    if (deltaR2Min < deltaR2Max && jmatch != -1){
      MuonJetPairs[&(inclusiveMuons[i])] = &(jets[jmatch]);
    }
    else {
      MuonJetPairs[&(inclusiveMuons[i])] = 0;
    }
  }
}

//print max(j.daughterPtrVector(), key = lambda x: x.pt() if x.charge()!=0 else 0).pt()

/*
float pTrel(const LorentzVector vec1, const LorentzVector vec2) {
  TLorentzVector o = TLorentzVector(vec1.Px(),vec2.Py(),vec1.Pz(),vec1.E());
  TVector3 a = TVector3(vec2.Px(),vec2.Py(),vec2.Pz());
  return o.Perp(a);
  }*/



float JetRegression::leadingTrackpT(const pat::Jet& jet) {
  float maxpt = 0;
  for(size_t i = 0; i < jet.daughterPtrVector().size(); i++) {
    if(jet.daughterPtrVector().at(i).get()->pt() > maxpt && jet.daughterPtrVector().at(i).get()->charge() != 0){
      maxpt = jet.daughterPtrVector().at(i).get()->pt();
    }
  }
  return maxpt;
}

