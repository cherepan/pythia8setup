// File: DsDPlusBackground.cc
// Vladimir Cherepanov


#include "Pythia.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include <algorithm>


using namespace Pythia8;


void removeDuplicates(std::vector<int>& vec)
{
  std::sort(vec.begin(), vec.end());
  vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}


int main(int argc,char **argv) {
  bool DEBUG(false);
  Pythia pythia; 
  pythia.readFile(argv[1]);
  pythia.readString("Random:setSeed = on");
  pythia.readString("Random:seed = " + to_string(TMath::Hash(argv[2])/10000.)); // Hash the pythia seed from the output file name
  pythia.init(); 
  int nEvent = pythia.mode("Main:numberOfEvents");

  // Measure the cpu runtime.
  clock_t start, stop;
  double t = 0.0;
  start = clock();
  int taucounter(0);
  std::vector<int> SignalParticle_pdgId;
  std::vector<std::vector<float> > SignalParticle_p4;
  std::vector<std::vector<int> > SignalParticle_child_pdgId;
  std::vector<std::vector<std::vector<float> > > SignalParticle_childp4;
  std::vector<std::vector<std::vector<int> > >   SignalParticle_child_child_pdgId;
  std::vector<std::vector<std::vector<std::vector<float> > >  >  SignalParticle_child_child_p4;

  std::vector<std::vector<int> > SignalParticle_child_decay_pdgid;
  std::vector<std::vector<std::vector<float> > > SignalParticle_child_decay_p4;


  std::vector<std::vector<int> > subchild_idx;

  TFile *file = TFile::Open("res/output" + TString(argv[2]) +".root","recreate");
  TTree * tree = new TTree("tree","pythia_tree");

  tree->Branch("SignalParticle_pdgId",&SignalParticle_pdgId);
  tree->Branch("SignalParticle_p4",&SignalParticle_p4);
  tree->Branch("SignalParticle_child_pdgId",&SignalParticle_child_pdgId);
  tree->Branch("SignalParticle_childp4",&SignalParticle_childp4);
  tree->Branch("SignalParticle_child_decay_pdgid",&SignalParticle_child_decay_pdgid);
  tree->Branch("SignalParticle_child_decay_p4",&SignalParticle_child_decay_p4);

  tree->Branch("SignalParticle_child_child_pdgId",&SignalParticle_child_child_pdgId);
  tree->Branch("SignalParticle_child_child_p4",&SignalParticle_child_child_p4);
  //  Hist pT("transverse momentum", 100, 0., 10.);
  // event loop
  std::vector<int> SignalParticleList;
  std::vector<int> SignalParticleIndex;
  SignalParticleList.push_back(431);  // Ds
  SignalParticleList.push_back(411);  // Dp
  for(int iEvent = 0; iEvent < nEvent; ++iEvent){
    std::vector<int> muon_indices;
    std::vector<int> tau_indices;
    std::vector<int> dump_index;
    
    SignalParticleIndex.clear();    
    SignalParticle_pdgId.clear();
    SignalParticle_p4.clear();
    SignalParticle_child_pdgId.clear();
    SignalParticle_childp4.clear();
    
    SignalParticle_child_decay_pdgid.clear();
    SignalParticle_child_decay_p4.clear();

    SignalParticle_child_child_pdgId.clear();
    SignalParticle_child_child_p4.clear();

    subchild_idx.clear();
    if (!pythia.next()) continue;
    if(DEBUG)std::cout<<"--------------------------------- "<<std::endl;
    for (int i = 0; i < pythia.event.size(); ++i) {
      int id=abs(pythia.event[i].id());
      if(std::find(SignalParticleList.begin(), SignalParticleList.end(), id) != SignalParticleList.end()){
	SignalParticleIndex.push_back(i);
      }
    }
  


    if(SignalParticleIndex.size()!=0){
      for (auto &m : SignalParticleIndex){
	
	SignalParticle_child_pdgId.push_back(std::vector<int>());
	SignalParticle_childp4.push_back(std::vector<std::vector<float> >());
	SignalParticle_pdgId.push_back(pythia.event[m].id());
	std::vector<float> iSig_p4;
	iSig_p4.push_back(pythia.event[m].e());
	iSig_p4.push_back(pythia.event[m].px());
	iSig_p4.push_back(pythia.event[m].py());
	iSig_p4.push_back(pythia.event[m].pz());
	SignalParticle_p4.push_back(iSig_p4);
	

	std::vector<std::vector<int> > temp_child_child_pdgid;
	std::vector<std::vector<std::vector<float> > > temp_child_child_p4;

	if(DEBUG)	std::cout<<"=======================>> SignalParticle  "<< pythia.event[m].name() <<"   daughtersize " << pythia.event[m].daughterList().size()<<std::endl;
	int mother_index = m;
	for (auto &d :  pythia.event[mother_index].daughterList()){
	  temp_child_child_pdgid.push_back(std::vector<int>());
	  temp_child_child_p4.push_back(std::vector<std::vector<float> >());

	  std::vector<float> ichildp4;
	  ichildp4.push_back(pythia.event[d].e());
	  ichildp4.push_back(pythia.event[d].px());
	  ichildp4.push_back(pythia.event[d].py());
	  ichildp4.push_back(pythia.event[d].pz());
	  
	  SignalParticle_child_pdgId.at(SignalParticle_child_pdgId.size() - 1).push_back(pythia.event[d].id());
	  SignalParticle_childp4.at(SignalParticle_child_pdgId.size() - 1).push_back(ichildp4);
	  subchild_idx.push_back(std::vector<int>());
	  if(DEBUG)  	  std::cout<<"================================>> daughters  "<< pythia.event[d].name() <<"   daughtersize " << pythia.event[d].daughterList().size()<<std::endl;
	  for (auto &dd :  pythia.event[d].daughterList()){
	    subchild_idx.at(subchild_idx.size() - 1).push_back(dd);
	    std::vector<float> ip4;
	    ip4.push_back(pythia.event[dd].e());
	    ip4.push_back(pythia.event[dd].px());
	    ip4.push_back(pythia.event[dd].py());
	    ip4.push_back(pythia.event[dd].pz());
	    temp_child_child_pdgid.at(temp_child_child_pdgid.size() - 1).push_back(pythia.event[dd].id());
	    temp_child_child_p4.at(temp_child_child_p4.size() - 1).push_back(ip4);


	    if(DEBUG){
	      std::cout<<"==========================================>> daughters  "<< pythia.event[dd].name() <<"   daughtersize " << pythia.event[dd].daughterList().size()<<std::endl;
	      for (auto &ddd :  pythia.event[dd].daughterList()){
		std::cout<<"==================================================>> daughters  "<< pythia.event[ddd].name() <<" daughtersize " << pythia.event[ddd].daughterList().size()<<std::endl;
		for (auto &dddd :  pythia.event[ddd].daughterList()){
		  std::cout<<"=========================================================>> daughters  "<< pythia.event[dddd].name() <<std::endl;
		}
		
	      }
	    }
	  }
	}
	SignalParticle_child_child_p4.push_back(temp_child_child_p4);
	SignalParticle_child_child_pdgId.push_back(temp_child_child_pdgid);
      }
      //std::cout<<"size: "<< SignalParticle_child_child_pdgId.size() << std::endl;
      //      for (auto &j:SignalParticle_pdgId){

      //	std::cout<< j <<std::endl;
      //      }

    if(DEBUG)      std::cout<<" subchild_idx size  "<< subchild_idx.size() << std::endl;
    for (auto &i:subchild_idx){
      if(DEBUG)        std::cout<<"  i.size  "<< i.size() << std::endl;
      SignalParticle_child_decay_pdgid.push_back(std::vector<int>());
      SignalParticle_child_decay_p4.push_back(std::vector<std::vector<float> >());
      for (auto &j:i){
	std::vector<float> ip4;
	ip4.push_back(pythia.event[j].e());
	ip4.push_back(pythia.event[j].px());
	ip4.push_back(pythia.event[j].py());
	ip4.push_back(pythia.event[j].pz());
	SignalParticle_child_decay_pdgid.at(SignalParticle_child_decay_pdgid.size() - 1).push_back(pythia.event[j].id());
	SignalParticle_child_decay_p4.at(SignalParticle_child_decay_pdgid.size() - 1).push_back(ip4);
	
	if(DEBUG)  	std::cout<<" sub sub child id   "<<pythia.event[j].name() <<   "  "<<ip4.at(1) <<std::endl;
	
      }
    }
    
    tree->Fill();
    }
  }
      
  // End event loop.


  // Statistics on event generation.
  // cout << pT;
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
