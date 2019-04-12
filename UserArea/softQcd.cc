// File: tree.cc
// This is a simple test program.
// Modified by Rene Brun and Axcel Naumann to put the Pythia::event 
// into a TTree.
// Copyright (C) 2013 Torbjorn Sjostrand

// Header file to access Pythia 8 program elements.
#include "Pythia.h"

// ROOT, for saving Pythia events as trees in a file.
#include "TTree.h"
#include "TFile.h"

using namespace Pythia8;

int main(int argc,char **argv) {

  // Create Pythia instance and set it up to generate hard QCD processes
  // above pTHat = 20 GeV for pp collisions at 14 TeV.
  //  Pythia pythia;
  //  pythia.readString("HardQCD:all = on");
  //  pythia.readString("PhaseSpace:pTHatMin = 20.");
  //  pythia.readString("Beams:eCM = 14000.");
  //  pythia.init();

  // Set up generation.
  Pythia pythia; // Declare Pythia object
  pythia.readFile(argv[1]);
  pythia.init(); // Initialize; incoming pp beams is default.
  // Generate event(s).

  int nEvent = pythia.mode("Main:numberOfEvents");


  double tau_p(0.);
  // Set up the ROOT TFile and TTree.
  TFile *file = TFile::Open("output.root","recreate");
  TTree * t = new TTree("T","pythia_tree");
  t->Branch("tau_p",&tau_p);
  //  Event *event = &pythia.event;
  //  TTree *T = new TTree("T","ev1 Tree");
  //  T->Branch("event",&event);

 // Begin event loop. Generate event; skip if generation aborted.
  for(int iEvent = 0; iEvent < nEvent; ++iEvent){
    if (!pythia.next()) continue;

    int iTau = 0;
    std::cout<< "   new event   "<< std::endl;
    for (int i = 0; i < pythia.event.size(); ++i) {
      std::cout<< pythia.event[i].id() << std::endl;
      if (pythia.event[i].id() == 11) iTau = i;
    }
    tau_p=pythia.event[iTau].pT();

    t->Fill();
    // Fill the pythia event into the TTree.  
    // Warning: the files will rapidly become large if all events 
    // are saved. In some cases it may be convenient to do some 
    // processing of events and only save those that appear 
    // interesting for future analyses.
    //    T->Fill();

  // End event loop.
  }

  // Statistics on event generation.
  pythia.stat();

  //  Write tree.
  //  T->Print();
  t->Write();
  delete file;

  // Done.
  return 0;
}
