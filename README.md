# 📡 5G QoS Analyzer (CLI)

![C++17](https://img.shields.io/badge/Standard-C++17-blue.svg)
![Build](https://img.shields.io/badge/Build-Passing-brightgreen.svg)
![Platform](https://img.shields.io/badge/Platform-Windows%2011%20%2F%20Linux-lightgrey.svg)
![Architecture](https://img.shields.io/badge/Architecture-Pipes%20%26%20Filters-orange.svg)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)

> Uma ferramenta CLI de baixo nível desenvolvida em **C++17 puro** para simulação estocástica de perfis de tráfego 5G e auditoria multi-critério de Acordos de Nível de Serviço (SLA), fundamentada na recomendação **ITU-R M.2083 / 3GPP**.

---

## 📑 Índice

1. [🌐 Introdução e Motivação](#-introdução-e-motivação)
2. [📶 Fundamentos Teóricos (5G Network Slicing)](#-fundamentos-teóricos-5g-network-slicing)
3. [🏗️ Arquitetura do Sistema](#️-arquitetura-do-sistema)
4. [🧠 O Motor Matemático (SLA & Confiabilidade)](#-o-motor-matemático-sla--confiabilidade)
5. [🚀 Guia de Instalação e Compilação](#-guia-de-instalação-e-compilação)
6. [💻 Como Executar](#-como-executar)
7. [🧪 Suíte de Testes e Validação](#-suíte-de-testes-e-validação)
8. [📁 Estrutura do Repositório](#-estrutura-do-repositório)
9. [🔮 Próximos Passos (Roadmap Acadêmico)](#-próximos-passos-roadmap-acadêmico)

---

## 🌐 Introdução e Motivação

O advento das redes **5G Standalone (5G SA)** trouxe o conceito de *Network Slicing*: a capacidade de fatiar uma única infraestrutura física de rádio e núcleo em múltiplas redes lógicas isoladas, cada uma moldada para uma finalidade comercial diametralmente oposta. 

O **5G QoS Analyzer** nasce para preencher uma lacuna acadêmica: **traduzir as normativas abstratas de Qualidade de Serviço do 3GPP em código determinístico de baixo nível**, permitindo que estudantes e pesquisadores simulem a degradação de rádio (Fading, Jitter e Packet Loss) e observem o impacto matemático imediato na quebra de um contrato de SLA.

---

## 📶 Fundamentos Teóricos (5G Network Slicing)

A ferramenta modela nativamente os três pilares canônicos definidos pela União Internacional de Telecomunicações (ITU):

```text
                        [eMBB] 
               (Banda Larga Otimizada)
                     /         \
                    /           \
                   /             \
             [URLLC] ------------ [mMTC]
        (Missão Crítica)      (IoT Massivo)
```

1. **`eMBB` (Enhanced Mobile Broadband):** Focado em altíssimo *throughput*. Aplicação típica: *Streaming mobile de vídeo em 4K/8K ou Realidade Virtual*. Tolerante a pequenos atrasos, mas faminto por largura de banda.
2. **`URLLC` (Ultra-Reliable Low-Latency Communications):** Focado no teto operacional do tempo real. Aplicação típica: *Cirurgias assistidas remotamente ou freio autônomo veicular*. Exige latência instantânea $\le 1\text{ ms}$ e confiabilidade de entrega de **$99,999\%$** (*Five-Nines*).
3. **`mMTC` (Massive Machine Type Communications):** Focado em altíssima densidade espacial. Aplicação típica: *Redes de hidrômetros inteligentes em Smart Cities*. Suporta latências altas, mas estrangula a banda por dispositivo para poupar bateria.

---

## 🏗️ Arquitetura do Sistema

O software foi desenhado sob o padrão arquitetural **Pipes and Filters** (Dutos e Filtros), garantindo o fluxo unidirecional e imutável dos dados simulados:

```text
  [Input / Config]
         │
         ▼
  (TrafficProfile)   ──► Objeto de Valor (Imutável + Invariantes de Domínio)
         │
         ▼
 (MetricsCalculator) ──► Filtro 1: Motor Estocástico (Monte Carlo + Gaussiana)
         │
         ▼
  (QoSEvaluator)     ──► Filtro 2: Matriz de Pesos 3GPP + Auditoria de SLA
         │
         ▼
 (ReportGenerator)   ──► Saída: Renderização Win32 UTF-8 + Persistência RAII em .txt
```

### Princípios de Engenharia Adotados:
* **Invariantes de Domínio (Fail-Fast):** Tentativas de instanciar fatias de rede fisicamente impossíveis (ex: *largura de banda negativa ou perda > 100%*) disparam exceções no construtor antes de alocar memória de cálculo.
* **RAII na Persistência:** Descritores de arquivo de log (`std::ofstream`) atrelados ao escopo do gerador; garantindo o fechamento seguro do *handle* no SO mesmo sob colapso de thread.
* **Zero-Dependencies:** Sem frameworks externos acoplados. A suíte de testes unitários foi escrita nativamente na base do C++17.

---

## 🧠 O Motor Matemático (SLA & Confiabilidade)

Em redes de missão crítica, a literatura crava: *"A late packet is a lost packet"* (Um pacote atrasado equivale a um pacote perdido). Para capturar este fenômeno, a pontuação de **Confiabilidade Efetiva ($R_{\text{eff}}$)** pune a dispersão de Jitter:

$$R_{\text{eff}} = \max\left(0.0, \, 100.0 - P_{\text{loss}} - \left( \frac{J_{\text{sim}}}{L_{\text{base}}} \times 2.5 \right)\right)$$

O **QoS Score** final (0 a 100) é obtido pelo produto vetorial de 4 sub-índices normalizados de conformidade física, multiplicados pela **Matriz de Pesos do 3GPP ($W$)**:

$$\text{QoS} = \left( W_{\text{bw}}\cdot I_{\text{bw}} + W_{\text{lat}}\cdot I_{\text{lat}} + W_{\text{jit}}\cdot I_{\text{jit}} + W_{\text{loss}}\cdot I_{\text{loss}} \right) \times 100$$

| Perfil de Rede | Peso Banda ($W_{\text{bw}}$) | Peso Latência ($W_{\text{lat}}$) | Peso Jitter ($W_{\text{jit}}$) | Peso Perda ($W_{\text{loss}}$) | Rótulo de Prioridade |
| :--- | :---: | :---: | :---: | :---: | :--- |
| **eMBB** | `0.50` | `0.20` | `0.10` | `0.20` | Faminto por Throughput |
| **URLLC** | `0.05` | `0.40` | `0.15` | `0.40` | Obcecado por Tempo |
| **mMTC** | `0.10` | `0.10` | `0.10` | `0.70` | Focado em Estabilidade |

> **Corte Físico de Canal:** Se a taxa de perda de pacotes simulada atingir $\ge 99.9\%$, o algoritmo assume canal morto e força a nota de QoS a `0.00`.

---

## 🚀 Guia de Instalação e Compilação

### Pré-requisitos
* **Sistema Operacional:** Windows 10/11 ou distribuições Linux.
* **Compilador:** `g++` (GCC) com suporte explícito à flag `-std=c++17`.
* **Orquestrador:** `make` (GNU Make ou `mingw32-make` no Windows).

### Passo a Passo (Windows PowerShell)

1. **Clone o repositório:**
   ```powershell
   git clone [https://github.com/Lionzio/5g-qos-analyzer.git](https://github.com/Lionzio/5g-qos-analyzer.git)
   cd 5g-qos-analyzer
   ```

2. **Compile a aplicação principal:**
   ```powershell
   mingw32-make clean
   mingw32-make
   ```

*(Nota: Se o seu terminal possuir o alias GNU padrão configurado, digite apenas `make`).*

---

## 💻 Como Executar

Para abrir o painel interativo de simulação:

```powershell
.\5g_qos_analyzer.exe
```

O menu apresentará os presets do 3GPP e a opção de injeção de tráfego customizado:

```text
===================================================
        5G QoS ANALYZER - PAINEL DE CONTROLE       
===================================================
 Escolha um cenário de tráfego para simular:

   [1] Perfil eMBB   (Streaming Ultra HD 8K)
   [2] Perfil URLLC  (Cirurgia Robótica Remota)
   [3] Perfil mMTC   (Rede de Sensores Smart City)
   [4] Criar Slice Customizado (Forçar Erro/Teste)
   [0] Encerrar o Analisador
```

Ao selecionar uma opção, o sistema imprimirá o laudo no console e **gerará automaticamente um arquivo de auditoria física persistido na pasta `outputs/`**.

---

## 🧪 Suíte de Testes e Validação

Para certificar a resiliência matemática do motor e a blindagem contra ponteiros soltos, execute a suíte de testes isolada:

```powershell
mingw32-make test
```

A suíte executará **13 asserções estritas**, auditando:
1. Conformidade de tetos operacionais (Throughput e Latência).
2. Interceptação de invariantes de domínio via `std::invalid_argument`.
3. Estancamento seguro no zero (*Clamping*) sob simulação catastrófica de torre.

---

## 📁 Estrutura do Repositório

```text
5g-qos-analyzer/
├── docs/               # Documentações de referência e normas do 3GPP
├── include/            # Contratos de interface (Cabeçalhos .h)
│   ├── MetricsCalculator.h
│   ├── QoSEvaluator.h
│   ├── ReportGenerator.h
│   └── TrafficProfile.h
├── outputs/            # Diretório de pouso dos laudos de auditoria (.txt)
├── src/                # Motores de implementação (.cpp)
│   ├── main.cpp
│   ├── MetricsCalculator.cpp
│   ├── QoSEvaluator.cpp
│   ├── ReportGenerator.cpp
│   └── TrafficProfile.cpp
├── tests/              # Micro-framework nativo de asserções
│   └── test_runner.cpp
├── .gitignore          # Blindagem de artefatos de compilação
├── Makefile            # Orquestrador de build multi-alvo desacoplado
└── README.md           # Este documento
```

---

## 🔮 Próximos Passos (Roadmap Acadêmico)

- [ ] **Modelo de Perda de Markov:** Substituir a distribuição de Bernoulli por *Cadeias de Gilbert-Elliott* para simular perdas de pacotes em rajadas (*Bursts*).
- [ ] **Sobrecarga de Tunelamento:** Adicionar o cálculo de custo de cabeçalho do protocolo GTP-U (GPRS Tunneling Protocol).
- [ ] **Exportação Estruturada:** Implementar geração de laudos em formato `.json` para consumo por esteiras de Big Data.

---
*Desenvolvido como projeto de pesquisa e portfólio de engenharia de software por [Vinícius](https://github.com/Lionzio).*