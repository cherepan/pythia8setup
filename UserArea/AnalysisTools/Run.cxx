#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <cstdlib>
#include <stdlib.h> 


#include <string.h>
#include <istream>
#include <strstream>

#include "TApplication.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TStyle.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"

#include "Tools.h"

void GetVectorString(std::vector<TString> &v,TString file){
  v.clear();
  // Open File
  ifstream input_file;
  input_file.open(file, std::ios::in);
  if (!(input_file)){
    std::cout << "Opening file with datasets "<< file <<"  has failed." << std::endl;
    return;
  }
  std::cout << "Opened Parameters xml file: "<< file <<"." << std::endl;

  std::string s;
  unsigned int a=0;
  while(getline(input_file, s)){
    a++;
    if(a>50000){std::cout << "More than 50000 line in file??? Breaking" << std::endl; break;}
    std::stringstream line(s);
    TString val;
    line  >> val;
    v.push_back(val);
    
  }
  input_file.close();
  return;
}

int main() {
        std::cout << "Program is Starting" << endl;
	system("date");
	time_t startTime;//, endTime, beforeLoop, afterLoop;
	time(&startTime);


	TString path= (TString)std::getenv("PWD");
	TString FileName  = "Output.root";
	TFile *file = new TFile(FileName,"RECREATE");

	TH1F *tauPT= new TH1F("tauPT","p_{T}(#tau), GeV ",50,0,20);
	TH1F *tauETA= new TH1F("tauETA","#eta(#tau) ",50,-5,5);



	//TApplication App(argv[0],&argc,argv);
	gROOT->ProcessLine("#include <vector>");
	gSystem->Load("$ROOTSYS/lib/libPhysics.so");
	gROOT->SetBatch(kTRUE);

	TString base = std::getenv("PWD");
	base += "/";
	TString FileList = base + "Files.dat";

	std::vector<TString> Files;
	GetVectorString(Files, FileList);

	Tools Tls(Files);
	Int_t nentries = Tls.Get_Entries();

	std::vector<TLorentzVector> Muons;
	std::vector<TLorentzVector> Sources;
	std::vector<int> Muon_source;

	for (int i = 0; i < nentries; i++) {
	  Muons.clear();
	  Muon_source.clear();
	  Tls.Get_Event(i);
	  TLorentzVector TauLV;
	  bool tau_found(false);
	  for(int isigp=0; isigp< Tls.NSignalParticles(); isigp++){

	    for(int j=0; j< Tls.NDecayProducts(isigp); j++){
	      if(abs(Tls.SignalParticle_child_pdgId(isigp,j))==15){
		tau_found = true;
		TauLV=Tls.SignalParticle_childp4(isigp,j);
	      }

	      if(abs(Tls.SignalParticle_child_pdgId(isigp,j))==13){
		Muons.push_back(Tls.SignalParticle_childp4(isigp,j));
		Muon_source.push_back(Tls.SignalParticle_pdgId(isigp));
		Sources.push_back(Tls.SignalParticle_p4(isigp));
	      }
	    }
	  }
	  if(tau_found){
	  tauPT->Fill(TauLV.Pt());
	  tauETA->Fill(TauLV.Eta());
	  }


	  //	  std::cout<<"Muons:  "<<Muons.size() << std::endl;
	  std::cout<<"----"<<std::endl;
	  for(int i=0; i < Muon_source.size(); i++){

	    std::cout<<"Muons:  "<<Muon_source.at(i) << std::endl;
	    Sources.at(i).Print();
	  }

	}
	file->Write();
	std::cout << "Program is Finished" << endl;
	system("date");
}

