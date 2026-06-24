#ifndef TRAFFIC_PROFILE_H
#define TRAFFIC_PROFILE_H

#include <string>
#include <string_view>

enum class SliceType {
  eMBB,  // Enhanced Mobile Broadband
  URLLC, // Ultra-Reliable Low-Latency Communications
  mMTC   // Massive Machine Type Communications
};

class TrafficProfile {
private:
  SliceType type;
  std::string name;
  double target_bandwidth; // em Mbps
  double base_latency;     // em ms
  double loss_probability; // em taxa decimal (0.0 a 1.0)

public:
  // C++17: std::string_view evita cópias desnecessárias de memória ao passar
  // textos
  TrafficProfile(SliceType t, std::string_view n, double bw, double lat,
                 double loss);

  // C++17: [[nodiscard]] avisa o programador se ele chamar a função e esquecer
  // de ler o retorno
  [[nodiscard]] SliceType getType() const;
  [[nodiscard]] std::string getName() const;
  [[nodiscard]] double getTargetBandwidth() const;
  [[nodiscard]] double getBaseLatency() const;
  [[nodiscard]] double getLossProbability() const;
};

#endif