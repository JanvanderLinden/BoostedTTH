////////////////////////////////////////////////////////////////////////////////
//
// SFJetProducer
// -----------------------
//
// For a description of the Subjet/Filter algorithm, see e.g.
// http://arXiv.org/abs/0802.2470 
//
// This implementation is largely based on fastjet_boosted_higgs.cc provided
// with the fastjet package.
//
// If you did, you know that the algorithm produces a 'fatjet', two
// associated 'subjets', and two or three associated 'filterjets'. This producer
// will therefore write three corresponding jet collections. The association
// between a fatjet and its subjets and filterjets is done by making all (!)
// of them daughters of the fatjet, such that the first two daughter jets always
// correspond to the subjets, while all remaining correspond to the filterjets.
//
// The real work is done in RecoJets/JetAlgorithms/src/SubjetFilterjetAlgorithm.cc
//       
// see: https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideSubjetFilterJetProducer
//
//                       David Lopes-Pegna                 <david.lopes@cern.ch>
//            25/11/2009 Philipp Schieferdecker <philipp.schieferdecker@cern.ch>
////////////////////////////////////////////////////////////////////////////////


#include "BoostedTTH/BoostedProducer/plugins/SFJetProducer.h"

#include "RecoJets/JetProducers/interface/JetSpecific.h"
#include "FWCore/Framework/interface/MakerMacros.h"


using namespace std;


////////////////////////////////////////////////////////////////////////////////
// construction / destruction
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
SFJetProducer::SFJetProducer(const edm::ParameterSet& iConfig)
  : VirtualJetProducer(iConfig),
    alg_(iConfig.getParameter<string>	("@module_label"),
	 iConfig.getParameter<bool> 	("verbose"),
	 iConfig.getParameter<bool>     ("doAreaFastjet"),
	 iConfig.getParameter<double>   ("rParam"),
	 iConfig.getParameter<unsigned> ("nFatMax"),
	 iConfig.getParameter<double>   ("jetPtMin"),
	 iConfig.getParameter<double>   ("centralEtaCut"),
	 iConfig.getParameter<double>   ("massDropCut"),
	 iConfig.getParameter<double>   ("rFilt"),
	 iConfig.getParameter<double>   ("asymmCut"),
	 iConfig.getParameter<bool>     ("asymmCutLater"),
	 iConfig.getParameter<double>   ("filterJetPtMin")
	 ),
    nSubJetAlgo1_(
		  1, // N
		  fastjet::Njettiness::AxesMode::kt_axes,
		  1., //beta
		  iConfig.getParameter<double>	("rParam") // cone size R0
		  ),
  nSubJetAlgo2_(
		2, // N
		fastjet::Njettiness::AxesMode::kt_axes,
		1., //beta
		iConfig.getParameter<double>	("rParam") // cone size R0
		),
  
  nSubJetAlgo3_(
		3, // N
		fastjet::Njettiness::AxesMode::kt_axes,
		1., //beta
		iConfig.getParameter<double>	("rParam") // cone size R0
		),
  
  nSubJetAlgo4_(
		4, // N
		fastjet::Njettiness::AxesMode::kt_axes,
		1., //beta
		iConfig.getParameter<double>	("rParam") // cone size R0
		)
{
	produces<reco::BasicJetCollection>("fatjets");
	makeProduces(moduleLabel_,"subjets");
	makeProduces(moduleLabel_,"filterjets");

	for(size_t N=1;N<=4;N++){
		char tau_char[50];
		sprintf (tau_char, "tau%i", (int) N);
		std::string tau_string = tau_char;

		produces<std::vector<double> >(tau_string);
	}
}


//______________________________________________________________________________
SFJetProducer::~SFJetProducer()
{

}


////////////////////////////////////////////////////////////////////////////////
// implementation of member functions
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
void SFJetProducer::produce(edm::Event& iEvent,
				      const edm::EventSetup& iSetup)
{
  VirtualJetProducer::produce(iEvent,iSetup);
}


//______________________________________________________________________________
void SFJetProducer::endJob()
{
  cout<<alg_.summary()<<endl;
}


//______________________________________________________________________________
void SFJetProducer::runAlgorithm(edm::Event& iEvent,
					   const edm::EventSetup& iSetup)
{ 
  if ( !doAreaFastjet_ && !doRhoFastjet_) {
    fjClusterSeq_ = ClusterSequencePtr( new fastjet::ClusterSequence( fjInputs_, *fjJetDefinition_ ) );
  } else if (voronoiRfact_ <= 0) {
    fjClusterSeq_ = ClusterSequencePtr( new fastjet::ClusterSequenceArea( fjInputs_, *fjJetDefinition_ , *fjAreaDefinition_ ) );
  } else {
    fjClusterSeq_ = ClusterSequencePtr( new fastjet::ClusterSequenceVoronoiArea( fjInputs_, *fjJetDefinition_ , fastjet::VoronoiAreaSpec(voronoiRfact_) ) );
  }
  
  alg_.run(fjInputs_, fjCompoundJets_, fjClusterSeq_);
  for(uint i=0; i<fjCompoundJets_.size();i++){
    // run nsubjettiness algorithms on fat jet
    double n1= nSubJetAlgo1_(fjCompoundJets_[i].hardJet());
    double n2= nSubJetAlgo2_(fjCompoundJets_[i].hardJet());
    double n3= nSubJetAlgo3_(fjCompoundJets_[i].hardJet());
    double n4= nSubJetAlgo4_(fjCompoundJets_[i].hardJet());
    nSjn1_.push_back(n1);
    nSjn2_.push_back(n2);
    nSjn3_.push_back(n3);
    nSjn4_.push_back(n4);
  }

}


//______________________________________________________________________________
void SFJetProducer::inputTowers()
{
  fjCompoundJets_.clear();
  nSjn1_.clear();
  nSjn2_.clear();
  nSjn3_.clear();
  nSjn4_.clear();
  VirtualJetProducer::inputTowers();
}


//______________________________________________________________________________
void SFJetProducer::output(edm::Event& iEvent,
				     edm::EventSetup const& iSetup)
{
  // Write jets and constitutents. Will use fjCompoundJets_. 
  switch( jetTypeE ) {
  case JetType::CaloJet :
    writeCompoundJets<reco::CaloJet>( iEvent, iSetup );
    break;
  case JetType::PFJet :
    writeCompoundJets<reco::PFJet>( iEvent, iSetup );
    break;
  case JetType::GenJet :
    writeCompoundJets<reco::GenJet>( iEvent, iSetup );
    break;
  case JetType::BasicJet :
    writeCompoundJets<reco::BasicJet>( iEvent, iSetup );
    break;
  default:
    edm::LogError("InvalidInput")<<" invalid jet type in SFJetProducer\n";
    break;
  };
  
}


//______________________________________________________________________________
template< class T>
void SFJetProducer::writeCompoundJets(edm::Event& iEvent,
						const edm::EventSetup& iSetup)
{
  auto_ptr<reco::BasicJetCollection> fatJets( new reco::BasicJetCollection() );
  auto_ptr<vector<T> >  subJets( new vector<T>() );
  auto_ptr<vector<T> >  filterJets( new vector<T>() );

  edm::OrphanHandle< vector<T> > subJetsAfterPut;
  edm::OrphanHandle< vector<T> > filterJetsAfterPut;
  
  vector< vector<int> > subIndices(fjCompoundJets_.size());
  vector< vector<int> > filterIndices(fjCompoundJets_.size());
  
  vector<math::XYZTLorentzVector> p4FatJets;
  vector<double>                  areaFatJets;
  
  vector<CompoundPseudoJet>::const_iterator itBegin(fjCompoundJets_.begin());
  vector<CompoundPseudoJet>::const_iterator itEnd(fjCompoundJets_.end());
  vector<CompoundPseudoJet>::const_iterator it(itBegin);
  
  for (;it!=itEnd;++it) {

    int jetIndex = it-itBegin;
    fastjet::PseudoJet fatJet = it->hardJet();
    p4FatJets.push_back(math::XYZTLorentzVector(fatJet.px(),fatJet.py(),
						fatJet.pz(),fatJet.e()));
    areaFatJets.push_back(it->hardJetArea());
    
    vector<CompoundPseudoSubJet>::const_iterator itSubBegin(it->subjets().begin());
    vector<CompoundPseudoSubJet>::const_iterator itSubEnd(it->subjets().end());
    vector<CompoundPseudoSubJet>::const_iterator itSub(itSubBegin);
    
    for (; itSub!=itSubEnd;++itSub) {
      int subJetIndex = itSub-itSubBegin;
      fastjet::PseudoJet fjSubJet = itSub->subjet();
      math::XYZTLorentzVector p4SubJet(fjSubJet.px(),fjSubJet.py(),
				       fjSubJet.pz(),fjSubJet.e());
      reco::Particle::Point point(0,0,0);
      
      vector<reco::CandidatePtr> subJetConstituents;
      const vector<int>& subJetConstituentIndices = itSub->constituents();
      vector<int>::const_iterator itIndexBegin(subJetConstituentIndices.begin());
      vector<int>::const_iterator itIndexEnd(subJetConstituentIndices.end());
      vector<int>::const_iterator itIndex(itIndexBegin);
      for (;itIndex!=itIndexEnd;++itIndex)
	if ((*itIndex) < static_cast<int>(inputs_.size())) 
	  subJetConstituents.push_back(inputs_[*itIndex]);
      
      T subJet;
      reco::writeSpecific(subJet,p4SubJet,point,subJetConstituents,iSetup);
      subJet.setJetArea(itSub->subjetArea());
      
      if (subJetIndex<2) {
			subIndices[jetIndex].push_back(subJets->size());
			subJets->push_back(subJet);
      }
      else {
			filterIndices[jetIndex].push_back(filterJets->size());
			filterJets->push_back(subJet);
      }
    }
  }
  
  subJetsAfterPut    = iEvent.put(subJets,   "subjets");
  filterJetsAfterPut = iEvent.put(filterJets,"filterjets");
  
  vector<math::XYZTLorentzVector>::const_iterator itP4Begin(p4FatJets.begin());
  vector<math::XYZTLorentzVector>::const_iterator itP4End(p4FatJets.end());
  vector<math::XYZTLorentzVector>::const_iterator itP4(itP4Begin);
  for (;itP4!=itP4End;++itP4) {
    int fatIndex = itP4-itP4Begin;
    vector<int>& fatToSub    = subIndices[fatIndex];
    vector<int>& fatToFilter = filterIndices[fatIndex];

    vector<reco::CandidatePtr> i_fatJetConstituents;

    vector<int>::const_iterator itSubBegin(fatToSub.begin());
    vector<int>::const_iterator itSubEnd(fatToSub.end());
    vector<int>::const_iterator itSub(itSubBegin);
    for(;itSub!=itSubEnd;++itSub) {
      reco::CandidatePtr candPtr(subJetsAfterPut,(*itSub),false);
      i_fatJetConstituents.push_back(candPtr);
    }

    vector<int>::const_iterator itFilterBegin(fatToFilter.begin());
    vector<int>::const_iterator itFilterEnd(fatToFilter.end());
    vector<int>::const_iterator itFilter(itFilterBegin);
    for(;itFilter!=itFilterEnd;++itFilter) {
      reco::CandidatePtr candPtr(filterJetsAfterPut,(*itFilter),false);
      i_fatJetConstituents.push_back(candPtr);
    }

    reco::Particle::Point point(0,0,0);
	 
    fatJets->push_back(reco::BasicJet((*itP4),point,i_fatJetConstituents));
    fatJets->back().setJetArea(areaFatJets[fatIndex]);
  }
  
  iEvent.put(fatJets,"fatjets");
  for(size_t N=1;N<=4;N++){
    auto_ptr< vector<double> > NSubjettiness(new vector<double>());
    if(N==1)*NSubjettiness = nSjn1_;
    if(N==2)*NSubjettiness = nSjn2_;
    if(N==3)*NSubjettiness = nSjn3_;
    if(N==4)*NSubjettiness = nSjn4_;
    
    char tau_char[50];
    sprintf (tau_char, "tau%i", (int) N);
    std::string tau_string = tau_char;
    
    iEvent.put(NSubjettiness, tau_string);
  }
}


////////////////////////////////////////////////////////////////////////////////
// DEFINE THIS AS A CMSSW FWK PLUGIN
////////////////////////////////////////////////////////////////////////////////

DEFINE_FWK_MODULE(SFJetProducer);
