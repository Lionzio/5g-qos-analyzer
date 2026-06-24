#include "TrafficProfile.h"

TrafficProfile::TrafficProfile(SliceType t, std::string_view n, double bw,
                               double lat, double loss)
    : type(t), name(n), target_bandwidth(bw), base_latency(lat),
      loss_probability(loss) {

  // BLINDAGEM DE CONSTRUTOR (RAII / Invariantes de Dominio)
  if (bw <= 0.0) {
    throw std::invalid_argument(
        "Erro: A largura de banda deve ser estritamente maior que 0 Mbps.");
  }
  if (lat <= 0.0) {
    throw std::invalid_argument(
        "Erro: A latencia base deve ser estritamente positiva.");
  }
  if (loss < 0.0 || loss > 1.0) {
    throw std::invalid_argument("Erro: A probabilidade de perda de pacotes "
                                "deve estar no intervalo [0.0, 1.0].");
  }
}

// Fabricas estáticas espelhando os requisitos minimos da recomendação ITU-R
// M.2083
TrafficProfile TrafficProfile::createDefaultEMBB() {
  return TrafficProfile(SliceType::eMBB, "eMBB (4K/8K Mobile Streaming)",
                        1000.0, 15.0, 0.001); // Perda alvo: 0.1%
}

TrafficProfile TrafficProfile::createDefaultURLLC() {
  return TrafficProfile(SliceType::URLLC,
                        "URLLC (Autonomous Driving / Tele-Surgery)", 50.0, 1.0,
                        0.00001); // Perda alvo: 10^-5 (99.999%)
}

TrafficProfile TrafficProfile::createDefaultMMTC() {
  return TrafficProfile(SliceType::mMTC,
                        "mMTC (Massive Smart City IoT Network)", 0.2, 120.0,
                        0.02); // Banda baixa, tolerante a perda de 2%
}

SliceType TrafficProfile::getType() const { return type; }
std::string TrafficProfile::getName() const { return name; }
double TrafficProfile::getTargetBandwidth() const { return target_bandwidth; }
double TrafficProfile::getBaseLatency() const { return base_latency; }
double TrafficProfile::getLossProbability() const { return loss_probability; }