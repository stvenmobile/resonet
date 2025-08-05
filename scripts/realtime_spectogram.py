import time
import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import spectrogram
from collections import deque
import board
import busio
import adafruit_ads1x15.ads1115 as ADS
from adafruit_ads1x15.analog_in import AnalogIn

# Constants
SAMPLE_RATE = 1000  # Hz
WINDOW_DURATION = 10  # seconds
BUFFER_SIZE = SAMPLE_RATE * WINDOW_DURATION

# Set up I2C and ADC (layer 1 = channel 0)
i2c = busio.I2C(board.SCL, board.SDA)
ads = ADS.ADS1115(i2c)
ads.gain = 16
chan1 = AnalogIn(ads, ADS.P0)  # layer 1 piezo

# Circular buffer
buffer = deque(maxlen=BUFFER_SIZE)

# Initialize with zeros
for _ in range(BUFFER_SIZE):
    buffer.append(0)

# Visualization setup
plt.ion()
fig, ax = plt.subplots()
img = None

print("Recording and visualizing... Press Ctrl+C to stop.")

try:
    while True:
        # Sample a single value
        value = chan1.voltage
        buffer.append(value)

        # Only update the plot every N samples (e.g., every 0.5 sec)
        if len(buffer) % (SAMPLE_RATE // 2) == 0:
            data = np.array(buffer)
            f, t, Sxx = spectrogram(
                data, fs=SAMPLE_RATE, nperseg=256, noverlap=128
            )

            ax.clear()
            img = ax.pcolormesh(t, f, 10 * np.log10(Sxx + 1e-12), shading='auto')
            ax.set_ylabel('Frequency [Hz]')
            ax.set_xlabel('Time [s]')
            ax.set_title('Layer 1 - Live Spectrogram')
            plt.pause(0.01)

except KeyboardInterrupt:
    print("\nStopped.")

plt.ioff()
plt.show()
