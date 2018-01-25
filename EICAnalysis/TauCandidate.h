#ifndef TAUCANDIDATE_H__
#define TAUCANDIDATE_H__

#include <phool/PHObject.h>
#include <cmath>
#include <climits>

class TauCandidate: public PHObject
{
public:
  TauCandidate() {}
  virtual ~TauCandidate() {}

  virtual void identify(std::ostream& os = std::cout) const;

  virtual void Copy(TauCandidate const &tc);

  friend std::ostream &operator<<(std::ostream & stream, const TauCandidate * tc);

  virtual void Reset();

  virtual void print() const {std::cout<<"TauCandidate base class - print() not implemented"<<std::endl;}

  //! Procedure to add a new PROPERTY tag:
  //! 1.add new tag below with unique value,
  //! 2.add a short name to TauCandidate::get_property_info
  enum PROPERTY
    {//

      //-- Truth properties: 1 - 100  --
      evtgen_is_tau = 1,
      evtgen_tau_etotal = 2,
      evtgen_tau_eta = 3,
      evtgen_tau_phi = 4,
      evtgen_tau_decay_prong = 5,
      evtgen_tau_decay_hcharged = 6,
      evtgen_tau_decay_lcharged = 7,

      evtgen_is_uds = 11,
      evtgen_uds_etotal = 12,
      evtgen_uds_eta = 13,
      evtgen_uds_phi = 14,

      //-- Jet properties: 101 - ?  --
      jet_id = 101,
      jet_eta = 102,
      jet_phi = 103,
      jet_etotal = 104,
      jet_etrans = 105,
      jet_ptotal = 106,
      jet_ptrans = 107,
      jet_minv   = 108,
      jet_mtrans = 109,
      jet_ncomp  = 110,
      jet_ncomp_above_0p1  = 111,
      jet_ncomp_above_1    = 112,
      jet_ncomp_above_10   = 113,
      jet_ncomp_emcal      = 115,

      //-- Jet shape in calorimeter: 120 - 200  --
      jetshape_radius = 121,
      jetshape_rms = 122,
      jetshape_r90 = 123,

      jetshape_econe_r01 = 131,
      jetshape_econe_r02 = 132,
      jetshape_econe_r03 = 133,
      jetshape_econe_r04 = 134,
      jetshape_econe_r05 = 135,

      jetshape_emcal_radius = 161,
      jetshape_emcal_rms = 162,
      jetshape_emcal_r90 = 163,

      jetshape_emcal_econe_r01 = 171,
      jetshape_emcal_econe_r02 = 172,
      jetshape_emcal_econe_r03 = 173,
      jetshape_emcal_econe_r04 = 174,
      jetshape_emcal_econe_r05 = 175,

      //-- Track information: 201 - 250  --
      tracks_count_r02 = 210,
      tracks_count_r04 = 211,

      tracks_rmax_r02 = 215,
      tracks_rmax_r04 = 216,

      tracks_chargesum_r02 = 220,
      tracks_chargesum_r04 = 221,

      //-- EM Candidates Cluster
      em_cluster_id = 301,
      em_cluster_prob = 302,
      em_cluster_posx = 303,
      em_cluster_posy = 304,
      em_cluster_posz = 305,
      em_cluster_e = 306,
      em_cluster_ecore = 307,
      em_cluster_et_iso = 308,
      em_cluster_theta = 309,
      em_cluster_phi = 310,
      em_cluster_pt = 311,
      em_cluster_ntower = 312,
      em_cluster_caloid = 313,

      //-- EM Candidates Tracks
      em_track_id = 351,
      em_track_quality = 352,
      em_track_theta = 353,
      em_track_phi = 354,
      em_track_ptotal = 355,
      em_track_ptrans = 356,
      em_track_charge = 357,
      em_track_dca = 358,
      em_track_section = 359,
      em_track_e3x3_cemc = 360,
      em_track_e3x3_femc = 361,
      em_track_e3x3_eemc = 362,
      em_track_e3x3_ihcal = 363,
      em_track_e3x3_ohcal = 364,
      em_track_e3x3_fhcal = 365,
      em_track_e3x3_ehcal = 366,
      em_track_cluster_dr = 367,

      //-- EM Candidates Truth
      em_evtgen_pid = 401,
      em_evtgen_ptotal = 402,
      em_evtgen_theta = 403,
      em_evtgen_phi = 404,
      em_evtgen_charge = 405,

      //-- Event kinematics reconstructed from candidate
      em_reco_x_e = 410,
      em_reco_y_e = 411,
      em_reco_q2_e = 412,
      em_reco_w_e = 413,

      //! max limit in order to fit into 8 bit unsigned number
      prop_MAX_NUMBER = UCHAR_MAX
    };

  enum PROPERTY_TYPE
    {//
      type_int = 1,
      type_uint = 2,
      type_float = 3,
      type_unknown = -1
    };

  virtual bool  has_property(const PROPERTY prop_id) const {return false;}
  virtual float get_property_float(const PROPERTY prop_id) const {return NAN;}
  virtual int   get_property_int(const PROPERTY prop_id) const {return INT_MIN;}
  virtual unsigned int   get_property_uint(const PROPERTY prop_id) const {return UINT_MAX;}
  virtual void  set_property(const PROPERTY prop_id, const float value) {return;}
  virtual void  set_property(const PROPERTY prop_id, const int value) {return;}
  virtual void  set_property(const PROPERTY prop_id, const unsigned int value) {return;}
  static std::pair<const std::string,PROPERTY_TYPE> get_property_info(PROPERTY prop_id);
  static bool check_property(const PROPERTY prop_id, const PROPERTY_TYPE prop_type);
  static std::string get_property_type(const PROPERTY_TYPE prop_type);

protected:
  virtual unsigned int get_property_nocheck(const PROPERTY prop_id) const {return UINT_MAX;}
  virtual void set_property_nocheck(const PROPERTY prop_id,const unsigned int) {return;}
  //ClassDef(TauCandidate,1)
};

#endif
