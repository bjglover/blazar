#pragma once
// Original procedural mathematics only. Included inside namespace quasar after Mode.
inline constexpr const char* fieldModelNames[]={"Original","Harmonic","Stretched","FM","Cluster","Hollow","Shimmer","Dense","Weird","Wave","Coupled"};
inline constexpr const char* particleModelNames[]={"Ring","Drop","Spark","Knock","Chirp","Scrape","Bubble","Dust","Weird"};
struct FieldBank {
 std::array<double,16> phase{},modPhase{},increment{},weight{},ratio{},modIncrement{},modDepth{};
 int model=1,resolutionCap=16;
 int partialCount()const{return std::min({Limits::fieldElements,resolutionCap,model==7?16:model==4||model==6?12:8});}
 void initialise(int m,const std::array<double,8>& old){model=m;for(int i=0;i<16;++i){phase[i]=old[i%8]+(i>=8?.173:0);modPhase[i]=phase[i]*2.7;}}
 void configure(const Settings& p,double f,double sr,double tone,double depth,double shared){
  resolutionCap=sr>=64000?8:16;
  double shape=clip(p.fieldShape+.18*(p.fieldStructure-.5)+depth*shared*.035);
  double bright=clip(tone+p.fieldBrightness-.5),spread=p.fieldSpread;
  weight.fill(0);
  double total=0;
  double amount=(.015+.18*p.fieldModulation*p.fieldModulation)*(model==3?6:1);
  if(model==4||model==7)amount*=.25;
  for(int i=0;i<partialCount();++i){
   double n=i+1.,r=n,energy=1;
   switch(model){
    case 1:r=n;energy=std::exp(-shape*.22*(n-1));break;
    case 2:r=std::pow(n,1+.52*shape);break;
    case 3:r=n*(1+.035*shape*(n-1));energy=1/(1+.06*n);break;
    case 4:{int family=i/4;double offset=(i%4-1.5)*(.002+.055*shape);
     r=(1+family*2)*(1+offset);energy=1/(1+.3*family);break;}
    case 5:r=2*n-1;energy=(i%3==1? .08+.35*(1-shape):1);break;
    case 6:r=std::pow(2.,i/4.)*(1+(i%4)*(.19+.18*shape));energy=.12+.88*clip(i/8.);break;
    case 7:r=(1+i/2)*(1+(i%2?1:-1)*(.001+.018*shape));energy=1;break;
    case 8:r=std::pow(n,.7+.5*shape)*(1+.15*std::sin(n*2.399963+shape*4));
     energy=.35+.65*std::pow(std::sin(n*1.71+shape),2);break;
   }
   // Spectral spread, never stereo width. Alternating dispersion avoids global pitch drift.
   r*=1+(i%2?1:-1)*spread*spread*.012*(1+i*.06);
   increment[i]=f*std::max(.2,r)/sr;
   weight[i]=energy*clip((.40-increment[i])/.10)/std::pow(std::max(1.,r),2.05-1.5*bright);
   ratio[i]=(model==3 ? 1+shape*2 : model==8 ? 1.618+.1*i : 2+.7*shape);
   modIncrement[i]=increment[i]*ratio[i];
   modDepth[i]=std::min(amount,std::max(0.,(.40-increment[i]*(1+ratio[i]))/(2*pi*std::max(increment[i],1e-9)*ratio[i])));
   total+=weight[i];
  }
  for(auto& w:weight)w/=std::max(1.,total);
 }
 Frame tick(const Settings& p,double feedback,double depth,double shared,const Sine& sine){
  Frame out{};

  for(int i=0;i<partialCount();++i){
   phase[i]+=increment[i];
   modPhase[i]+=modIncrement[i];
   if(weight[i]<=1e-12)continue;
   double mod=sine(modPhase[i]+i*.13+depth*shared*.08);
   double a=sine(phase[i]+modDepth[i]*mod+feedback)*weight[i];
   double pan=i%2?.2:-.2;out.l+=a*(1-pan);out.r+=a*(1+pan);
  }
  return out;
 }
};
struct ParticleEvent {
 bool active=false;int model=0;
 double phase=0,second=0,age=0,seconds=.1,f=440,amplitude=0,pan=0,shape=0,spread=0,low=0,previous=0;
 double invRate=0,invDuration=0,envelope=1,envelopePole=1,noiseCoefficient=0,curve=0,curveStep=1;
 Mode a{},b{};Random random;
 void start(int m,double base,double decay,double variation,double energy,double rate,uint32_t seed){
  *this=ParticleEvent{};active=true;model=m;random.state=seed?seed:1;spread=variation;
  pan=random.bi();shape=random.unit();amplitude=energy;
  static const double scales[]={1,1.25,8,.55,2,1.3,2.3,9,3};
  f=clip(base*scales[m]*std::pow(2.,random.bi()*variation*1.5),12,rate*.29);
  static const double lifetimes[]={1,.32,.018,.095,.14,.22,.22,.012,.38};
  seconds=clip(lifetimes[m]*decay*(.7+.6*shape),.003,8.);
  invRate=1/rate;invDuration=1/seconds;
  envelopePole=std::exp(-6.907755*invRate*invDuration);
  noiseCoefficient=1-std::exp(-2*pi*f*(m==2?.35:.3)*invRate);
  if(m==1){curve=.8;curveStep=std::exp(-12*invRate*invDuration);}
  if(m==4){double direction=shape<.5?-1:1;curve=f*std::pow(2.,-direction*(2+2*spread)*.5);curveStep=std::pow(2.,direction*(2+2*spread)*invRate*invDuration);}
  a.pan=pan;a.tune(f,rate,seconds);
  b.tune(f*(m==3?2.756:m==8?1.414:1.67),rate,seconds*.63);
  if(m==3){a.x=energy;b.x=energy*.35;}
 }
 Frame tick(double sr,const Sine& sine){
  if(!active)return {};
  age+=invRate;double t=age*invDuration;
  if(t>=1){active=false;return {};}
  envelope*=envelopePole;double shapedEnvelope=envelope*clip(age*sr/12.)*clip((seconds-age)*sr/24.);
  double noise=(model==2||model==5||model==7)?random.bi():0,value=0,frequency=f;
  switch(model){
   case 1:curve*=curveStep;frequency=f*(1+curve);break; // rounded downward droplet
   case 4:curve*=curveStep;frequency=curve;break;
   case 6:frequency=f*(.45+1.9*t*t);break; // accelerating upward bubble curvature
   case 8:frequency=f*(1+.35*sine((19*t+shape*6)/(2*pi)));break;
  }
  frequency=clip(frequency,10,sr*.32);
  phase+=frequency*invRate;second+=std::min(sr*.38,frequency*(model==8?1.414:1.67))*invRate;
  switch(model){
   case 1:value=(sine(phase)+.15*sine(second))*amplitude*shapedEnvelope;break;
   case 2:low+=noiseCoefficient*(noise-low);value=(noise-low+.12*sine(phase))*amplitude*shapedEnvelope;break;
   case 3:value=(a.tick(0)+b.tick(0))*(1-.65*t);break;
   case 4:value=sine(phase+.08*spread*sine(second))*amplitude*shapedEnvelope;break;
   case 5:{
    double impulse=random.unit()<(70+450*shape)/sr?random.bi()*amplitude*.38:0;
    value=(a.tick(impulse)+.3*b.tick(impulse))*std::pow(1-t,2);break;}
   case 6:value=(sine(phase)+.07*sine(second))*amplitude*shapedEnvelope;break;
   case 7:low+=noiseCoefficient*(noise-low);value=(low*.8+(noise-low)*(.1+.6*shape))*amplitude*shapedEnvelope*.45;break;
   case 8:value=(sine(phase+.2*sine(second))+.45*sine(second+shape))*amplitude*shapedEnvelope;break;
  }
  value=std::clamp(value,-1.,1.);
  return {value*(1-.5*pan),value*(1+.5*pan),0};
 }
};
class FiniteParticles {
 std::array<ParticleEvent,Limits::events> events{};
public:
 void trigger(int model,double f,double decay,double spread,double amplitude,double sr,uint32_t seed){
  ParticleEvent* slot=&events[0];
  int limit=std::min(Limits::events,sr>=64000?((model==4||model==8)?4:10):((model==4||model==8)?8:16));
  for(int i=0;i<limit;++i){auto& e=events[i];if(!e.active){slot=&e;break;}if(e.age/e.seconds>slot->age/slot->seconds)slot=&e;}
  // A occupied slot is not abruptly overwritten. Saturation drops arrivals, bounding population.
  if(slot->active)return;
  slot->start(model,f,decay,spread,amplitude,sr,seed);
 }
 Frame tick(double sr,const Sine& sine){Frame out;for(auto& e:events){auto a=e.tick(sr,sine);out.l+=a.l;out.r+=a.r;}return out;}
};
