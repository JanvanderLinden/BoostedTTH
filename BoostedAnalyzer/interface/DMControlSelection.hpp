#ifndef DMControlSelection_HPP
#define DMControlSelection_HPP

#include <vector>

#include "BoostedTTH/BoostedAnalyzer/interface/Selection.hpp"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


class DMControlSelection: public Selection {

public:

	DMControlSelection(const edm::ParameterSet& iConfig);

	~DMControlSelection();
	void InitCutflow(Cutflow& cutflow);
	bool IsSelected(const InputCollections& input, Cutflow& cutflow);


private:
	std::string channel;
	double max_mt;
	double min_met;
	double min_pt;
	double min_mass_ll;
	double max_mass_ll;







};

#endif