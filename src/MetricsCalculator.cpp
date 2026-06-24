#include "MetricsCalculator.h"

CalculatedMetrics
MetricsCalculator::calculate(const TrafficProfile &profile) const {
  // Implementação Dummy (Mock) da Sprint 1 para testar o acoplamento das
  // classes
  return CalculatedMetrics{
      profile.getBaseLatency() * 1.05, // Latência sofre 5% de degradação base
      profile.getBaseLatency() * 0.10, // Jitter simulado como 10% da latência
      profile.getTargetBandwidth() *
          0.92, // Entregando 92% do Throughput prometido
      profile.getLossProbability() *
          100.0 // Transformando probabilidade decimal em %
  };
}