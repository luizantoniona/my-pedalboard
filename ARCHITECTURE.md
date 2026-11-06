# Architecture

My Pedalboard is a real-time guitar effects processor: a serial chain of pedals,
editable while audio runs, with a path towards amp and cabinet simulation and
presets on disk. This document describes the target architecture and the decisions
that fix it.

## 1. Overview

```
┌─────────────────────────────────────────────────────────────┐
│                        UI THREAD                            │
│  ┌──────────┐    ┌──────────┐    ┌──────────────────────┐   │
│  │   QML    │───▶│ Control  │───▶│       Models         │   │
│  │ (views)  │◀───│  (VMs)   │◀───│ (QObject/QAbstract)  │   │
│  └──────────┘    └──────────┘    └──────────┬───────────┘   │
└──────────────────────────────────────────────┼──────────────┘
                                               │ queued calls
┌──────────────────────────────────────────────▼──────────────┐
│                  ENGINE THREAD (Qt, no GUI)                 │
│            AudioEngine → AudioWorker → AudioGraph           │
└──────────────────────────────────────────────┬──────────────┘
                                               │ atomic pointer swap
┌──────────────────────────────────────────────▼──────────────┐
│                     AUDIO THREAD (RT)                       │
│          backend callback → AudioGraph::process()           │
└─────────────────────────────────────────────────────────────┘
```

There are three targets, and they drive every decision below:

1. Windows / Linux desktop: Qt6 + QML + RtAudio.
2. Embedded Linux with Qt on screen (eglfs), audio over USB or a codec.
3. Microcontroller (Cortex-M4F / M7 / H7) with its own P10 input board.

## 2. Folder structure

This is the **target** layout. Differences between it and the disk are migration
debt, not a free choice.

The main cut is by **portability**, not by feature. What runs on the microcontroller
lives in `Core/` and `Platform/`, and nothing else.

```
my-pedalboard/
│
├── Core/                         # Portable C++20. No Qt, no RtAudio, no OS.
│   ├── Analysis/
│   │   ├── LevelMeter            # peak follower written on the audio thread
│   │   ├── NoteHelper            # frequency -> MIDI note + cents
│   │   └── PitchDetector         # YIN, runs off the audio thread
│   ├── DSP/
│   │   ├── AudioBuffer.h         # pointers + frames + mono/stereo flag
│   │   ├── AudioNode.h
│   │   ├── DSPModule.h           # effect base class, with enabled (bypass)
│   │   ├── EffectNode            # graph node that wraps a DSPModule
│   │   ├── ParameterInfo.h       # static description of one parameter
│   │   └── Effect/
│   │       ├── Utility/          # Gain
│   │       ├── Dynamics/         # NoiseGate
│   │       ├── Distortion/       # Overdrive, HighGain
│   │       ├── Filter/           # Biquad, Equalizer
│   │       ├── Modulation/       # Chorus
│   │       ├── Delay/            # Delay
│   │       ├── Reverb/           # Reverb
│   │       └── Amp/              # AmpSim, CabinetIR (future)
│   ├── Graph/
│   │   └── AudioGraph            # execution order published as a raw pointer
│   ├── Project/
│   │   ├── NodeRegistry          # algorithm name -> DSPModule instance
│   │   ├── PedalRegistry         # pedal id -> algorithm + starting values
│   │   ├── PedalDefinition.h
│   │   ├── ParameterOverride.h
│   │   └── PresetStore           # (future)
│   └── Utils/
│       ├── AtomicValue.h
│       ├── AudioMath.h
│       ├── DenormalGuard.h       # FTZ/DAZ for the scope of the callback
│       ├── RingBuffer.h          # lock-free single-producer single-consumer
│       └── SmoothedValue.h       # one-pole ramp against clicks
│
├── Platform/                     # I/O backends, one per target
│   ├── AudioBackend.h            # interface; Core and STL types only
│   ├── AudioDeviceInfo.h
│   ├── Null/                     # tests: drives the graph without hardware
│   ├── RtAudio/                  # Windows / Linux desktop
│   ├── Alsa/                     # embedded Linux (future)
│   └── I2S/                      # microcontroller (future)
│
├── Engine/                       # Qt orchestration layer
│   └── Audio/
│       ├── AudioEngine           # facade on the UI thread; owns the QThread
│       └── AudioWorker           # lives on the engine thread; injectable backend
│
├── Models/                       # QObject / QAbstractListModel
│   ├── EffectChainModel
│   ├── LevelModel
│   └── TunerModel
│
├── App/                          # The executable
│   ├── main.cpp
│   ├── AppContext                # SINGLE OWNER of engine, models and controls
│   ├── RegisterAppTypes
│   ├── Main.qml
│   └── Application/
│       ├── ApplicationWindow.qml
│       ├── About/
│       ├── Home/                 # HomePage.qml + HomePageControl
│       └── Settings/             # SettingsPage.qml + SettingsPageControl
│
├── GUI/                          # QML module MyPedalboardGUIComponents
│   ├── Assets/icons/
│   ├── Component/                # one folder per family
│   │   ├── Bar/                  # LevelBar
│   │   ├── Button/               # ButtonIcon, ButtonText
│   │   ├── Fader/                # Fader
│   │   ├── Input/                # ComboBoxCustom
│   │   ├── Knob/                 # Knob
│   │   ├── Panel/                # PanelTop, PanelBottom
│   │   ├── Pedal/                # PedalCard
│   │   └── Tuner/                # TunerDial
│   └── Theme/                    # Borders, Colors, Fonts, Icons, Radiuses, Spaces
│
└── Tests/
    ├── Core/                     # no Qt, no sound card
    ├── Engine/                   # uses the null backend
    └── Support/                  # SignalGenerator, AudioHarness
```

### 2.1 Decisions that fix this structure

**Dependencies point one way.**

```
Core/      ->  nothing
Platform/  ->  Core
Engine/    ->  Core + Platform + QtCore
Models/    ->  Engine
Control    ->  Models + Engine   (lives next to its view)
QML        ->  Control
```

If a feature seems to need the arrow reversed, the answer is a Qt signal going up
(the engine emits, the model updates, QML is notified), never an include.

**`Core/` is the portability boundary.** No file in `Core/` may include a Qt, RtAudio
or operating-system header. This is the strictest rule in the project: breaking it
costs the whole microcontroller target. A `Core` test must compile and pass without
Qt installed.

**Samples are `float32`.** The microcontroller targets all have a single-precision
FPU, so the same DSP code runs on all three targets without fixed point. If a target
without an FPU ever appears, every effect has to be rewritten; that premise is what
supports a single `Core`. No `double` on the audio path.

**Heap only at setup.** `prepare()` may allocate; `process()` never allocates or
frees. Outside the audio path, `std::vector` and `std::unordered_map` are fine in
`Core`. The graph is fully built before the stream starts.

**`Platform/` isolates the hardware.** `AudioBackend` is a pure interface that speaks
only Core and STL types (no `QStringList`, no RtAudio). Device enumeration returns
`std::vector<std::string>`; conversion to Qt types happens in `Engine`. Changing
target means swapping the backend implementation, without touching `Core/` or
`Engine/`.

**`Engine/` is the Qt layer, and the only one that orchestrates.** `AudioEngine` owns
the `QThread`, the `AudioWorker` and the backend. It contains no DSP.

**`AppContext` is the single owner, and `AudioEngine` is not a singleton.** There is
one `AudioEngine`, created and owned by `AppContext`, which is created in `main.cpp`
right after `QGuiApplication` and destroyed before it. Page controls receive
references from it; no control owns the engine.

Why not a singleton: (a) a global access point lets any layer call the engine, which
defeats the dependency rule above; (b) tests cannot inject a fake engine or isolate
scenarios; (c) a static `unique_ptr` is destroyed after `main` returns, that is,
after `QGuiApplication`, and `AudioEngine` owns a `QThread` and an audio stream that
must be stopped before that.

**Controls live with their view.** There is no `Control/` module at the root. The C++
controller of a page lives in the page folder and compiles with the `App` target;
the controller of a reusable component lives next to its `.qml` in `GUI/Component/`.
The reason is proximity: whoever edits the screen edits both files together.

**Effects are grouped by category.** `Effect/` has one subfolder level per family.
The category is file organisation only: it does not appear in `typeName()`, in the
preset JSON or in `NodeRegistry`, which stay flat.

**An algorithm and a pedal are different things.** `NodeRegistry` catalogues DSP
algorithms (`Reverb`, `Delay`, `HighGain`): code, one per topology. `PedalRegistry`
catalogues pedals: an id, a display name, the algorithm that implements it and a list
of starting parameter values (`PedalDefinition`). Two reverbs that differ a little
are two data entries over the same algorithm, with no new class. A pedal only gets a
DSP class when the circuit really changes, as with `HighGain`, whose structure (two
clipping stages and an EQ after the distortion) cannot be obtained by reparametrising
`Overdrive`.

**Clones and custom pedals take the same path.** A clone of a real pedal is a
`PedalDefinition` that points at the closest algorithm and fixes the values that
characterise it; if the algorithm cannot reach the tone, the work is a new
`DSPModule`, and the definition is still one line of data. A user's custom pedal is
the same `PedalDefinition`, read from disk by `Engine` and registered at runtime with
`registerPedal()`. `Core` knows no JSON, so the store that reads those files belongs
in `Engine`. Current limit: an override only sets the starting value, and the user
can still change every parameter of the algorithm. Hiding or locking parameters per
pedal does not exist yet.

**No atomic design in the GUI.** `GUI/Component/` is organised by family, not by
abstraction level. The only other distinction is components (visual elements) versus
`Theme` (design tokens, as QML singletons).

**No source globbing.** Every new `.cpp`/`.h` is listed in its module's
`PROJECT_SOURCES`; every new `.qml` in the module's `QML_FILES`. Only the SVGs in
`GUI/Assets/` are globbed.

**Desktop comes first.** Windows/Linux has to work first; embedded Linux and the
microcontroller come later. What is not negotiable is the direction: nothing new
enters `Core/` carrying Qt with it.

## 3. Engine responsibilities

| Class | Responsibility |
|---|---|
| `AudioEngine` | Facade on the UI thread. Owns the engine `QThread` and the `AudioWorker`; every call is forwarded as a queued invocation. Does not process audio. |
| `AudioWorker` | Lives on the engine thread. Owns the backend and the `AudioGraph`, and is the only object the audio callback touches. |
| `AudioGraph` | Graph of `AudioNode`s. Resolves the execution order (topological sort) and runs it. |
| `AudioBackend` | One implementation per target. Opens and closes streams and invokes the callback. Knows nothing about the graph. |

Planned, not yet present: MIDI input, a plugin host, project and preset storage, a
transport.

## 4. AudioGraph

A pedalboard is `input → effects in sequence → output`. The graph is generic (a DAG)
so that parallel paths, mixers and sends can be added later without rewriting it.

```
 ┌────────┐     ┌────────────────┐     ┌────────┐
 │ input  │────▶│ effects in a   │────▶│ output │
 │        │     │ serial chain   │     │        │
 └────────┘     └────────────────┘     └────────┘
```

The classes below are design sketches, not transcriptions of the code (see the
accuracy note at the end).

```cpp
class AudioNode {
public:
    virtual ~AudioNode() = default;
    virtual void process( AudioBuffer& buffer ) = 0;
};

class EffectNode : public AudioNode {
public:
    explicit EffectNode( std::unique_ptr<DSPModule> effect );
    void process( AudioBuffer& buffer ) override;
    DSPModule* effect();
};

class AudioGraph {
public:
    void addNode( std::string id, std::shared_ptr<AudioNode> node );
    void removeNode( const std::string& id );
    void connect( const std::string& from, const std::string& to );
    void setSerialChain( const std::vector<std::string>& orderedIds );

    void process( AudioBuffer& buffer );   // audio thread only
};
```

### Thread-safe rebuilding

```
UI / engine thread              Audio thread
       │                             │
       │ addNode()                   │
       │ rebuildExecutionOrder()     │
       │  -> new list allocated      │
       │  -> pointer published ──────▶ process() walks the new list
       │                             │   without a lock
```

The rebuild always happens off the audio thread. Publishing is an atomic pointer
store, with no mutex on the critical path. The audio thread does one acquire load and
walks raw pointers; it never copies a `shared_ptr`, because
`std::atomic<std::shared_ptr>` is not lock-free on the common standard libraries. A
retired list is freed on the engine thread only after the audio thread has certainly
left it: one completed `process()` after the swap is not enough, since the thread may
have loaded the old pointer just before the swap, so two are required.

### 4.1 The serial chain

The graph is generic, but a pedalboard is a degenerate case of it: one line. Wiring
that line edge by edge is what broke in practice, so there is one primitive just for
it:

```cpp
// Replaces ALL edges with a single serial chain
void setSerialChain( const std::vector<std::string>& orderedIds );
```

Why not pairwise `connect()`: `connect()` is additive. Calling it for each
consecutive pair after removing or reordering a pedal leaves the previous
arrangement's edges in the graph. The topological sort then orders a graph that no
longer matches the chain, so the order comes out wrong and, with enough reordering, a
cycle appears. `setSerialChain` clears the edge list before rebuilding it and runs a
single `rebuildExecutionOrder()` instead of N.

Ids with no matching node are skipped. This matters because the sort orders roots
alphabetically: if the chain split into two disconnected components, the order
between them would be `node-10` before `node-2`, not the pedalboard order.

The caller is `EffectChainModel`, on every insertion, removal and reorder. The model
is the source of truth for the order; the graph only mirrors it.

```
EffectChainModel::publishChain()
    │ std::vector<std::string> in list order
    ▼
AudioEngine::setChain()          [UI thread → queued]
    ▼
AudioGraph::setSerialChain()     [engine thread]
    │ clears the edges, relinks in series, rebuilds the execution order
    ▼
execution order published ─────▶ process() [audio thread] walks the new list
```

## 5. DSPModule

```cpp
class DSPModule {
public:
    virtual ~DSPModule() = default;

    virtual void process( float* left, float* right, uint32_t frames ) = 0;

    virtual void prepare( uint32_t sampleRate, uint32_t maxFrames ) {}
    virtual void reset() {}

    virtual void setParameter( std::string_view name, float value ) {}
    virtual float parameter( std::string_view name ) const { return 0.0f; }

    virtual std::span<const ParameterInfo> parameters() const { return {}; }

    virtual std::string_view typeName() const = 0;

    bool isEnabled() const;
    void setEnabled( bool enabled );

private:
    AtomicValue<bool> _enabled{ true };
};
```

`process()` runs on the audio thread (and, on the microcontroller, straight in the
DMA interrupt). `prepare()` and `reset()` run outside it. `prepare()` is where every
allocation happens, sized for the worst case; `reset()` zeroes state without
reallocating.

### Parameter descriptors

```cpp
class ParameterInfo {
public:
    std::string_view name;    // key used by setParameter and by presets
    std::string_view label;   // text under the knob
    std::string_view unit;    // "dB", "ms", "x", ""
    float minimum;
    float maximum;
    float defaultValue;
};
```

Each effect returns a `std::span` over a `static constexpr` array in its `.cpp`: no
allocation and no RAM per instance, so the mechanism stays valid on the microcontroller,
where the UI does not even exist.

This is what lets the UI build its controls from the descriptor and write back
through `setParameter( name, value )`. A new effect shows up in the interface with the
right controls without any QML or UI C++ knowing its concrete type. Nothing outside
the effect casts to the derived class. Parameters are always reached by name, so
presets and automation stay agnostic to the effect type.

### Bypass

`isEnabled()` lives in the base class, not in each effect: `EffectNode` checks the flag
and simply does not call `process()` when it is false. A bypassed pedal costs one
`bool` test per block, and the effect does not need to know it was switched off. That
also means it keeps its internal state (delay line, envelope, reverb tail) for when
it comes back.

### Effects

The levels of the tree are file folders, not classes: every effect derives directly
from `DSPModule`.

```
DSPModule
├── Gain                       Effect/Utility/
├── NoiseGate                  Effect/Dynamics/
├── Overdrive                  Effect/Distortion/   voiced after the Boss SD-1
├── HighGain                   Effect/Distortion/   metal-style, two clipping stages
├── Equalizer                  Effect/Filter/       seven bands, GE-7 style
├── Chorus                     Effect/Modulation/
├── Delay                      Effect/Delay/
├── Reverb                     Effect/Reverb/       Freeverb-style
└── (future) Compressor, Fuzz, Phaser, Flanger, ConvolutionReverb, AmpSim, CabinetIR
```

Effects that multiply the signal by a user-controlled value (gain, mix, level) smooth
it with `SmoothedValue`, so turning a knob does not click. Effects with feedback
(delay, reverb, filters) flush denormals, which cost cycles the microcontroller does
not have.

## 6. Threading

```
┌──────────────────────────────────────────────────────────────┐
│  UI thread (Qt main thread)                                  │
│  • QML rendering                                             │
│  • user events                                               │
│  • Model updates (Q_PROPERTY, signals/slots)                 │
│  • NEVER blocks for more than ~16 ms                         │
└───────────────────────┬──────────────────────────────────────┘
                        │ Qt signals (queued connection)
┌───────────────────────▼──────────────────────────────────────┐
│  Engine thread (dedicated QThread)                           │
│  • AudioWorker lives here                                    │
│  • receives commands from the UI                             │
│  • mutates the AudioGraph off the audio thread               │
│  • preset I/O                                                │
└───────────────────────┬──────────────────────────────────────┘
                        │ execution order published atomically
┌───────────────────────▼──────────────────────────────────────┐
│  Audio thread (RT priority, managed by the OS)               │
│  • backend callback                                          │
│  • ONLY AudioGraph::process()                                │
│  • must finish within buffer_size / sample_rate              │
│  ✗ NEVER: allocate/free, lock, I/O, Qt signals, exceptions,  │
│    copy a shared_ptr                                         │
└──────────────────────────────────────────────────────────────┘
```

On the microcontroller there is no engine thread and no UI thread: `Core` runs
directly in the I2S DMA interrupt. That is why the audio-thread rules apply to all of
`Core`, without exception.

**Golden rule.** Nothing that can block indefinitely may be in the callback. If it
needs to communicate, it uses a lock-free `RingBuffer` or `std::atomic`.

UI to audio thread:

```
Control (UI thread)
    │
    ▼
AtomicValue<float> (effect parameter)   ← written on the UI thread
    │
    ▼
DSPModule::process()                    ← read on the audio thread
```

Audio thread to UI (for example a level meter):

```
AudioWorker::process() → RingBuffer<float>::push()
                                │
              QTimer (UI thread) → RingBuffer::pop() → Model::update()
```

### Thread affinity of the backend

`AudioWorker` is created on the UI thread and moved with `moveToThread()`. The audio
backend does **not** follow that creation: it is built in `AudioWorker::initialize()`
(connected to `QThread::started`, so already on the engine thread) and destroyed in
`shutdown()`, on the same thread.

This is not style; it is mandatory on Windows. `RtApiAsio` calls `CoInitialize` in its
constructor, which enters a COM single-threaded apartment, and a COM apartment is per
thread. `openStream()` later calls `CoCreateInstance` on the thread that opens the
stream. Building on one thread and opening on another makes every ASIO open fail with
"unable to load driver", while device enumeration keeps working (it reads names from
the registry and reuses the cache built at construction), which hides the cause. The
same applies to any backend object with thread affinity.

```
UI thread        new AudioWorker()        ← no backend here
                 moveToThread(engine)
Engine thread    initialize()  → new RtAudioBackend()   [CoInitialize]
                 start()       → openStream()           [CoCreateInstance]
                 shutdown()    → backend.reset()        [CoUninitialize]
```

The worker's constructor still accepts an injected backend; that is how tests bring in
the null backend, which has no affinity at all.

## 7. View layers: QML → Control → Models → Engine

```
┌─────────────────────────────────────────────────────────────┐
│  QML (view)                                                 │
│  • only reads properties and calls Control methods          │
│  • no business logic                                        │
└──────────────────────────┬──────────────────────────────────┘
                           │ Q_PROPERTY / invokable
┌──────────────────────────▼──────────────────────────────────┐
│  Control (view model)                                       │
│  • exposes display-ready data                               │
│  • turns UI intent into engine operations                   │
│  • holds UI state (selection, edit mode)                    │
│  • does NOT know QML                                        │
└──────────────────────────┬──────────────────────────────────┘
                           │ synchronous calls
┌──────────────────────────▼──────────────────────────────────┐
│  Models                                                     │
│  • QAbstractListModel, QObject with Q_PROPERTY              │
│  • mirror the engine state in a form QML can observe        │
│  • updated through signals from the engine                  │
└──────────────────────────┬──────────────────────────────────┘
                           │ queued Qt signals / direct calls
┌──────────────────────────▼──────────────────────────────────┐
│  Engine                                                     │
│  • emits Qt signals to notify the models                    │
└─────────────────────────────────────────────────────────────┘
```

Example, changing the output device: the combo box on the settings page calls its
control, the control asks `AudioEngine` (a queued call), `AudioWorker` reopens the
stream on the engine thread, and the signal that comes back updates a property on the
UI thread, which notifies QML.

### 7.1 The effect chain in the UI

`EffectChainModel` is a `QAbstractListModel` with six roles:

| Role | Meaning |
|---|---|
| `effectId` | unique id of this instance in the chain |
| `pedalId` | id of the `PedalDefinition` it was created from |
| `displayName` | the name shown on the pedal |
| `typeName` | the algorithm; the UI uses it to pick the pedal colour |
| `effectEnabled` | bypass state |
| `parameters` | the effect's `ParameterInfo` list, built once |

The bypass role is called `effectEnabled`, not `enabled`, on purpose: the `ListView`
delegate is a `PedalCard`, which is an `Item`, and a required property named
`enabled` shadows `Item.enabled`. Qt warns "overrides a member of the base object" and
the item's native property starts following the pedal's state.

Adding a pedal asks `PedalRegistry` to build it: the registry creates the algorithm
through `NodeRegistry`, applies every parameter's default, then applies the pedal's
own starting values.

**Two routes, and the difference matters.**

A structural change (insert, remove, reorder) goes through the engine thread, because
it touches the node map and the edge list:

```
QML → EffectChainModel → AudioEngine::addNode / setChain   [queued]
                               → AudioGraph                [engine thread]
```

A value change (knob, bypass) does **not**. The model keeps a `shared_ptr` to the
`EffectNode` it created and writes straight to the module:

```
QML → EffectChainModel::setParameter → DSPModule::setParameter   [UI thread]
```

This is safe because the target is an `AtomicValue`: the audio thread only reads. And
it is necessary because a knob produces dozens of events per second; queueing each
one to the engine thread would add visible latency to the control for no gain.

The model's `shared_ptr` is what keeps the node alive on the UI side; the graph holds
its own. The audio thread still walks only raw pointers in the execution list, with no
per-block reference counting.

An effect whose setter converts units (the noise gate recomputes coefficients from the
sample rate) performs several atomic stores that are not atomic together. The audio
thread may see a new attack with an old release for one block. That is acceptable on
purpose: both are valid coefficients, and the alternative would be a lock on the audio
path.

## 8. Project and preset serialisation

Not implemented yet. The target format is JSON, written by `Serializer`:

```json
{
  "name": "Blues Crunch",
  "chain": [
    { "id": "node-1", "type": "Gain",      "params": { "gainDb": -3.0 } },
    { "id": "node-2", "type": "Overdrive", "params": { "drive": 4.0, "tone": 0.5, "level": -6.0 } },
    { "id": "node-3", "type": "Reverb",    "params": { "size": 0.4, "mix": 0.2 } }
  ]
}
```

The chain order is the pedalboard order, and the edges are derived from it with
`setSerialChain`. The serializer rebuilds the chain by asking `NodeRegistry` to map
`"Overdrive"` to `new Overdrive()`, so adding a new effect means registering it, with
no change to the serializer.

Once a preset records the pedal and not only the algorithm, the `"type"` field above
becomes the id of a `PedalDefinition` (`"RoomReverb"`), which `PedalRegistry` resolves
to the algorithm, applying its starting values before the preset's `"params"`. Old
presets stay valid as long as every algorithm has a pedal with the same id.

JSON in `Core` cannot use `QJsonDocument`. The portable design is that `Core` only
knows a plain `PresetData` structure; whoever parses JSON is `Engine` (Qt, desktop),
and the microcontroller writes the same structure in binary to flash.

## 9. Growing towards a DAW

The architecture above is already prepared for it. The extension points are:

| Future feature | Extension needed |
|---|---|
| Multiple tracks | `TrackModel` plus several `AudioGraph`s managed by `AudioEngine` |
| Mixer | `MixerNode : AudioNode` with N inputs and gain/pan per channel |
| Buses / sends | `BusNode`, `SendNode`, `ReturnNode`, all `AudioNode`s |
| MIDI | a MIDI engine; `MidiNode : AudioNode` for routing |
| Piano roll | transport, `MidiTrackModel` and a new GUI module |
| Timeline | `TimelineModel`, `Clip` and an `AudioFileNode` for file playback |
| Recording | `RecordNode` writes to a `RingBuffer`; a worker thread drains it to disk |
| Plugin host | `PluginNode : AudioNode` wrapping a plugin interface |
| Automation | `AutomationLane` records and replays values through `setParameter()` |
| Export | a worker thread walks the graph offline (no backend) and writes a file |

The key is that all of these are new nodes in the graph or new models, with no need to
rewrite the existing core.

## 10. Accuracy note

The class listings in section 4 are design sketches, not transcriptions of the code.
Sections 4.1, 5 (`DSPModule` and `ParameterInfo`) and 7.1 were written against the real
code and are correct. The rest may differ in signature details; in particular,
`AudioNode` no longer keeps a list of output `weak_ptr`s, and there are no `InputNode`
or `OutputNode` classes: input and output are buffers that `AudioWorker` hands to
`AudioGraph::process()`.
