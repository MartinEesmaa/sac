#ifndef BIAS_H
#define BIAS_H

#include "../global.h"
#include "../common/utils.h"
#include "lms.h"

#define BIAS_ROUND_PRED 1
#define BIAS_MIX_N 3
#define BIAS_MIX_NUMCTX 4
#define BIAS_MIX 0
#define BIAS_NAVG 5

class BiasEstimator {
  class CntAvg {
    struct bias_cnt {
      int cnt;
      double val;
    };
    public:
      CntAvg(int nb_scale=5,int freq0=4)
      :nscale(1<<nb_scale)
      {
         bias.cnt=freq0;
         bias.val=0.0;
      }
      double get() const
      {
        return bias.val/double(bias.cnt);
      }
      void update(double delta) {
        bias.val+=delta;
        bias.cnt++;
        if (bias.cnt>=nscale) {
          bias.val/=2.0;
          bias.cnt>>=1;
        }
      }
    private:
      const int nscale;
      bias_cnt bias;
  };

  public:
    BiasEstimator(double lms_mu=0.003,int nb_scale=5,double nd_sigma=1.5,double nd_lambda=0.998)
    :
    #if BIAS_MIX == 0
      mix_ada(BIAS_MIX_NUMCTX,SSLMS(BIAS_MIX_N,lms_mu)),
    #elif BIAS_MIX == 1
      mix_ada(BIAS_MIX_NUMCTX,LAD_ADA(BIAS_MIX_N,lms_mu,0.96)),
    #elif BIAS_MIX == 2
      mix_ada(BIAS_MIX_NUMCTX,LMS_ADA(BIAS_MIX_N,lms_mu,0.965,0.005)),
    #endif
    hist_input(8),hist_delta(8),
    cnt0(1<<6,CntAvg(nb_scale)),
    cnt1(1<<6,CntAvg(nb_scale)),
    cnt2(1<<6,CntAvg(nb_scale)),
    sigma(nd_sigma),lambda(nd_lambda)
    {
      ctx0=ctx1=ctx2=mix_ctx=0;
      p=0.0;
      mean_est=var_est=0.;
    }
    void CalcContext()
    {
      int b0=hist_input[0]>p?0:1;
      //int b1=hist_input[1]>p?0:1;

      int b2=hist_delta[0]<0?0:1;
      int b3=hist_delta[1]<0?0:1;
      int b4=hist_delta[2]<0?0:1;
      //int b42=hist_delta[3]<0?0:1;
      int b5=hist_delta[1]<hist_delta[0]?0:1;
      int b6=hist_delta[2]<hist_delta[1]?0:1;
      int b7=hist_delta[3]<hist_delta[2]?0:1;
      int b8=hist_delta[4]<hist_delta[3]?0:1;
      int b9=(fabs(hist_delta[0]))>32?0:1;
      int b10=2*hist_input[0]-hist_input[1]>p?0:1;
      int b11=3*hist_input[0]-3*hist_input[1]+hist_input[2]>p?0:1;

      double sum=0;
      for (int i=0;i<BIAS_NAVG;i++)
        sum+=fabs(hist_delta[i]);
      sum /= static_cast<double>(BIAS_NAVG);

      int t=0;
      if (sum>512) t=2;
      else if (sum>32) t=1;

      ctx0=0;
      ctx0+=b0<<0;
      //ctx0+=b1<<1;
      ctx0+=b2<<1;
      ctx0+=b9<<2;
      ctx0+=b10<<3;
      ctx0+=b11<<4;

      ctx1=0;
      ctx1+=b2<<0;
      ctx1+=b3<<1;
      ctx1+=b4<<2;
      //ctx1+=t<<3;

      ctx2=0;
      ctx2+=b5<<0;
      ctx2+=b6<<1;
      ctx2+=b7<<2;
      ctx2+=b8<<3;
      //ctx2+=mix_ctx<<4;

      mix_ctx=t;
    }
    double Predict(double pred)
    {
      p=pred;
      CalcContext();

      const double bias0=cnt0[ctx0].get();
      const double bias1=cnt1[ctx1].get();
      const double bias2=cnt2[ctx2].get();
      const double pbias=mix_ada[mix_ctx].Predict({bias0,bias1,bias2});
      return pred+pbias;
    }
    void Update(double val) {
      #ifdef BIAS_ROUND_PRED
        const double delta=val-round(p);
      #else
        const double delta=val-p;
      #endif
      miscUtils::RollBack(hist_input,val);
      miscUtils::RollBack(hist_delta,delta);

      const double q=sigma*sqrt(var_est);
      const double lb=mean_est-q;
      const double ub=mean_est+q;

      if ( (delta>lb) && (delta<ub)) {
        cnt0[ctx0].update(delta);
        cnt1[ctx1].update(delta);
        cnt2[ctx2].update(delta);
      }

      mix_ada[mix_ctx].Update(delta);

      mean_est=lambda*mean_est+(1.0-lambda)*delta;
      var_est=lambda*var_est+(1.0-lambda)*((delta-mean_est)*(delta-mean_est));
    }
  private:
    #if BIAS_MIX == 0
      std::vector<SSLMS> mix_ada;
    #elif BIAS_MIX == 1
      std::vector<LAD_ADA> mix_ada;
    #elif BIAS_MIX == 2
      std::vector<LMS_ADA> mix_ada;
    #endif
    vec1D hist_input,hist_delta;
    int ctx0,ctx1,ctx2,mix_ctx;
    double p;
    //double alpha,p,bias0,bias1,bias2;
    std::vector<CntAvg> cnt0,cnt1,cnt2;
    double mean_est,var_est;
    const double sigma,lambda;
};


#endif // BIAS_H
