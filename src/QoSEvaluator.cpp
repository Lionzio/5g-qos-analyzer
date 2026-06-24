#include "QoSEvaluator.h"
#include <algorithm> // std::min, std::max, std::clamp
#include <cmath>     // std::pow

std::string QoSEvaluator::classifyQoS(double qos_score) const {
  if (qos_score >= 85.0)
    return "Excelente";
  if (qos_score >= 70.0)
    return "Boa";
  if (qos_score >= 50.0)
    return "Regular";
  return "Ruim";
}

QoSAssessment QoSEvaluator::evaluate(const TrafficProfile &profile,
                                     const CalculatedMetrics &metrics) const {
  // =========================================================================
  // 1. CALCULO DE CONFIABILIDADE (Reliability Score)
  // =========================================================================
  double jitter_penalty = (metrics.jitter_ms / profile.getBaseLatency()) * 2.5;
  double raw_reliability = 100.0 - metrics.packet_loss_percent - jitter_penalty;

  double reliability = std::clamp(raw_reliability, 0.0, 100.0);

  // =========================================================================
  // 2. NORMALIZACAO DOS SUB-INDICES [0.0 a 1.0]
  // =========================================================================
  double idx_bw =
      std::min(1.0, metrics.throughput_mbps / profile.getTargetBandwidth());
  double idx_lat = std::min(1.0, profile.getBaseLatency() / metrics.latency_ms);
  double idx_jit =
      std::max(0.0, 1.0 - (metrics.jitter_ms / profile.getBaseLatency()));

  double loss_rate = metrics.packet_loss_percent / 100.0;
  double idx_loss = std::max(0.0, 1.0 - std::pow(loss_rate, 0.5));

  // =========================================================================
  // 3. MATRIZ DE PESOS DO 3GPP
  // =========================================================================
  double w_bw = 0.0, w_lat = 0.0, w_jit = 0.0, w_loss = 0.0;

  switch (profile.getType()) {
  case SliceType::eMBB:
    w_bw = 0.50;
    w_lat = 0.20;
    w_jit = 0.10;
    w_loss = 0.20;
    break;
  case SliceType::URLLC:
    w_bw = 0.05;
    w_lat = 0.40;
    w_jit = 0.15;
    w_loss = 0.40;
    break;
  case SliceType::mMTC:
    w_bw = 0.10;
    w_lat = 0.10;
    w_jit = 0.10;
    w_loss = 0.70;
    break;
  case SliceType::Custom:
  default:
    w_bw = 0.25;
    w_lat = 0.25;
    w_jit = 0.25;
    w_loss = 0.25;
    break;
  }

  // =========================================================================
  // 4. PRODUTO VETORIAL FINAL E CORTE DE CANAL MORTO
  // =========================================================================
  double raw_qos =
      (w_bw * idx_bw + w_lat * idx_lat + w_jit * idx_jit + w_loss * idx_loss) *
      100.0;

  // REGRA DE COLAPSO FÍSICO: Se a perda de pacotes for quase total (>= 99.9%),
  // o QoS zera.
  if (metrics.packet_loss_percent >= 99.9) {
    raw_qos = 0.0;
  }

  double qos_score = std::clamp(raw_qos, 0.0, 100.0);

  return QoSAssessment{qos_score, reliability, classifyQoS(qos_score)};
}