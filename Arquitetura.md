Agora tenho uma visão completa do projeto atual. Vou elaborar a arquitetura completa:

Arquitetura — My Pedalboard → DAW
1. Visão Geral

┌─────────────────────────────────────────────────────────────┐
│                        UI THREAD                            │
│  ┌──────────┐    ┌──────────┐    ┌──────────────────────┐  │
│  │   QML    │───▶│ Control  │───▶│      Models          │  │
│  │ (Views)  │◀───│ (VMs)    │◀───│ (QObject/QAbstract)  │  │
│  └──────────┘    └──────────┘    └──────────┬───────────┘  │
└──────────────────────────────────────────────┼──────────────┘
                                               │ thread-safe calls
┌──────────────────────────────────────────────▼──────────────┐
│                      ENGINE (sem Qt GUI)                     │
│  ┌────────────┐  ┌──────────┐  ┌───────────┐  ┌─────────┐  │
│  │   Audio    │  │   DSP    │  │  Project  │  │  MIDI   │  │
│  │  (Graph)   │  │(Modules) │  │(Serialize)│  │         │  │
│  └─────┬──────┘  └────┬─────┘  └───────────┘  └─────────┘  │
│        │              │                                      │
│  ┌─────▼──────────────▼──────────────────────────────────┐  │
│  │              AudioGraph (nós / edges)                  │  │
│  └────────────────────────────┬──────────────────────────┘  │
└───────────────────────────────┼─────────────────────────────┘
                                │
┌───────────────────────────────▼─────────────────────────────┐
│                     AUDIO THREAD (RT)                        │
│              RtAudio callback → AudioGraph::process()        │
└─────────────────────────────────────────────────────────────┘

2. Estrutura de Pastas

my-pedalboard/
├── App/                        # Executável Qt
│   ├── main.cpp
│   ├── Main.qml
│   ├── ApplicationWindow.qml
│   ├── Assets/
│   │   └── icons/
│   └── Pages/
│       ├── HomePage.qml
│       ├── SettingsPage.qml
│       └── AboutPage.qml
│
├── Engine/                     # Biblioteca estática, zero Qt GUI
│   ├── Audio/
│   │   ├── AudioEngine.h/cpp   # Orquestrador
│   │   ├── AudioDevice.h/cpp   # Abstração RtAudio
│   │   ├── AudioWorker.h/cpp   # Executa na audio thread
│   │   └── AudioGraph.h/cpp    # Grafo de nós
│   ├── DSP/
│   │   ├── AudioNode.h         # Classe base do grafo
│   │   ├── DSPModule.h         # Base dos efeitos
│   │   ├── Gain.h/cpp
│   │   ├── Compressor.h/cpp
│   │   ├── Delay.h/cpp
│   │   └── ...
│   ├── MIDI/
│   │   ├── MidiEngine.h/cpp
│   │   └── MidiMessage.h
│   ├── Plugins/
│   │   ├── PluginManager.h/cpp
│   │   ├── IPlugin.h           # Interface abstrata
│   │   └── VST3/CLAP/ (futuro)
│   ├── Project/
│   │   ├── ProjectManager.h/cpp
│   │   ├── PresetManager.h/cpp
│   │   └── Serializer.h/cpp    # JSON
│   ├── Transport/
│   │   ├── Transport.h/cpp     # BPM, play/stop/record
│   │   └── Clock.h/cpp
│   └── Utils/
│       ├── RingBuffer.h        # Lock-free para áudio
│       ├── AtomicValue.h
│       └── AudioMath.h
│
├── Models/                     # QObject/QAbstractItemModel
│   ├── AudioDeviceModel.h/cpp
│   ├── EffectChainModel.h/cpp
│   ├── PresetModel.h/cpp
│   └── TransportModel.h/cpp    # (futuro: TrackModel, MixerModel)
│
├── Control/                    # ViewModels / Controllers
│   ├── AudioControl.h/cpp
│   ├── EffectChainControl.h/cpp
│   ├── SettingsControl.h/cpp
│   └── RegisterControlTypes.h/cpp
│
├── GUI/                        # Componentes QML reutilizáveis
│   ├── Atoms/
│   ├── Molecules/
│   ├── Organisms/
│   ├── Templates/
│   └── Theme/
│
└── Tests/
    ├── Engine/
    └── Models/

3. Engine — Responsabilidades por Módulo
Audio/
Responsável pelo ciclo de vida do hardware de áudio e coordenação do processamento em tempo real.

Classe	Responsabilidade
AudioEngine	Orquestrador. Cria e conecta AudioDevice, AudioGraph, Transport. Não processa áudio diretamente.
AudioDevice	Abstração sobre RtAudio/PortAudio. Abre/fecha streams. Invoca o callback. Agnóstico ao grafo.
AudioWorker	Executa na audio thread. Recebe o callback do AudioDevice e delega ao AudioGraph::process().
AudioGraph	Grafo de AudioNodes. Resolve a ordem de processamento (topological sort).
DSP/
Implementação dos algoritmos de processamento de sinal. Nenhuma dependência de Qt ou de hardware.

AudioNode — nó do grafo (pode ser fonte, processador ou saída)
DSPModule — base dos efeitos (sem conexões, apenas process())
Cada efeito é uma subclasse de DSPModule
MIDI/
MidiEngine — gerencia dispositivos MIDI via RtMidi
MidiMessage — estrutura de dados de mensagem MIDI
No futuro: roteamento MIDI para plugins e automação
Plugins/
IPlugin — interface comum para VST3, CLAP, efeitos internos
PluginManager — escaneia, carrega e instancia plugins em background thread
No futuro: sandboxing por processo
Project/
ProjectManager — carrega/salva o projeto completo (JSON)
PresetManager — gerencia presets por efeito e por patch
Serializer — serialização/deserialização JSON (usa nlohmann/json ou QJsonDocument)
Transport/
Transport — estado de playback (play, stop, record, loop, BPM, posição)
Clock — gerador de pulso de tempo; base para automação e sincronismo MIDI
Utils/
RingBuffer<T> — fila lock-free para comunicação entre threads
AtomicValue<T> — wrapper de parâmetros que podem mudar sem lock no callback
AudioMath — utilitários DSP (dBFS ↔ linear, etc.)


4. AudioGraph — Arquitetura de Nós
Conceito base

 ┌───────────┐     ┌───────────────┐     ┌──────────────┐
 │  InputNode│────▶│  DSPNodeChain │────▶│  OutputNode  │
 │(mic/guitar│     │ (efeitos em   │     │(speakers/DAC)│
 │   source) │     │  sequência)   │     │              │
 └───────────┘     └───────────────┘     └──────────────┘

Futuro (paralelo):

 ┌──────────┐     ┌──────────┐     ┌──────────┐
 │  Input   │────▶│ Splitter │────▶│  DryPath │──┐
 └──────────┘     └────┬─────┘     └──────────┘  │
                       │                          ▼
                       │           ┌──────────┐ ┌────────┐
                       └──────────▶│  FxPath  │▶│ Mixer  │──▶ Output
                                   └──────────┘ └────────┘

Classes do grafo

// Porta de dados de áudio entre nós
struct AudioBuffer {
    float* left;
    float* right;
    uint32_t frames;
    uint32_t sampleRate;
};

// Nó base — pode ter N entradas e N saídas
class AudioNode {
public:
    virtual ~AudioNode() = default;
    virtual void process(AudioBuffer& buffer) = 0;

    void connect(std::shared_ptr<AudioNode> destination);
    void disconnect(std::shared_ptr<AudioNode> destination);

    std::string id() const;

protected:
    std::vector<std::weak_ptr<AudioNode>> _outputs;
};

// Nó que encapsula um DSPModule
class EffectNode : public AudioNode {
public:
    explicit EffectNode(std::unique_ptr<DSPModule> effect);
    void process(AudioBuffer& buffer) override;

    DSPModule& effect();

private:
    std::unique_ptr<DSPModule> _effect;
};

// Nó de entrada (fonte de áudio do hardware)
class InputNode : public AudioNode { ... };

// Nó de saída (escreve no buffer de saída do hardware)
class OutputNode : public AudioNode { ... };

// Grafo — resolve ordenação e executa
class AudioGraph {
public:
    void addNode(std::shared_ptr<AudioNode> node);
    void removeNode(const std::string& id);
    void connect(const std::string& fromId, const std::string& toId);
    void disconnect(const std::string& fromId, const std::string& toId);

    // Chamado exclusivamente na audio thread
    void process(AudioBuffer& buffer);

private:
    // Topological sort — reprocessado fora da audio thread
    // resultado armazenado em lista atômica
    void rebuildExecutionOrder();

    std::unordered_map<std::string, std::shared_ptr<AudioNode>> _nodes;
    std::atomic<std::vector<AudioNode*>*> _executionOrder; // swap atômico
};

Rebuilding thread-safe

UI/Control Thread          Audio Thread
       │                        │
       │ addNode()               │
       │ rebuildExecutionOrder() │
       │ → nova lista alocada    │
       │ → _executionOrder.store()──────────────▶ process() usa nova lista
       │                        │                 sem lock

O rebuild sempre ocorre fora da audio thread. A troca é um store atômico de ponteiro — sem mutex no caminho crítico.

5. DSPModule — Classe Base e Hierarquia

class DSPModule {
public:
    virtual ~DSPModule() = default;

    // Processamento em tempo real — NUNCA aloca memória aqui
    virtual void process(float* left, float* right, uint32_t frames) = 0;

    // Chamado fora da audio thread (setup, mudança de sampleRate)
    virtual void prepare(uint32_t sampleRate, uint32_t maxFrames) {}
    virtual void reset() {}

    // Parâmetros são acessados via AtomicValue<float>
    // para permitir automação thread-safe
    virtual void setParameter(std::string_view name, float value) = 0;
    virtual float getParameter(std::string_view name) const = 0;

    virtual std::string_view typeName() const = 0;
};

Hierarquia de efeitos:

DSPModule
├── Gain
├── NoiseGate
├── Compressor
│   └── FeedbackCompressor
├── EQ
│   ├── ParametricEQ
│   └── GraphicEQ
├── Distortion
│   ├── Overdrive
│   ├── Fuzz
│   └── HardClipper
├── Modulation
│   ├── Chorus
│   ├── Phaser
│   └── Flanger
├── TimeDelay
│   ├── Delay
│   └── Echo
├── Reverb
│   ├── ConvolutionReverb  ← usa CabinetIR também
│   └── AlgorithmicReverb
└── CabinetIR              ← convolução com IR de cabinet


Cada efeito expõe parâmetros nomeados via setParameter/getParameter, permitindo que automação e presets sejam completamente agnósticos ao tipo do efeito.

6. Threading

┌──────────────────────────────────────────────────────────────┐
│  UI Thread (Qt main thread)                                   │
│  • Renderização QML                                           │
│  • Eventos de usuário                                         │
│  • Atualização de Models (Q_PROPERTY, signals/slots)          │
│  • NUNCA bloqueia por mais de ~16ms                           │
└───────────────────────┬──────────────────────────────────────┘
                        │ sinais Qt (queued connection)
┌───────────────────────▼──────────────────────────────────────┐
│  Engine Thread (QThread dedicado)                             │
│  • AudioEngine vive aqui                                      │
│  • Recebe comandos da UI (via signals)                        │
│  • Modifica AudioGraph fora da audio thread                   │
│  • Operações de Project/Preset (I/O em background)            │
└───────────────────────┬──────────────────────────────────────┘
                        │ _executionOrder.store() (atômico)
┌───────────────────────▼──────────────────────────────────────┐
│  Audio Thread (prioridade RT — gerenciado pelo SO)            │
│  • Callback do RtAudio                                        │
│  • APENAS: AudioGraph::process()                              │
│  • Duração < buffer_size / sample_rate (ex: <5ms a 48kHz)    │
│  ✗ NUNCA: malloc/free, I/O, mutex, Qt signals, std::cout      │
└──────────────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────────────┐
│  Worker Threads (QThreadPool)                                 │
│  • Plugin scanning                                            │
│  • File loading (samples, IRs, projetos)                      │
│  • Waveform generation                                        │
│  • Export de áudio                                            │
└──────────────────────────────────────────────────────────────┘

Regra de ouro da audio thread:

Nada que possa bloquear indefinidamente pode estar no callback. Se precisa comunicar, use RingBuffer lock-free ou std::atomic.

Comunicação UI → Audio Thread:

Control (UI Thread)
    │
    ▼
AtomicValue<float> (parâmetro do efeito)  ← escrita na UI thread
    │
    ▼
DSPModule::process()                       ← leitura na audio thread

Comunicação Audio Thread → UI (ex: VU meter):

AudioWorker::process() → RingBuffer<MeterData>::push()
                                    │
                        QTimer (UI Thread) → RingBuffer::pop() → Model::update()


7. Camada MVC — QML → Control → Models → Engine
┌─────────────────────────────────────────────────────────────┐
│  QML (View)                                                  │
│  • Apenas lê properties e chama métodos de Controls         │
│  • Sem lógica de negócio                                     │
│  • Exemplo: AudioControl.inputDevice, AudioControl.start()  │
└──────────────────────────┬──────────────────────────────────┘
                           │ Q_PROPERTY / invokable
┌──────────────────────────▼──────────────────────────────────┐
│  Control (ViewModel)                                         │
│  • Expõe dados prontos para exibição                         │
│  • Traduz intenções de UI em operações do Engine             │
│  • Mantém estado de UI (seleção, modo de edição)             │
│  • NÃO conhece QML                                           │
│  • Comunica-se com Models por referência direta              │
└──────────────────────────┬──────────────────────────────────┘
                           │ métodos síncronos
┌──────────────────────────▼──────────────────────────────────┐
│  Models                                                      │
│  • QAbstractListModel, QObject com Q_PROPERTY                │
│  • Refletem o estado do Engine de forma observável pelo QML  │
│  • Atualizam via signals conectados ao Engine                │
│  • NÃO conhecem QML                                          │
└──────────────────────────┬──────────────────────────────────┘
                           │ queued Qt signals / direct calls
┌──────────────────────────▼──────────────────────────────────┐
│  Engine                                                      │
│  • Zero dependência de Qt GUI                                │
│  • Emite Qt signals para notificar Models                    │
│  • Pode ser reutilizado em CLI, testes unitários, etc.       │
└─────────────────────────────────────────────────────────────┘

Exemplo concreto: trocar dispositivo de áudio

HomePage.qml
  ComboBox.onActivated → audioControl.setOutputDevice(index)
                               │
                         AudioControl::setOutputDevice(int)
                               │ → audioEngine.setOutputDevice(index) [queued signal]
                               │
                         AudioEngine::setOutputDevice(int)   [Engine Thread]
                               │ → AudioDevice::reopen()
                               │ → emit devicesChanged()
                               │
                         AudioDeviceModel::onDevicesChanged() [UI Thread]
                               │ → update Q_PROPERTY → QML notificado

8. Serialização de Projeto / Preset

// preset.json
{
  "name": "Blues Crunch",
  "chain": [
    {
      "id": "node-1",
      "type": "Gain",
      "params": { "gainDb": -3.0 }
    },
    {
      "id": "node-2",
      "type": "Overdrive",
      "params": { "drive": 0.7, "tone": 0.5, "level": 0.8 }
    },
    {
      "id": "node-3",
      "type": "Reverb",
      "params": { "roomSize": 0.4, "wet": 0.2 }
    }
  ],
  "graph": {
    "edges": [
      { "from": "input",  "to": "node-1" },
      { "from": "node-1", "to": "node-2" },
      { "from": "node-2", "to": "node-3" },
      { "from": "node-3", "to": "output" }
    ]
  }
}

O Serializer reconstrói o grafo consultando uma NodeFactory que mapeia "Overdrive" → new Overdrive(). Adicionar um novo efeito é registrar na factory — sem alterar o serializer.

9. Plano de Evolução para DAW
A arquitetura acima já está preparada. Os pontos de extensão são:

Funcionalidade futura	Extensão necessária
Múltiplas tracks	TrackModel + múltiplos AudioGraph gerenciados pelo AudioEngine
Mixer	MixerNode : AudioNode com N entradas e ganho/pan por canal
Buses / Sends	BusNode, SendNode, ReturnNode — todos AudioNode
MIDI	MidiEngine já previsto; MidiNode : AudioNode para roteamento
Piano Roll	Transport + MidiTrackModel + novo módulo GUI
Timeline / Arrange	TimelineModel + Clip + AudioFileNode (reprodução de arquivo)
Gravação	RecordNode escreve em RingBuffer → worker thread drena para disco
Plugin Host VST3	PluginNode : AudioNode wrappeia IPlugin; PluginManager já previsto
Automação	AutomationLane grava/reproduz valores de parâmetros via setParameter()
Export	Worker thread percorre o grafo off-line (sem RtAudio) e escreve arquivo
A chave é que todos esses recursos são novos nós no grafo ou novos Models — sem reescrever o núcleo existente.

10. Próximos Passos Concretos no Código Atual
Com base no código que já existe, as mudanças prioritárias são:

Substituir AudioChain por AudioGraph — a cadeia linear atual vira um caso especial do grafo (lista de nós em série).

Expandir DSPModule com prepare(), reset() e setParameter()/getParameter() para viabilizar presets e automação.

Criar AudioDevice extraindo a lógica RtAudio de AudioWorker — AudioWorker passa a só orquestrar; AudioDevice abre/fecha o stream.

Criar a pasta Models/ com AudioDeviceModel e EffectChainModel, movendo estado observável que hoje provavelmente está no AudioControl.

Criar Project/Serializer com suporte a salvar/carregar um grafo de efeitos em JSON.

Criar Utils/RingBuffer e Utils/AtomicValue para comunicação thread-safe entre audio thread e UI.