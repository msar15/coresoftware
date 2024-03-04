// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef G4OHCAL_PHG4OHCALDISPLAYACTION_H
#define G4OHCAL_PHG4OHCALDISPLAYACTION_H

#include <g4main/PHG4DisplayAction.h>

#include <set>
#include <string>  // for string
#include <vector>

class G4LogicalVolume;
class G4VisAttributes;
class G4VPhysicalVolume;

class PHG4OHCalDisplayAction : public PHG4DisplayAction
{
 public:
  explicit PHG4OHCalDisplayAction(const std::string &name);

  ~PHG4OHCalDisplayAction() override;

  void ApplyDisplayAction(G4VPhysicalVolume *physvol) override;
  void SetMyTopVolume(G4VPhysicalVolume *vol) { m_MyTopVolume = vol; }
  void AddScintiVolume(G4LogicalVolume *vol) { m_ScintiLogVolSet.insert(vol); }
  void AddSteelVolume(G4LogicalVolume *vol) { m_SteelVol = vol; }
  void AddChimSteelVolume(G4LogicalVolume *vol) { m_ChimSteelVol = vol; }

  void AddRing1Volume(G4LogicalVolume *vol) { m_Ring1Vol = vol; }
  void AddRing2Volume(G4LogicalVolume *vol) { m_Ring2Vol = vol; }
  void AddRing3Volume(G4LogicalVolume *vol) { m_Ring3Vol = vol; }
  void AddRing4Volume(G4LogicalVolume *vol) { m_Ring4Vol = vol; }
  void AddRing5Volume(G4LogicalVolume *vol) { m_Ring5Vol = vol; }
  void AddRing6Volume(G4LogicalVolume *vol) { m_Ring6Vol = vol; }


 private:
  G4VPhysicalVolume *m_MyTopVolume = nullptr;
  G4LogicalVolume *m_SteelVol = nullptr;
  G4LogicalVolume *m_ChimSteelVol = nullptr;

  G4LogicalVolume *m_Ring1Vol = nullptr;
  G4LogicalVolume *m_Ring2Vol = nullptr;
  G4LogicalVolume *m_Ring3Vol = nullptr;
  G4LogicalVolume *m_Ring4Vol = nullptr;
  G4LogicalVolume *m_Ring5Vol = nullptr;
  G4LogicalVolume *m_Ring6Vol = nullptr;

  std::vector<G4VisAttributes *> m_VisAttVec;
  std::vector<G4VisAttributes *> m_VisAttVec2;

  std::vector<G4VisAttributes *> m_VisAttVec3;
  std::vector<G4VisAttributes *> m_VisAttVec4;
  std::vector<G4VisAttributes *> m_VisAttVec5;


  std::set<G4LogicalVolume *> m_ScintiLogVolSet;


};

#endif  // G4OHCAL_PHG4OHCALDISPLAYACTION_H
