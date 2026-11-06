# 🎸 My Pedalboard

A real-time guitar effects processor: a serial chain of pedals that you edit while
the audio is running. Written in C++20 with Qt6/QML for the interface and RtAudio
for the sound card.

The long-term goal is an AmpliTube-like rig (stompboxes → amp → cabinet IR) with
presets on disk. The current phase covers the stompboxes and the pedalboard UI.

---

## 📦 Current Features

| Feature | Status |
|---------|--------|
| Audio passthrough, device and driver selection (ASIO on Windows) | ✅ Working |
| Input/output level meters and round-trip latency readout | ✅ Working |
| Chromatic tuner (YIN) | ✅ Working |
| Editable pedal chain: add, remove, reorder, bypass, knobs and faders | ✅ Working |
| Pedal catalog with variants and clones | ✅ Working |
| Presets on disk | ⬜ Planned |
| Amp simulation and cabinet IR | ⬜ Planned |

### Pedals

| Pedal | Controls | Notes |
|-------|----------|-------|
| Gain | Gain | Smoothed, click-free |
| Noise Gate | Threshold, Attack, Release, Hold | |
| Overdrive | Drive, Tone, Level, Mix | Voiced after the Boss SD-1: tight bass, mid hump, asymmetric soft clipping |
| High Gain | Dist, Low, Mid, Mid Freq, High, Level | Metal-style, two clipping stages with a post-distortion EQ |
| Equalizer | 7 bands (100 Hz – 6.4 kHz) + Level | Seven faders, ±15 dB, like the Boss GE-7 |
| Chorus | Rate, Depth, Mix | Stereo, modulated short delay |
| Delay | Time, Feedback, Tone, Mix | Also available as **Slapback Delay** |
| Reverb | Size, Damping, Mix | Also available as **Room Reverb** and **Hall Reverb** |

The pedal list comes from a catalog, not from the UI code. A pedal is an algorithm
plus starting values, so two similar pedals are two catalog entries over one DSP
class. See [ARCHITECTURE.md](ARCHITECTURE.md) for the reasoning.

---

## 🔧 Architecture

The dependency arrow points one way only:

```
Core/      →  nothing. No Qt, no RtAudio, no OS headers.    [desktop + embedded + MCU]
Platform/  →  Core. One audio backend per target.           [one per target]
Engine/    →  Core + Platform + QtCore. Orchestration.      [desktop + embedded]
Models/    →  Engine. Observable state for QML.             [desktop + embedded]
App, GUI   →  Models + Engine. Pages, controls, components. [desktop + embedded]
```

`Core/` is the portability boundary. It holds the audio graph, every effect, the
meters and the tuner, and it must compile and pass its tests without Qt installed.
That is what keeps a Cortex-M4F/M7/H7 microcontroller target possible later. Audio
samples are `float` everywhere for the same reason.

```
┌──────────────────────────────────────────────────────────────┐
│  QML (App/, GUI/)        pages, Knob, Fader, PedalCard       │
├──────────────────────────────────────────────────────────────┤
│  Models/                 EffectChainModel, LevelModel, Tuner │
├──────────────────────────────────────────────────────────────┤
│  Engine/                 AudioEngine → AudioWorker           │
├──────────────────────────────────────────────────────────────┤
│  Platform/               RtAudio backend, Null backend       │
├──────────────────────────────────────────────────────────────┤
│  Core/                   AudioGraph, DSP effects, registries │
└──────────────────────────────────────────────────────────────┘
```

### Threading

| Thread | What lives there |
|--------|------------------|
| UI (Qt main) | QML, controls, models. Never blocks for long. |
| Engine (`QThread`) | `AudioWorker`, graph changes, the audio backend |
| Audio (driver callback) | Only `AudioGraph::process()`: no allocation, no locks, no I/O |

The UI talks to the engine through queued connections. Effect parameters are
atomics, so a knob reaches the audio thread without a lock. Level meters travel the
other way through a lock-free ring buffer drained by a timer.

---

## 🚀 Getting Started

### 🧰 Prerequisites

- **CMake** and **Ninja**
- **A C++20 compiler** (MSVC on Windows)
- **Qt 6.8 or newer**, installed locally
- **vcpkg**, with `VCPKG_ROOT` set. It provides RtAudio (with ASIO on Windows) and
  GoogleTest through `vcpkg.json`.

### 🧱 Build Instructions

```bash
# Clone the repository
git clone https://github.com/luizantoniona/my-pedalboard.git
cd my-pedalboard

# Configure (uses VCPKG_ROOT from the environment)
cmake -B build -G Ninja

# Build
cmake --build build

# Run (Windows)
./build/App/Debug/MyPedalboard.exe
```

On Windows, run the configure step from a shell that has the Visual Studio
developer environment loaded (`vcvars64.bat`), and point `CMAKE_PREFIX_PATH` at
your Qt install if CMake does not find it.

---

## 🎛 Usage

1. Open **Settings**, choose the audio driver, input and output devices.
2. Press **Play**. The signal passes straight through and the tuner follows the
   note you play.
3. On the home page, pick a pedal from the list and press **+** to add it.
4. Turn knobs and drag faders. Double-click a control to reset it, hold **Shift**
   on a knob for fine adjustment.
5. Use the arrows on a pedal to reorder it, the trash icon to remove it, and the
   footswitch to bypass it.

---

## 🧪 Testing

```bash
ctest --test-dir build -C Debug
```

No sound card is needed. `Tests/Support/SignalGenerator` produces sines, noise,
plucked strings with harmonics, ramps and DC, and `AudioHarness` drives a real
`AudioWorker` over a null backend. The `Core` tests link neither Qt nor RtAudio.

---

## 📊 Status

### Phase 1: Passthrough and tuner ✅
- [x] Audio passthrough and device selection
- [x] Level metering and latency readout
- [x] Chromatic tuner

### Phase 2: Pedal chain ✅
- [x] Editable chain with bypass and smooth parameters
- [x] Noise gate, overdrive, high gain, equalizer, chorus, delay, reverb
- [x] Pedal catalog with variants and clones
- [ ] Hide or lock parameters per pedal, user-defined pedals on disk

### Phase 3: Amp and cabinet
- [ ] Impulse response loading and partitioned convolution (cabinet)
- [ ] Amp simulation: preamp → tone stack → power amp

### Phase 4: Presets
- [ ] Save and load a full pedalboard
- [ ] Preset browser and A/B compare

### Phase 5: Embedded targets
- [ ] Embedded Linux backend (ALSA)
- [ ] Microcontroller backend (I2S codec + DMA)

---
<p align="center">
  <strong>Copyright © 2025</strong><br>
  <a href="https://github.com/luizantoniona">
    Luiz Antonio Nicolau Anghinoni
  </a>
</p>

<p align="center">
  <img alt="GitHub count language" src="https://img.shields.io/github/languages/count/luizantoniona/my-pedalboard" />
  <img alt="GitHub top language" src="https://img.shields.io/github/languages/top/luizantoniona/my-pedalboard" />
  <img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/luizantoniona/my-pedalboard" />
</p>
