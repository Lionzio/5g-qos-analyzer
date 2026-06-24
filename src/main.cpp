#include "MetricsCalculator.h"
#include "QoSEvaluator.h"
#include "ReportGenerator.h"
#include "TrafficProfile.h"
#include <iostream>
#include <limits>

void clearInputBuffer() {
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void runSimulation(const TrafficProfile &profile, const MetricsCalculator &calc,
                   const QoSEvaluator &eval, const ReportGenerator &rep) {
  CalculatedMetrics metrics = calc.calculate(profile);

  QoSAssessment qos = eval.evaluate(profile, metrics);

  rep.generateConsoleReport(profile, metrics, qos);
}

int main() {
  MetricsCalculator calculator;
  QoSEvaluator evaluator;
  ReportGenerator reporter;

  int option = -1;

  while (option != 0) {
    std::cout << "\n===================================================\n";
    std::cout << "        5G QoS ANALYZER - PAINEL DE CONTROLE       \n";
    std::cout << "===================================================\n";
    std::cout << " Escolha um cenario de trafego para simular:\n\n";
    std::cout << "   [1] Perfil eMBB   (Streaming Ultra HD 8K)\n";
    std::cout << "   [2] Perfil URLLC  (Cirurgia Robotica Remota)\n";
    std::cout << "   [3] Perfil mMTC   (Rede de Sensores Smart City)\n";
    std::cout << "   [4] Criar Slice Customizado (Forçar Erro/Teste)\n";
    std::cout << "   [0] Encerrar o Analisador\n";
    std::cout << "---------------------------------------------------\n";
    std::cout << " Opcao: ";

    if (!(std::cin >> option)) {
      clearInputBuffer();
      option = -1;
    }

    try {
      switch (option) {
      case 1:
        runSimulation(TrafficProfile::createDefaultEMBB(), calculator,
                      evaluator, reporter);
        break;
      case 2:
        runSimulation(TrafficProfile::createDefaultURLLC(), calculator,
                      evaluator, reporter);
        break;
      case 3:
        runSimulation(TrafficProfile::createDefaultMMTC(), calculator,
                      evaluator, reporter);
        break;
      case 4: {
        std::string nome;
        double bw, lat, loss;

        std::cout << "\n Nome do Slice Customizado: ";
        std::cin >> std::ws; // Limpa espaços em branco vazados
        std::getline(std::cin, nome);

        std::cout << " Banda Desejada (Mbps) : ";
        std::cin >> bw;
        std::cout << " Latencia Alvo (ms)    : ";
        std::cin >> lat;
        std::cout << " Probabilidade Perda (ex: 0.05 para 5%): ";
        std::cin >> loss;

        TrafficProfile custom(SliceType::Custom, nome, bw, lat, loss);
        runSimulation(custom, calculator, evaluator, reporter);
        break;
      }
      case 0:
        std::cout << "\n[Desligando modems de radio base... Ate logo!]\n";
        break;
      default:
        std::cout << "\n[!] Opcao invalida. Selecione de 0 a 4.\n";
      }
    }
    // AQUI ESTA O TRATAMENTO DE ERROS SOLICITADO NA SPRINT 2
    catch (const std::invalid_argument &e) {
      std::cerr << "\n[BLOQUEIO DE SEGURANCA DO 3GPP]\n" << e.what() << "\n\n";
      clearInputBuffer();
    } catch (const std::exception &e) {
      std::cerr << "\n[ERRO CRITICO DE EXECUCAO] " << e.what() << "\n\n";
    }
  }

  return 0;
}