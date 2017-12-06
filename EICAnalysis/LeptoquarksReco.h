#ifndef __LeptoquarksReco_H__
#define __LeptoquarksReco_H__

/* Fun4All includes */
#include <fun4all/SubsysReco.h>

/* STL includes */
#include <math.h>
#include <map>

/*HepMC include */
#include <phhepmc/PHHepMCGenEvent.h>
#include <phhepmc/PHHepMCGenEventMap.h>


class TNtuple;
class TFile;

class PHCompositeNode;
class CaloRawTowerEval;
class JetMap;
class SvtxTrackMap;
class PHHepMCGenEventMap;

class TauCandidate;

typedef std::map<float, TauCandidate> map_tcan;

class LeptoquarksReco : public SubsysReco
{

public:

  LeptoquarksReco(std::string filename);

  int
  Init(PHCompositeNode*);
  int
  process_event(PHCompositeNode*);
  int
  End(PHCompositeNode*);

  void
  set_beam_energies( float beam_electron , float beam_proton )
  {
    _ebeam_E = fabs( beam_electron );
    _pbeam_E = fabs( beam_proton );
  }

  void
  set_reco_jet_collection( std::string newname )
  {
    _jetcolname = newname;
  }

private:

  bool _verbose;

  int _ievent;
  int _total_pass;

  std::string _filename;
  TFile *_tfile;

  /* output tree and variables */
  TNtuple* _ntp_jet;
  TNtuple* _ntp_jet2;

  /* beam energies electron and proton */
  float _ebeam_E;
  float _pbeam_E;

  /** name of jet collection */
  std::string _jetcolname;

  /* CaloRawTowerEvaluators to access tru particle info for
   * given towers */
  std::map< std::string, CaloRawTowerEval* > _map_towereval;

  int AddTrueTauTag( map_tcan&, PHHepMCGenEventMap* );

  int AddJetStructureInformation( map_tcan&, JetMap* );

  int AddTrackInformation( map_tcan&, SvtxTrackMap* );

  int WriteTauCandidatesToTree( map_tcan& );

  /** Check if this is the final state particle or not- update pointer if needed */
  void UpdateFinalStateParticle( HepMC::GenParticle *& );
};

#endif // __LeptoquarksReco_H__
