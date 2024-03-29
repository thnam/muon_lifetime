#include "RunAction.hh"
#include "Analysis.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
// #include "Run.hh"

#include "G4AccumulableManager.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

RunAction::RunAction() : G4UserRunAction() {
  auto anaMan = G4AnalysisManager::Instance();
  G4cout << "Using " << anaMan->GetType() << G4endl;

  // Create directories
  // anaMan->SetHistoDirectoryName("histos");
  // anaMan->SetNtupleDirectoryName("ntuples");
  anaMan->SetVerboseLevel(0);
  anaMan->SetNtupleMerging(true);

  anaMan->CreateNtuple("edep", "Energy deposit [MeV]");
  anaMan->CreateNtupleDColumn("sc0");
  anaMan->CreateNtupleDColumn("sc1");
  anaMan->CreateNtupleDColumn("sc2");
  anaMan->CreateNtupleDColumn("sc3");
  anaMan->FinishNtuple();
}

RunAction::~RunAction() { delete G4AnalysisManager::Instance(); }

void RunAction::BeginOfRunAction(const G4Run *) {
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // Get analysis manager
  auto anaMan = G4AnalysisManager::Instance();
  anaMan->Reset();

  // Open an output file
  //
  G4String fileName = "edep.root";
  anaMan->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run *run) {
  auto anaMan = G4AnalysisManager::Instance();
  anaMan->Write();
  anaMan->CloseFile(false);
}
