#include "Tools.h"
//#include "PDG_Var.h"
#include "TF1.h"


Tools::Tools(std::vector<TString> RootFiles):
  copyTree(false)
  ,isInit(false)
{
  // TChains the ROOTuple file
  TChain *chain = new TChain("tree");
  std::cout << "Loading " << RootFiles.size() << " files" << std::endl;
  for(unsigned int i=0; i<RootFiles.size(); i++){
    chain->Add(RootFiles[i]);
  }
  TTree *tree = (TTree*)chain;  
  if(chain==0){
    std::cout<< "chain points to NULL" << std::endl;
  }
  std::cout<< "Number of Events in Ntuple: " << chain->GetEntries() << std::endl;
  Ntp=new ReadNtuple(tree);
  nbytes=0; 
  nb=0;

  // Fit setup 
  gRandom->SetSeed(1234);
}


void Tools::InitEvent(){

	// after everything is initialized
	isInit = true;
}


Int_t Tools::Get_Entries(){
  return Int_t(Ntp->fChain->GetEntries());
}


void Tools::Get_Event(int _jentry){
  jentry=_jentry;
  Ntp->LoadTree(jentry);
  nb = Ntp->fChain->GetEntry(jentry);   nbytes += nb;
  isInit = false;
  InitEvent();
}

Int_t Tools::Get_EventIndex(){
  return jentry;
}


TString Tools::Get_File_Name(){
  return Ntp->fChain->GetCurrentFile()->GetName();
}

void Tools::Branch_Setup(TString B_Name, int type){   
  Ntp->fChain->SetBranchStatus(B_Name,type);
}


unsigned int Tools::GetType(TString name){
  name.ToLower();
  //  StoreType(name);
  if(name=="dsetamumu")             return Type::DsEtaMuMu;
  if(name=="dsetamumugamma")        return Type::DsEtaMuMuGamma;
  if(name=="dsetamumupi0")          return Type::DsEtaMuMuGammaPi0;
  if(name=="dsphimumu")             return Type::DsPhiMuMu;
  if(name=="dsphimumugamma")        return Type::DsPhiMuMuGamma;
  if(name=="dsetaprimemumu")        return Type::DsEtaPrimeMuMu;
  if(name=="dsetaprimemumugamma")   return Type::DsEtaPrimeMuMuGamma;
  if(name=="dpetamumu")             return Type::DpEtaMuMu;
  if(name=="dpetamumugamma")        return Type::DpEtaMuMuGamma;
  if(name=="dpetamumugammapi0")     return Type::DpEtaMuMuGammaPi0;
  if(name=="dpphimumu")             return Type::DpPhiMuMu;
  if(name=="dpphimumugamma")        return Type::DpPhiMuMuGamma;
  if(name=="dpetaprimemumu")        return Type::DpEtaPrimeMuMu;
  if(name=="dpetaprimemumugamma")   return Type::DpEtaPrimeMuMuGamma;
  if(name=="dprhomumu")             return Type::DpRhoMuMu;
  if(name=="dpomegamumu")           return Type::DpOmegaMuMu;
  if(name=="dpomegamumupi0")        return Type::DpOmegaMuMuPi0;

  std::cout << "ERROR: Type " << name << " UNKNOWN!!!! " << std::endl;
  return Type::unknown;
}


std::vector<TLorentzVector> Tools::sortMuons(std::vector<std::pair<int,TLorentzVector> >  pairs){
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


//std::vector<int> Tools::SortMuonsByCharge(){


//}


void Tools::PrintDecay(unsigned int i){
  std::cout<<"decay chain:  "<< Ntp->SignalParticle_pdgId->at(i) << std::endl;
  for (unsigned int j =0; j< Ntp->SignalParticle_child_pdgId->at(i).size(); j++){
    std::cout<<"                -> "<<Ntp->SignalParticle_child_pdgId->at(i).at(j) << std::endl;
    for(unsigned int k=0; k < Ntp->SignalParticle_child_child_pdgId->at(i).at(j).size(); k++){
      std::cout<<"                      ->"<<  Ntp->SignalParticle_child_child_pdgId->at(i).at(j).at(k) << std::endl;
    }
  }
}

int Tools::DecayID(unsigned int i){

  if(abs(Ntp->SignalParticle_pdgId->at(i)) == 431){ 
    for (unsigned int j =0; j< Ntp->SignalParticle_child_pdgId->at(i).size(); j++){
      if(Ntp->SignalParticle_child_pdgId->at(i).at(j)==221){
	std::vector<int> lastdecay;
	for(unsigned int k=0; k < Ntp->SignalParticle_child_child_pdgId->at(i).at(j).size(); k++){
	  lastdecay.push_back(Ntp->SignalParticle_child_child_pdgId->at(i).at(j).at(k));
	}
	if(AnalyzeMesonDecay(lastdecay) == 1) return GetType("dsetamumu");
	if(AnalyzeMesonDecay(lastdecay) == 2) return GetType("dsetamumugamma");
	if(AnalyzeMesonDecay(lastdecay) == 4) return GetType("dsetamumugammapi0");
      }
      if(Ntp->SignalParticle_child_pdgId->at(i).at(j)==333){
	std::vector<int> lastdecay;
	for(unsigned int k=0; k < Ntp->SignalParticle_child_child_pdgId->at(i).at(j).size(); k++){
	  lastdecay.push_back(Ntp->SignalParticle_child_child_pdgId->at(i).at(j).at(k));
	}
	if(AnalyzeMesonDecay(lastdecay) == 1) return GetType("dsphimumu");
	if(AnalyzeMesonDecay(lastdecay) == 2) return GetType("dsphimumugamma");
      }
      if(Ntp->SignalParticle_child_pdgId->at(i).at(j)==331){
	std::vector<int> lastdecay;
        for(unsigned int k=0; k < Ntp->SignalParticle_child_child_pdgId->at(i).at(j).size(); k++){
          lastdecay.push_back(Ntp->SignalParticle_child_child_pdgId->at(i).at(j).at(k));
        }
        if(AnalyzeMesonDecay(lastdecay) == 1) return GetType("dsetaprimemumu");
        if(AnalyzeMesonDecay(lastdecay) == 2) return GetType("dsetaprimemumugamma");
      }
    }
  }
  if(abs(Ntp->SignalParticle_pdgId->at(i)) == 411){
    for (unsigned int j =0; j< Ntp->SignalParticle_child_pdgId->at(i).size(); j++){
      if(Ntp->SignalParticle_child_pdgId->at(i).at(j)==221){
	std::vector<int> lastdecay;
        for(unsigned int k=0; k < Ntp->SignalParticle_child_child_pdgId->at(i).at(j).size(); k++){
          lastdecay.push_back(Ntp->SignalParticle_child_child_pdgId->at(i).at(j).at(k));
        }
        if(AnalyzeMesonDecay(lastdecay) == 1) return GetType("dpetamumu");
        if(AnalyzeMesonDecay(lastdecay) == 2) return GetType("dpetamumugamma");
        if(AnalyzeMesonDecay(lastdecay) == 4) return GetType("dpetamumugammapi0");
      }
      if(Ntp->SignalParticle_child_pdgId->at(i).at(j)==333){
	std::vector<int> lastdecay;
        for(unsigned int k=0; k < Ntp->SignalParticle_child_child_pdgId->at(i).at(j).size(); k++){
          lastdecay.push_back(Ntp->SignalParticle_child_child_pdgId->at(i).at(j).at(k));
        }
        if(AnalyzeMesonDecay(lastdecay) == 1) return GetType("dpphimumu");
        if(AnalyzeMesonDecay(lastdecay) == 2) return GetType("dpphimumugamma");
      }
      if(Ntp->SignalParticle_child_pdgId->at(i).at(j)==331){
	std::vector<int> lastdecay;
        for(unsigned int k=0; k < Ntp->SignalParticle_child_child_pdgId->at(i).at(j).size(); k++){
          lastdecay.push_back(Ntp->SignalParticle_child_child_pdgId->at(i).at(j).at(k));
        }
        if(AnalyzeMesonDecay(lastdecay) == 1) return GetType("dpetaprimemumu");
        if(AnalyzeMesonDecay(lastdecay) == 2) return GetType("dpetaprimemumugamma");
      }
      if(Ntp->SignalParticle_child_pdgId->at(i).at(j)==223){
	std::vector<int> lastdecay;
        for(unsigned int k=0; k < Ntp->SignalParticle_child_child_pdgId->at(i).at(j).size(); k++){
          lastdecay.push_back(Ntp->SignalParticle_child_child_pdgId->at(i).at(j).at(k));
        }
        if(AnalyzeMesonDecay(lastdecay) == 1) return GetType("dpomegamumu");
        if(AnalyzeMesonDecay(lastdecay) == 3) return GetType("dpomegamumupi0");
      }
      if(Ntp->SignalParticle_child_pdgId->at(i).at(j)==113){
	std::vector<int> lastdecay;
        for(unsigned int k=0; k < Ntp->SignalParticle_child_child_pdgId->at(i).at(j).size(); k++){
          lastdecay.push_back(Ntp->SignalParticle_child_child_pdgId->at(i).at(j).at(k));
        }
        if(AnalyzeMesonDecay(lastdecay) == 1) return GetType("dprhomumu");

      }
    }  
  }
  std::cout<<"Could not classify the decay! return 0; "<< std::endl;
  return 0;

}


int Tools::AnalyzeMesonDecay(std::vector<int> v){

  if(   std::find(v.begin(), v.end(), 13) != v.end() && std::find(v.begin(), v.end(), -13) != v.end() && v.size() ==2 ) return 1;
  if(   std::find(v.begin(), v.end(), 13) != v.end() && std::find(v.begin(), v.end(), -13) != v.end() && std::find(v.begin(), v.end(), 22) != v.end()  && v.size() ==3 ) return 2;
  if(   std::find(v.begin(), v.end(), 13) != v.end() && std::find(v.begin(), v.end(), -13) != v.end() && std::find(v.begin(), v.end(), 111) != v.end()  && v.size() ==3 ) return 3;
  if(   std::find(v.begin(), v.end(), 13) != v.end() && std::find(v.begin(), v.end(), -13) != v.end() && std::find(v.begin(), v.end(), 111) != v.end()  &&  std::find(v.begin(), v.end(), 22) != v.end()  && v.size() ==4 ) return 4;

  std::cout<<"Cant analyze meson decay, return 0;"<<std::endl;
  return 0;
}
Tools::~Tools() {
  delete Ntp;

}






