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

	double m12,m13,m23,tau_m, tau_eta, tau_pt;
	double pt1,pt2,pt3;
	double eta1,eta2,eta3;
	float costheta;
	double id;

	double mu_1_Pt_sorted_pt,mu_2_Pt_sorted_pt,
	  mu_3_Pt_sorted_pt,mu_1_Pt_sorted_eta,
	  mu_2_Pt_sorted_eta,mu_3_Pt_sorted_eta,
	  mu_1_Eta_sorted_pt,mu_2_Eta_sorted_pt,
	  mu_3_Eta_sorted_pt,mu_1_Eta_sorted_eta,
	  mu_2_Eta_sorted_eta,mu_3_Eta_sorted_eta;


	t->Branch("m12",&m12);
	t->Branch("m13",&m13);
	t->Branch("m23",&m23);


	t->Branch("pt1",&pt1);
	t->Branch("pt2",&pt2);
	t->Branch("pt3",&pt3);

	t->Branch("eta1",&eta1);
	t->Branch("eta2",&eta2);
	t->Branch("eta3",&eta3);

	t->Branch("mu_1_Pt_sorted_pt",&mu_1_Pt_sorted_pt);
	t->Branch("mu_2_Pt_sorted_pt",&mu_2_Pt_sorted_pt);
	t->Branch("mu_3_Pt_sorted_pt",&mu_3_Pt_sorted_pt);

	t->Branch("mu_1_Pt_sorted_eta",&mu_1_Pt_sorted_eta);
	t->Branch("mu_2_Pt_sorted_eta",&mu_2_Pt_sorted_eta);
	t->Branch("mu_3_Pt_sorted_eta",&mu_3_Pt_sorted_eta);


	t->Branch("mu_1_Eta_sorted_pt",&mu_1_Eta_sorted_pt);
	t->Branch("mu_2_Eta_sorted_pt",&mu_2_Eta_sorted_pt);
	t->Branch("mu_3_Eta_sorted_pt",&mu_3_Eta_sorted_pt);

	t->Branch("mu_1_Eta_sorted_eta",&mu_1_Eta_sorted_eta);
	t->Branch("mu_2_Eta_sorted_eta",&mu_2_Eta_sorted_eta);
	t->Branch("mu_3_Eta_sorted_eta",&mu_3_Eta_sorted_eta);



	//	t->Branch("costheta",&costheta);

	t->Branch("tau_m",&tau_m);
	t->Branch("tau_eta",&tau_eta);
	t->Branch("tau_pt",&tau_pt);

	t->Branch("id",&id);
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
	TString FileList = base + "SignalFileList.dat";

	std::vector<TString> Files;
	GetVectorString(Files, FileList);
	std::vector<std::vector<TLorentzVector> > SignalMuons;
	std::vector<std::vector<TLorentzVector> > SignalMuonsUnsorted;
	std::vector<int> SignalIDS;
	Tools Tls(Files);
	Int_t nentries = Tls.Get_Entries();



	for (int i = 0; i < nentries; i++) {
	  SignalMuons.clear();
	  SignalMuonsUnsorted.clear();
	  SignalIDS.clear();
	  Tls.Get_Event(i);
	  TLorentzVector TauLV;
	  for(int isigp=0; isigp< Tls.NSignalParticles(); isigp++){
	    //	    Tls.PrintDecay(isigp);
	    SignalIDS.push_back(Tls.DecayID(isigp));
	    //	    std::cout<<"   "<< Tls.DecayID(isigp) <<std::endl;
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
	    SignalMuons.push_back(Tls.sortMuons(temp));
	    //	    SignalMuonsUnsorted.push_back(temp);
	  }

	  for(unsigned int i=0; i< SignalMuons.size(); i++){
	    //	  for (auto &i:SignalMuons){
	    id=SignalIDS.at(i);
	    if(id!=0){
	    TLorentzVector osmuon=SignalMuons.at(i).at(0);
	    TLorentzVector ss1muon=SignalMuons.at(i).at(1);
	    TLorentzVector ss2muon=SignalMuons.at(i).at(2);




	    m12 = (osmuon+ss1muon).M();
	    m13 = (osmuon+ss2muon).M();
	    m23 = (ss1muon+ss2muon).M();	    
	    tau_m   = (osmuon+ss1muon+ss2muon).M();
	    tau_eta   = (osmuon+ss1muon+ss2muon).Eta();
	    tau_pt   = (osmuon+ss1muon+ss2muon).Pt();

	    std::vector<TLorentzVector> vecTosort;
	    vecTosort.push_back(osmuon);
	    vecTosort.push_back(ss1muon);
	    vecTosort.push_back(ss2muon);


	    std::vector<TLorentzVector> PtSortedVector = Tls.sortByPt(vecTosort);
	    std::vector<TLorentzVector> EtaSortedVector = Tls.sortByEta(vecTosort);


	    //	    std::cout<<"  1  "<< PtSortedVector.at(0).Pt() << std::endl;
	    //	    std::cout<<"  2  "<< PtSortedVector.at(1).Pt() << std::endl;
	    //	    std::cout<<"  3  "<< PtSortedVector.at(2).Pt() << std::endl;


	    std::cout<<"  1  "<< EtaSortedVector.at(0).Eta() << std::endl;
	    std::cout<<"  2  "<< EtaSortedVector.at(1).Eta() << std::endl;
	    std::cout<<"  3  "<< EtaSortedVector.at(2).Eta() << std::endl;


	    mu_1_Pt_sorted_pt = PtSortedVector.at(0).Pt();
	    mu_2_Pt_sorted_pt = PtSortedVector.at(1).Pt();
	    mu_3_Pt_sorted_pt = PtSortedVector.at(2).Pt();
	    mu_1_Pt_sorted_eta = PtSortedVector.at(0).Eta();
	    mu_2_Pt_sorted_eta = PtSortedVector.at(1).Eta();
	    mu_3_Pt_sorted_eta = PtSortedVector.at(2).Eta();


	    mu_1_Eta_sorted_pt =   EtaSortedVector.at(0).Pt();
	    mu_2_Eta_sorted_pt  =   EtaSortedVector.at(1).Pt();
	    mu_3_Eta_sorted_pt   =   EtaSortedVector.at(2).Pt();
	    mu_1_Eta_sorted_eta =  EtaSortedVector.at(0).Eta();
	    mu_2_Eta_sorted_eta  =  EtaSortedVector.at(1).Eta();
	    mu_3_Eta_sorted_eta  =  EtaSortedVector.at(2).Eta();
	  




	    TLorentzVector Tau = osmuon+ss1muon+ss2muon;

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
	    //	    std::cout<<"------- id "<< id << std::endl;
	    //	    costheta=0;
	    //	    if(id ==555){
	    //	      std::cout<<"------- id "<< id << std::endl;
	      //	      std::cout<<"m12  "<< m12 <<std::endl;
	      //	      std::cout<<"m13  "<< m13 <<std::endl;


	      TVector3 Rotate=Tau.Vect();
	      TLorentzVector TauRotated = Tau;
	      TauRotated.SetVect(Tls.Rotate(TauRotated.Vect(), Rotate));
	      //	      std::cout<<"rotated  "<< std::endl;
	      //	      TauRotated.Print();

	      TLorentzVector TauRest = Tls.Boost( TauRotated, TauRotated);
	      //	      TauRest.Print();


	      TLorentzVector MuOsRotated = osmuon;
	      TLorentzVector MuSS1Rotated = ss1muon;
	      TLorentzVector MuSS2Rotated = ss2muon;


	      MuOsRotated.SetVect(Tls.Rotate( MuOsRotated.Vect(), Rotate));
	      MuSS1Rotated.SetVect(Tls.Rotate( MuSS1Rotated.Vect(), Rotate));
	      MuSS2Rotated.SetVect(Tls.Rotate( MuSS2Rotated.Vect(), Rotate));

	      TLorentzVector OsMuTauRest = Tls.Boost(MuOsRotated, TauRotated);
	      TLorentzVector Ss1MuTauRest = Tls.Boost(MuSS1Rotated, TauRotated);
	      TLorentzVector Ss2MuTauRest = Tls.Boost(MuSS2Rotated, TauRotated);


	      TVector3 nss1= Ss1MuTauRest.Vect()*(1/Ss1MuTauRest.Vect().Mag());
	      TVector3 nss2= Ss2MuTauRest.Vect()*(1/Ss2MuTauRest.Vect().Mag());
	      TVector3 noss= OsMuTauRest.Vect()*(1/OsMuTauRest.Vect().Mag());
	      TVector3 nPerp= (nss1.Cross(nss2))*(1/(nss1.Cross(nss2)).Mag());


	      //	      std::cout<<"   "<< nss1*nPerp <<"  " << nss2*nPerp << "   "<< noss*nPerp << std::endl;


	      //	      OsMuTauRest.Print();
	      //	      (Ss1MuTauRest + Ss2MuTauRest).Print();


	      TLorentzVector Pi1Rotated = osmuon;
	      Pi1Rotated.SetVect(Tls.Rotate(Pi1Rotated.Vect(), Rotate));

	      TLorentzVector Pi2Rotated = ss2muon;
	      Pi2Rotated.SetVect(Tls.Rotate(Pi2Rotated.Vect(), Rotate));

	      TLorentzVector PhiRest=Tls.Boost(TauRotated,TauRotated);
	      TLorentzVector Pi1Rest = Tls.Boost(Pi1Rotated,TauRotated);
	      TLorentzVector Pi2Rest = Tls.Boost(Pi2Rotated,TauRotated);
	      TVector3 n(0,0,1);
	      TVector3 r = Pi1Rest.Vect()*(1/Pi1Rest.Vect().Mag());
	      //	      r.Print();
	      costheta = noss*nPerp;
	      //	      std::cout<<"costheta   "<< costheta << std::endl;
	      //	      PhiRest.Print();
	      //	      Pi1Rest.Print();
	      //	      Pi2Rest.Print();


	      //	      TLorentzVector PhiRest=Tls.Boost(Phi,Phi);
	      //	      PhiRest.Print();

	      //	    }
	    t->Fill();
	    }
	  }
	}

	file->Write();

	std::cout << "Total Events number analysed:  "<<nentries<< " With Tau Lepton:  "<< tau_category << " With three mu  " << threemu_category <<std::endl;
	std::cout << "Program is Finished" << endl;

	system("date");
}

