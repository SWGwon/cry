#include "CRYGenerator.h"
#include "CRYSetup.h"

#include "TROOT.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <math.h>
#include <stdlib.h>  // For Ubuntu Linux

int main( int argc, const char *argv[]) {

  if (!TROOT::Initialized()) {
    static TROOT root("RooTuple", "RooTuple ROOT God in CosmicRaY simulation");
  }

  int nEv = 100000;
  if (argc > 2 ) nEv = atoi(argv[2]);

  if ( argc < 2 ) {
    std::cout << "usage " << argv[0] << " <setup file name> <N events>\n";
    std::cout << "N events = 10k by default\n";
    return 0;
  }

  // Output ROOT file and TTree creation
  TFile *outputFile = new TFile("ouput_cry_flux.root", "RECREATE");
  TTree *tree = new TTree("tree", "CRY simulation data");

  // Variables to store event and particle data
  int eventNumber;
  int nParticles;
  std::vector<int> particleID;
  std::vector<double> ke;
  std::vector<double> x;
  std::vector<double> y;
  std::vector<double> z;
  std::vector<double> w;
  std::vector<double> charge;
  std::vector<double> theta;
  std::vector<double> phi;


  // Set branches
  tree->Branch("eventNumber", &eventNumber, "eventNumber/I");
  tree->Branch("nParticles", &nParticles, "nParticles/I");
  tree->Branch("particleID", &particleID);
  tree->Branch("ke", &ke);
  tree->Branch("x", &x);
  tree->Branch("y", &y);
  tree->Branch("z", &z);
  tree->Branch("w", &w);
  tree->Branch("charge", &charge);
  tree->Branch("theta", &theta);
  tree->Branch("phi", &phi);

  // Read the setup file into setupString
  std::ifstream inputFile;
  inputFile.open(argv[1], std::ios::in);
  char buffer[1000];

  std::string setupString("");
  while (!inputFile.getline(buffer, 1000).eof()) {
    setupString.append(buffer);
    setupString.append(" ");
  }

  // Parse the contents of the setup file
  CRYSetup *setup = new CRYSetup(setupString, DATA_DIR);
  //CRYSetup *setup = new CRYSetup(setupString,"./data");

  // Setup the CRY event generator
  CRYGenerator gen(setup);

  // Generate the events
  int nMuon = 0;
  std::vector<CRYParticle*> *ev = new std::vector<CRYParticle*>;
  for (int i = 0; i < nEv; i++) {
    ev->clear();
    gen.genEvent(ev);

    if (i % 1000 == 0) std::cout << "Event: " << i << std::endl;

    // Clear vectors for new event
    particleID.clear();
    ke.clear();
    x.clear();
    y.clear();
    z.clear();
    w.clear();
    charge.clear();
      theta.clear();
      phi.clear();

    eventNumber = i;
    nParticles = ev->size();

    for (unsigned j = 0; j < ev->size(); j++) {
      CRYParticle *part = (*ev)[j];

      // Store particle data
      particleID.push_back(part->id());
      ke.push_back(part->ke());
      x.push_back(part->x());
      y.push_back(part->y());
      z.push_back(part->z());
      w.push_back(part->w());
      charge.push_back(part->charge());

      double thetaValue = acos(part->w()); // w = cos(theta)
      double phiValue = atan2(part->u(), part->v());
      theta.push_back(thetaValue);
      phi.push_back(phiValue);

      if (part->id() == CRYParticle::Muon) {
        nMuon++;
      }

      delete (*ev)[j];
    }

    // Fill the tree
    tree->Fill();
  }

  std::cout << "Run completed.\n";
  std::cout << "Total time simulated: " << gen.timeSimulated() << " seconds\n";
  double muonsPerSecondPerm2 = nMuon / (300.0 * 300.0 * gen.timeSimulated());
  std::cout << "Muons per second per m2: " << muonsPerSecondPerm2 << std::endl;

  // Write the tree to the file
  outputFile->Write();
  outputFile->Close();

  delete setup;

  return 0;
}
