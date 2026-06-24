#include "MetricsCalculator.h"
#include "QoSEvaluator.h"
#include "TrafficProfile.h"
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

// HOMOLOGAÇÃO DO SISTEMA: Cabeçalhos da API do SO estritamente no escopo global
#ifdef _WIN32
#include <windows.h>
#endif

// CORES ANSI PARA O TERMINAL DO WINDOWS BRILHAR EM VERDE E VERMELHO
const std::string GREEN = "\033[1;32m";
const std::string RED = "\033[1;31m";
const std::string RESET = "\033[0m";

int total_passed = 0;
int total_failed = 0;

// Mini-framework de asserção construído do zero
void assertTest(bool condition, const std::string &test_name,
                const std::string &details = "") {
  if (condition) {
    std::cout << GREEN << " [PASS] " << RESET << test_name << "\n";
    total_passed++;
  } else {
    std::cout << RED << " [FAIL] " << RESET << test_name;
    if (!details.empty())
      std::cout << " -> " << details;
    std::cout << "\n";
    total_failed++;
  }
}

// ============================================================================
// 1. CASOS POSITIVOS (Validação de Física de Tráfego)
// ============================================================================
void testPositiveEMBB() {
  std::cout << "\n--- TESTE: Sair da Fila eMBB (Throughput Massivo) ---\n";
  auto profile = TrafficProfile::createDefaultEMBB();
  MetricsCalculator calc;
  QoSEvaluator eval;

  auto metrics = calc.calculate(profile);
  auto qos = eval.evaluate(profile, metrics);

  assertTest(metrics.throughput_mbps > 850.0,
             "eMBB entregou Throughput aceitavel (> 850 Mbps)",
             "Obtido: " + std::to_string(metrics.throughput_mbps));
  assertTest(qos.qos_score >= 90.0,
             "QoS Score eMBB manteve padrao Premium (>= 90.0)");
  assertTest(qos.classification == "Excelente",
             "Classificacao final eMBB == 'Excelente'");
}

void testPositiveURLLC() {
  std::cout << "\n--- TESTE: Missao Critica URLLC (Baixissima Latencia) ---\n";
  auto profile = TrafficProfile::createDefaultURLLC();
  MetricsCalculator calc;
  QoSEvaluator eval;

  auto metrics = calc.calculate(profile);
  auto qos = eval.evaluate(profile, metrics);

  assertTest(metrics.latency_ms <= 2.0,
             "URLLC cravou latencia instantanea (<= 2.0 ms)",
             "Obtido: " + std::to_string(metrics.latency_ms) + " ms");
  assertTest(metrics.packet_loss_percent == 0.0,
             "Integridade URLLC estrita (0.0% de Perda)");
  assertTest(qos.reliability_score > 99.0,
             "Confiabilidade efetiva manteve SLA (> 99.0%)");
}

void testPositiveMMTC() {
  std::cout << "\n--- TESTE: Formigueiro IoT mMTC (Alta Densidade) ---\n";
  auto profile = TrafficProfile::createDefaultMMTC();
  MetricsCalculator calc;

  auto metrics = calc.calculate(profile);
  assertTest(metrics.throughput_mbps <= 0.25,
             "mMTC estrangulou a banda corretamente (<= 0.25 Mbps)");
  assertTest(metrics.latency_ms > 50.0,
             "Latencia mMTC operou no modo tolerante (> 50 ms)");
}

// ============================================================================
// 2. CASOS NEGATIVOS (Sanitização e Invariantes de Domínio)
// ============================================================================
void testNegativeDomainInvariants() {
  std::cout << "\n--- TESTE: Interceptacao de Ataques / Dados Invalidos ---\n";

  // Teste A: Tentar criar rádio com Banda Zero
  bool caught_zero_bw = false;
  try {
    TrafficProfile custom(SliceType::Custom, "Ataque Zero BW", 0.0, 10.0, 0.0);
  } catch (const std::invalid_argument &) {
    caught_zero_bw = true;
  }
  assertTest(caught_zero_bw,
             "Invariante capturou tentativa de Banda = 0.0 Mbps");

  // Teste B: Tentar criar rádio com Latência Negativa
  bool caught_neg_lat = false;
  try {
    TrafficProfile custom(SliceType::Custom, "Ataque Viagem no Tempo", 100.0,
                          -20.0, 0.0);
  } catch (const std::invalid_argument &) {
    caught_neg_lat = true;
  }
  assertTest(caught_neg_lat,
             "Invariante capturou tentativa de Latencia = -20.0 ms");
}

// ============================================================================
// 3. CASOS EXTREMOS (Boundary e Proteção Matemática)
// ============================================================================
void testExtremeCatastrophicFailure() {
  std::cout << "\n--- TESTE: Colapso Total da Infraestrutura (Clamping) ---\n";

  // Forçamos uma métrica falsa simulando uma catástrofe de rádio
  CalculatedMetrics doomsday_metrics{5000.0, 2000.0, 0.001,
                                     100.0}; // 5s de atraso, 100% de perda
  auto profile = TrafficProfile::createDefaultEMBB();
  QoSEvaluator eval;

  auto qos = eval.evaluate(profile, doomsday_metrics);

  assertTest(qos.qos_score == 0.0,
             "QoS Score estancou exatamente em 0.0 (Nao gerou nota negativa)");
  assertTest(qos.reliability_score == 0.0,
             "Confiabilidade estancou exatamente em 0.0%");
  assertTest(qos.classification == "Ruim",
             "Veredito do SLA para colapso cravou como 'Ruim'");
}

int main() {
  // HOMOLOGAÇÃO ANSI PARA WINDOWS POWERSHELL (Apenas a invocação local)
#ifdef _WIN32
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  DWORD dwMode = 0;
  GetConsoleMode(hOut, &dwMode);
  dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(hOut, dwMode);
#endif

  std::cout << "===================================================\n";
  std::cout << "       SUITE DE AUDITORIA DE TESTES (SPRINT 5)     \n";
  std::cout << "===================================================\n";

  testPositiveEMBB();
  testPositiveURLLC();
  testPositiveMMTC();
  testNegativeDomainInvariants();
  testExtremeCatastrophicFailure();

  std::cout << "\n===================================================\n";
  std::cout << " DIAGNOSTICO FINAL DA AUDITORIA:\n";
  std::cout << GREEN << "   Testes Aprovados : " << total_passed << RESET
            << "\n";
  if (total_failed > 0) {
    std::cout << RED << "   Testes Reprovados: " << total_failed << RESET
              << "\n";
  } else {
    std::cout << "   Testes Reprovados: 0 (100% de Taxa de Sucesso)\n";
  }
  std::cout << "===================================================\n";

  return (total_failed == 0) ? 0 : 1;
}