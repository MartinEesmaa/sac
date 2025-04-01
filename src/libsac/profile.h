#ifndef PROFILE_H
#define PROFILE_H

#include <vector>
#include "map.h"
#include "pred.h"

class SACProfile {
  public:
    struct FrameStats {
      int maxbpn,maxbpn_map;
      bool enc_mapped;
      int32_t blocksize,minval,maxval,mean;
      Remap mymap;
    };
    struct elem {
      float vmin, vmax;
      enum Type { FLOAT, UINT16 } type;
      union {
        float floatVal;
        uint16_t uint16Val;
      } val;

      elem(float vmin, float vmax, float floatVal) : vmin(vmin), vmax(vmax), type(FLOAT) {
        val.floatVal = floatVal;
      }

      elem(float vmin, float vmax, uint16_t uint16Val) : vmin(vmin), vmax(vmax), type(UINT16) {
        val.uint16Val = uint16Val;
      }
    };
    void add_float(float vmin,float vmax,float val) {
      vparam.push_back(elem{vmin,vmax,val});
    }
    float get_float()
    {
      if (vparam[index].type == elem::FLOAT) {
        return vparam[index].val.floatVal;
      }
    }
    void add_ols() {
      add_float(0.99,0.9999,0.998); // lambda
      add_float(0.001,10.0,0.001); // ols-nu
      add_float(4,32,16); //
    }
    void get_ols(Predictor::tparam &param)
    {
      param.lambda0 = get_float();
      param.ols_nu0 = get_float();
      param.nA = get_float();
    }

    void set_profile()
    {
      add_ols();
    }
    void get_profile(Predictor::tparam &param,bool optimize=false)
    {
      if (optimize) param.k=4;
      else param.k=1;
      index = 0;
      get_ols(param);
    }
    SACProfile()
    {
    }
    std::vector<elem> vparam;
  protected:
    int index;
};

class SacProfile {
  public:
    struct FrameStats {
      int maxbpn,maxbpn_map;
      bool enc_mapped;
      int32_t blocksize,minval,maxval,mean;
      Remap mymap;
    };

    struct coef {
      float vmin,vmax,vdef;
    };

      SacProfile(){};
      void Init(int numcoefs)
      {
         coefs.resize(numcoefs);
      }
      SacProfile(int numcoefs)
      :coefs(numcoefs)
      {

      }
      int LoadBaseProfile();
      std::size_t get_size() {return coefs.size();};
      void Set(int num,double vmin,double vmax,double vdef)
      {
        if (num>=0 && num< static_cast<int>(coefs.size())) {
          coefs[num].vmin=vmin;
          coefs[num].vmax=vmax;
          coefs[num].vdef=vdef;
        }
      }
      void Set(int num,const std::vector<float>&x)
      {
        if (num>=0 && num<static_cast<int>(coefs.size()) && (x.size()>=3)) {
          coefs[num].vmin=x[0];
          coefs[num].vmax=x[1];
          coefs[num].vdef=x[2];
        }
      }
      float Get(std::size_t num) const {
        if (num<coefs.size()) {
            return coefs[num].vdef;
        } else return 0.;
      }
      std::vector <coef> coefs;
};

#endif // PROFILE_H
