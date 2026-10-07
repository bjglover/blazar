#pragma once
#ifndef QUASAR_VOICES
#define QUASAR_VOICES 8
#endif
#ifndef QUASAR_MODES
#define QUASAR_MODES 8
#endif
#ifndef QUASAR_FIELD_ELEMENTS
#define QUASAR_FIELD_ELEMENTS 16
#endif
#ifndef QUASAR_PARTICLE_EVENTS
#define QUASAR_PARTICLE_EVENTS 16
#endif
#ifndef QUASAR_SPACE_ENABLED
#define QUASAR_SPACE_ENABLED 1
#endif
struct Limits{
 static constexpr bool spaceEnabled=QUASAR_SPACE_ENABLED!=0;
 static constexpr int voices=QUASAR_VOICES,modes=QUASAR_MODES,fieldElements=QUASAR_FIELD_ELEMENTS,events=QUASAR_PARTICLE_EVENTS;
 static_assert(voices>0&&modes>0&&modes<=8&&fieldElements>=4&&fieldElements<=16&&events>0&&events<=16);
};
