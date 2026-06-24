#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include "MetricsCalculator.h"
#include "QoSEvaluator.h"
#include "TrafficProfile.h"


class ReportGenerator {
public:
  // Classe puramente utilitária de saída
  void generateConsoleReport(const TrafficProfile &profile,
                             const CalculatedMetrics &metrics,
                             const QoSAssessment &qos) const;
};

#endif