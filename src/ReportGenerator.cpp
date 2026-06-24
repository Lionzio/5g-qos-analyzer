#include "ReportGenerator.h"
#include <algorithm> // std::replace_if
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>


std::string ReportGenerator::getFormattedTimestamp() const {
  auto now = std::chrono::system_clock::now();
  auto now_time_t = std::chrono::system_clock::to_time_t(now);

  std::stringstream ss;
  ss << std::put_time(std::localtime(&now_time_t), "%Y-%m-%d_%H-%M-%S");
  return ss.str();
}

void ReportGenerator::generateConsoleReport(const TrafficProfile &profile,
                                            const CalculatedMetrics &metrics,
                                            const QoSAssessment &qos) const {
  std::cout << "\n===================================================\n";
  std::cout << "           5G QoS ANALYZER - SLICE REPORT          \n";
  std::cout << "===================================================\n";
  std::cout << " Perfil de Tráfego  : " << profile.getName() << "\n";
  std::cout << "---------------------------------------------------\n";

  std::cout << std::fixed << std::setprecision(2);

  std::cout << " [MÉTRICAS FÍSICAS (SIMULADAS)]\n";
  std::cout << "   Throughput Real  : " << metrics.throughput_mbps
            << " Mbps (Alvo: " << profile.getTargetBandwidth() << " Mbps)\n";
  std::cout << "   Latência Média   : " << metrics.latency_ms
            << " ms (Base: " << profile.getBaseLatency() << " ms)\n";
  std::cout << "   Jitter Estimado  : " << metrics.jitter_ms << " ms\n";
  std::cout << "   Perda de Pacotes : " << metrics.packet_loss_percent
            << " %\n";
  std::cout << "---------------------------------------------------\n";
  std::cout << " [AUDITORIA DE SLA / ITU-R M.2083]\n";
  std::cout << "   Confiabilidade   : " << qos.reliability_score << " %\n";
  std::cout << "   QoS Score        : " << qos.qos_score << " / 100.00\n";
  std::cout << "   Status do Slice  : [" << qos.classification << "]\n";
  std::cout << "===================================================\n";
}

std::string ReportGenerator::exportToTXT(const TrafficProfile &profile,
                                         const CalculatedMetrics &metrics,
                                         const QoSAssessment &qos) const {
  std::string clean_name = profile.getName();

  // SANITIZAÇÃO DE FILENAME: Troca caracteres inválidos no Windows/Linux por
  // '_'
  std::replace_if(
      clean_name.begin(), clean_name.end(),
      [](char c) {
        return c == '/' || c == '\\' || c == ':' || c == '*' || c == '?' ||
               c == '"' || c == '<' || c == '>' || c == '|';
      },
      '_');

  std::string timestamp = getFormattedTimestamp();
  std::string filepath =
      "outputs/QoS_Report_" + clean_name + "_" + timestamp + ".txt";

  // Abertura do arquivo em modo de escrita via RAII
  std::ofstream outfile(filepath);
  if (!outfile.is_open()) {
    throw std::runtime_error(
        "Falha de I/O: Não foi possível criar o arquivo de auditoria em " +
        filepath);
  }

  outfile << std::fixed << std::setprecision(2);
  outfile << "===================================================\n";
  outfile << "       RELATÓRIO DE AUDITORIA DE QoS 5G (SLA)      \n";
  outfile << "===================================================\n";
  outfile << " Data da Simulação    : " << timestamp << "\n";
  outfile << " Perfil Analisado     : " << profile.getName() << "\n";
  outfile << " Categoria de Slice   : ";
  switch (profile.getType()) {
  case SliceType::eMBB:
    outfile << "eMBB (Enhanced Mobile Broadband)\n";
    break;
  case SliceType::URLLC:
    outfile << "URLLC (Ultra-Reliable Low-Latency)\n";
    break;
  case SliceType::mMTC:
    outfile << "mMTC (Massive Machine Type)\n";
    break;
  default:
    outfile << "Slice Customizado\n";
    break;
  }
  outfile << "---------------------------------------------------\n";
  outfile << " [PARÂMETROS FÍSICOS CONFIGURADOS]\n";
  outfile << "   Banda Alvo           : " << profile.getTargetBandwidth()
          << " Mbps\n";
  outfile << "   Latência Base Alvo   : " << profile.getBaseLatency()
          << " ms\n";
  outfile << "   Probabilidade Perda  : "
          << (profile.getLossProbability() * 100.0) << " %\n";
  outfile << "---------------------------------------------------\n";
  outfile << " [RESULTADOS DA SIMULAÇÃO ESTOCÁSTICA]\n";
  outfile << "   Throughput Efetivo   : " << metrics.throughput_mbps
          << " Mbps\n";
  outfile << "   Latência Obtida      : " << metrics.latency_ms << " ms\n";
  outfile << "   Jitter Estimado      : " << metrics.jitter_ms << " ms\n";
  outfile << "   Taxa de Perda Real   : " << metrics.packet_loss_percent
          << " %\n";
  outfile << "---------------------------------------------------\n";
  outfile << " [DIAGNÓSTICO DE MISSÃO CRÍTICA (3GPP)]\n";
  outfile << "   Confiabilidade Real  : " << qos.reliability_score << " %\n";
  outfile << "   QoS Score Final      : " << qos.qos_score << " / 100.00\n";
  outfile << "   VEREDITO DO SLA      : " << qos.classification << "\n";
  outfile << "===================================================\n";
  outfile << "Gerado automaticamente pelo motor do 5G QoS Analyzer CLI.\n";

  return filepath;
}