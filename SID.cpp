/*
 * SID.cpp
 *
 * Created: 22.05.2019 10:13:48
 *  Author: koehn
 */ 
#include "SID.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Voice
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SID::Voice::Voice(unsigned char id, SID *sid)
{
  this->id = id;
  this->sid= sid;
  
  for ( unsigned int i= 0; i < SID_VOICE_NUM_REGS; i++ )
    regs[i]= 0;
}

void SID::Voice::writeRegister(eRegister reg, unsigned char value)
{
  unsigned char addr= ( ( id * 7 ) + reg );
  sid->writeRegister(addr, value);
  regs[reg]= value;
}

void SID::Voice::setFrequence(unsigned int freq)
{
  writeRegister(Register_FrequencyLow, ( freq & 0x00FF ) );
  writeRegister(Register_FrequencyHigh, ( (freq & 0xFF00 ) >> 8 ) ) ;
}

void SID::Voice::setPulseWidth(unsigned int width)
{
  writeRegister(Register_PulseWidthLow, ( width & 0x00FF ) );
  writeRegister(Register_PulseWidthHigh, ( ( width & 0xFF00 ) >> 8 ) ) ;
}

void SID::Voice::setGate(bool gate)
{
  unsigned char ctrlReg= regs[Register_Control];
  
  if ( gate )
    ctrlReg|= Control_Gate;
  else
    ctrlReg&= ~Control_Gate;
  
  writeRegister(Register_Control, ctrlReg);
}

void SID::Voice::setSync(bool sync)
{
  unsigned char ctrlReg= regs[Register_Control];
  
  if ( sync )
    ctrlReg|= Control_Sync;
  else
    ctrlReg&= ~Control_Sync;
  
  writeRegister(Register_Control, ctrlReg);
}

void SID::Voice::setRingMode(bool ringMode)
{
  unsigned char ctrlReg= regs[Register_Control];
  
  if ( ringMode )
    ctrlReg|= Control_RingMode;
  else
    ctrlReg&= ~Control_RingMode;
  
  writeRegister(Register_Control, ctrlReg);
}

void SID::Voice::setTest(bool test)
{
  unsigned char ctrlReg= regs[Register_Control];
  
  if ( test )
    ctrlReg|= Control_Test;
  else
    ctrlReg&= ~Control_Test;
  
  writeRegister(Register_Control, ctrlReg);
}

void SID::Voice::setWaveform(eWaveform waveForm)
{
  unsigned char ctrlReg= regs[Register_Control];
  ctrlReg= ( ( waveForm << 4 ) | ( ctrlReg & 0x0F ) );
  writeRegister(Register_Control, ctrlReg);
}

void SID::Voice::setADSR(unsigned char attack,unsigned char decay,unsigned char sustain,unsigned char release)
{
  setAttack(attack);
  setDecay(decay);
  setSustain(sustain);
  setRelease(release);
}

void SID::Voice::setAttack(unsigned char attack)
{
  unsigned char adReg= regs[Register_AttackDecay];
  adReg= ( ( attack << 4 ) | ( adReg & 0x0F ) );
  writeRegister(Register_AttackDecay, adReg);
}

void SID::Voice::setDecay(unsigned char decay)
{
  unsigned char adReg= regs[Register_AttackDecay];
  adReg= ( ( decay & 0x0F ) | ( adReg & 0xF0 ) );
  writeRegister(Register_AttackDecay, adReg);
}

void SID::Voice::setSustain(unsigned char sustain)
{
  unsigned char srReg= regs[Register_SystainRelease];
  srReg= ( ( sustain << 4 ) | ( srReg & 0x0F ) );
  writeRegister(Register_SystainRelease, srReg);
}

void SID::Voice::setRelease(unsigned char release)
{
  unsigned char srReg= regs[Register_SystainRelease];
  srReg= ( ( release & 0x0F ) | ( srReg & 0xF0 ) );
  writeRegister(Register_SystainRelease, srReg);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Filter
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SID::Filter::Filter(SID *sid)
{
  this->sid= sid;
}

void SID::Filter::writeRegister(eRegister type, unsigned char value)
{
  sid->writeRegister( 0x19 + type,value);
  regs[type]= value;  
}

void SID::Filter::setCutoffFrequency(unsigned int freq)
{
  writeRegister(Register_CutoffFrequencyLow, ( freq & 0x0007 ) );
  writeRegister(Register_CutoffFrequencyHigh , ( ( freq & 0xFFF8 ) >> 8 ) ) ;
}

void SID::Filter::setResonance(unsigned char value)
{
  unsigned char rfReg= regs[Register_ResonanceFilterSwitch];
  rfReg= ( ( value << 4 )  | ( rfReg & 0x0F ) );
  writeRegister(Register_ResonanceFilterSwitch, rfReg);
}

void SID::Filter::setActiveVoice(unsigned char voice, bool active)
{
  unsigned char vBit = 1 << ( voice - 1 );
  unsigned char rfReg= regs[Register_ResonanceFilterSwitch];
  
  if ( !active )
    vBit= ~vBit;
    
  rfReg= ( vBit | ( rfReg & 0xF0 ) );
  
  writeRegister(Register_ResonanceFilterSwitch, rfReg);
}

void SID::Filter::setMode(unsigned char modes)
{
  unsigned char mvReg= regs[Register_ModeVolume];
  mvReg= ( ( modes << 4 )  | ( mvReg & 0x0F ) );
  writeRegister(Register_ModeVolume, mvReg);
}

void SID::Filter::setVolume(unsigned char volume)
{
  unsigned char mvReg= regs[Register_ModeVolume];
  mvReg= ( ( volume & 0x0F ) | ( mvReg & 0x0F ) );
  writeRegister(Register_ModeVolume, mvReg);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SID
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SID::SID()
{
  voices[0]= new Voice(0,this);
  voices[1]= new Voice(1,this);
  voices[2]= new Voice(2,this);
  
  filter= new Filter(this);
}

void SID::writeRegister(unsigned char address, unsigned char value)
{
  
}

