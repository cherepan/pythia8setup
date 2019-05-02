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
	TTree * t = new TTree("T","T");

	double m12,m13,m23,m;

	t->Branch("m12",&m12);
	t->Branch("m13",&m13);
	t->Branch("m23",&m23);
	t->Branch("m",&m);

	TH1F *tauPT= new TH1F("tauPT","p_{T}(#tau), GeV ",50,0,20);
	TH1F *tauETA= new TH1F("tauETA","#eta(#tau) ",50,-5,5);

	TH1F *ThreeMuPT= new TH1F("ThreeMuPT","p_{T}(3#mu), GeV ",50,0,20);
	TH1F *ThreeMuETA= new TH1F("ThreeMuETA","#eta(3#mu) ",50,-5,5);


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

	Tools Tls(Files);
	Int_t nentries = Tls.Get_Entries();

	std::vector<TLorentzVector> Muons;

	std::vector<TLorentzVector> MuonsPos;
	std::vector<TLorentzVector> MuonsNeg;
	std::vector<TLorentzVector> SortedMuons;
	std::vector<TLorentzVector> Sources;
	std::vector<int> Muon_source;
	std::vector<int> Muons_charge;
	std::vector<int> Muon_source_index;

	for (int i = 0; i < nentries; i++) {
	  Muons.clear();
	  SortedMuons.clear();
	  MuonsPos.clear();
	  MuonsNeg.clear();
	  Muon_source.clear();
	  Muon_source_index.clear();
	  Muons_charge.clear();

	  Tls.Get_Event(i);
	  TLorentzVector TauLV;
	  bool tau_found(false);
	  bool threemu_found(false);
	  for(int isigp=0; isigp< Tls.NSignalParticles(); isigp++){
	    std::cout<<"signal_particle id:  "<< Tls.SignalParticle_pdgId(isigp) << std::endl;
	    Tls.SignalParticle_p4(isigp).Print();
	    for(int j=0; j< Tls.NDecayProducts(isigp); j++){
	      std::cout<<"--- decay  "<< Tls.SignalParticle_child_pdgId(isigp,j) << "    " << Tls.NChildDecayProducts(j) <<std::endl;
	      std::cout<<"  N  "<< Tls.NChildChildProducts(isigp,j)<<std::endl;
	      Tls.SignalParticle_childp4(isigp,j).Print();
	      if(Tls.SignalParticle_child_pdgId(isigp,j)==13)MuonsNeg.push_back(Tls.SignalParticle_childp4(isigp,j));
	      if(Tls.SignalParticle_child_pdgId(isigp,j)==-13)MuonsPos.push_back(Tls.SignalParticle_childp4(isigp,j));

	      TLorentzVector Mass(0,0,0,0);
	      for(int k=0; k< Tls.NChildDecayProducts(j); k++){
		std::cout<<"  -------  "  << Tls.SignalParticle_child_decay_pdgId(j,k) << std::endl;
		Tls.SignalParticle_child_decay_p4(j,k).Print();
		if(Tls.SignalParticle_child_decay_pdgId(j,k)==13)MuonsNeg.push_back(Tls.SignalParticle_child_decay_p4(j,k));
		if(Tls.SignalParticle_child_decay_pdgId(j,k)==-13)MuonsPos.push_back(Tls.SignalParticle_child_decay_p4(j,k));

		Mass+=Tls.SignalParticle_child_decay_p4(j,k);
	      }
	      std::cout<<" Mass   "<< Mass.M()<< std::endl;
	      if(abs(Tls.SignalParticle_child_pdgId(isigp,j))==15){
		tau_found = true;
		TauLV=Tls.SignalParticle_childp4(isigp,j);
	      }

	      if(abs(Tls.SignalParticle_child_pdgId(isigp,j))==13){



		Muons.push_back(Tls.SignalParticle_childp4(isigp,j));
		Muon_source.push_back(Tls.SignalParticle_pdgId(isigp));
		Muon_source_index.push_back(isigp);
		Sources.push_back(Tls.SignalParticle_p4(isigp));
		int muoncharge(0.);
		if(Tls.SignalParticle_child_pdgId(isigp,j)==13)muoncharge=-1;
		if(Tls.SignalParticle_child_pdgId(isigp,j)==-13)muoncharge=1;
		Muons_charge.push_back(muoncharge);
	      }
	    }
	  }

	  if(Muons.size() > 2) threemu_found=true;

	  std::cout<<"  neg: "<< MuonsNeg.size()  << "  pos  " <<MuonsPos.size() << std::endl;



	  if(tau_found){
	    tau_category++;
	    tauPT->Fill(TauLV.Pt());
	    tauETA->Fill(TauLV.Eta());
	  }

	  if((MuonsNeg.size() + MuonsPos.size()) ==3){
	    TLorentzVector OsMuon(0,0,0,0);
	    TLorentzVector Ss1Muon(0,0,0,0);
	    TLorentzVector Ss2Muon(0,0,0,0);
	    ///	    std::cout<<" Neg:  "<< MuonsNeg.size() << "  Pos:  "<< MuonsPos.size() << std::endl;
	    if(MuonsNeg.size()==1 && MuonsPos.size() ==2){
	      OsMuon = MuonsNeg.at(0);
	      Ss1Muon =  MuonsPos.at(0);
	      Ss2Muon =  MuonsPos.at(1);
	    }

	    if(MuonsNeg.size()==2 && MuonsPos.size() ==1){
	      OsMuon = MuonsPos.at(0);
	      Ss1Muon =  MuonsNeg.at(0);
	      Ss2Muon =  MuonsNeg.at(1);
	    }

	    //	    OsMuon.Print();
	    //	    Ss1Muon.Print();
	    //	    Ss2Muon.Print();

	    m12 = (OsMuon+Ss1Muon).M();
	    m23 = (Ss1Muon+ Ss2Muon).M();
	    m13 = (OsMuon + Ss2Muon).M();

	    std::cout<<" m12:   "<< m12 << std::endl;
	    std::cout<<" m13:   "<< m13 << std::endl;
	    std::cout<<" m23:   "<< m23 << std::endl;

	    OsMuon.Print();
	    Ss1Muon.Print();
	    Ss2Muon.Print();


	    m = (OsMuon+Ss1Muon + Ss2Muon).M();
	    t->Fill();

	  }

	  if(threemu_found){
	    TLorentzVector ThreeMuLV(0,0,0,0);
	    //	    std::cout<<"--- "<< std::endl;
	    for(int i=0; i < Muon_source.size(); i++){
	      ThreeMuLV+=Muons.at(i);
	      //	      std::cout<<"Muons:  "<<Muon_source.at(i) << "    index:   " <<Muon_source_index.at(i) <<" charge:   " << Muons_charge.at(i) <<std::endl;
	      for(int j=0; j< Tls.NDecayProducts(Muon_source_index.at(i)); j++){

		//		std::cout<<" ====  decay:  "<< Tls.SignalParticle_child_pdgId(Muon_source_index.at(i),j) <<"   decay producsts  " << Tls.NChildDecayProducts(j) <<std::endl;
		TLorentzVector massCheck(0,0,0,0);
		for(int k =0; k < Tls.NChildDecayProducts(j); k ++){
		  massCheck+=Tls.SignalParticle_child_decay_p4(j,k);
		  //		  Tls.SignalParticle_child_decay_p4(j,k).Print();
		  //		  std::cout<<"========= "<<Tls.SignalParticle_child_decay_pdgId(j,k) << std::endl;
		}
		//		std::cout<<"Mass:  "<< massCheck.M() <<std::endl;
		//		massCheck.Print();
	      }

	      //	    Sources.at(i).Print();
	    }




	    ThreeMuPT->Fill(ThreeMuLV.Pt());
	    ThreeMuETA->Fill(ThreeMuLV.Eta());
	    threemu_category++;
	    

	  }


	}
	t->Write();
	file->Write();

	std::cout << "Total Events number analysed:  "<<nentries<< " With Tau Lepton:  "<< tau_category << " With three mu  " << threemu_category <<std::endl;
	std::cout << "Program is Finished" << endl;

	system("date");
}

