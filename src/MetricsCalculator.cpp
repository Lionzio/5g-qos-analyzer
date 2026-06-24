#include "MetricsCalculator.h"
#include <chrono>
#include <cmath>
#include <random>


CalculatedMetrics
MetricsCalculator::calculate(const TrafficProfile &profile) const {
  // 1. Instanciação do gerador pseudoaleatório de alta performance (Mersenne
  // Twister 64-bit) Semeado com os nanossegundos exatos do relógio do sistema
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937_64 generator(seed);

  // =========================================================================
  // SIMULACAO 1: LATENCIA E JITTER VIA DISTRIBUICAO GAUSSIANA (NORMAL)
  // =========================================================================
  // Em redes reais, o atraso de fila segue uma curva de sino em torno da média.
  // Assumimos que o desvio padrão da latência é 12% do valor base.
  double stddev = profile.getBaseLatency() * 0.12;
  std::normal_distribution<double> latency_dist(profile.getBaseLatency(),
                                                stddev);

  double simulated_latency = latency_dist(generator);

  // O piso físico da latência da fibra/ar não pode ser menor que 60% do
  // projetado
  double min_physical_latency = profile.getBaseLatency() * 0.6;
  if (simulated_latency < min_physical_latency) {
    simulated_latency = min_physical_latency;
  }

  // O Jitter é, por definição estrita, o desvio absoluto entre a latência
  // instantânea e a esperada
  double simulated_jitter =
      std::abs(simulated_latency - profile.getBaseLatency());

  // =========================================================================
  // SIMULACAO 2: PACKET LOSS VIA DISTRIBUICAO DE BERNOULLI (MONTE CARLO LITE)
  // =========================================================================
  // Disparamos 10.000 pacotes virtuais contra o rádio base.
  std::bernoulli_distribution packet_drop_dist(profile.getLossProbability());

  const int TOTAL_PACKETS = 10000;
  int dropped_count = 0;

  for (int i = 0; i < TOTAL_PACKETS; ++i) {
    if (packet_drop_dist(generator)) {
      dropped_count++;
    }
  }

  double simulated_loss_pct =
      (static_cast<double>(dropped_count) / TOTAL_PACKETS) * 100.0;

  // =========================================================================
  // SIMULACAO 3: DEGRADACAO DE THROUGHPUT POR CONGESTIONAMENTO E RE-TRANSMISSAO
  // =========================================================================
  // Se pacotes caem, o TCP reduz a janela de transmissão (Multiplicador de
  // Eficiência).
  std::uniform_real_distribution<double> channel_fading(0.94, 1.0);

  double loss_penalty = 1.0 - (simulated_loss_pct / 100.0);
  double efficiency_factor = channel_fading(generator) * loss_penalty;

  double simulated_throughput =
      profile.getTargetBandwidth() * efficiency_factor;

  return CalculatedMetrics{simulated_latency, simulated_jitter,
                           simulated_throughput, simulated_loss_pct};
}