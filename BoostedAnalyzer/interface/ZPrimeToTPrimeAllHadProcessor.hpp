#ifndef BOOSTEDTTH_BOOSTEDANALYZER_ZPRIMETOTPRIMEALLHADPROCESSOR_HPP
#define BOOSTEDTTH_BOOSTEDANALYZER_ZPRIMETOTPRIMEALLHADPROCESSOR_HPP

#include <vector>
#include <cstdlib>




#include "BoostedTTH/BoostedAnalyzer/interface/TreeProcessor.hpp"
#include "BoostedTTH/BoostedAnalyzer/interface/BoostedUtils.hpp"
#include "BoostedTTH/BoostedAnalyzer/interface/InputCollections.hpp"
#include "BoostedTTH/BoostedAnalyzer/interface/VariableContainer.hpp"
#include "MiniAOD/MiniAODHelper/interface/TTM_SDM.h"
#include "MiniAOD/MiniAODHelper/interface/MiniAODHelper.h"
#include "MiniAOD/MiniAODHelper/interface/CSVHelper.h"

#include "TH1F.h"
/*class TreeProcessor{
public:
    TreeProcessor();
};
*/

class ZPrimeToTPrimeAllHadProcessor: public TreeProcessor{
  
public:
  
  ZPrimeToTPrimeAllHadProcessor();
  ~ZPrimeToTPrimeAllHadProcessor();
  
  
  void Init(const InputCollections& input,VariableContainer& var);
  void Process(const InputCollections& input,VariableContainer& var);
  
  void InitGenVars(VariableContainer& vars);
  void InitBasicRecoVars(VariableContainer& vars);
  void InitSignalandSidbandVars(VariableContainer& vars);
  void InitTaggingVars(VariableContainer& vars);
  void InitABCDVars(VariableContainer& vars);
  
  
  std::vector<pat::Jet> SelectSeparatedBottoms(std::vector<pat::Jet>& tops, std::vector<pat::Jet>& Ws, std::vector<pat::Jet>& bottoms); 
  math::XYZTLorentzVector TPrimeReconstructionWtb( std::vector<pat::Jet>& Ws,  std::vector<pat::Jet>& bottoms);
  math::XYZTLorentzVector ZPrimeReconstructionWtb( std::vector<pat::Jet>& tops, math::XYZTLorentzVector& TPrime);
  std::vector<pat::Jet> SelectfromTopSeparatedWs( std::vector<pat::Jet>& tops, std::vector<pat::Jet>& Ws);
  std::vector<pat::Jet> SelectfromWSeparatedTops( std::vector<pat::Jet>& tops, std::vector<pat::Jet>& Ws);
  void ZPrimeReconstructionWtbComplete(VariableContainer& vars,  std::vector<pat::Jet>& tops, std::vector<pat::Jet>& Ws,  std::vector<pat::Jet>& bottoms, std::vector<pat::Jet>& bottoms_anti, bool toptag, bool toptag_withbtag,  bool Wtag, bool toptag_anti, bool toptag_withbtag_anti,  bool Wtag_anti, std::string str_AK8_selected_first);
  void FillSignalSidebandVars(VariableContainer& vars, std::vector<pat::Jet>& tops, std::vector<pat::Jet>& Ws,  std::vector<pat::Jet>& bottoms,  math::XYZTLorentzVector TPrime,  math::XYZTLorentzVector ZPrime,  bool toptag, bool toptag_withbtag,  bool Wtag, bool bottomtag, bool toptag_anti, bool toptag_withbtag_anti,  bool Wtag_anti, std::string str_AK8_selected_first="");
  void FillZprimeVars(VariableContainer& vars, std::vector<pat::Jet>& tops, std::vector<pat::Jet>& Ws,  std::vector<pat::Jet>& bottoms,  math::XYZTLorentzVector Tprime,  math::XYZTLorentzVector Zprime,std::string string);
  
  bool Bottomtag(pat::Jet bottom,std::string workingpoint);
  bool Wtag(pat::Jet W,std::string workingpoint);
  bool Toptag(pat::Jet top,std::string workingpoint);
  bool Top_subbtag(pat::Jet top,std::string subjet_workingpoint);
  
private:

	TTMHelper foo;
        TH1F* histo_ABCD_QCD_MSD_top_nobtag;
        TH1F* histo_ABCD_QCD_MSD_top_withbtag;
        TH1F* histo_ABCD_QCD_MSD_W_nobtag;
        TH1F* histo_ABCD_QCD_MSD_W_withbtag;
        TH1F* histo_TTM_SDM;
//         sysType::sysType systype_;
//         const CSVHelper* csvReweighter_;
	//bool fEnv=setenv("BoostedTTH/BoostedAnalyzer/data/mistagrateweights/","CMSSW_SEARCH_PATH",0);
	//edm::FileInPath fQCD;
	//fQCD.init("../data/mistagrateweights/","BKG_QCD_false_negativ.root");
	//edm::FileInPath fSDM;
	//fSDM.init("../data/mistagrateweights/","BKG_QCD_SDM_Cut.root");*/

  
};

#endif