// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef QAG4SIMULATIONDISTORTIONS_H
#define QAG4SIMULATIONDISTORTIONS_H

#include <fun4all/SubsysReco.h>
#include <trackbase/TrkrDefs.h>

#include <string>
#include <vector>
class PHCompositeNode;
class SvtxTrackMap;
class TrkrClusterContainer;
class SvtxTrack;
class ActsGeometry;

class QAG4SimulationDistortions : public SubsysReco
{
 public:

  QAG4SimulationDistortions(const std::string &name = "QAG4SimulationDistortions");

  ~QAG4SimulationDistortions() override;


  int Init(PHCompositeNode*) override;
  int InitRun(PHCompositeNode *topNode) override;
  int process_event(PHCompositeNode*) override;

 private:

  std::string get_histo_prefix()
    {
      return std::string("h_") + Name() + std::string("_");
    }
  
  std::vector<TrkrDefs::cluskey> get_cluster_keys(SvtxTrack* track);
  bool checkTrack(SvtxTrack* track);
  SvtxTrackMap* m_trackMap = nullptr;
  TrkrClusterContainer* m_clusterContainer = nullptr;
  ActsGeometry* m_tGeometry = nullptr;
};

#endif // QAG4SIMULATIONDISTORTIONS_H
