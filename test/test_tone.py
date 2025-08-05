# test_tone.py
import numpy as np
import sounddevice as sd
import time

# Configuration
SAMPLE_RATE = 44100  # Hz
DURATION = 5         # seconds
FREQUENCY = 440.0    # Hz (A4)

print(f"Playing {FREQUENCY} Hz sine wave for {DURATION} seconds...")

# Generate waveform
t = np.linspace(0, DURATION, int(SAMPLE_RATE * DURATION), endpoint=False)
waveform = 0.4 * np.sin(2 * np.pi * FREQUENCY * t)  # 0.4 = volume scaling

try:
    sd.play(waveform, samplerate=SAMPLE_RATE)
    sd.wait()  # Wait until playback finishes
    print("Done.")
except KeyboardInterrupt:
    sd.stop()
    print("\nPlayback interrupted.")

