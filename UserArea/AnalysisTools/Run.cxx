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

std::vector<TLorentzVector> sortMuons(std::vector<std::pair<int,TLorentzVector> >  pairs){
  int mum(0), mup(0);
  TLorentzVector OSMu(0,0,0,0);
  TLorentzVector SSMu1(0,0,0,0);
  TLorentzVector SSMu2(0,0,0,0);
  for (auto &i:pairs){
    if(i.first == 13)mum++;
    if(i.first == -13)mup++;
  }
  if(mup==1&&mum==2){
    int nss=0;
    for (auto &i:pairs){
      if(i.first ==-13){
	OSMu=i.second;
      }
      if(i.first==13 && nss==0){
	nss++;
	SSMu1=i.second;
      }
      if(i.first==13 && nss==1){
	SSMu2=i.second;
      }
    }
  }

  if(mup==2&&mum==1){
    int nss=0;

    for (auto &i:pairs){
      if(i.first ==13){
        OSMu=i.second;
      }
      if(i.first==-13 && nss==0){
        nss++;
        SSMu1=i.second;
      }
      if(i.first==-13 && nss==1){
        SSMu2=i.second;
      }
    }
  }

  std::vector<TLorentzVector> out;
  out.push_back(OSMu);
  out.push_back(SSMu1);
  out.push_back(SSMu2);
  return out;
}


int main() {
        std::cout << "Program is Starting" << endl;
	system("date");
	time_t startTime;//, endTime, beforeLoop, afterLoop;
	time(&startTime);


	TString path= (TString)std::getenv("PWD");
	TString FileName  = "Output.root";
	TFile *file = new TFile(FileName,"RECREATE");
	TTree * t = new TTree("T","T");

	double m12,m13,m23,m;
	double pt1,pt2,pt3;
	double eta1,eta2,eta3;


	t->Branch("m12",&m12);
	t->Branch("m13",&m13);
	t->Branch("m23",&m23);


	t->Branch("pt1",&pt1);
	t->Branch("pt2",&pt2);
	t->Branch("pt3",&pt3);

	t->Branch("eta1",&eta1);
	t->Branch("eta2",&eta2);
	t->Branch("eta3",&eta3);

	t->Branch("m",&m);

	/*	TH1F *tauPT= new TH1F("tauPT","p_{T}(#tau), GeV ",50,0,20);
	TH1F *tauETA= new TH1F("tauETA","#eta(#tau) ",50,-5,5);

	TH1F *ThreeMuPT= new TH1F("ThreeMuPT","p_{T}(3#mu), GeV ",50,0,20);
	TH1F *ThreeMuETA= new TH1F("ThreeMuETA","#eta(3#mu) ",50,-5,5);
	*/

	int tau_category(0);
	int threemu_category(0);
	//TApplication App(argv[0],&argc,argv);
	gROOT->ProcessLine("#include <vector>");
	gSystem->Load("$ROOTSYS/lib/libPhysics.so");
	gROOT->SetBatch(kTRUE);

	TString base = std::getenv("PWD");
	base += "/";
	TString FileList = base + "InputFilesSignal.dat";

	std::vector<TString> Files;
	GetVectorString(Files, FileList);
	std::vector<std::vector<TLorentzVector> > SignalMuons;
	Tools Tls(Files);
	Int_t nentries = Tls.Get_Entries();



	for (int i = 0; i < nentries; i++) {
	  SignalMuons.clear();
	  Tls.Get_Event(i);
	  TLorentzVector TauLV;
	  for(int isigp=0; isigp< Tls.NSignalParticles(); isigp++){
	    std::vector<std::pair<int,TLorentzVector> > temp;
	    for(int j=0; j< Tls.NDecayProducts(isigp); j++){
	      //	      std::cout<<"id and mass   "<< Tls.SignalParticle_child_pdgId(isigp,j)<< "    "<<Tls.SignalParticle_childp4(isigp,j).M() <<  std::endl;
	      if(abs(Tls.SignalParticle_child_pdgId(isigp,j))==13 ){
		temp.push_back(std::make_pair(Tls.SignalParticle_child_pdgId(isigp,j),Tls.SignalParticle_childp4(isigp,j)));
	      }
	      for(int k=0; k<Tls.NChildChildProducts(isigp,j); k++){
		//		std::cout<<"--------------   "<<  Tls.SignalParticle_child_child_pdgId(isigp,j,k) << "    "<<Tls.SignalParticle_child_child_p4(isigp,j,k).Px() <<  std::endl;
		if(abs(Tls.SignalParticle_child_child_pdgId(isigp,j,k))==13){
		  temp.push_back(std::make_pair(Tls.SignalParticle_child_child_pdgId(isigp,j,k),Tls.SignalParticle_child_child_p4(isigp,j,k)));
		}
	      }
	    }
	    //	    std::vector<TLorentzVector>  vec=sortMuons(temp);
	    //	    for (auto &v:vec){ v.Print();}
	    SignalMuons.push_back(sortMuons(temp));
	  }


	  for (auto &i:SignalMuons){
	    TLorentzVector osmuon=i.at(0);
	    TLorentzVector ss1muon=i.at(1);
	    TLorentzVector ss2muon=i.at(2);

	    m12 = (osmuon+ss1muon).M();
	    m13 = (osmuon+ss2muon).M();
	    m23 = (ss1muon+ss2muon).M();	    
	    m   = (osmuon+ss1muon+ss2muon).M();

	    /*	    std::cout<<"px1  "<< osmuon.Px() << std::endl;
	    std::cout<<"px2  "<< ss1muon.Px() << std::endl;
	    std::cout<<"px3  "<< ss2muon.Px() << std::endl;

	    std::cout<<"m12  "<< m12 <<std::endl;
	    std::cout<<"m13  "<< m13 <<std::endl;
	    std::cout<<"m23  "<< m23 <<std::endl;
	    */
	    pt1=osmuon.Pt();   eta1=osmuon.Eta();
	    pt2=ss1muon.Pt();  eta2=ss1muon.Eta();
	    pt3=ss2muon.Pt();  eta3=ss2muon.Eta();

	    t->Fill();
	  }
	}

	file->Write();

	std::cout << "Total Events number analysed:  "<<nentries<< " With Tau Lepton:  "<< tau_category << " With three mu  " << threemu_category <<std::endl;
	std::cout << "Program is Finished" << endl;

	system("date");
}

