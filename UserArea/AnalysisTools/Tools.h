//Tools.h HEADER FILE

#ifndef Tools_h
#define Tools_h


// Root include files
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TMath.h"
#include "TObject.h"
#include "TClonesArray.h"
#include "TRefArray.h"
#include "TRef.h"
#include "TH1.h"
#include "TBits.h"
#include "TRandom3.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TMatrixT.h"
#include "TMatrixTSym.h"
#include "TVectorT.h"
#include "TSystem.h"

// Include files (C & C++ libraries)
#include <iostream>
#include <vector>
#include <string.h>

#include "ReadNtuple.h"

using namespace std;
class Tools{
 private:
  ReadNtuple *Ntp;
  TFile *newfile;
  TTree *SkimmedTree;
  int nbytes;
  int jentry;
  int nb;
  bool copyTree;

  int currentEvent;

  // Ntuple Access Functions
  virtual void Branch_Setup(TString B_Name, int type);
  virtual void Branch_Setup(){}


  bool                                isInit;

  // muon correction related objects


 public:
  // Constructor
  Tools(std::vector<TString> RootFiles);

  // Destructor
  virtual ~Tools() ;

  // Event initializer
  void InitEvent();




  // Ntuple Access Functions 
  virtual Int_t Get_Entries();
  virtual void Get_Event(int _jentry);
  virtual Int_t Get_EventIndex();
  virtual TString Get_File_Name();

  int NSignalParticles(){return  Ntp->SignalParticle_pdgId->size();}
  float SignalParticle_pdgId(unsigned int i){return Ntp->SignalParticle_pdgId->at(i);}
  TLorentzVector  SignalParticle_p4(unsigned int i){return TLorentzVector(Ntp->SignalParticle_p4->at(i).at(1),
									  Ntp->SignalParticle_p4->at(i).at(2),
									  Ntp->SignalParticle_p4->at(i).at(3),Ntp->SignalParticle_p4->at(i).at(0));}



  int NDecayProducts(unsigned int i){return Ntp->SignalParticle_child_pdgId->at(i).size();}
  TLorentzVector SignalParticle_childp4(unsigned int i, unsigned int j){return TLorentzVector(Ntp->SignalParticle_childp4->at(i).at(j).at(1),
											      Ntp->SignalParticle_childp4->at(i).at(j).at(2),
											      Ntp->SignalParticle_childp4->at(i).at(j).at(3),Ntp->SignalParticle_childp4->at(i).at(j).at(0));}


  int SignalParticle_child_pdgId(unsigned int i, unsigned int j){return Ntp->SignalParticle_child_pdgId->at(i).at(j);}


  int NChildDecayProducts(unsigned int i){return Ntp->SignalParticle_child_decay_pdgid->at(i).size();}
  TLorentzVector SignalParticle_child_decay_p4(unsigned int i, unsigned int j){return TLorentzVector(Ntp->SignalParticle_child_decay_p4->at(i).at(j).at(1),
												     Ntp->SignalParticle_child_decay_p4->at(i).at(j).at(2),
												     Ntp->SignalParticle_child_decay_p4->at(i).at(j).at(3),
												     Ntp->SignalParticle_child_decay_p4->at(i).at(j).at(0));}


  int SignalParticle_child_decay_pdgId(unsigned int i, unsigned int j){return Ntp->SignalParticle_child_decay_pdgid->at(i).at(j);}

};

#endif

