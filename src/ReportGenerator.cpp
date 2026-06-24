#include "ReportGenerator.h"
#include <iomanip>
#include <iostream>


void ReportGenerator::generateConsoleReport(const TrafficProfile &profile,
                                            const CalculatedMetrics &metrics,
                                            const QoSAssessment &qos) const {
  std::cout << "\n===================================================\n";
  std::cout << "           5G QoS ANALYZER - SLICE REPORT          \n";
  std::cout << "===================================================\n";
  std::cout << " Perfil de Trafego  : " << profile.getName() << "\n";
  std::cout << "---------------------------------------------------\n";

  // Trava a formatação de números decimais em 2 casas
  std::cout << std::fixed << std::setprecision(2);

  std::cout << " [METRICAS FISICAS (SIMULADAS)]\n";
  std::cout << "   Throughput real  : " << metrics.throughput_mbps << " Mbps\n";
  std::cout << "   Latencia media   : " << metrics.latency_ms << " ms\n";
  std::cout << "   Jitter estimado  : " << metrics.jitter_ms << " ms\n";
  std::cout << "   Perda de pacotes : " << metrics.packet_loss_percent
            << " %\n";
  std::cout << "---------------------------------------------------\n";
  std::cout << " [AVALIACAO ITU-R M.2083]\n";
  std::cout << "   Confiabilidade   : " << qos.reliability_score << " %\n";
  std::cout << "   QoS Score        : " << qos.qos_score << " / 100.00\n";
  std::cout << "   Status do Slice  : [" << qos.classification << "]\n";
  std::cout << "===================================================\n";
}