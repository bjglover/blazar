#include "../Source/Engine.h"
#include "../Source/Parameters.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <chrono>
using namespace quasar;
void require(bool x,const char* s){if(!x)throw std::runtime_error(s);}
int main(){try{
 Engine e;double peak=0,maxState=0,maxDC=0,maxPre=0;int cases=0;
 auto run=[&](double seconds,double sr){double energy=0,dc=0;for(int n=0;n<int(seconds*sr);++n){
  auto x=e.tick();require(std::isfinite(x.l)&&std::isfinite(x.r),"nonfinite output");
  require(std::abs(x.l)<.951&&std::abs(x.r)<.951,"output bound");
  maxPre=std::max(maxPre,e.preSafetyPeak());require(e.preSafetyPeak()<8,"pre-limiter explosion");
  peak=std::max(peak,std::max(std::abs(x.l),std::abs(x.r)));energy+=x.l*x.l+x.r*x.r;dc+=x.l+x.r;
  if((n&255)==0){double s=e.internalEnergy();require(std::isfinite(s)&&s<100,"internal state runaway");maxState=std::max(maxState,s);}
 }maxDC=std::max(maxDC,std::abs(dc/(seconds*sr*2)));return std::sqrt(energy/(seconds*sr*2));};
 for(int ex=0;ex<6;++ex){Settings p;p.field=p.particles=p.plasma=p.space=p.coupling=p.force=0;p.exciter=ex;e.prepare(48000);e.setSettings(p);e.noteOn(60,0,.8);require(run(.5,48000)==0,"Force zero still excites");}
 std::cout<<"PASS Force zero silences all exciters"<<std::endl;
 for(double sr:{44100.,48000.,96000.})for(int ex=0;ex<6;++ex)for(int model=0;model<8;++model)for(int corner=0;corner<2;++corner)for(int note:{36,60,84}){
  Settings p;p.field=p.particles=p.plasma=p.space=0;p.coupling=corner;p.matter=1;p.matterModel=model;p.exciter=ex;p.force=corner?1:.25;p.speed=corner;p.hardness=corner;p.position=corner?.97:.03;p.structure=corner;p.energy=1;p.evolve=corner;p.attack=.01;p.release=.5;
  e.prepare(sr);e.setSettings(p);e.noteOn(note,0,corner?1:.2);run(.6,sr);e.releaseAll();run(.3,sr);++cases;
 }
 std::cout<<"PASS "<<cases<<" exciter/model/rate/position/force/speed/note corners"<<std::endl;
 // DONG test: all continuous exciters, all resonators, no other energy source.
 for(int ex=2;ex<6;++ex)for(int model=0;model<8;++model){
  Settings p;p.field=p.particles=p.plasma=p.space=0;p.coupling=0;p.matter=1;p.matterModel=model;p.exciter=ex;p.force=.65;p.speed=.4;p.attack=.01;p.release=1;
  e.prepare(48000);e.setSettings(p);e.noteOn(60,0,.8);
  double first=run(2,48000),late=run(8,48000);
  require(late>1e-4,"continuous exciter failed to sustain");
  e.releaseAll();run(8,48000);double tail=run(1,48000);require(e.activeVoices()==0&&tail<1e-7,"continuous retirement");
  std::cout<<"sustain ex "<<ex<<" matter "<<model<<" first "<<first<<" late "<<late<<" tail "<<tail<<std::endl;
 }
 for(int particle=0;particle<9;++particle){
  auto p=patch(30);p.particleModel=particle;p.particleRate=.8;p.particleSelf=0;p.particleImpact=1;p.matterModel=particle%8;
  e.prepare(48000);e.setSettings(p);e.noteOn(60,0,.8);double invisible=run(5,48000);
  require(invisible>1e-6,"invisible particles produce no Matter");
  p.particleImpact=0;e.prepare(48000);e.setSettings(p);e.noteOn(60,0,.8);require(run(2,48000)<1e-12,"SELF zero IMPACT zero leakage");
  p.particleSelf=1;e.prepare(48000);e.setSettings(p);e.noteOn(60,0,.8);require(run(2,48000)>1e-5,"particle self broken");
  std::cout<<"invisible particle "<<particle<<" RMS "<<invisible<<std::endl;
 }
 for(int plasma=0;plasma<3;++plasma){
  auto p=patch(36);p.plasmaModel=plasma;p.field=0;p.plasmaSelf=0;p.plasmaDrive=1;
  e.prepare(48000);e.setSettings(p);e.noteOn(60,0,.8);double invisible=run(5,48000);require(invisible>1e-6,"invisible Plasma no Matter");
  p.plasmaDrive=0;e.prepare(48000);e.setSettings(p);e.noteOn(60,0,.8);require(run(2,48000)<1e-12,"Plasma SELF/DRIVE leakage");
  std::cout<<"invisible plasma "<<plasma<<" RMS "<<invisible<<std::endl;
 }
 for(double sr:{44100.,48000.,96000.})for(int f=0;f<11;++f)for(int plasma=0;plasma<3;++plasma){
  auto p=patch(34);p.fieldModel=f;p.plasmaModel=plasma;p.plasma=1;p.particles=1;p.particleModel=f%9;p.force=1;p.energy=1;p.coupling=1;p.space=1;p.evolve=1;p.motion=1;p.release=1;
  p.fieldShape=p.fieldBrightness=p.fieldModulation=p.fieldSpread=f%2;
  e.prepare(sr);e.setSettings(p);for(int n=48;n<56;++n)e.noteOn(n,0,1);run(.8,sr);e.releaseAll();run(.4,sr);++cases;
 }
 std::cout<<"PASS "<<cases<<" combined short cases"<<std::endl;
 // Long, fully interconnected worst-case systems; note-off must remove drive.
 for(int ex=0;ex<6;++ex){
  auto p=patch(37);p.exciter=ex;p.fieldModel=ex%2?10:9;p.plasmaModel=ex%3;p.plasma=1;p.particles=1;p.particleRate=1;p.particleDecay=1;p.particleModel=ex%9;p.density=1;p.force=1;p.energy=1;p.coupling=1;p.space=1;p.evolve=1;p.motion=1;p.release=2;
  e.prepare(48000);e.setSettings(p);for(int n=48;n<56;++n)e.noteOn(n,0,1);run(30,48000);e.releaseAll();run(24,48000);require(e.activeVoices()==0&&run(1,48000)<1e-7,"long feedback release");
  std::cout<<"long feedback exciter "<<ex<<" PASS"<<std::endl;
 }
 // Automation, sustain, performance MIDI, model switching and stealing.
 e.prepare(96000);auto p=patch(34);p.plasma=1;p.particles=1;e.setSettings(p);
 for(int i=0;i<96;++i){
  p.exciter=i%6;p.matterModel=i%8;p.fieldModel=i%11;p.particleModel=i%9;p.plasmaModel=i%3;
  for(auto& param:floatParameters)if(param.id!=std::string("attack")&&param.id!=std::string("release"))p.*param.member=i%2;
  p.attack=.01;p.release=.5;e.setSettings(p);e.noteOn(48+i%24,0,.8);e.pressure(0,(i%3)/2.);e.wheel(0,(i%4)/3.);e.bend(0,(i%3)-1.);run(.025,96000);e.noteOff(48+i%24,0);
 }
 e.releaseAll();run(10,96000);require(e.activeVoices()==0,"switching/stealing stuck voices");
 e.prepare(48000);e.setSettings(patch(39));e.noteOn(60,0,.8);e.sustain(0,true);e.noteOff(60,0);run(2,48000);require(e.activeVoices()==1,"sustain hold");e.sustain(0,false);run(12,48000);require(e.activeVoices()==0,"sustain release");
 require(maxDC<.1,"runaway DC");
 std::cout<<"PASS all safety: peak "<<peak<<" internal state max "<<maxState<<" max window DC "<<maxDC<<" max pre-safety "<<maxPre<<std::endl;
 return 0;
}catch(std::exception& x){std::cerr<<"FAIL "<<x.what()<<std::endl;return 1;}}
