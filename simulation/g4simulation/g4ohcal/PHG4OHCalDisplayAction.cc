#include "PHG4OHCalDisplayAction.h"

#include <g4main/PHG4DisplayAction.h>  // for PHG4DisplayAction

#include <Geant4/G4Colour.hh>
#include <Geant4/G4LogicalVolume.hh>
#include <Geant4/G4VisAttributes.hh>

PHG4OHCalDisplayAction::PHG4OHCalDisplayAction(const std::string &name)
  : PHG4DisplayAction(name)
{
}

PHG4OHCalDisplayAction::~PHG4OHCalDisplayAction()
{
  for (auto &it : m_VisAttVec)
  {
    delete it;
  }
  m_VisAttVec.clear();
  m_ScintiLogVolSet.clear();
}

void PHG4OHCalDisplayAction::ApplyDisplayAction(G4VPhysicalVolume * /*physvol*/)
{
  for (auto &it : m_ScintiLogVolSet)
  {
    if (it->GetVisAttributes())
    {
      return;
    }
    G4VisAttributes *m_VisAtt = new G4VisAttributes();
    m_VisAtt->SetVisibility(true);
    m_VisAtt->SetForceSolid(true);
    m_VisAtt->SetColor(G4Colour::Green());
    it->SetVisAttributes(m_VisAtt);
    m_VisAttVec.push_back(m_VisAtt);
  }

  if (m_SteelVol)
  {
    if (m_SteelVol->GetVisAttributes())
    {
      return;
    }
    G4VisAttributes *m_VisAtt = new G4VisAttributes();
    m_VisAtt->SetVisibility(true);
    m_VisAtt->SetForceSolid(true);
    m_VisAtt->SetColor(G4Colour::Grey());
    m_SteelVol->SetVisAttributes(m_VisAtt);
    m_VisAttVec.push_back(m_VisAtt);
  }

  if (m_ChimSteelVol)
  {
    if (m_ChimSteelVol->GetVisAttributes())
    {
      return;
    }
    G4VisAttributes *m_VisAtt2 = new G4VisAttributes();
    m_VisAtt2->SetVisibility(true);
    m_VisAtt2->SetForceSolid(true);
    m_VisAtt2->SetColor(G4Colour::Grey());
    m_ChimSteelVol->SetVisAttributes(m_VisAtt2);
    m_VisAttVec2.push_back(m_VisAtt2);
  }

  //=====================================
  G4VisAttributes *m_VisAtt3 = new G4VisAttributes();
  m_VisAtt3->SetVisibility(true);
  m_VisAtt3->SetForceSolid(true);
  m_VisAtt3->SetColor(G4Colour::Red());
  if (m_Ring1Vol)
    {
      if (m_Ring1Vol->GetVisAttributes())
	{
	  return;
	}
      m_Ring1Vol->SetVisAttributes(m_VisAtt3);
      m_VisAttVec3.push_back(m_VisAtt3);
    }
  
  if (m_Ring2Vol)
    {
      if (m_Ring2Vol->GetVisAttributes())
	{
	  return;
    }
      m_Ring2Vol->SetVisAttributes(m_VisAtt3);
      m_VisAttVec4.push_back(m_VisAtt3);
    }

  G4VisAttributes *m_VisAtt4 = new G4VisAttributes();
  m_VisAtt4->SetVisibility(true);
  m_VisAtt4->SetForceSolid(true);
  m_VisAtt4->SetColor(G4Colour::Yellow());
  if (m_Ring3Vol)
    {
      if (m_Ring3Vol->GetVisAttributes())
	{
	  return;
	}
      m_Ring3Vol->SetVisAttributes(m_VisAtt4);
      m_VisAttVec3.push_back(m_VisAtt4);
    }
  
  if (m_Ring4Vol)
    {
      if (m_Ring4Vol->GetVisAttributes())
	{
	  return;
	}
      m_Ring4Vol->SetVisAttributes(m_VisAtt4);
      m_VisAttVec4.push_back(m_VisAtt4);
    }
  G4VisAttributes *m_VisAtt5 = new G4VisAttributes();
  m_VisAtt5->SetVisibility(true);
  m_VisAtt5->SetForceSolid(true);
  m_VisAtt5->SetColor(G4Colour::Magenta());
  if (m_Ring5Vol)
    {
      if (m_Ring5Vol->GetVisAttributes())
	{
	  return;
	}
      m_Ring5Vol->SetVisAttributes(m_VisAtt5);
      m_VisAttVec3.push_back(m_VisAtt5);
    }
  
  if (m_Ring6Vol)
    {
      if (m_Ring6Vol->GetVisAttributes())
	{
	  return;
    }
      m_Ring6Vol->SetVisAttributes(m_VisAtt5);
      m_VisAttVec4.push_back(m_VisAtt5);
    }
  
}
