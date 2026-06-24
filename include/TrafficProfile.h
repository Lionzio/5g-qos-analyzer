#ifndef TRAFFIC_PROFILE_H
#define TRAFFIC_PROFILE_H

#include <stdexcept>
#include <string>
#include <string_view>


enum class SliceType { eMBB, URLLC, mMTC, Custom };

class TrafficProfile {
private:
  SliceType type;
  std::string name;
  double target_bandwidth; // Mbps
  double base_latency;     // ms
  double loss_probability; // Taxa decimal (0.0 a 1.0)

public:
  // Construtor principal (Executa validação estrita dos dados)
  TrafficProfile(SliceType t, std::string_view n, double bw, double lat,
                 double loss);

  // ==========================================
  // PADRAO ARQUITETURAL: STATIC FACTORY METHODS
  // ==========================================
  static TrafficProfile createDefaultEMBB();
  static TrafficProfile createDefaultURLLC();
  static TrafficProfile createDefaultMMTC();

  [[nodiscard]] SliceType getType() const;
  [[nodiscard]] std::string getName() const;
  [[nodiscard]] double getTargetBandwidth() const;
  [[nodiscard]] double getBaseLatency() const;
  [[nodiscard]] double getLossProbability() const;
};

#endif