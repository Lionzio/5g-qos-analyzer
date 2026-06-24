#ifndef METRICS_CALCULATOR_H
#define METRICS_CALCULATOR_H

#include "TrafficProfile.h"

// Struct POD (Plain Old Data) para trafegar os dados calculados de forma leve
struct CalculatedMetrics {
  double latency_ms;
  double jitter_ms;
  double throughput_mbps;
  double packet_loss_percent;
};

class MetricsCalculator {
public:
  // Passamos o Profile por referência constante (&) para não duplicar o objeto
  // na memória
  [[nodiscard]] CalculatedMetrics
  calculate(const TrafficProfile &profile) const;
};

#endif