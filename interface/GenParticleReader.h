#ifndef tthAnalysis_HiggsToTauTau_GenParticleReader_h
#define tthAnalysis_HiggsToTauTau_GenParticleReader_h

#include "tthAnalysis/HiggsToTauTau/interface/GenParticle.h" // GenParticle
#include "tthAnalysis/HiggsToTauTau/interface/ReaderBase.h" // ReaderBase

#include <Rtypes.h> // Int_t, Double_t
#include <TTree.h> // TTree

#include <string>
#include <vector>
#include <map>

class GenParticleReader
  : public ReaderBase
{
 public:
  GenParticleReader();
  GenParticleReader(const std::string& branchName_nParticles, const std::string& branchName_particles);
  ~GenParticleReader();

  /**
   * @brief Call tree->SetBranchAddress for all GenParticle branches
   */
  void setBranchAddresses(TTree* tree) override;

  /**
   * @brief Read branches from tree and use information to fill collection of GenParticle objects
   * @return Collection of GenParticle objects
   */
  std::vector<GenParticle> read() const;

 protected:
 /**
   * @brief Initialize names of branches to be read from tree
   */
  void setBranchNames();

  const int max_nParticles_;
  std::string branchName_nParticles_;
  std::string branchName_particles_;

  std::string branchName_particle_pt_;
  std::string branchName_particle_eta_;
  std::string branchName_particle_phi_;
  std::string branchName_particle_mass_;
  std::string branchName_particle_pdgId_;
  std::string branchName_particle_charge_;

  Int_t nParticles_;
  Float_t* particle_pt_;
  Float_t* particle_eta_;
  Float_t* particle_phi_;
  Float_t* particle_mass_;
  Int_t* particle_pdgId_;
  Float_t* particle_charge_;

  // CV: make sure that only one GenParticleReader instance exists for a given branchName,
  //     as ROOT cannot handle multiple TTree::SetBranchAddress calls for the same branch.
  static std::map<std::string, int> numInstances_;
  static std::map<std::string, GenParticleReader*> instances_;
};

#endif // tthAnalysis_HiggsToTauTau_GenParticleReader_h

