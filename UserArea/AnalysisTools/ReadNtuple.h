//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Apr 19 18:33:28 2019 by ROOT version 6.12/07
// from TTree tree/pythia_tree
// found on file: outputRun400.root
//////////////////////////////////////////////////////////

#ifndef ReadNtuple_h
#define ReadNtuple_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"
#include "vector"

class ReadNtuple {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   std::vector<float>   *SignalParticle_pdgId;
   std::vector<std::vector<float> > *SignalParticle_p4;
   std::vector<std::vector<int> > *SignalParticle_child_pdgId;
   std::vector<std::vector<std::vector<float> > > *SignalParticle_childp4;
   std::vector<std::vector<int> > *SignalParticle_child_decay_pdgid;
   std::vector<std::vector<std::vector<float> > > *SignalParticle_child_decay_p4;

   // List of branches
   TBranch        *b_SignalParticle_pdgId;   //!
   TBranch        *b_SignalParticle_p4;   //!
   TBranch        *b_SignalParticle_child_pdgId;   //!
   TBranch        *b_SignalParticle_childp4;   //!
   TBranch        *b_SignalParticle_child_decay_pdgid;   //!
   TBranch        *b_SignalParticle_child_decay_p4;   //!

   ReadNtuple(TTree *tree=0);
   virtual ~ReadNtuple();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef ReadNtuple_cxx
ReadNtuple::ReadNtuple(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("outputRun400.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("outputRun400.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

ReadNtuple::~ReadNtuple()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t ReadNtuple::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t ReadNtuple::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void ReadNtuple::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   SignalParticle_pdgId = 0;
   SignalParticle_p4 = 0;
   SignalParticle_child_pdgId = 0;
   SignalParticle_childp4 = 0;
   SignalParticle_child_decay_pdgid = 0;
   SignalParticle_child_decay_p4 = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("SignalParticle_pdgId", &SignalParticle_pdgId, &b_SignalParticle_pdgId);
   fChain->SetBranchAddress("SignalParticle_p4", &SignalParticle_p4, &b_SignalParticle_p4);
   fChain->SetBranchAddress("SignalParticle_child_pdgId", &SignalParticle_child_pdgId, &b_SignalParticle_child_pdgId);
   fChain->SetBranchAddress("SignalParticle_childp4", &SignalParticle_childp4, &b_SignalParticle_childp4);
   fChain->SetBranchAddress("SignalParticle_child_decay_pdgid", &SignalParticle_child_decay_pdgid, &b_SignalParticle_child_decay_pdgid);
   fChain->SetBranchAddress("SignalParticle_child_decay_p4", &SignalParticle_child_decay_p4, &b_SignalParticle_child_decay_p4);
   Notify();
}

Bool_t ReadNtuple::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void ReadNtuple::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t ReadNtuple::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef ReadNtuple_cxx
