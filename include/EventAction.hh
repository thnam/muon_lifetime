
#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class RunAction;

/// Event action class
///

class EventAction : public G4UserEventAction {
public:
  EventAction(RunAction *runAction);
  virtual ~EventAction();

  virtual void BeginOfEventAction(const G4Event *event);
  virtual void EndOfEventAction(const G4Event *event);

private:
  RunAction *fRunAction;
  G4int fCollID_cryst;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
