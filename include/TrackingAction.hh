#ifndef TrackingAction_h
#define TrackingAction_h 1

#include "G4UserTrackingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TrackingAction : public G4UserTrackingAction {

public:
  TrackingAction();
  virtual ~TrackingAction(){};

  virtual void PostUserTrackingAction(const G4Track *);
  virtual void PreUserTrackingAction(const G4Track *);
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
