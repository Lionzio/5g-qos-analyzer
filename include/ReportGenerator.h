#ifndef REPORT_GENERATOR_H
#define REPORT_GENERATOR_H

#include "MetricsCalculator.h"
#include "QoSEvaluator.h"
#include "TrafficProfile.h"
#include <string>


class ReportGenerator {
private:
  // Método utilitário interno para carregar o timestamp do sistema
  [[nodiscard]] std::string getFormattedTimestamp() const;

public:
  void generateConsoleReport(const TrafficProfile &profile,
                             const CalculatedMetrics &metrics,
                             const QoSAssessment &qos) const;

  // NOVO: Exporta o relatório completo para TXT dentro da pasta outputs/
  // Retorna o caminho do arquivo gerado ou atira std::runtime_error em caso de
  // falha
  std::string exportToTXT(const TrafficProfile &profile,
                          const CalculatedMetrics &metrics,
                          const QoSAssessment &qos) const;
};

#endif