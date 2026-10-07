#pragma once
#include <array>
#include <cmath>
#include <algorithm>
namespace quasar {
constexpr int matterModelCount=8;
inline const char* matterModelName(int i){
 static const char* names[]={"Original","String","Bar","Plate","Membrane","Glass","Bell","Weird"};
 return names[std::clamp(i,0,7)];
}
struct ModalCell {double ratio=1,seconds=1,flow=1,strike=1,thermal=1,read=1;};
using ModalProfile=std::array<ModalCell,8>;
// Short convergent series: contact arguments stay below 4, angular order <=4.
inline double membraneParticipation(int order,double x){
 double term=1;
 for(int i=1;i<=order;++i)term*=x/(2*i);
 double sum=term;
 for(int k=1;k<=16;++k){term*=-(x*x)/(4*k*(k+order));sum+=term;}
 return std::abs(sum);
}
// Reduced models, not measured object fits. All coefficients are original.
// Original's actual equations remain in Engine.h for regression preservation.
inline ModalProfile makeMatterProfile(int model,double structure,double tone,double velocity,double amount){
 ModalProfile result{};
 const double s=std::clamp(structure,0.,1.),t=std::clamp(tone,0.,1.);
 const double hardness=.18+.62*velocity+.2*t;
 constexpr double modalPi=3.14159265358979323846;
 static const double beamRoots[]={4.7300407449,7.8532046241,10.9956078380,14.1371654913,17.2787596574,20.4203522456,23.5619449019,26.7035375555};
 static const int plateM[]={1,1,2,2,1,3,2,3},plateN[]={1,2,1,2,3,1,3,2};
 static const double membraneRoots[]={2.4048255577,3.8317059702,5.1356223018,5.5200781103,6.3801618959,7.0155866698,7.5883424345,8.4172441404};
 static const int membraneOrder[]={0,1,2,0,3,1,4,2};
 static const double glassLoss[]={.95,.62,1.25,.70,.65,1.15,.36,.75};
 static const double bellRatio[]={.5,1,1.2,1.5,2,2.74,3,4.07};
 static const double bellGeometry[]={-.07,0,.1,-.05,.025,.1,-.065,.08};
 static const double bellLoss[]={1.4,1.1,.85,.58,.74,.35,.48,.26};
 static const double bellStrike[]={.42,1,.85,.65,1.15,.62,.55,.35};
 static const double weirdA[]={.73,1,1.017,1.93,2.071,3.62,3.647,7.11};
 static const double weirdB[]={.41,1,1.083,1.31,1.379,2.57,5.83,11.47};
 static const double weirdLoss[]={1.2,.22,1.4,.32,.95,.16,1.15,.36};
 for(int i=0;i<8;++i){
  auto& c=result[i];double n=i+1.;
  switch(model){
  case 1:{ // Stiff tensioned string, fundamental kept at the MIDI anchor.
   double B=.025*s*s;c.ratio=n*std::sqrt((1+B*n*n)/(1+B));
   c.seconds=(.8+4*amount+2*s+1.2*t)/(1+.12*(1.1-t)*c.ratio*c.ratio);
   double contact=std::abs(std::sin(modalPi*n*(.22+.1*s)));
   c.flow=.35+.65*contact;c.strike=.45+.75*contact;c.thermal=.85;c.read=.95/std::pow(n,.14);
   break;}
  case 2:{ // First eight non-rigid free-free Euler-Bernoulli bending roots.
   double q=beamRoots[i]/beamRoots[0];c.ratio=q*q*(1-.18*s*(1-std::exp(-i*.5)));
   c.seconds=(.3+2*amount+6*s+1.5*t)/(1+.035*std::pow(c.ratio,1.1)/(.3+t));
   double contact=std::abs(std::cos(modalPi*(i+.5)*(.22+.18*s)));
   c.flow=.25+.6*contact;c.strike=.7+.65*contact;c.thermal=.45;c.read=1/std::pow(c.ratio,.12);
   break;}
  case 3:{ // Tensioned simply supported rectangular thin-plate reduction.
   double aspect=1.12+.55*s,inv=1/(aspect*aspect);
   double q=(plateM[i]*plateM[i]+plateN[i]*plateN[i]*inv)/(1+inv);
   double bend=.18+.82*s;c.ratio=std::sqrt(bend*q*q+(1-bend)*q);
   c.seconds=(.7+4*amount+4*s)/(1+(.06+.14*(1-t))*c.ratio);
   double contact=std::abs(std::sin(modalPi*plateM[i]*(.27+.08*s))*std::sin(modalPi*plateN[i]*(.37-.04*s)));
   c.flow=.3+.7*contact;c.strike=.6+.8*contact;c.thermal=.65;c.read=.85;
   break;}
  case 4:{ // Circular fixed-rim membrane, one representative per angular family.
   double q=membraneRoots[i]/membraneRoots[0],B=.025*s*s;
   c.ratio=q*std::sqrt((1+B*q*q)/(1+B))*(1+.018*s*membraneOrder[i]/4.);
   c.seconds=(.3+2*amount+2*s)/(1+.25*c.ratio+.08*(1-t)*c.ratio*c.ratio);
   double contact=membraneParticipation(membraneOrder[i],membraneRoots[i]*(.23+.22*s));
   c.flow=.25+.9*contact;c.strike=.4+1.1*contact;c.thermal=.65;c.read=.9;
   break;}
  case 5:{ // Thin-ring/shell inspired circumferential families n=2..5, split pairs.
   double order=2+i/2;
   double ring=order*(order*order-1)/std::sqrt(order*order+1);
   double first=6/std::sqrt(5.);
   double split=(i%2?1:-1)*(.001+.007*s)*(1+.12*(i/2));
   c.ratio=ring/first*(1+split)*(1+.06*s*std::sin((i/2)*2.1));
   c.seconds=(3+5*amount+3*s)*glassLoss[i]/(1+(.04+.12*(1-t))*c.ratio);
   c.flow=.24+.12*(i%2);c.strike=1.2*(i%2?.8:1);c.thermal=.22;c.read=1.05/std::pow(c.ratio,.08);
   break;}
  case 6:{ // Hum/prime/tierce/quint/nominal plus upper bell-inspired families.
   c.ratio=bellRatio[i]*(1+bellGeometry[i]*s);
   c.seconds=(2+7*amount+3*s)*bellLoss[i]/(1+(.03+.08*(1-t))*c.ratio);
   c.flow=.22+.28*bellStrike[i];c.strike=1.3*bellStrike[i];c.thermal=.18;c.read=i==0?.75:1;
   break;}
  case 7:{ // Intentionally impossible clusters and alternating dissipation/force polarity.
   c.ratio=std::exp((1-s)*std::log(weirdA[i])+s*std::log(weirdB[i]));
   c.seconds=(.4+7*amount+4*s)*weirdLoss[i]/(1+.02*c.ratio);
   c.flow=i%2?-.55:.85;c.strike=i%2?-.8:1.2;c.thermal=i%2?.4:.8;c.read=.85;
   break;}
  default:break;
  }
  // Contact-duration proxy: softer strikes preferentially lose high-frequency energy.
  c.strike*=std::exp(-(.018+.055*(1-hardness))*std::max(0.,c.ratio-1));
  c.seconds=std::clamp(c.seconds,.08,12.);
 }
 return result;
}
}
