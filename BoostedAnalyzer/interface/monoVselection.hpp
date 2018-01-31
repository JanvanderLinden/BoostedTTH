#ifndef monoVselection_HPP
#define monoVselection_HPP

#include <vector>

#include "BoostedTTH/BoostedAnalyzer/interface/Selection.hpp"


class monoVselection: public Selection {

public:

	monoVselection(const edm::ParameterSet& iConfig);
	monoVselection(double maxpt_, double maxeta_, double minPrunedMass_, double maxPrunedMass_, double tau21_);
	~monoVselection();
	void InitCutflow(Cutflow& cutflow);
	bool IsSelected(const InputCollections& input, Cutflow& cutflow);


private:
	double maxpt, maxeta, minPrunedMass, maxPrunedMass, tau21;

};

#endif