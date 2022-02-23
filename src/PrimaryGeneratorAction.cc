
#include "PrimaryGeneratorAction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(const char *inputfile)
  : G4VUserPrimaryGeneratorAction(), fParticleGun(0), fEnvelopeBox(0) {
  // define a particle gun
  fParticleGun = new G4ParticleGun();

  // Read the cry input file
  std::ifstream inputFile;
  inputFile.open(inputfile, std::ios::in);
  char buffer[1000];

  if (inputFile.fail()) {
    if (*inputfile != 0) //....only complain if a filename was given
      G4cout << "PrimaryGeneratorAction: Failed to open CRY input file= "
             << inputfile << G4endl;
    InputState = -1;
  } else {
    std::string setupString("");
    while (!inputFile.getline(buffer, 1000).eof()) {
      setupString.append(buffer);
      setupString.append(" ");
    }

    CRYSetup *setup = new CRYSetup(setupString, "../data");

    gen = new CRYGenerator(setup);

    // set random number generator
    RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),
                                            &CLHEP::HepRandomEngine::flat);
    setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
    InputState = 0;
  }
  // create a vector to store the CRY particle properties
  vect = new std::vector<CRYParticle *>;

  // Create the table containing all particle names
  particleTable = G4ParticleTable::GetParticleTable();

  // Create the messenger file
  gunMessenger = new PrimaryGeneratorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete fParticleGun; }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
  if (InputState != 0) {
    G4String *str =
      new G4String("CRY library was not successfully initialized");
    // G4Exception(*str);
    G4Exception("PrimaryGeneratorAction", "1", RunMustBeAborted, *str);
  }
  G4String particleName;
  vect->clear();
  gen->genEvent(vect);

  //....debug output
  G4cout << "\nEvent=" << anEvent->GetEventID() << " "
         << "CRY generated nparticles=" << vect->size() << G4endl;

  for (unsigned j = 0; j < vect->size(); j++) {
    particleName = CRYUtils::partName((*vect)[j]->id());

    //....debug output
    G4cout << "  " << particleName << " "
           << "charge=" << (*vect)[j]->charge() << " "
           << "energy (MeV)=" << (*vect)[j]->ke() * MeV << " "
           << "pos (m)"
           << G4ThreeVector((*vect)[j]->x(), (*vect)[j]->y(), (*vect)[j]->z())
           << " "
           << "direction cosines "
           << G4ThreeVector((*vect)[j]->u(), (*vect)[j]->v(), (*vect)[j]->w())
           << " " << G4endl;

    fParticleGun->SetParticleDefinition(
      particleTable->FindParticle((*vect)[j]->PDGid()));
    fParticleGun->SetParticleEnergy((*vect)[j]->ke() * MeV);
    fParticleGun->SetParticlePosition(G4ThreeVector(
      (*vect)[j]->x() * m, (*vect)[j]->y() * m, (*vect)[j]->z() * m));
    fParticleGun->SetParticleMomentumDirection(
      G4ThreeVector((*vect)[j]->u(), (*vect)[j]->v(), (*vect)[j]->w()));
    fParticleGun->SetParticleTime((*vect)[j]->t());
    fParticleGun->GeneratePrimaryVertex(anEvent);
    delete (*vect)[j];
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::InputCRY() { InputState = 1; }

void PrimaryGeneratorAction::UpdateCRY(std::string *MessInput) {
  CRYSetup *setup =
    new CRYSetup(*MessInput, "./CRY-1.7-prefix/src/CRY-1.7/data");

  gen = new CRYGenerator(setup);

  // set random number generator
  RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),
                                          &CLHEP::HepRandomEngine::flat);
  setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
  InputState = 0;
}

void PrimaryGeneratorAction::CRYFromFile(G4String newValue) {
  // Read the cry input file
  std::ifstream inputFile;
  inputFile.open(newValue, std::ios::in);
  char buffer[1000];

  if (inputFile.fail()) {
    G4cout << "Failed to open input file " << newValue << G4endl;
    G4cout << "Make sure to define the cry library on the command line"
           << G4endl;
    InputState = -1;
  } else {
    std::string setupString("");
    while (!inputFile.getline(buffer, 1000).eof()) {
      setupString.append(buffer);
      setupString.append(" ");
    }

    CRYSetup *setup = new CRYSetup(setupString, "../data");

    gen = new CRYGenerator(setup);

    // set random number generator
    RNGWrapper<CLHEP::HepRandomEngine>::set(CLHEP::HepRandom::getTheEngine(),
                                            &CLHEP::HepRandomEngine::flat);
    setup->setRandomFunction(RNGWrapper<CLHEP::HepRandomEngine>::rng);
    InputState = 0;
  }
}
