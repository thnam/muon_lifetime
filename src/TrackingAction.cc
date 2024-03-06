#include "TrackingAction.hh"

#include "G4RunManager.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction()
  :G4UserTrackingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PreUserTrackingAction(const G4Track * aTrack){
  if (aTrack->GetCurrentStepNumber() == 0) {
    G4cout<<"new particle: ";
    G4cout<<", trackid: " << aTrack->GetTrackID();
    G4cout<<", parentid: " << aTrack->GetParentID();
    G4cout<<", "<<aTrack->GetDefinition()->GetParticleName();
    G4cout << ", energy: " <<aTrack->GetKineticEnergy()/MeV << " MeV";
    G4cout<<", position "<<aTrack->GetVertexPosition()/mm << " mm"<<G4endl;
  }
}

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

