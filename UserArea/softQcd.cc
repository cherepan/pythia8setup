B0;136;0c// File: softQcd.cc
// Vladimir Cherepanov


#include "Pythia.h"
#include "TTree.h"
#include "TFile.h"
#include "TString.h"
#include "TMath.h"
#include "TLorentzVector.h"

using namespace Pythia8;


void removeDuplicates(std::vector<int>& vec)
{
  std::sort(vec.begin(), vec.end());
  vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}


int main(int argc,char **argv) {
  bool DEBUG(false);
  bool doTaus(true);
  bool doThreeMu(true);
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
  start = clock();
  int taucounter(0);
  std::vector<float> SignalParticle_pdgId;
  std::vector<std::vector<float> > SignalParticle_p4;
  std::vector<std::vector<int> > SignalParticle_child_pdgId;
  std::vector<std::vector<std::vector<float> > > SignalParticle_childp4;

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

  //  Hist pT("transverse momentum", 100, 0., 10.);
  // event loop
  for(int iEvent = 0; iEvent < nEvent; ++iEvent){
    std::vector<int> muon_indices;
    std::vector<int> tau_indices;
    std::vector<int> dump_index;


    SignalParticle_pdgId.clear();
    SignalParticle_p4.clear();
    SignalParticle_child_pdgId.clear();
    SignalParticle_childp4.clear();

    SignalParticle_child_decay_pdgid.clear();
    SignalParticle_child_decay_p4.clear();

    subchild_idx.clear();
    if (!pythia.next()) continue;
    for (int i = 0; i < pythia.event.size(); ++i) {
      if(abs(pythia.event[i].id()) == 15){
	tau_indices.push_back(i);
	dump_index.push_back(i);
	taucounter++;
      }

      if (abs(pythia.event[i].id()) == 13){ 
	if( fabs(pythia.event[i].eta()) < 2.8 && pythia.event[i].pT() > 0.2){
	  muon_indices.push_back(i);
	  dump_index.push_back(i);
	}
      }
    }

    std::vector<int> SignalParticleIndex;
    std::vector<int> SelectedIndexCollection;
    if(tau_indices.size()>0 &&  muon_indices.size() <3) SelectedIndexCollection = tau_indices;
    else if(muon_indices.size()>2) SelectedIndexCollection=dump_index;
    
    if(!doTaus && muon_indices.size()>2)SelectedIndexCollection=muon_indices;

    for ( auto &k : SelectedIndexCollection ) {
      for (auto &m : pythia.event[k].motherList()){
	SignalParticleIndex.push_back(m);
      }
    }
    removeDuplicates(SignalParticleIndex);
    


    if(SignalParticleIndex.size()!=0){
      if(DEBUG)std::cout<<"N taus "<< tau_indices.size() <<" N three mu  " <<muon_indices.size() << std::endl;
      for (auto &m : SignalParticleIndex){
	
	if(DEBUG)std::cout<<"signal particle  "<< pythia.event[m].name() <<std::endl;
	SignalParticle_child_pdgId.push_back(std::vector<int>());
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
	  
	  std::vector<float> ichildp4;
	  ichildp4.push_back(pythia.event[d].e());
	  ichildp4.push_back(pythia.event[d].px());
	  ichildp4.push_back(pythia.event[d].py());
	  ichildp4.push_back(pythia.event[d].pz());
	  
	  SignalParticle_child_pdgId.at(SignalParticle_child_pdgId.size() - 1).push_back(pythia.event[d].id());
	  SignalParticle_childp4.at(SignalParticle_child_pdgId.size() - 1).push_back(ichildp4);
	  subchild_idx.push_back(std::vector<int>());
	  for (auto &dd :  pythia.event[d].daughterList()){
	    subchild_idx.at(subchild_idx.size() - 1).push_back(dd);
	         /*		
		std::cout<<"==========================================>> daughters  "<< pythia.event[dd].name() <<"   daughtersize " << pythia.event[dd].daughterList().size()<<std::endl;
		for (auto &ddd :  pythia.event[dd].daughterList()){
		  std::cout<<"==================================================>> daughters  "<< pythia.event[ddd].name() <<" daughtersize " << pythia.event[ddd].daughterList().size()<<std::endl;
		  for (auto &dddd :  pythia.event[ddd].daughterList()){
		    std::cout<<"=========================================================>> daughters  "<< pythia.event[dddd].name() <<std::endl;
		  }
		}
		*/
	      }
	    }
	  }
	  
	  
	  for (auto &i:subchild_idx){
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
	    }
	  }
	
	  tree->Fill();
      }
  
      
  // End event loop.
  }

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
