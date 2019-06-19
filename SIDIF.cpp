/*
 * SIDIF.cpp
 *
 * Created: 22.05.2019 10:13:48
 *  Author: koehn
 */ 
#include <string.h>

#include "SIDIF.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Voice
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SIDIF::Voice::create(SIDIF *sidif, unsigned char id)
{
  this->id   = id;
  this->sidif= sidif;
  
  for ( unsigned int i= 0; i < SID_VOICE_NUM_REGS; i++ )
    regs[i]= 0;
}

void SIDIF::Voice::writeRegister(eRegister reg, unsigned char value)
{
  unsigned char addr= ( ( id * 7 ) + reg );
  sidif->writeRegister(addr, value);
  regs[reg]= value;
}

void SIDIF::Voice::setFrequence(unsigned int freq)
{
  writeRegister(Register_FrequencyLow, ( freq & 0x00FF ) );
  writeRegister(Register_FrequencyHigh, ( (freq & 0xFF00 ) >> 8 ) );
}

void SIDIF::Voice::setPulseWidth(unsigned int width)
{
  writeRegister(Register_PulseWidthLow, ( width & 0x00FF ) );
  writeRegister(Register_PulseWidthHigh, ( ( width & 0xFF00 ) >> 8 ) ) ;
}

void SIDIF::Voice::setGate(bool gate)
{
  unsigned char ctrlReg= regs[Register_Control];
  
  if ( gate )
    ctrlReg|= Control_Gate;
  else
    ctrlReg&= ~Control_Gate;
  
  writeRegister(Register_Control, ctrlReg);
}

void SIDIF::Voice::setSync(bool sync)
{
  unsigned char ctrlReg= regs[Register_Control];
  
  if ( sync )
    ctrlReg|= Control_Sync;
  else
    ctrlReg&= ~Control_Sync;
  
  writeRegister(Register_Control, ctrlReg);
}

void SIDIF::Voice::setRingMode(bool ringMode)
{
  unsigned char ctrlReg= regs[Register_Control];
  
  if ( ringMode )
    ctrlReg|= Control_RingMode;
  else
    ctrlReg&= ~Control_RingMode;
  
  writeRegister(Register_Control, ctrlReg);
}

void SIDIF::Voice::setTest(bool test)
{
  unsigned char ctrlReg= regs[Register_Control];
  
  if ( test )
    ctrlReg|= Control_Test;
  else
    ctrlReg&= ~Control_Test;
  
  writeRegister(Register_Control, ctrlReg);
}

void SIDIF::Voice::setWaveform(eWaveform waveForm)
{
  unsigned char value= (unsigned char)waveForm;
  if ( value > 0x0F)
    value= 0x0F;

  unsigned char ctrlReg= regs[Register_Control];
  ctrlReg= ( ( value << 4 ) | ( ctrlReg & 0x0F ) );
  writeRegister(Register_Control, ctrlReg);
}

void SIDIF::Voice::setADSR(unsigned char attack,unsigned char decay,unsigned char sustain,unsigned char release)
{
  setAttack(attack);
  setDecay(decay);
  setSustain(sustain);
  setRelease(release);
}

void SIDIF::Voice::setAttack(unsigned char attack)
{
  unsigned char adReg= regs[Register_AttackDecay];
  adReg= ( ( attack << 4 ) | ( adReg & 0x0F ) );
  writeRegister(Register_AttackDecay, adReg);
}

void SIDIF::Voice::setDecay(unsigned char decay)
{
  unsigned char adReg= regs[Register_AttackDecay];
  adReg= ( ( decay & 0x0F ) | ( adReg & 0xF0 ) );
  writeRegister(Register_AttackDecay, adReg);
}

void SIDIF::Voice::setSustain(unsigned char sustain)
{
  unsigned char srReg= regs[Register_SystainRelease];
  srReg= ( ( sustain << 4 ) | ( srReg & 0x0F ) );
  writeRegister(Register_SystainRelease, srReg);
}

void SIDIF::Voice::setRelease(unsigned char release)
{
  unsigned char srReg= regs[Register_SystainRelease];
  srReg= ( ( release & 0x0F ) | ( srReg & 0xF0 ) );
  writeRegister(Register_SystainRelease, srReg);
}

void SIDIF::Voice::getRegs(VoiceRegisters *regs)
{
  memcpy(*regs,this->regs,sizeof(VoiceRegisters));
}

void SIDIF::Voice::setRegs(VoiceRegisters *regs)
{
  memcpy(this->regs,*regs,sizeof(VoiceRegisters));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Filter
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SIDIF::Filter::create(SIDIF *sidif)
{
  this->sidif= sidif;

  for ( unsigned int i= 0; i < SID_FILTER_NUM_REGS; i++ )
    regs[i]= 0;
}

void SIDIF::Filter::writeRegister(eRegister type, unsigned char value)
{
  sidif->writeRegister( 0x15 + type,value);
  regs[type]= value;  
}

void SIDIF::Filter::setCutoffFrequency(unsigned int freq)
{
  writeRegister(Register_CutoffFrequencyLow, ( freq & 0x0007 ) );
  writeRegister(Register_CutoffFrequencyHigh , ( ( freq & 0xFFF8 ) >> 8 ) ) ;
}

void SIDIF::Filter::setResonance(unsigned char value)
{
  unsigned char rfReg= regs[Register_ResonanceFilterSwitch];
  rfReg= ( ( value << 4 )  | ( rfReg & 0x0F ) );
  writeRegister(Register_ResonanceFilterSwitch, rfReg);
}

void SIDIF::Filter::setActiveVoice(unsigned char voice, bool active)
{
  unsigned char vBit = 1 << ( voice - 1 );
  unsigned char rfReg= regs[Register_ResonanceFilterSwitch];
  
  if ( !active )
    vBit= ~vBit;
    
  rfReg= ( vBit | ( rfReg & 0xF0 ) );
  
  writeRegister(Register_ResonanceFilterSwitch, rfReg);
}

void SIDIF::Filter::setMode(unsigned char modes)
{
  unsigned char mvReg= regs[Register_ModeVolume];
  mvReg= ( ( modes << 4 )  | ( mvReg & 0x0F ) );
  writeRegister(Register_ModeVolume, mvReg);
}

void SIDIF::Filter::setVolume(unsigned char volume)
{
  unsigned char mvReg= regs[Register_ModeVolume];
  mvReg= ( ( volume & 0x0F ) | ( mvReg & 0x0F ) );
  writeRegister(Register_ModeVolume, mvReg);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SID
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SIDIF::SIDIF()
{
  voices[0].create(this,0);
  voices[1].create(this,1);
  voices[2].create(this,2);
  
  filter.create(this);
}