#include "QoSEvaluator.h"

QoSAssessment QoSEvaluator::evaluate(const CalculatedMetrics &metrics) const {
  // Mock simples: Subtrai as perdas de um teto ideal de 100 pontos
  double score =
      100.0 - metrics.packet_loss_percent - (metrics.latency_ms / 10.0);

  // Trava matemática de segurança (Clamp) entre 0 e 100
  if (score < 0.0)
    score = 0.0;
  if (score > 100.0)
    score = 100.0;

  std::string label;
  if (score >= 80.0)
    label = "Excelente";
  else if (score >= 60.0)
    label = "Boa";
  else if (score >= 40.0)
    label = "Regular";
  else
    label = "Ruim";

  return QoSAssessment{
      score,
      99.999, // Confiabilidade estática de teste (5-nines do 5G)
      label};
}