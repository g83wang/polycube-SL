#pragma once
#include <string>
const std::string helloworld_datamodel = R"POLYCUBE_DP(
module helloworld {
  yang-version 1.1;
  namespace "http://polycube.network/helloworld";
  prefix "helloworld";

  import polycube-base { prefix "polycube-base"; }
  import polycube-standard-base { prefix "polycube-standard-base"; }

  organization "Polycube open source project";
  description "YANG data model for the Polycube Helloworld service";

  polycube-base:service-description "Helloworld Service";
  polycube-base:service-version "2.0";
  polycube-base:service-name "helloworld";
  polycube-base:service-min-kernel-version "4.14.0";

  uses "polycube-standard-base:standard-base-yang-module";

  leaf action {
    type enumeration {
      enum DROP;
      enum SLOWPATH;
      enum FORWARD;
    }
    default DROP;
    description "Action performed on the received packet (i.e., DROP, SLOWPATH, or FORWARD; default: DROP)";
  }
}
)POLYCUBE_DP";
