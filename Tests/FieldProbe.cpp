#include "../Source/Engine.h"
#include <fstream>
#include <string>
#include <iostream>
int main(){
 quasar::Sine sine;
 for(int m:{9,10,11}){quasar::DynamicField f;quasar::Settings p;p.fieldShape=.65;p.fieldBrightness=.65;p.fieldModulation=.65;p.fieldSpread=.65;
 f.configure(m,p,261.625565,48000,0);std::ofstream file("prototype-field-"+std::to_string(m)+".f32",std::ios::binary);
 for(int n=0;n<48000*12;++n){auto x=f.tick(m,0,0,sine);float l=float(x.l),r=float(x.r);file.write((char*)&l,4);file.write((char*)&r,4);}
 std::cout<<"prototype "<<m<<" energy "<<f.stateEnergy()<<std::endl;
 }
}
