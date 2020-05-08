// -*- C++ -*-
//
// Package:    TopAnalysis/TopNtupler
// Class:      TopNtupler
// 
/**\class TopNtupler TopNtupler.cc TopAnalysis/TopNtupler/plugins/TopNtupler.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Ridhi Chawla
//         Created:  Thu, 19 Sep 2019 18:16:53 GMT
//
//

// system include files
#include <memory>
#include <vector>
#include <iostream>
#include <TLorentzVector.h>

// user include files
#include "TTree.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/Particle.h"
//#include "PhysicsTools/HepMCCandAlgos/interface/MCTruthHelper.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"

using namespace edm;
using namespace std;
using namespace reco;

// Typedef for Lorentz Vector as used in ntuples
typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > LV;

class TopNtupler : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit TopNtupler(const edm::ParameterSet&);
      ~TopNtupler();
      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   protected:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      void KalmanAnalysis(const edm::Event& iEvent, const edm::EventSetup& iSetup, const pat::Jet &j, const reco::Vertex &primVtx);
      pair<float,float> ctau(const TLorentzVector &p4, const reco::Vertex &fittedVertex, const reco::Vertex &primVtx, vector<pair<float,float>> &lsigma);
      //pair<float,float> ctau(const TLorentzVector &p4, const reco::Vertex &fittedVertex, const reco::Vertex &primVtx, pair<float,float> &lsigma);

      // Typedef for Lorentz Vector as used in ntuples
      //typedef ROOT::Math::LorentzVector<ROOT::Math::PtEtaPhiM4D<double> > LV;

      template<typename TYPE> LV getLV(const TYPE&) const;
      void Ancestors(const reco::Candidate* daughter, const reco::Candidate* ancestor);
      void FinalStates(const reco::Candidate* particle, const reco::Candidate* daughter);
      bool isTopMother(const reco::Candidate* daughter, const reco::Candidate* particle);
      void isFinal(const reco::Candidate* mother, const reco::Candidate* particle);
      bool isBHadron(int id);
      bool isBHadronMother(int id);

      // ----------member data ---------------------------
      edm::EDGetTokenT<edm::View<reco::GenParticle> > prunedGenToken_;
      edm::EDGetTokenT<edm::View<pat::PackedGenParticle> > packedGenToken_;

      edm::EDGetTokenT<reco::VertexCollection> vtxToken_;
      edm::EDGetTokenT<edm::View<pat::Jet> > jetToken_;

      // Ntuple and branch variables ...
      TTree* ntuple_;
      edm::Service<TFileService> fileService;
      //if(!fileService) throw edm::Exception(edm::errors::Configuration, "TFileService is not registered in cfg file");

      // General
      double count;
      unsigned long long eventNumber_;

      pair<float,float> l3dval;
      vector<pair<float,float>> l3d;

      pair<float,float> lsigmax;
      pair<float,float> lsigmay;
      pair<float,float> lsigmaz;
      vector<pair<float,float>> lsigma;

      // Vectors
      std::vector<double> pdgIdTOP_;
      std::vector<double> pdgIdTOPDAUG_;
      std::vector<double> pdgIdTop_;
      std::vector<double> pdgIdTopdaug_;
      std::vector<double> pdgIdWdaug_;
      std::vector<double> pdgIdMoth_;
      std::vector<double> pdgId_1BHadron_;
      std::vector<double> pdgId_2BHadron_;
      std::vector<double> pdgIdBHadronmoth_;
      std::vector<double> pdgIdBHadrondaug_;
      std::vector<double> pdgIdDMeson_;
      std::vector<double> pdgIdDMesondaug_;

      std::vector<LV> p4Top_;
      std::vector<LV> p4Topdaug_;
      std::vector<LV> p4Wdaug_;
      std::vector<LV> p4BHadron_;
      std::vector<LV> p4BHadronmoth_;
      std::vector<LV> p4BHadrondaug_;
      std::vector<LV> p4DMesondaug_;

      std::vector<double> id1_;
      std::vector<double> id2_;

      std::vector<double> pdgIdPf1_;
      std::vector<double> pdgIdPf2_;
      
      std::vector<double> jetPt_;
      std::vector<double> jetEta_;
      std::vector<double> kaonPt_;
      std::vector<double> kaonEta_;
      std::vector<double> pionPt_;
      std::vector<double> pionEta_;
      
      std::vector<double> d0TrkPt_;
      std::vector<double> d0TrkEta_;
      std::vector<double> d0TrkMass_;
      
      std::vector<double> dxy_;
      std::vector<double> dxyE_;

      std::vector<float> chi2_;
      std::vector<float> ndof_;
      std::vector<float> vtxProb_;
      std::vector<double> angleXY_;
};

TopNtupler::TopNtupler(const edm::ParameterSet& iConfig) :
  prunedGenToken_(consumes<edm::View<reco::GenParticle> >(iConfig.getParameter<edm::InputTag>("pruned"))),
  vtxToken_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices"))),
  jetToken_(consumes<edm::View<pat::Jet> >(iConfig.getParameter<edm::InputTag>("jets")))
{
  ntuple_ = fileService->make<TTree>("Ntuple", "Ntuple");

  ntuple_->Branch("eventNumber", &eventNumber_);

  ntuple_->Branch("pdgIdTOP", &pdgIdTOP_);
  ntuple_->Branch("pdgIdTOPDAUG", &pdgIdTOPDAUG_);
  ntuple_->Branch("pdgIdTop", &pdgIdTop_);
  ntuple_->Branch("pdgIdTopdaug", &pdgIdTopdaug_);
  ntuple_->Branch("pdgIdMoth", &pdgIdMoth_);
  ntuple_->Branch("pdgIdWdaug", &pdgIdWdaug_);
  ntuple_->Branch("pdgId_1BHadron", &pdgId_1BHadron_);
  ntuple_->Branch("pdgId_2BHadron", &pdgId_2BHadron_);
  ntuple_->Branch("pdgIdBHadronmoth", &pdgIdBHadronmoth_);
  ntuple_->Branch("pdgIdBHadrondaug", &pdgIdBHadrondaug_);
  ntuple_->Branch("pdgIdDMeson", &pdgIdDMeson_);
  ntuple_->Branch("pdgIdDMesondaug", &pdgIdDMesondaug_);

  ntuple_->Branch("p4Top", &p4Top_);
  ntuple_->Branch("p4Topdaug", &p4Topdaug_);
  ntuple_->Branch("p4Wdaug", &p4Wdaug_);
  ntuple_->Branch("p4BHadron", &p4BHadron_);
  ntuple_->Branch("p4BHadronmoth", &p4BHadronmoth_);
  ntuple_->Branch("p4BHadrondaug", &p4BHadrondaug_);
  ntuple_->Branch("p4DMesondaug", &p4DMesondaug_);

  ntuple_->Branch("id1", &id1_);
  ntuple_->Branch("id2", &id2_);

  ntuple_->Branch("pdgIdPf1", &pdgIdPf1_);
  ntuple_->Branch("pdgIdPf2", &pdgIdPf2_);
  
  ntuple_->Branch("jetPt", &jetPt_);
  ntuple_->Branch("jetEta", &jetEta_);
  ntuple_->Branch("kaonPt", &kaonPt_);
  ntuple_->Branch("kaonEta", &kaonEta_);
  ntuple_->Branch("pionPt", &pionPt_);
  ntuple_->Branch("pionEta", &pionEta_);

  ntuple_->Branch("d0TrkPt", &d0TrkPt_);
  ntuple_->Branch("d0TrkEta", &d0TrkEta_);
  ntuple_->Branch("d0TrkMass", &d0TrkMass_);

  ntuple_->Branch("dxy", &dxy_);
  ntuple_->Branch("dxyE", &dxyE_);

  ntuple_->Branch("l3d", &l3d);
  ntuple_->Branch("lsigma", &lsigma);

  ntuple_->Branch("chi2", &chi2_);
  ntuple_->Branch("ndof", &ndof_);
  ntuple_->Branch("vtxProb", &vtxProb_);
  ntuple_->Branch("angleXY", &angleXY_);
}

TopNtupler::~TopNtupler()
{
}

void TopNtupler::Ancestors(const reco::Candidate* daughter, const reco::Candidate* ancestor)
{
  cout<<"      mother: pdgId = "<<ancestor->pdgId()<<"   status = "<<ancestor->status()<<"   daughter: pdgId = "<<daughter->pdgId()<<"   status = "<<daughter->status()<<endl;
  
  for(size_t i=0; i<ancestor->numberOfMothers(); i++)
    Ancestors(ancestor, ancestor->mother(i));
}

void TopNtupler::FinalStates(const reco::Candidate* particle, const reco::Candidate* daughter)
{
  cout<<"  mother: pdgId = "<<particle->pdgId()<<"   status = "<<particle->status()<<"   daughter: pdgId = "<<daughter->pdgId()<<"   status = "<<daughter->status()<<endl;

  //cout<<"number of daughters = "<<particle->numberOfDaughters()<<endl;
  
  for(size_t i=0; i<daughter->numberOfDaughters(); i++)
    FinalStates(daughter, daughter->daughter(i));
}

bool TopNtupler::isTopMother(const reco::Candidate* daughter, const reco::Candidate* particle)
{
  cout<<"  mother: pdgId = "<<particle->pdgId()<<"   status = "<<particle->status()<<"   daughter: pdgId = "<<daughter->pdgId()<<"   status = "<<daughter->status()<<endl;
  
  //top is the mother
  if(abs(particle->pdgId()) == 6 && particle->status() == 62)
    {
      cout<<"    found b quark from top decay:"<<endl;
      return true;
    }

  //otherwise loop on mothers, if any and return true if top is found
  //for(size_t i=0; i<particle->numberOfMothers(); i++)
    //{
      //if(isTopMother(particle, particle->mother(i)))
        //return true;
    //}

  //if we did not return yet
  return false;
}

void TopNtupler::isFinal(const reco::Candidate* mother, const reco::Candidate* particle)
{
  //cout<<"        PdgId = "<<particle->pdgId()<<"   Status = "<<particle->status()<<endl;
  
  // final state particle
  if(particle->status() == 1)
    {
      //cout<<"        Mother: PdgId = "<<mother->pdgId()<<"   Status = "<<mother->status()<<"   Daughter: PdgId = "<<particle->pdgId()<<"   Status = "<<particle->status()<<endl;
      //cout<<"        PdgId = "<<particle->pdgId()<<"   Status = "<<particle->status()<<endl;
      
      pdgIdMoth_.push_back(mother->pdgId());
      pdgIdWdaug_.push_back(particle->pdgId());
    }

  // if not, keep looking
  for(size_t i=0; i<particle->numberOfDaughters(); i++)
    isFinal(particle, particle->daughter(i));
}

// ------------ method called for each event  ------------
void
TopNtupler::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  eventNumber_ = iEvent.id().event();
  if(eventNumber_ ==  17 || eventNumber_ == 27 || eventNumber_ == 40 || eventNumber_ == 84) {
  
  cout<<"Event "<<eventNumber_<<endl;

  // Gen Analysis
  edm::Handle<edm::View<reco::GenParticle>> pruned;
  iEvent.getByToken(prunedGenToken_,pruned); 

  //edm::Handle<edm::View<pat::PackedGenParticle>> packed;
  //iEvent.getByToken(packedGenToken_,packed);

  for (size_t i = 0; i < pruned->size(); i++)
    {
      const Candidate *bq = &(*pruned)[i];
          
      if((abs((*pruned)[i].pdgId()) == 5))
        {
          cout<<"PdgId = "<<bq->pdgId()<<"   Status = "<<bq->status()<<endl;
          cout<<"pt = "<<bq->pt()<<"   "<<"eta = "<<bq->eta()<<"   "<<"phi = "<<bq->phi()<<"   "<<"energy = "<<bq->energy()<<"   "<<"mass = "<<bq->mass()<<endl;
	  
	  cout<<"  found b quark mothers: "<<endl;
	  cout<<"  Number of mothers = "<<bq->numberOfMothers()<<endl;

	  for(size_t j = 0; j < bq->numberOfMothers(); j++)
            {
	      const Candidate *mother = bq->mother(j);
	      cout<<"  PdgId = "<<mother->pdgId()<<"   Status = "<<mother->status()<<endl;

	      if(mother != nullptr && isTopMother(bq, mother))
	        {
		  cout<<"    PdgId = "<<bq->pdgId()<<"   Status = "<<bq->status()<<endl;
		  cout<<"    pt = "<<bq->pt()<<"   "<<"eta = "<<bq->eta()<<"   "<<"phi = "<<bq->phi()<<"   "<<"energy = "<<bq->energy()<<"   "<<"mass = "<<bq->mass()<<endl;

                  cout<<"      found b quark daughters: "<<endl;
          
	          for(size_t k = 0; k < bq->numberOfDaughters(); k++)
                    {
                      const Candidate *daughter = bq->daughter(k);
                      cout<<"      PdgId = "<<daughter->pdgId()<<"   Status = "<<daughter->status()<<endl;
                      cout<<"      pt = "<<daughter->pt()<<"   "<<"eta = "<<daughter->eta()<<"   "<<"phi = "<<daughter->phi()<<"   "<<"energy = "<<daughter->energy()<<"   "<<"mass = "<<daughter->mass()<<endl;

                      cout<<"        found b daughters: "<<endl;

	              for(size_t l = 0; l < daughter->numberOfDaughters(); l++)
	                {
		          const Candidate *bHadron = daughter->daughter(l);
		          cout<<"        PdgId = "<<bHadron->pdgId()<<"   Status = "<<bHadron->status()<<endl;
                          cout<<"        pt = "<<bHadron->pt()<<"   "<<"eta = "<<bHadron->eta()<<"   "<<"phi = "<<bHadron->phi()<<"   "<<"energy = "<<bHadron->energy()<<"   "<<"mass = "<<bHadron->mass()<<endl;
		        
			  cout<<"          found b daughters: "<<endl;

			  for(size_t m = 0; m < bHadron->numberOfDaughters(); m++)
			    {
			      const Candidate *bHadron1 = bHadron->daughter(m);
			      cout<<"          PdgId = "<<bHadron1->pdgId()<<"   Status = "<<bHadron1->status()<<endl;
			      cout<<"          pt = "<<bHadron1->pt()<<"   "<<"eta = "<<bHadron1->eta()<<"   "<<"phi = "<<bHadron1->phi()<<"   "<<"energy = "<<bHadron1->energy()<<"   "<<"mass = "<<bHadron1->mass()<<endl;
			      cout<<"            found daughters: "<<endl;
			    
			      for(size_t n = 0; n < bHadron1->numberOfDaughters(); n++)
			        {
			          const Candidate *daugb = bHadron1->daughter(n);
				  cout<<"            PdgId = "<<daugb->pdgId()<<"   Status = "<<daugb->status()<<endl;
				  cout<<"            pt = "<<daugb->pt()<<"   "<<"eta = "<<daugb->eta()<<"   "<<"phi = "<<daugb->phi()<<"   "<<"energy = "<<daugb->energy()<<"   "<<"mass = "<<daugb->mass()<<endl;
			          cout<<"              found daughters: "<<endl;

				  for(size_t n = 0; n < daugb->numberOfDaughters(); n++)
				    {
				      const Candidate *daugd = daugb->daughter(n);
				      cout<<"              PdgId = "<<daugd->pdgId()<<"   Status = "<<daugd->status()<<endl;
				      cout<<"              pt = "<<daugd->pt()<<"   "<<"eta = "<<daugd->eta()<<"   "<<"phi = "<<daugd->phi()<<"   "<<"energy = "<<daugd->energy()<<"   "<<"mass = "<<daugd->mass()<<endl;
				    } // b daughter loop 4
				} // b daughter loop 3 
			    } // b daughter loop 2 
			} // b daughter loop 1
                    } // b quark daughter loop
		  //break;
	        } // is Top mother
	    } // mother loop
        } // b quark
    } // pruned gen loop

  cout<<""<<endl;

  // Vertices
  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByToken(vtxToken_, vertices);
  if(vertices->empty()) return; // skip the event if no PV found
  //const reco::Vertex &primVtx = vertices->front();
  
  // Jets
  edm::Handle<edm::View<pat::Jet>> jets;
  iEvent.getByToken(jetToken_,jets);

  for(auto j = jets->begin(); j!= jets->end(); ++j)
    {
      //KalmanAnalysis(iEvent,iSetup,*j,primVtx);
    }
  
  //cout<<""<<endl;
  
  ntuple_->Fill();

  // Clear variables

  // Event information
  eventNumber_ = 0;

  // Gen level information
  pdgIdTOP_.clear();
  pdgIdTOPDAUG_.clear();
  pdgIdTop_.clear();
  pdgIdTopdaug_.clear();
  pdgIdMoth_.clear();
  pdgIdWdaug_.clear();
  pdgId_1BHadron_.clear();
  pdgId_2BHadron_.clear();
  pdgIdBHadronmoth_.clear();
  pdgIdBHadrondaug_.clear();
  pdgIdDMeson_.clear();
  pdgIdDMesondaug_.clear();

  p4Top_.clear();
  p4Topdaug_.clear();
  p4Wdaug_.clear();
  p4BHadron_.clear();
  p4BHadronmoth_.clear();
  p4BHadrondaug_.clear();
  p4DMesondaug_.clear();

  id1_.clear();
  id2_.clear();

  pdgIdPf1_.clear();
  pdgIdPf2_.clear();
  
  jetPt_.clear();
  jetEta_.clear();
  kaonPt_.clear();
  kaonEta_.clear();
  pionPt_.clear();
  pionEta_.clear();

  d0TrkPt_.clear();
  d0TrkEta_.clear();
  d0TrkMass_.clear();

  dxy_.clear();
  dxyE_.clear();

  l3d.clear();
  lsigma.clear();

  chi2_.clear();
  ndof_.clear();
  vtxProb_.clear();
  angleXY_.clear();

}
} // event loop

template<typename TYPE>
LV TopNtupler::getLV(const TYPE& p4) const
{   
  return LV(p4.pt(), p4.eta(), p4.phi(), p4.mass());
}

bool TopNtupler::isBHadron(int id)
{
  if(id < 0) id*=-1;
  if(id < 500) return false;
  return (fmod(id/100,5.)==0.0 || id/1000==5);
}

bool TopNtupler::isBHadronMother(int id)
{
  if(id < 0) id*=-1;
  if(id < 500) return false;
  return (fmod(id/100,5.)==0.0);
}

void TopNtupler::KalmanAnalysis(const edm::Event& iEvent, const edm::EventSetup& iSetup, const pat::Jet &j, const reco::Vertex &primVtx)
{
  edm::ESHandle<TransientTrackBuilder> ttB;
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", ttB);
  TransientTrackBuilder thebuilder = *(ttB.product());

  //cout<<"jet: pt = "<<j.pt()<<"   eta = "<<j.eta()<<endl;
  
  if (j.pt() < 30 || fabs(j.eta()) > 2.4) return;
  
  //jetPt_.push_back(j.pt());
  //jetEta_.push_back(j.eta());
  
  cout<<"Jet: pt = "<<j.pt()<<"   eta = "<<j.eta()<<endl;
  
  unsigned int ndau = j.numberOfDaughters();
  cout<<"number of daughters = "<<ndau<<endl;

  if(ndau < 4) return;
  unsigned int maxdau = min(ndau, (unsigned)4);
  //cout<<"maxdau = "<<maxdau<<endl;

  for(unsigned int id1 = 0; id1 < ndau; ++id1)
    {
      for(unsigned int id2 = 0; id2 < maxdau; ++id2)
        {
	  if(id1 == id2) continue;
	  
	  // All candidates reconstructed by the ParticleFlow algorithm are saved in MiniAOD in the packedPFCandidates collection, using as dataformat pat::PackedCandidate
	  const pat::PackedCandidate &pf1 = dynamic_cast<const pat::PackedCandidate &>(*j.daughter(id1));
	  const pat::PackedCandidate &pf2 = dynamic_cast<const pat::PackedCandidate &>(*j.daughter(id2));
	  //cout<<"id1: "<<id1<<"   id2: "<<id2<<endl;
	  
	  //cout<<"  pf1 pdgId = "<<pf1.pdgId()<<endl;
	  //cout<<"  pf2 pdgId = "<<pf2.pdgId()<<endl;

	  if(pf1.pdgId()*pf2.pdgId() != -211*211) continue;

	  //cout<<"  Kaon: pt = "<<pf1.pt()<<"   eta = "<<pf1.eta()<<endl;
	  //cout<<"  Pion: pt = "<<pf2.pt()<<"   eta = "<<pf2.eta()<<endl;
	  
	  if(pf1.pt() < 1.0 || pf2.pt() < 5.0) continue; //K pT > 1 GeV, pi pT > 5 GeV
	  if(fabs(pf1.eta()) > 2.4 || fabs(pf2.eta())> 2.4) continue;
	  
	  TLorentzVector p_track1, p_track2;
	  const float gMassK(0.4937);
	  const float gMassPi(0.1396);
		            
	  p_track1.SetPtEtaPhiM(pf1.pt(), pf1.eta(), pf1.phi(), gMassK);
	  p_track2.SetPtEtaPhiM(pf2.pt(), pf2.eta(), pf2.phi(), gMassPi);

	  //cout<<"  High purity"<<"  pf1 = "<<pf1.trackHighPurity()<<"   pf2 = "<<pf2.trackHighPurity()<<endl;
	  
	  if(!pf1.trackHighPurity());
	  if(!pf2.trackHighPurity());

	  //cout<<"  Track details"<<"  pf1 = "<<pf1.hasTrackDetails()<<"   pf2 = "<<pf2.hasTrackDetails()<<endl;

	  if(!pf1.hasTrackDetails()) continue;
	  if(!pf2.hasTrackDetails()) continue;

	  double pt12 = (p_track1+p_track2).Pt();
	  double eta12 = (p_track1+p_track2).Eta();
	  double mass12 = (p_track1+p_track2).M();
	  
	  //cout<<"  mass = "<<mass12<<endl;

	  if(mass12<1.7 || mass12>2.0) continue;
	  
	  cout<<"id1: "<<id1<<"   id2: "<<id2<<endl;

	  cout<<"  pf1 pdgId = "<<pf1.pdgId()<<endl;
	  cout<<"  pf2 pdgId = "<<pf2.pdgId()<<endl;
	  
	  cout<<"  kaon: pt = "<<pf1.pt()<<"   eta = "<<pf1.eta()<<endl;
	  cout<<"  pion: pt = "<<pf2.pt()<<"   eta = "<<pf2.eta()<<endl;
	  
	  cout<<"  mass = "<<mass12<<endl;
	  
	  id1_.push_back(id1);
	  id2_.push_back(id2);
	  
	  pdgIdPf1_.push_back(pf1.pdgId());
	  pdgIdPf2_.push_back(pf2.pdgId());
	  
	  jetPt_.push_back(j.pt());
	  jetEta_.push_back(j.eta());
	  
	  kaonPt_.push_back(pf1.pt());
	  kaonEta_.push_back(pf1.eta());
	  pionPt_.push_back(pf2.pt());
	  pionEta_.push_back(pf2.eta());
	  
	  d0TrkPt_.push_back(pt12);
	  d0TrkEta_.push_back(eta12);
	  d0TrkMass_.push_back(mass12);
	  
	  // Vertex fitting
	  // the task of Vertex Fitting, given a set of tracks, is to compute the best estimate of the vertex parameters (position, covariance matrix, constrained track parameters and their covariances) as well as indicators of the success of the fit (total chi^2, number of degrees of freedom, track weights)
	  // reco::Track of the candidate is provided by the pseudoTrack() method
	  const reco::Track* trk1 = &pf1.pseudoTrack();
	  const reco::Track* trk2 = &pf2.pseudoTrack();

	  // reco::Track is unsuitable since it does not have access to the magnetic field, which prevents one estimating the track parameters at arbitrary points along its trajectory
	  // reco::TransientTrack has a pointer to the magnetic field
	  reco::TransientTrack trTrack1 = thebuilder.build(*trk1);
	  reco::TransientTrack trTrack2 = thebuilder.build(*trk2);

	  vector<reco::TransientTrack> trTrackVec;
	  trTrackVec.push_back(trTrack1);
	  trTrackVec.push_back(trTrack2);
	  
	  // a vertex fitter that uses the Kalman filter algorithm
	  KalmanVertexFitter kvf(true); 
	  //TransientVertex tv;
	  TransientVertex tv = kvf.vertex(trTrackVec);
	  float vtxProb(0);
	  //try { tv = kvf.vertex(trTrackVec); }
	  //catch (...) { cout<<"Problem computing vertex"<<endl; continue; }
	  reco::Vertex fittedVertex = tv;

	  cout<<"  fitted vertex = "<<fittedVertex.x()<<"   "<<fittedVertex.y()<<endl;
	  cout<<"  primVtx = "<<primVtx.x()<<"   "<<primVtx.y()<<endl;
	  
	  double dxy = sqrt(pow(fittedVertex.x()-primVtx.x(),2) + pow(fittedVertex.y()-primVtx.y(),2));
	  double dxyE = sqrt(pow(fittedVertex.xError(),2) + pow(primVtx.xError(),2) + pow(fittedVertex.yError(),2) + pow(primVtx.yError(),2));

	  dxy_.push_back(dxy);
	  dxyE_.push_back(dxyE);
	  
	  cout<<"  dxy = "<<dxy<<"   dxyE = "<<dxyE<<endl;
	  
	  TLorentzVector d0 = (p_track1+p_track2);
	  //lsigma = {};

	  //l3dval = ctau(d0, fittedVertex, primVtx, lsigma);
	  l3d.push_back(ctau(d0, fittedVertex, primVtx, lsigma));
	  
	  //cout<<"  ctau = "<<l3dval.first<<"   "<<l3dval.second<<endl;

	  vtxProb = TMath::Prob(tv.totalChiSquared(), tv.degreesOfFreedom());
          
	  chi2_.push_back(tv.totalChiSquared());
	  ndof_.push_back(tv.degreesOfFreedom());
	  vtxProb_.push_back(vtxProb);
	  
	  cout<<"  chi2 = "<<tv.totalChiSquared()<<"  ndof = "<<tv.degreesOfFreedom()<<endl;
	  cout<<"  vertex prob = "<<vtxProb<<endl;

	  //Opening angle with respect to primary vertex
	  GlobalPoint vtxPos(fittedVertex.x(), fittedVertex.y(), fittedVertex.z()); // GlobalPoint represent a 3-dim space point, with a float precision, in the CMS global coordinate system
	  auto_ptr<TrajectoryStateClosestToPoint> trajPlus;
	  auto_ptr<TrajectoryStateClosestToPoint> trajMins;
	  vector<reco::TransientTrack> theRefTracks;
	  if (tv.hasRefittedTracks())
	    theRefTracks = tv.refittedTracks();

	  reco::TransientTrack* thePositiveRefTrack = nullptr;
	  reco::TransientTrack* theNegativeRefTrack = nullptr;
	  for (vector<reco::TransientTrack>::iterator iTrack = theRefTracks.begin(); iTrack != theRefTracks.end(); ++iTrack)
	    {
	      if (iTrack->track().charge() > 0.) thePositiveRefTrack = &*iTrack;
	      else if (iTrack->track().charge() < 0.) theNegativeRefTrack = &*iTrack;
	    }
	  
	  if (thePositiveRefTrack == nullptr || theNegativeRefTrack == nullptr) continue;
	  trajPlus.reset(new TrajectoryStateClosestToPoint(thePositiveRefTrack->trajectoryStateClosestToPoint(vtxPos)));
	  trajMins.reset(new TrajectoryStateClosestToPoint(theNegativeRefTrack->trajectoryStateClosestToPoint(vtxPos)));

	  if (trajPlus.get() == nullptr || trajMins.get() == nullptr || !trajPlus->isValid() || !trajMins->isValid()) continue;
	  GlobalVector positiveP(trajPlus->momentum());
	  GlobalVector negativeP(trajMins->momentum());
	  GlobalVector totalP(positiveP + negativeP);

	  double dx = fittedVertex.x()-(primVtx.position().x());
	  double dy = fittedVertex.y()-(primVtx.position().y());
	  double px = totalP.x();
	  double py = totalP.y();
	  double angleXY = (dx*px+dy*py)/(sqrt(dx*dx+dy*dy)*sqrt(px*px+py*py));
	  
	  angleXY_.push_back(angleXY);
	  
	  cout<<"  angle = "<<angleXY<<endl;

    	  // D*
    	  for (unsigned int id3 = 0; id3 < ndau; ++id3)
      	    {
    	      if(id1 == id3) continue;
	      if(id2 == id3) continue;

	      const pat::PackedCandidate &pf3 = dynamic_cast<const pat::PackedCandidate &>(*j.daughter(id3));
              if(pf1.pdgId()*pf3.pdgId() != -211*211) continue;

	      if(!pf3.trackHighPurity());

	      TLorentzVector p_track3;
	      p_track3.SetPtEtaPhiM(pf3.pt(), pf3.eta(), pf3.phi(), gMassPi);

	      float mass123 = (p_track1+p_track2+p_track3).M();
	      if (mass123<1.9 || mass123>2.2) continue;
            } // D*
        } // D0 id2
    } // D0 id1
  cout<<""<<endl;
} // KalmanAnalysis

pair<float,float> TopNtupler::ctau(const TLorentzVector &p4, const reco::Vertex &fittedVertex,
                           const reco::Vertex &primVtx, vector<pair<float,float>> &lsigma)
{
  float sigmax = sqrt(pow(fittedVertex.xError(),2) + pow(primVtx.xError(),2));
  float sigmay = sqrt(pow(fittedVertex.yError(),2) + pow(primVtx.yError(),2));
  float sigmaz = sqrt(pow(fittedVertex.zError(),2) + pow(primVtx.zError(),2));

  //cout<<"  sigma: x = "<<sigmax<<"   y = "<<sigmay<<"   z = "<<sigmaz<<endl;
  
  float sigmaL3D = 1.0/sqrt(pow((p4.Px()/p4.M())/sigmax, 2) + 
  			    pow((p4.Py()/p4.M())/sigmay, 2) +
			    pow((p4.Pz()/p4.M())/sigmaz, 2));
  
  float L3D = (p4.Px()/p4.M()) * pow(sigmaL3D/sigmax, 2) * (fittedVertex.x()-primVtx.x()) +
              (p4.Py()/p4.M()) * pow(sigmaL3D/sigmay, 2) * (fittedVertex.y()-primVtx.y()) +
	      (p4.Pz()/p4.M()) * pow(sigmaL3D/sigmaz, 2) * (fittedVertex.z()-primVtx.z());

  lsigmax = pair<float,float>((p4.Px()/p4.M()) * pow(sigmaL3D/sigmax,2) * (fittedVertex.x()-primVtx.x()), sigmax);
  lsigmay = pair<float,float>((p4.Py()/p4.M()) * pow(sigmaL3D/sigmay,2) * (fittedVertex.y()-primVtx.y()), sigmay);
  lsigmaz = pair<float,float>((p4.Pz()/p4.M()) * pow(sigmaL3D/sigmaz,2) * (fittedVertex.z()-primVtx.z()), sigmaz);

  cout<<"  sigma x = "<<lsigmax.first<<"   "<<lsigmax.second<<endl;
  cout<<"  sigma y = "<<lsigmay.first<<"   "<<lsigmay.second<<endl;
  cout<<"  sigma z = "<<lsigmaz.first<<"   "<<lsigmaz.second<<endl;
  
  lsigma.push_back(pair<float,float>((p4.Px()/p4.M()) * pow(sigmaL3D/sigmax,2) * (fittedVertex.x()-primVtx.x()), sigmax));
  lsigma.push_back(pair<float,float>((p4.Py()/p4.M()) * pow(sigmaL3D/sigmay,2) * (fittedVertex.y()-primVtx.y()), sigmay));
  lsigma.push_back(pair<float,float>((p4.Pz()/p4.M()) * pow(sigmaL3D/sigmaz,2) * (fittedVertex.z()-primVtx.z()), sigmaz));

  return pair<float,float>(L3D,sigmaL3D);
}

// ------------ method called once each job just before starting event loop  ------------
void 
TopNtupler::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TopNtupler::endJob() 
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TopNtupler::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TopNtupler);
