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

