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


//std::vector<int> Tools::SortMuonsByCharge(){


//}

Tools::~Tools() {
  delete Ntp;

}






