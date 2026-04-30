# 🎸 My Pedalboard

A cross-platform Qt6 application for real-time guitar effects processing.

---

## 📦 Current Features

| Feature | Status |
|---------|--------|
| Audio passthrough (input → output) | ✅ Working |
| Device selection (input/output) | ✅ Working |
| Threaded audio processing | ✅ Working |

---

## 🔧 Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                        QML Layer                            │
│  ApplicationWindow → HomePage → AudioControl               │
└─────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                     C++ Backend                             │
│                                                             │
│  AudioControl     (QML-exposed API)                        │
│        │                                                    │
│        ▼                                                    │
│  AudioEngine      (thread management, method routing)      │
│        │                                                    │
│        ▼                                                    │
│  AudioWorker     (RtAudio, buffer processing)              │
│        │                                                    │
│        ▼                                                    │
│  RtAudio         (native audio API)                        │
└─────────────────────────────────────────────────────────────┘
```

### Threading Model

- **QML Thread**: `AudioControl` — GUI-facing, exposed to QML
- **Worker Thread**: `AudioWorker` — dedicated thread for audio processing
- **Communication**: `Qt::QueuedConnection` via `QMetaObject::invokeMethod`

---

## 🚀 Getting Started

### 🧰 Prerequisites

- **CMake**
- **Qt6**
- **C++**
- **RtAudio** (included via vcpkg)
---

### 🧱 Build Instructions

```bash
# Clone the repository
git clone https://github.com/luizantoniona/my-pedalboard.git
cd my-pedalboard

# Configure with CMake
cmake -B build -G "Ninja"

# Build
cmake --build build

# Run
./build/debug/my-pedalboard.exe   # Windows
./build/my-pedalboard            # Linux/macOS
```

---

## 🎛 Usage

1. **Select Input Device** — Choose your audio input (microphone/line-in)
2. **Select Output Device** — Choose your audio output (speakers/headphones)
3. **Press PLAY** — Audio passthrough starts

---

## 📊 Status

### Phase 1: Core Audio (In Progress)
- [x] Audio passthrough
- [x] Device selection
- [ ] Volume/gain control
- [ ] Input/output level metering

### Phase 2: Effects Chain
- [ ] Gain/Overdrive pedal
- [ ] Delay pedal
- [ ] Reverb pedal
- [ ] Pedal chaining (multiple effects)

### Phase 3: Presets & Storage
- [ ] Save/load pedalboard presets
- [ ] Preset browser UI

### Phase 4: Advanced Features
- [ ] Latency compensation
- [ ] ASIO support (Windows)
- [ ] Audio recording

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
