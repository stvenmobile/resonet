# ResoNet

**ResoNet** is an experimental platform for exploring how resonance, harmonic interaction, and physical feedback can give rise to emergent structure — without explicit instruction, training, or AI constructs.

## 🔍 Project Overview

In a universe that began as chaos and has steadily refined itself into stars, molecules, and minds, **ResoNet** is a minimalist attempt to align with that same creative force: the natural evolution of persistent patterns from vibration and feedback.

This system is designed to:

1. **Observe** a sea of external vibrations using an array of piezoelectric sensors.
2. **Intervene** in that field through controllable actuators that apply vibrational energy.
3. **Analyze** the resulting vibrational landscape for symmetry, structure, and resonance.
4. **Prefer** (but not mandate) modifications that increase order or coherence.
5. **Allow** the harmonic field to evolve — not by human design, but through emergent interaction.

> This is not an AI project (yet).  
> It is a study of whether physical systems — given the ability to sense and act — can begin to shape themselves toward *clarity*, *symmetry*, and *persistent resonance*, without needing to be told what those things mean.

## 🧱 Technical Foundations

- **Hardware**: Raspberry Pi, multiple piezo sensors, vibration actuators, ADS1115 ADCs
- **Signal Processing**: Real-time FFT, energy tracking, harmonic detection
- **Visualization**: 3D plots and spectrograms to represent vibrational structure
- **Control Loop**: Feedback cycle where the system gently modifies its own input signals and observes the result

## 🧬 Guiding Questions

- Can harmonic structures emerge in a chaotic field with minimal intervention?
- Do certain resonant patterns naturally persist over time?
- What does “preference” look like in a system without goals?
- Could this substrate eventually serve as a tool — or even a metaphor — for AI?

## 🚧 Current Status

This is the first exploratory phase:
- [x] Project name and concept defined
- [ ] Simulated sensor input and FFT visualization
- [ ] Real-time plotting of harmonic surfaces
- [ ] Hardware integration with Raspberry Pi and ADS1115
- [ ] Closed-loop resonance modification system

## 🧭 Architecture Update: Multi-Layer Physical Neural System

ResoNet now includes an experimental **two-layer resonant substrate**, where piezo disks serve as **resonators (outputs)** and **receivers (inputs)** in a physical neural network.

### Layer Design
- **Layer 1**: 4 piezo sensors mounted symmetrically on the top of the aluminum substrate
- **Layer 2**: 4 piezo sensors mounted symmetrically on the top of the aluminum substrate
- Each layer has a single 4ohm, 25w piezo resonator on the bottom side of the aluminum substrate

### Signal Generation
- A dedicated **ESP32** generates 2 independent PWM signals for the resonators
- PWM signals pass through RC filters into the LOSC Audio Amplifier
- Output from the amplifier drives the piezo resonators


### ⚡ ESP32 Signal Generator (signal_gen.ino)

This firmware runs on the ESP32 to generate two independent PWM signals for driving the left and right resonators in the ResoNet substrate. It provides both a web interface and a simple HTTP API for remote control.
Features

    Independent control of Left and Right channels

    Adjustable frequency and duty cycle per channel

    Start/Stop control per channel without losing settings

    12-bit PWM resolution (0–4095 duty internally) for extended low-frequency range

    HTTP API for scripting and automation

#### Parameters

| Parameter        | Range (UI)    | Range (internal) | Notes |
|------------------|---------------|------------------|-------|
| `freqL`, `freqR` | 1 – 40000 Hz  | 1 – 40000 Hz     | Practical usable range depends on resolution, amplifier, and load. At 12-bit, can drive well below 300 Hz; efficiency may drop for small exciters below ~200 Hz. |
| `dutyL`, `dutyR` | 0 – 255       | 0 – 4095         | UI values are scaled internally to 12-bit. A value of `0` stops output; near-max values can cause distortion or clipping depending on amplifier/load. |
| `leftOn`, `rightOn` | 0 or 1     | —                | Enable/disable channel output without losing frequency/duty settings. |


HTTP API

    GET /status → JSON snapshot of current settings

    GET /startL, GET /stopL → Start/Stop left channel

    GET /startR, GET /stopR → Start/Stop right channel

    GET /set?... → Set any combination of freqL, freqR, dutyL, dutyR, leftOn, rightOn (e.g. /set?freqL=440&dutyL=128&leftOn=1)

Practical Notes

    Lower frequency limit: Software supports 1 Hz, but most small resonators/amplifiers have poor response below ~100–200 Hz.

    Upper frequency limit: PWM supports up to ~40 kHz in this config, though acoustic output >20 kHz is inaudible.

    Duty cycle: For testing, start with 100–150 (≈ 40–60% duty) in UI. Extreme values can stress hardware.

    Resolution trade-off: 12-bit resolution allows low-frequency operation but slightly reduces max achievable frequency compared to 8-bit.

### Feedback Loop
- The **Raspberry Pi** reads vibration signals from the ADS1115
- Based on the output and a training target, the Pi computes error and weight adjustments using a form of **gradient descent**
- The Pi sends new drive amplitudes to the ESP32 over UART, updating PWM duty cycles

Schematic and PCB files for the sensor PCB are in the `pcb/` directory.





### 🔬 Phase 1 Experimental Questions

- How do pure tones manifest across the plate?
- Does cross-talk occur between layers?
- How does interference affect resonance quality?
- Can spectrograms detect persistent or self-reinforcing tones?

The first phase of experimentation centers around observing and visualizing tonal patterns over time using live spectrograms.
🎯 Objective

To develop a deeper understanding of how vibrations propagate across the physical substrate, with a focus on:

    How individual tones appear in time-frequency space

    Whether resonance persists beyond a tone's duration

    How simultaneous or overlapping signals affect neighboring nodes

    Whether interference between layers leads to destructive noise or constructive resonance

This is not yet a training phase — it is a calibration and sensory tuning phase. The goal is to define and observe the characteristics of vibration, including:

    Amplitude: Intensity of the signal over time

    Frequency: Dominant and harmonic tones

    Envelope: The attack, sustain, and decay profile

    Resonance: Sustained feedback or self-reinforcement

👁️ Visualization Tool

A real-time spectrogram visualization system has been implemented. It captures piezo sensor data (via ADS1115) and displays a rolling time-frequency plot of vibration:

    Layer 1 is plotted in its own window

    Layer 2 (if active) will appear in a second subplot

    Tones played through connected actuators appear as stripes or blobs in the frequency domain

🧬 Experimental Design

Each test run spans 10 seconds, during which a predefined sequence of tones (e.g., A–B–C) is played through one layer. Variants of each experiment will modify tone timing or introduce interfering tones from the other layer:
Trial	Layer 1 Sequence	Layer 2 Tone
1	A – B – C	None
2	A – B – C	Tone during B
3	A – B – C	Tone during C
...	...	...

By comparing the spectrograms, we can analyze:

    How clearly each note registers

    Whether simultaneous tones blur or amplify

    Whether one layer becomes background noise or actively distorts the other

🔍 Broader Goal

These tests help identify useful signal characteristics that may later serve as inputs to a learning network or be used to define resonant attractor states. They also help define what "interference" and "clarity" mean in this physical system.

This process is intentionally open-ended — we are mapping the sonic and harmonic landscape to see what kinds of cognitive primitives may emerge.


 Optional Enhancement: Dual Mono Output Mode

For experiments where signal clarity, symmetry, and energy uniformity are critical, the system can be configured to operate in dual mono mode using the HiFiBerry DAC2 Pro.

In this mode, both the left and right audio channels output the same mono signal, effectively doubling the signal path and reducing distortion or channel imbalance.
✅ Benefits for Resonance Experiments

    Increased signal power: Both DAC channels reinforce the same waveform, providing greater amplitude for driving actuators.

    Improved symmetry: Eliminates phase drift or imbalance between stereo channels.

    Cleaner resonance analysis: Ideal when driving symmetrical actuator setups or when visualizing frequency harmonics in the spectrogram.

    Predictable input state: Simplifies A/B experiments by reducing the number of signal variables.

🛠️ How to Enable Dual Mono Mode

    Open the ALSA configuration:

sudo nano /etc/asound.conf

Add or replace with the following:

pcm.!default {
    type hw card 0
}
ctl.!default {
    type hw card 0
}

Enable dual mono mode via amixer:

amixer -c 0 sset 'Dual Mono' on

Test with a tone:

    speaker-test -c 2 -t sine -f 1000

You should now hear the same tone on both output channels simultaneously, confirming dual mono is active.
🧪 Experimental Use

You can now route each mono output from the DAC2 Pro or Amp2 to separate actuators (e.g., two on layer 1), confident that both channels are outputting identical and reinforced signals. This is especially useful for:

    Testing physical symmetry in the aluminum plate

    Determining resonance thresholds with maximum tone energy

    Creating baseline measurements before introducing interference from layer 2
	
	
	

## 🤝 Contribution

This is a solo prototype for now, but contributions, questions, and speculative discussions are welcome. Feel free to open issues or suggest papers, metaphors, or mechanisms.

## 🧠 Inspiration

> “The system clarifies the field.”

ResoNet is inspired by the idea that form and meaning can emerge — not from code or cognition — but from **structure**, **feedback**, and **resonance** itself.

---

## 🛠️ Setup Instructions

These instructions assume a Raspberry Pi 4 or 5 running Raspberry Pi OS (Bookworm) or Debian 12, with `git`, Python 3.11+, and `pip` installed.

### 1. Clone the repository

```bash
git clone git@github.com:FutureSteve/resonet.git
cd resonet
```

### 2. Create and activate a Python virtual environment

```bash
python3 -m venv venv
source venv/bin/activate
```

### 3. Install required Python packages

```bash
pip install -r requirements.txt
```

If you encounter issues with `sounddevice`, ensure `portaudio` is installed:

```bash
sudo apt install libportaudio2
```

### 4. Test tone playback

Connect your speaker or vibration transducer via amplifier, then run:

```bash
python test_tone.py
```

You should hear a brief tone. Adjust volume as needed on the amplifier.

### 5. Visualize waveform and frequency

To plot both the time-domain waveform and frequency spectrum:

```bash
python harmonic_plot.py
```

You can adjust the tone composition by modifying `harmonic_plot.py`.

---


