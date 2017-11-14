#include "tthAnalysis/HiggsToTauTau/interface/RecoJet.h"

#include <iomanip>

RecoJet::RecoJet(Double_t pt,
                 Double_t eta,
                 Double_t phi,
                 Double_t mass,
     Double_t jecUncertTotal,
                 Double_t BtagCSV,
		 Double_t BtagWeight,
                 Double_t QGDiscr,
		 Int_t heppyFlavour,
                 Int_t idx)
  : GenJet(pt, eta, phi, mass)
  , jecUncertTotal_(jecUncertTotal)
  , BtagCSV_(BtagCSV)
  , BtagWeight_(BtagWeight)
  , QGDiscr_(QGDiscr)
  , heppyFlavour_(heppyFlavour)
  , idx_(idx)
  , genLepton_(0)
  , genLepton_isOwner_(false)
  , genHadTau_(0)
  , genHadTau_isOwner_(false)
  , genJet_(0)
  , genJet_isOwner_(false)
{}

RecoJet::~RecoJet()
{
  if ( genLepton_isOwner_ ) delete genLepton_;
  if ( genHadTau_isOwner_ ) delete genHadTau_;
  if ( genJet_isOwner_    ) delete genJet_;
}

std::ostream& operator<<(std::ostream& stream, const RecoJet& jet)
{
  stream << " pT = " << jet.pt() << ","
	 << " eta = " << jet.eta() << "," 
	 << " phi = " << jet.phi() << "," 
	 << " mass = " << jet.mass() << "," 
	 << " CSV = " << jet.BtagCSV() << "," 
	 << " heppyFlavour = " << jet.heppyFlavour() << std::endl; 
  return stream;
}
