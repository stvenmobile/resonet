import numpy as np
import matplotlib.pyplot as plt
import sounddevice as sd

# Constants
SAMPLE_RATE = 44100  # Samples per second
DURATION = 2.0       # Duration in seconds
FREQUENCY = 440.0    # Frequency of sine wave (Hz)

# Generate time and waveform arrays
t = np.linspace(0, DURATION, int(SAMPLE_RATE * DURATION), endpoint=False)
waveform = 0.4 * np.sin(2 * np.pi * FREQUENCY * t)

# Play the tone
print(f"Playing {FREQUENCY} Hz sine wave...")
sd.play(waveform, samplerate=SAMPLE_RATE)
sd.wait()
print("Playback finished.")

# FFT computation
fft_result = np.fft.fft(waveform)
fft_freq = np.fft.fftfreq(len(fft_result), 1/SAMPLE_RATE)
magnitude = np.abs(fft_result) / len(fft_result)

# Limit to positive frequencies only
half_n = len(fft_freq) // 2
fft_freq = fft_freq[:half_n]
magnitude = magnitude[:half_n]

# Plotting both time-domain and frequency-domain
plt.figure(figsize=(12, 6))

# Time domain plot
plt.subplot(2, 1, 1)
plt.plot(t[:1000], waveform[:1000])
plt.title(f"{FREQUENCY} Hz Sine Wave - Time Domain (First 1000 Samples)")
plt.xlabel("Time (seconds)")
plt.ylabel("Amplitude")
plt.grid(True)

# Frequency domain plot
plt.subplot(2, 1, 2)
plt.plot(fft_freq, magnitude)
plt.title("Frequency Spectrum")
plt.xlabel("Frequency (Hz)")
plt.ylabel("Magnitude")
plt.grid(True)
plt.xlim(0, 2000)  # Zoom in around the frequency of interest

plt.tight_layout()
plt.show()
