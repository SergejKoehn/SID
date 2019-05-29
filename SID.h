/*
 * SID.h
 *
 * Created: 22.05.2019 10:13:38
 *  Author: koehn
 */ 

#ifndef SID_H_
#define SID_H_

#define SID_VOICE_NUM_REGS  7
#define SID_FILTER_NUM_REGS 4

typedef unsigned char VoiceRegisters[SID_VOICE_NUM_REGS];

class SID
{
public:
  class Voice
  {
    protected:
      enum eRegister {
        Register_FrequencyLow   = 0,  
        Register_FrequencyHigh  = 1,  
        Register_PulseWidthLow  = 2,  
        Register_PulseWidthHigh = 3,  
        Register_Control        = 4,  
        Register_AttackDecay    = 5,  
        Register_SystainRelease = 6,  
      };
      
      SID           *sid;
      unsigned char  id;
      VoiceRegisters regs;
      
    public:
      enum eWaveform {
        Waveform_Triangle  = 0x01,
        Waveform_Sawtooth  = 0x02,
        Waveform_Pulse     = 0x04,
        Waveform_Noise     = 0x08,
      };
      
      enum eControl {
        Control_Gate     = 0x01,
        Control_Sync     = 0x02,
        Control_RingMode = 0x04,
        Control_Test     = 0x08,
      };
      
    protected:
      void writeRegister(eRegister reg, unsigned char value);
            
    public:
      void setFrequence(unsigned int freq);
      void setPulseWidth(unsigned int width);
      void setGate(bool gate);
      void setSync(bool sync);
      void setRingMode(bool ringMode);
      void setTest(bool test);
      void setWaveform(eWaveform waveForm);
      void setADSR(unsigned char attack,unsigned char decay,unsigned char sustain,unsigned char release);
      void setAttack(unsigned char attack);
      void setDecay(unsigned char decay);
      void setSustain(unsigned char sustain);
      void setRelease(unsigned char release);
      
      void getRegs(VoiceRegisters *regs);
      void setRegs(VoiceRegisters *regs);
      
      void create(SID *sid,unsigned char id);
  };

  class Filter
  {
    protected:
      SID          *sid;
      unsigned char regs[SID_FILTER_NUM_REGS];
      
      enum eRegister{
        Register_CutoffFrequencyLow   = 0,
        Register_CutoffFrequencyHigh  = 1,
        Register_ResonanceFilterSwitch= 2,
        Register_ModeVolume           = 3,
      };      
      
    public:
      enum eMode
      { 
        Mode_LowPass = 0x01,
        Mode_BandPass= 0x02,
        Mode_HighPass= 0x04,
        Mode_Off3    = 0x08,
      };
      
    protected:
      void writeRegister(eRegister type, unsigned char value);
      
    public:
      void setCutoffFrequency(unsigned int freq);
      void setResonance(unsigned char value);
      void setActiveVoice(unsigned char voice, bool active);
      void setMode(unsigned char modes);
      void setVolume(unsigned char volume);
      
      void create(SID *sid);
  };
  
public:
  Voice  voices[3];
  Filter filter;
  
public:
  virtual void writeRegister(unsigned char address, unsigned char value);
  SID();
};


#endif /* SID_H_ */