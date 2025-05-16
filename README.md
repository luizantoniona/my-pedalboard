# 🎸 my-pedalboard

**my-pedalboard** is a cross-platform Qt application designed to simulate a modular guitar pedalboard. It uses [PortAudio](http://www.portaudio.com/) for low-latency audio input/output and a QML-based UI for fast prototyping and visual control.

---

## 📦 Features

- 🎛 Modular GUI with QML controls
- 🎚 Real-time audio I/O with PortAudio
- 🖥 Supports **Windows**, **Linux**, and **macOS**
- 🧱 Qt5 and Qt6 compatible
- 🧩 Clean code structure (Controllers, QML Controls, etc.)
- 📡 Multilingual support with `.ts` translation files

---

## 🛠 Structure

my-pedalboard/
├── main.cpp # Entry point
├── qml.qrc # QML resource file
├── qml/ # QML UI files
├── Controllers/ # C++ backend logic (e.g. audio controllers)
├── Control/ # Reusable QML UI components
├── third_party/
│ └── portaudio/ # PortAudio as a submodule
├── CMakeLists.txt
└── README.md

---

## 🚀 Getting Started

### 🧰 Prerequisites

- **CMake ≥ 3.14**
- **Qt5 or Qt6** (`QtQuick`, `QtCore`, `LinguistTools`)
- **C++17 compiler**

---

## 📚 References

- [Qt Documentation](https://doc.qt.io/)
- [PortAudio](http://www.portaudio.com/)