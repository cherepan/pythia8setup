// File: tree.cc
// This is a simple test program.
// Modified by Rene Brun and Axcel Naumann to put the Pythia::event 
// into a TTree.
// Copyright C 2011 Torbjorn Sjostrand

// Header file to access Pythia 8 program elements.
#include "Pythia.h"

// ROOT, for saving Pythia events as trees in a file.
#include "TTree.h"
#include "TFile.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include <algorithm>
#include "TString.h"



using namespace Pythia8;


int main(int argc,char **argv) {


  Pythia pythia; 
  pythia.readFile(argv[1]);
  pythia.readString("Random:setSeed = on");
  pythia.readString("Random:seed = " + to_string(TMath::Hash(argv[2])/10000.)); // Hash the pythia seed from the output file name
  pythia.init(); 
  int nEvent = pythia.mode("Main:numberOfEvents");



  clock_t start, stop;
  double t = 0.0;
  start = clock();

  // Set up the ROOT TFile and TTree.
  TFile *file = TFile::Open("res/output" + TString(argv[2]) +".root","recreate");
  Event *event = &pythia.event;
  TTree *T = new TTree("T","ev1 Tree");
  T->Branch("event",&event);
  vector<int> Muons;
  // Begin event loop. Generate event; skip if generation aborted.
  for (int iEvent = 0; iEvent < nEvent; ++iEvent) {
    if (!pythia.next()) continue;

    Muons.clear();
    //    std::cout<<"  Muons size  -----  "<< Muons.size() <<std::endl;
    if (iEvent < 1) {pythia.info.list(); pythia.event.list();}
    int nMuons(0);
    for (int i = 0; i < pythia.event.size(); ++i) {
      int id=abs(pythia.event[i].id());
      // 
      if(abs(id) == 13){
	TLorentzVector MuLV(pythia.event[i].px(),pythia.event[i].py(),pythia.event[i].pz(), pythia.event[i].e());
	//	std::cout<<"signal particle  "<< pythia.event[m].name() <<std::endl;
	if(MuLV.Pt() > 3 && fabs(MuLV.Eta()) < 2.41)
	  {
	    //	  std::cout<<"  id  "<< id <<std::endl;
	    Muons.push_back(i);
	  }
      }
    }
    //    std::cout<<"  Muons size   "<< Muons.size() <<std::endl;
    if(Muons.size() > 2 ){
      std::cout<<" ---   "<<std::endl;
      for (auto &m : Muons){
	std::cout<<"  for muon  "<<std::endl;
	for(auto &s : pythia.event[m].sisterList()){
	  std::cout<<"Sisters ::  "<< s << "  name   "<<  pythia.event[s].name() << std::endl;
	  for(auto &sd:pythia.event[s].daughterList()){
	    std::cout<<"Sisters daughters ::  "<< sd << "  name   "<<  pythia.event[sd].name() << std::endl;
	  }
	}

	for (auto &k : pythia.event[m].motherList()){
	  std::cout<<"mother ID  "<< pythia.event[k].id() << "  name   "<<  pythia.event[k].name()  << "  mother id " <<  k <<std::endl;
	  for (auto &l : pythia.event[k].motherList()){
	    std::cout<<"Sub   mother ID  "<< pythia.event[l].id() << "  name   "<<  pythia.event[l].name()  << "  mother id " <<  l <<std::endl;


	  }
	}
      }
    }
    
    // End event loop.
  }

  // Statistics on event generation.
  pythia.statistics();

  //  Write tree.
  T->Print();
  T->Write();
  delete file;

  // Done.
  return 0;


}
