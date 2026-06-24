#ifndef QOS_EVALUATOR_H
#define QOS_EVALUATOR_H

#include "MetricsCalculator.h"
#include <string>

struct QoSAssessment {
  double qos_score;           // 0.0 a 100.0
  double reliability_score;   // 0.0 a 100.0
  std::string classification; // "Excelente", "Boa", "Regular", "Ruim"
};

class QoSEvaluator {
public:
  [[nodiscard]] QoSAssessment evaluate(const CalculatedMetrics &metrics) const;
};

#endif