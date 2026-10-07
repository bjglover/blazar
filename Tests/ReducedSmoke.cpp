#define QUASAR_VOICES 2
#define QUASAR_MODES 4
#define QUASAR_FIELD_ELEMENTS 4
#define QUASAR_PARTICLE_EVENTS 4
#define QUASAR_SPACE_ENABLED 0
#include "../Source/Engine.h"
#include <iostream>
int main(){quasar::Engine e;e.prepare(48000);auto p=quasar::patch(35);p.exciter=2;p.fieldModel=10;p.particles=1;e.setSettings(p);e.noteOn(48,0,.8);e.noteOn(60,0,.8);
for(int n=0;n<48000*4;++n){auto x=e.tick();if(!std::isfinite(x.l)||!std::isfinite(x.r)||e.internalEnergy()>100)return 1;}
e.releaseAll();for(int n=0;n<48000*15;++n)e.tick();if(e.activeVoices())return 2;std::cout<<"PASS reduced: 2 voices / 4 modes / 4 Field elements / 4 events / Space disabled"<<std::endl;}
