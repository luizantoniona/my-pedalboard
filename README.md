# 🎸 My Pedalboard

- **my-pedalboard** is a cross-platform Qt application designed to simulate a modular guitar pedalboard.
- It uses [PortAudio](http://www.portaudio.com/) for low-latency audio input/output and a QML-based UI for fast prototyping and visual control.

---

## 📦 Features

- 🎛 Modular GUI with QML controls
- 🎚 Real-time audio I/O with PortAudio
- 🖥 Supports **Windows**, **Linux**, and **macOS**
- 🧱 Qt5 and Qt6 compatible
- 🧩 Clean code structure (Controllers, QML Controls, etc.)
- 📡 Multilingual support with `.ts` translation files

---

## 🚀 Getting Started

### 🧰 Prerequisites

- **CMake ≥ 3.14**
- **Qt6** (`QtQuick`, `QtCore`, `LinguistTools`)
- **C++17 compiler**
- **PortAudio**
---

```bash
  qt6-base-dev
  qt6-base-runtime
  qt6-declarative-dev
  qt6-l10n-tools
  qt6-quickcontrols2-dev
  qt6-multimedia-dev
  libportaudio2
  portaudio19-dev
```

### 📥 Clone with Submodules

```bash
git clone --recurse-submodules https://github.com/your-username/my-pedalboard.git
cd my-pedalboard
git submodule update --init --recursive
```

### 🧱 Build Instructions

#### Linux/macOS

TODO

#### Windows

TODO

---



## 🧪 TODO

- Add basic audio passthrough
- Design QML-based pedal layout
- Support pedal chaining and presets
- Add VST or DSP module integration
- Improve UI and add parameter controls


## 📚 References

- [Qt Documentation](https://doc.qt.io/)
- [PortAudio](http://www.portaudio.com/)

---

Copyright © 2025 [Luiz Antonio Nicolau Anghinoni](https://github.com/luizantoniona)

---

## 📊 Status

<p align="center">
  <img alt="GitHub count language" src="https://img.shields.io/github/languages/count/luizantoniona/my-pedalboard" />
  <img alt="GitHub top language" src="https://img.shields.io/github/languages/top/luizantoniona/my-pedalboard" />
  <img alt="GitHub repo size" src="https://img.shields.io/github/repo-size/luizantoniona/my-pedalboard" />
  <img alt="GitHub last commit" src="https://img.shields.io/github/last-commit/luizantoniona/my-pedalboard" />
</p>