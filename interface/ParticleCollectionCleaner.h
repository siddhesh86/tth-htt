#ifndef tthAnalysis_HiggsToTauTau_ParticleCollectionCleaner_h
#define tthAnalysis_HiggsToTauTau_ParticleCollectionCleaner_h

#include <DataFormats/Math/interface/deltaR.h> // deltaR()

template <typename T>
class ParticleCollectionCleaner
{
public:
  ParticleCollectionCleaner(double dR = 0.4)
    : dR_(dR)
  {}
  ~ParticleCollectionCleaner() {}

  /**
   * @brief Select subset of particles not overlapping with any of the other particles passed as function argument
   * @return Collection of non-overlapping particles
   */
  template <typename Toverlap>
  std::vector<const T*> operator()(const std::vector<const T*>& particles,
                                   const std::vector<const Toverlap*>& overlaps) const
  {
    std::vector<const T*> cleanedParticles;
    for(const T* particle: particles)
    {
      bool isOverlap = false;
      for(const Toverlap* overlap: overlaps)
      {
        const double dRoverlap = deltaR(particle->eta(), particle->phi(), overlap->eta(), overlap->phi());
        if(dRoverlap < dR_)
        {
          isOverlap = true;
          break;
        }
      }
      if(! isOverlap)
      {
        cleanedParticles.push_back(particle);
      }
    }
    return cleanedParticles;
  }

  template <typename Toverlap,
            typename... Args>
  std::vector<const T*> operator()(const std::vector<const T*>& particles,
                                   const std::vector<const Toverlap*>& overlaps, Args... args) const
  {
    std::vector<const T*> cleanedParticles;
    for(const T* particle: particles)
    {
      bool isOverlap = false;
      for(const Toverlap* overlap: overlaps)
      {
        const double dRoverlap = deltaR(particle->eta(), particle->phi(), overlap->eta(), overlap->phi());
        if(dRoverlap < dR_)
        {
          isOverlap = true;
          break;
        }
      }
      if(! isOverlap)
      {
        cleanedParticles.push_back(particle);
      }
    }
    return this->operator()(cleanedParticles, args...);
  }

protected:
  double dR_;
};

#include "tthAnalysis/HiggsToTauTau/interface/RecoElectron.h"

typedef ParticleCollectionCleaner<RecoElectron> RecoElectronCollectionCleaner;

#include "tthAnalysis/HiggsToTauTau/interface/RecoMuon.h"

typedef ParticleCollectionCleaner<RecoMuon> RecoMuonCollectionCleaner;

#include "tthAnalysis/HiggsToTauTau/interface/RecoHadTau.h"

typedef ParticleCollectionCleaner<RecoHadTau> RecoHadTauCollectionCleaner;

#include "tthAnalysis/HiggsToTauTau/interface/RecoJet.h"

typedef ParticleCollectionCleaner<RecoJet> RecoJetCollectionCleaner;

#endif // tthAnalysis_HiggsToTauTau_ParticleCollectionCleaner_h

