#include "../Source/Engine.h"
#include <iostream>
#include <chrono>
#include <iomanip>
using namespace quasar;
int main(int argc,char**){bool quick=argc>1;
 for(double sr:{44100.,48000.,96000.})for(int field:{7,10})for(int ex:{2,3,4}){
  if(quick&&(sr==44100||field!=7||ex!=2))continue;
  Engine e;e.prepare(sr);auto p=patch(37);p.exciter=ex;p.fieldModel=field;p.fieldModulation=1;p.fieldBrightness=1;p.fieldShape=1;p.fieldSpread=1;
  p.particles=1;p.particleModel=8;p.particleRate=1;p.particleDecay=1;p.density=1;
  p.plasma=1;p.plasmaModel=2;p.coupling=1;p.force=1;p.space=1;p.energy=1;p.release=12;
  e.setSettings(p);for(int n=48;n<56;++n)e.noteOn(n,0,1);
  for(int n=0;n<int(sr);++n)e.tick();
  auto start=std::chrono::steady_clock::now();double peak=0;
  for(int n=0;n<int(sr*6);++n){auto x=e.tick();peak=std::max(peak,std::abs(x.l));}
  double sec=std::chrono::duration<double>(std::chrono::steady_clock::now()-start).count();
  std::cout<<std::setprecision(7)<<"rate "<<sr<<" voices 8 field "<<field<<" exciter "<<ex<<" fraction "<<sec/6<<" peak "<<peak<<" state "<<e.internalEnergy()<<std::endl;
 }
 if(quick)return 0;
 for(int voices:{1,4,8}){
  Engine e;e.prepare(48000);e.setSettings(patch(39));for(int n=48;n<48+voices;++n)e.noteOn(n,0,.8);
  auto start=std::chrono::steady_clock::now();for(int n=0;n<48000*6;++n)e.tick();
  double sec=std::chrono::duration<double>(std::chrono::steady_clock::now()-start).count();
  std::cout<<"typical PlayableFriction voices "<<voices<<" fraction "<<sec/6<<std::endl;
 }
 Engine e;e.prepare(48000);auto p=patch(35);p.field=1;p.fieldModel=7;p.particles=1;p.particleModel=8;p.particleRate=1;p.density=1;p.exciter=2;p.force=1;e.setSettings(p);
 for(int n=48;n<56;++n)e.noteOn(n,0,1);for(int n=0;n<48000;++n)e.tick();for(int n=60;n<68;++n)e.noteOn(n,0,1);
 double worst=0;for(int b=0;b<6;++b){auto t=std::chrono::steady_clock::now();for(int n=0;n<256;++n)e.tick();worst=std::max(worst,std::chrono::duration<double,std::milli>(std::chrono::steady_clock::now()-t).count());}
 std::cout<<"steal burst 8 voices + 8 retired worst256ms "<<worst<<" budgetms "<<256000./48000<<std::endl;
}
