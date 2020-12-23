//
// Grumpycorp Studios MidiBite
//

//
// Configuration
//

// I/O
uint8_t const c_AnalogInputPin = A9;
float const c_AnalogInput_MinValue = 500;
float const c_AnalogInput_MaxValue = 1000;

uint8_t const c_LEDPin = 13;

// Midi
uint8_t const c_MidiChannel = 0x0;
uint8_t const c_MidiCC = 0x55;

// Timing
unsigned long const c_LoopDesiredCadence_msec = 50;


//
// State
//

uint32_t g_LoopIteration = 0;
uint8_t g_LastValueSent = 0;

//
// Setup
//

void setup()
{
  //
  // Initialize I/O
  //
  pinMode(c_LEDPin, OUTPUT);
}

//
// Helpers
//

namespace std
{
template <class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi)
{
  return (v < lo) ? lo : (hi < v) ? hi : v;
}
}  // namespace std

//
// Loop
//

void loop()
{
  unsigned long const loopStartTime_msec = millis();

  //
  // Process incoming MIDI
  //

  while (usbMIDI.read())
  {
    // MIDI Controllers should discard incoming MIDI messages so the queue doesn't get blocked.
    // http://forum.pjrc.com/threads/24179-Teensy-3-Ableton-Analog-CC-causes-midi-crash
  }

  //
  // Process outgoing MIDI
  //

  bool fDidSendMidi = false;
  {
    float const analogValue = analogRead(c_AnalogInputPin);

    float const value0to100 =
        (std::clamp(analogValue, c_AnalogInput_MinValue, c_AnalogInput_MaxValue) - c_AnalogInput_MinValue) /
        (c_AnalogInput_MaxValue - c_AnalogInput_MinValue);

    uint8_t const c_MidiValue_Range = 127;  // 7 bit value
    uint8_t const midiValue = static_cast<uint8_t>(value0to100 * c_MidiValue_Range);

    if (false)
    {
      Serial.print("Analog: ");
      Serial.print(analogValue);
      Serial.print(" -> MIDI value: ");
      Serial.println(midiValue);
    }

    // Rate limit
    if ((midiValue != g_LastValueSent))
    {
      usbMIDI.sendControlChange(c_MidiCC, midiValue, c_MidiChannel);

      g_LastValueSent = midiValue;
      fDidSendMidi = true;
    }
  }

  //
  // Outward signs of life
  //

  digitalWrite(c_LEDPin, ((g_LoopIteration % 20 == 0) || fDidSendMidi) ? HIGH : LOW);

  //
  // Delay
  //

  while (true)
  {
    // (Carefully phrased to deal with rollovers)
    unsigned long const currentTime_msec = millis();
    unsigned long const timeSinceLastLoopStart_msec = currentTime_msec - loopStartTime_msec;

    if (timeSinceLastLoopStart_msec > c_LoopDesiredCadence_msec)
    {
      // No further delay required
      break;
    }

    unsigned long const remainingTotalDelay_msec = c_LoopDesiredCadence_msec - timeSinceLastLoopStart_msec;

    delay(remainingTotalDelay_msec);
  }

  ++g_LoopIteration;
}
