#include "TrafficProfile.h"

TrafficProfile::TrafficProfile(SliceType t, std::string_view n, double bw,
                               double lat, double loss)
    : type(t), name(n), target_bandwidth(bw), base_latency(lat),
      loss_probability(loss) {}

SliceType TrafficProfile::getType() const { return type; }
std::string TrafficProfile::getName() const { return name; }
double TrafficProfile::getTargetBandwidth() const { return target_bandwidth; }
double TrafficProfile::getBaseLatency() const { return base_latency; }
double TrafficProfile::getLossProbability() const { return loss_probability; }