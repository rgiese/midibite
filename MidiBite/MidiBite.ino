/* Grumpycorp Studios MidiBite
 *
 * To build, select MIDI from the "Tools > USB Type" menu
 */

const int led = 13;

uint8_t g_currentValue = 0;

void setup()
{
  //
  // Initialize I/O
  //
  pinMode(led, OUTPUT);
}

void loop()
{
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
  Serial.print("Current controller value: ");
  Serial.println(g_currentValue);

  usbMIDI.sendControlChange(0x55, g_currentValue, 1);

  ++g_currentValue;

  //
  // Outward signs of life
  //
  digitalWrite(led, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(led, LOW);   // turn the LED off by making the voltage LOW
  delay(200);               // wait for a second
}
