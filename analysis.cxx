void analysis() {
    TFile *file = new TFile("ouput_cry_flux.root", "READ");
    if (!file || file->IsZombie()) {
        return;
    }

    TTree *tree = (TTree*)file->Get("tree");
    if (!tree) {
        return;
    }

    int eventNumber;
    int nParticles;
    std::vector<int> *particleID = 0;
    std::vector<double> *ke = 0;
    std::vector<double> *x = 0;
    std::vector<double> *y = 0;
    std::vector<double> *z = 0;
    std::vector<double> *w = 0;
    std::vector<double> *charge = 0;
    std::vector<double> *theta = 0;

    tree->SetBranchAddress("eventNumber", &eventNumber);
    tree->SetBranchAddress("nParticles", &nParticles);


    tree->SetBranchAddress("particleID", &particleID);
    tree->SetBranchAddress("ke", &ke);
    tree->SetBranchAddress("x", &x);
    tree->SetBranchAddress("y", &y);
    tree->SetBranchAddress("z", &z);
    tree->SetBranchAddress("w", &w);
    tree->SetBranchAddress("charge", &charge);
    tree->SetBranchAddress("theta", &theta);

    TH1D* histMuonKE = new TH1D("histMuonKE", "histMuonKE", 100, 0,4000);
    TH1D* histMuonTheta = new TH1D("histMuonTheta", "histMuonTheta", 100, 0,0);

    enum class CRYId { CRYIdMin, Neutron=CRYIdMin, Proton, Pion, Kaon, Muon, Electron, Gamma, CRYIdMax=Gamma};

    std::cout << (int)CRYId::Muon << std::endl;

    for (int i = 0; i < tree->GetEntries(); ++i) {
        tree->GetEntry(i);
        for (int j = 0; j < nParticles; ++j) {
            if (particleID->at(j) == (int)CRYId::Muon){
                histMuonKE->Fill(ke->at(j));
                histMuonTheta->Fill(theta->at(j));
            }
        }
    }

    TCanvas* can = new TCanvas;
    //can->SetLogx();
    can->SetLogy();
    histMuonKE->Draw();

    //histMuonTheta->Draw();

    //file->Close();
}
