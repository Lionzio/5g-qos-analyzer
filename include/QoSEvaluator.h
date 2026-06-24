#ifndef QOS_EVALUATOR_H
#define QOS_EVALUATOR_H

#include "MetricsCalculator.h"
#include "TrafficProfile.h"
#include <string>


struct QoSAssessment {
  double qos_score;           // 0.0 a 100.0
  double reliability_score;   // 0.0 a 100.0
  std::string classification; // "Excelente", "Boa", "Regular", "Ruim"
};

class QoSEvaluator {
private:
  [[nodiscard]] std::string classifyQoS(double qos_score) const;

public:
  // REFATORACAO ARQUITETURAL: Injetando o TrafficProfile no motor
  [[nodiscard]] QoSAssessment evaluate(const TrafficProfile &profile,
                                       const CalculatedMetrics &metrics) const;
};

#endif