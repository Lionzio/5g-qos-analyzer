#include "MetricsCalculator.h"
#include "QoSEvaluator.h"
#include "ReportGenerator.h"
#include "TrafficProfile.h"
#include <iostream>


int main() {
  std::cout
      << "[5G QoS Analyzer] Inicializando Pipeline de Arquitetura Base...\n";

  // 1. Instanciação do tráfego (Exemplo: Slice de Vídeo 4K eMBB)
  TrafficProfile embbSlice(SliceType::eMBB, "eMBB - Ultra HD 4K Live Stream",
                           1000.0, 15.0, 0.001);

  // 2. Instanciação dos motores de processamento
  MetricsCalculator calculator;
  QoSEvaluator evaluator;
  ReportGenerator reporter;

  // 3. Execução sequencial do duto (O Fluxo de Dados)
  CalculatedMetrics metrics = calculator.calculate(embbSlice);
  QoSAssessment qos = evaluator.evaluate(metrics);

  // 4. Entrega
  reporter.generateConsoleReport(embbSlice, metrics, qos);

  return 0;
}