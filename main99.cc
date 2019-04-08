// Headers and Namespaces.
#include "Pythia8/Pythia.h" // Include Pythia headers.


using namespace Pythia8; // Let Pythia8:: be implicit.
int main(int argc, char* argv[]) {

  // Set up generation.
  Pythia pythia; // Declare Pythia object
  pythia.readFile(argv[1]);
  pythia.init(); // Initialize; incoming pp beams is default.
  // Generate event(s).

  int nEvent = pythia.mode("Main:numberOfEvents");

  Hist pT("top transverse momentum", 100, 0., 200.);
  Hist eta("top pseudorapidity", 100, -5., 5.);


  for(int iEvent = 0; iEvent < nEvent; ++iEvent){

    pythia.next(); // Generate an(other) event. Fill event record.
    int iTop = 0;
    for (int i = 0; i < pythia.event.size(); ++i) {
      if (pythia.event[i].id() == 6) iTop = i;
    }


    pT.fill( pythia.event[iTop].pT() );
    eta.fill( pythia.event[iTop].eta() );

  }

  cout << pT << eta;

  pythia.stat();
  return 0;
} // End main program with error-free return
