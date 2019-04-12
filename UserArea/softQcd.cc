// File: softQcd.cc
// Vladimir Cherepanov

// Header file to access Pythia 8 program elements.
#include "Pythia.h"


#include "TTree.h"
#include "TFile.h"

using namespace Pythia8;

int main(int argc,char **argv) {

  Pythia pythia; 
  pythia.readFile(argv[1]);
  pythia.init(); 

  int nEvent = pythia.mode("Main:numberOfEvents");

  double tau_p(0.);
  TFile *file = TFile::Open("output.root","recreate");
  TTree * t = new TTree("T","pythia_tree");
  t->Branch("tau_p",&tau_p);



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
  // End event loop.
  }

  // Statistics on event generation.
  pythia.stat();

  //  T->Print();
  t->Write();
  delete file;

  // Done.
  return 0;
}
