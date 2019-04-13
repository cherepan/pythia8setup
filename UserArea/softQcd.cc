// File: softQcd.cc
// Vladimir Cherepanov

// Header file to access Pythia 8 program elements.
#include "Pythia.h"


#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TMath.h"


using namespace Pythia8;

int main(int argc,char **argv) {

  Pythia pythia; 
  pythia.readFile(argv[1]);
  double pythia_seed = TMath::Hash(argv[2])/10000.;
  string pythiaseedline = "Random:seed = " + to_string(pythia_seed);
  pythia.readString("Random:setSeed = on");
  pythia.readString(pythiaseedline);
  pythia.init(); 
  int nEvent = pythia.mode("Main:numberOfEvents");

  // Measure the cpu runtime.
  clock_t start, stop;
  double t = 0.0;
  // Depending on operating system, either of lines below gives warning.
  //assert((start = clock()) != -1); // Start timer; clock_t signed.
  //assert((start = clock()) != -1u); // Start timer; clock_t unsigned.
  // Simpler option, not using assert.
  start = clock();



  double tau_p(0.);
  double tau_eta(0.);
  double source_id(0.);

  TFile *file = TFile::Open("res/output" + TString(argv[2]) +".root","recreate");
  TTree * tree = new TTree("tree","pythia_tree");
  tree->Branch("tau_p",&tau_p);
  tree->Branch("tau_eta",&tau_eta);
  tree->Branch("source_id",&source_id);


  Hist pT("transverse momentum", 100, 0., 10.);

  int taucounter(0);

  // event loop
  for(int iEvent = 0; iEvent < nEvent; ++iEvent){
    if (!pythia.next()) continue;


    bool taufound(false);    int iTau = 0;
    //    std::cout<< "   new event   "<< std::endl;
    for (int i = 0; i < pythia.event.size(); ++i) {
      //      std::cout<< pythia.event[i].id() << std::endl;
      if (abs(pythia.event[i].id()) == 15){ 
	taucounter++;
	iTau = i; 
	taufound = true; 
	//	for(int d = 0; d < pythia.event[i].daughterList().size(); d++){
	//  std::cout<<"==================>> dau "<< pythia.event[pythia.event[i].daughterList().at(d)].id() << "   "<<std::endl;  
	//	}

	for(int m = 0; m < pythia.event[i].motherList().size(); m++){
	  std::cout<<"==================>> mot "<< pythia.event[pythia.event[i].motherList().at(m)].name() << "   "<<std::endl;  
	  source_id=pythia.event[pythia.event[i].motherList().at(m)].id();
	}

      }
    }

    tau_p=pythia.event[iTau].pT();
    tau_eta=pythia.event[iTau].eta();
    pT.fill( pythia.event[iTau].pT() );
    if(taufound)    tree->Fill();
  // End event loop.
  }

  // Statistics on event generation.
  cout << pT;
  pythia.stat();

  //  T->Print();
  tree->Write();
  delete file;


  stop = clock(); // Stop timer
  t = (double) (stop-start)/CLOCKS_PER_SEC;
  cout << "\n" << "|------------------------------------------------------|" << endl;
  cout << "| CPU Runtime = " << t << " sec" <<  "   per event:  " << t/nEvent <<endl;
  cout << "|------------------------------------------------------|" << "\n" << endl;


  cout << "\n" << "|------------------------------------------------------------|" << endl;
  cout<< "| Event generated: " <<  nEvent <<"  taus found: "<< taucounter<< "   per event: "<<(float)taucounter/(float)nEvent <<std::endl;
  cout << "|------------------------------------------------------------|" << "\n" << endl;
  // Done.
  return 0;
}
