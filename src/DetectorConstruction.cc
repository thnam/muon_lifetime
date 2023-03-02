#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4NistManager.hh"
#include "G4PSDoseDeposit.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4RotationMatrix.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4Transform3D.hh"
#include "G4Tubs.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4VisAttributes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(), fCheckOverlaps(true) {
  DefineMaterials();
}

DetectorConstruction::~DetectorConstruction() {}

void DetectorConstruction::DefineMaterials() {}

G4VPhysicalVolume *DetectorConstruction::Construct() {
  G4NistManager *nist = G4NistManager::Instance();
  G4Material *default_mat = nist->FindOrBuildMaterial("G4_AIR");
  // G4Material *cryst_mat = nist->FindOrBuildMaterial("Lu2SiO5");
  // G4Material *target_mat = nist->FindOrBuildMaterial("G4_Cu");
  G4Material *plastic_sc_mat =
    nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

  // World and crystals
  G4double scSizeX = 5 * cm;
  G4double scSizeY = 1.0 * cm;
  G4double scSizeZ = 17. * cm;
  G4double scGapY = 0.2 * cm;

  G4double worldSizeX = 10 * scSizeX;
  G4double worldSizeY = 10 * scSizeY;
  G4double worldSizeZ = 10 * scSizeZ;

  G4Box *solidWorld =
    new G4Box("World", 0.5 * worldSizeX, 0.5 * worldSizeY, 0.5 * worldSizeZ);
  G4LogicalVolume *logicWorld =
    new G4LogicalVolume(solidWorld, default_mat, "World");
  G4VPhysicalVolume *physWorld =
    new G4PVPlacement(0,               // no rotation
                      G4ThreeVector(), // at (0,0,0)
                      logicWorld,      // its logical volume
                      "World",         // its name
                      0,               // its mother  volume
                      false,           // no boolean operation
                      0,               // copy number
                      fCheckOverlaps); // overlaps checking

  G4Box *solidSc =
    new G4Box("Scintillator", 0.5 * scSizeX, 0.5 * scSizeY, 0.5 * scSizeZ);
  G4LogicalVolume *logicSc =
    new G4LogicalVolume(solidSc, plastic_sc_mat, "logicSc");

  G4double scOffset = 0. * cm;
  G4double copyNo = 0;

  // place 4 scintillators, copy #0 is at the bottom, copy #3 is on top
  for (int i = -1; i <= 2; i++) {
    scOffset = i * (scGapY + 2 * scSizeY / 2);
    new G4PVPlacement(0,                             // no rotation
                      G4ThreeVector(0, scOffset, 0), // at some offset
                      logicSc,                       // its logical volume
                      "Scintillator",                // its name
                      logicWorld,                    // its mother  volume
                      false,                         // no boolean operation
                      copyNo,                        // copy number
                      fCheckOverlaps);               // overlaps checking
    copyNo++;
  }
  //
  // always return the physical World
  //
  return physWorld;
}

void DetectorConstruction::ConstructSDandField() {
  G4SDManager::GetSDMpointer()->SetVerboseLevel(0);

  G4MultiFunctionalDetector *scSD = new G4MultiFunctionalDetector("scSD");
  G4SDManager::GetSDMpointer()->AddNewDetector(scSD);
  G4VPrimitiveScorer *primitiv1 = new G4PSEnergyDeposit("edep");
  scSD->RegisterPrimitive(primitiv1);
  SetSensitiveDetector("logicSc", scSD);
}
