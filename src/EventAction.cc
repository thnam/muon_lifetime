#include "EventAction.hh"
#include "Analysis.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4THitsMap.hh"
#include "G4UnitsTable.hh"

EventAction::EventAction(RunAction *runAction)
  : G4UserEventAction(), fRunAction(runAction), fCollID_cryst(-1) {}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event *) {}

void EventAction::EndOfEventAction(const G4Event *evt) {
  G4HCofThisEvent *HCE = evt->GetHCofThisEvent();
  if (!HCE)
    return;

  if (fCollID_cryst < 0) {
    G4SDManager *SDMan = G4SDManager::GetSDMpointer();
    fCollID_cryst = SDMan->GetCollectionID("scSD/edep");
  }

  auto anaMan = G4AnalysisManager::Instance();

  G4THitsMap<G4double> *evtMap =
    (G4THitsMap<G4double> *)(HCE->GetHC(fCollID_cryst));
  std::map<G4int, G4double *>::iterator itr;
  for (itr = evtMap->GetMap()->begin(); itr != evtMap->GetMap()->end(); itr++) {
    G4int copyNb = (itr->first);
    G4double edep = *(itr->second);
    // G4cout << "\n  cryst" << copyNb << ": " << edep/keV << " keV ";
    anaMan->FillNtupleDColumn(copyNb, edep / MeV);
  }

  anaMan->AddNtupleRow();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
