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

	//TApplication App(argv[0],&argc,argv);
	gROOT->ProcessLine("#include <vector>");
	gSystem->Load("$ROOTSYS/lib/libPhysics.so");
	gROOT->SetBatch(kTRUE);

	TString base = std::getenv("PWD");
	base += "/";
	TString FileList = base + "Filelist.dat";

	std::vector<TString> Files;
	GetVectorString(Files, FileList);

	Tools Tls(Files);
	Int_t nentries = Tls.Get_Entries();

	for (int i = 0; i < nentries; i++) {
	  Tls.Get_Event(i);
	  std::cout<<"  N Signal Particles  : "<<Tls.NSignalParticles()<<std::endl;
	  for(int isigp=0; isigp< Tls.NSignalParticles(); isigp++){
	    std::cout <<"  mass  " <<Tls.SignalParticle_p4(isigp).M() << " N Decay products  " <<  Tls.NDecayProducts(isigp) << std::endl;
	  }
	}
	std::cout << "Program is Finished" << endl;
	system("date");
}

