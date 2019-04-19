// File: softQcd.cc
// Vladimir Cherepanov

// Header file to access Pythia 8 program elements.
#include "Pythia.h"


#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TMath.h"
#include "TLorentzVector.h"

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
  double threemu_mass(0.);

  std::vector<float> SignalParticle_pdgId;
  std::vector<std::vector<float> > SignalParticle_p4;
  std::vector<std::vector<int> > SignalParticle_childpdgid;
  std::vector<std::vector<std::vector<float> > > SignalParticle_childp4;

  TFile *file = TFile::Open("res/output" + TString(argv[2]) +".root","recreate");
  TTree * tree = new TTree("tree","pythia_tree");

  tree->Branch("SignalParticle_pdgId",&SignalParticle_pdgId);
  tree->Branch("SignalParticle_p4",&SignalParticle_p4);
  tree->Branch("SignalParticle_childpdgid",&SignalParticle_childpdgid);
  tree->Branch("SignalParticle_childp4",&SignalParticle_childp4);
  //  tree->Branch("tau_eta",&tau_eta);
  //  tree->Branch("source_id",&source_id);


  Hist pT("transverse momentum", 100, 0., 10.);

  int taucounter(0);

  // event loop
  for(int iEvent = 0; iEvent < nEvent; ++iEvent){
    std::vector<int> muon_indices;
    std::vector<int> tau_indices;
    std::vector<int> dump_index;


    SignalParticle_pdgId.clear();
    SignalParticle_p4.clear();
    SignalParticle_childpdgid.clear();
    SignalParticle_childp4.clear();




    //    ClearEvent();

    //    std::cout<<"---------------- "<< std::endl;
    if (!pythia.next()) continue;


    for (int i = 0; i < pythia.event.size(); ++i) {
      TLorentzVector particle = TLorentzVector(pythia.event[i].px(),pythia.event[i].py(),pythia.event[i].pz(),pythia.event[i].e());
      if(abs(pythia.event[i].id()) == 15){
	tau_indices.push_back(i);
	dump_index.push_back(i);
      }

      if (abs(pythia.event[i].id()) == 13){ 
	if( fabs(pythia.event[i].eta()) < 2.8 && pythia.event[i].pT() > 0.2){
	  muon_indices.push_back(i);
	  dump_index.push_back(i);
	}
      }
    }


    
    if(tau_indices.size()>0 or  muon_indices.size()  > 2){
    std::cout<<"tau size  "<< tau_indices.size() << std::endl;
    std::cout<<"muonsize  "<< muon_indices.size() << std::endl;
      for ( auto &k : dump_index ) {
	for (auto &m : pythia.event[k].motherList()){
	  for (auto &mm : pythia.event[k].motherList()){  std::cout<<"  indixes  "<< mm  <<std::endl;}

	  std::cout<<"signal particle  "<< pythia.event[m].name() <<std::endl;
	  SignalParticle_childpdgid.push_back(std::vector<int>());
	  SignalParticle_childp4.push_back(std::vector<std::vector<float> >());
	  SignalParticle_pdgId.push_back(pythia.event[m].id());

	  std::vector<float> iSig_p4;
	  iSig_p4.push_back(pythia.event[m].e());
	  iSig_p4.push_back(pythia.event[m].px());
	  iSig_p4.push_back(pythia.event[m].py());
	  iSig_p4.push_back(pythia.event[m].pz());
	  SignalParticle_p4.push_back(iSig_p4);


	  int mother_index = m;
	  for (auto &d :  pythia.event[mother_index].daughterList()){
	    std::cout<<"=============================>> daughters  "<< pythia.event[d].name() <<std::endl;

	    std::vector<float> ichildp4;
	    ichildp4.push_back(pythia.event[d].e());
	    ichildp4.push_back(pythia.event[d].px());
	    ichildp4.push_back(pythia.event[d].py());
	    ichildp4.push_back(pythia.event[d].pz());

	    SignalParticle_childpdgid.at(SignalParticle_childpdgid.size() - 1).push_back(pythia.event[d].id());
	    SignalParticle_childp4.at(SignalParticle_childpdgid.size() - 1).push_back(ichildp4);
	    for (auto &dd :  pythia.event[d].daughterList()){
	      std::cout<<"==========================================>> daughters  "<< pythia.event[dd].name() <<std::endl;
	      for (auto &ddd :  pythia.event[dd].daughterList()){
		std::cout<<"==================================================>> daughters  "<< pythia.event[ddd].name() <<std::endl;
		for (auto &dddd :  pythia.event[ddd].daughterList()){
		  std::cout<<"=========================================================>> daughters  "<< pythia.event[dddd].name() <<std::endl;
		}
	      }
	    }

	  }
	}
      }
      tree->Fill();
    }


    if(muon_indices.size()==3){
      for(unsigned int k =0; k< muon_indices.size(); k++){
	//	std::cout<<"==================>> mu index "<< muon_indices.at(k) <<std::endl;
	for(int m = 0; m < pythia.event[muon_indices.at(k)].motherList().size(); m++){
	  //	  std::cout<<  " source:   " <<  pythia.event[pythia.event[ muon_indices.at(k)].motherList().at(m)].name() << "   "<<std::endl;  
	}
      }

      unsigned int i1 = muon_indices.at(0);
      unsigned int i2 = muon_indices.at(1);
      unsigned int i3 = muon_indices.at(2);

      TLorentzVector Mu1 = TLorentzVector(pythia.event[i1].px(),pythia.event[i1].py(),pythia.event[i1].pz(),pythia.event[i1].e());
      TLorentzVector Mu2 = TLorentzVector(pythia.event[i2].px(),pythia.event[i2].py(),pythia.event[i2].pz(),pythia.event[i2].e());
      TLorentzVector Mu3 = TLorentzVector(pythia.event[i3].px(),pythia.event[i3].py(),pythia.event[i3].pz(),pythia.event[i3].e());
      //      std::cout<<" reading  index   "<< i1 << "  "<< i2  << "  mass  "  <<Mu1.M() <<std::endl;      Mu1.Print();
      TLorentzVector ThreeMuLV = Mu1 + Mu2 + Mu3;
      threemu_mass = ThreeMuLV.M();
      //      std::cout<<" three mu mass " << ThreeMuLV.M() << " mu mass check  " <<Mu1.M() << std::endl;

    }

    //    tau_p=pythia.event[iTau].pT();
    //    tau_eta=pythia.event[iTau].eta();
    //    pT.fill( pythia.event[iTau].pT() );

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




//void ClearEvent(){
//  tau_index.clear();
//  muon_indices.clear();
//}
