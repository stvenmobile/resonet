import numpy as np
import matplotlib.pyplot as plt
import sounddevice as sd

# Constants
SAMPLE_RATE = 44100
DURATION = 2.0  # seconds
FREQUENCIES = [440.0, 880.0, 1320.0]  # Fundamental + 2 harmonics
AMPLITUDES = [0.4, 0.2, 0.1]  # Relative amplitudes

# Time array
t = np.linspace(0, DURATION, int(SAMPLE_RATE * DURATION), endpoint=False)

# Generate combined waveform
waveform = sum(a * np.sin(2 * np.pi * f * t) for f, a in zip(FREQUENCIES, AMPLITUDES))

# Normalize waveform
waveform = waveform / np.max(np.abs(waveform))

# Play the waveform
print(f"Playing combined tone: {FREQUENCIES} Hz")
sd.play(waveform, samplerate=SAMPLE_RATE)
sd.wait()
print("Playback finished.")

# Plot time-domain waveform
plt.figure(figsize=(10, 4))
plt.plot(t[:1000], waveform[:1000])
plt.title(f"Time-Domain Signal (First 1000 Samples)")
plt.xlabel("Time (seconds)")
plt.ylabel("Amplitude")
plt.tight_layout()
plt.savefig("time_domain_plot.png")

# Frequency-domain plot
fft = np.fft.fft(waveform)
frequencies = np.fft.fftfreq(len(fft), 1 / SAMPLE_RATE)
magnitude = np.abs(fft)[:len(fft)//2]
frequencies = frequencies[:len(fft)//2]

plt.figure(figsize=(10, 4))
plt.plot(frequencies, magnitude)
plt.title("Frequency Spectrum")
plt.xlabel("Frequency (Hz)")
plt.ylabel("Magnitude")
plt.xlim(0, 5000)  # Optional zoom
plt.tight_layout()
plt.savefig("frequency_spectrum_plot.png")

print("Plots saved. Exiting.")
