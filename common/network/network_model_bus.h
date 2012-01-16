#ifndef NETWORK_MODEL_BUS_H
#define NETWORK_MODEL_BUS_H

// Define to use the queue model, rather than ContentionModel
//#define BUS_USE_QUEUE_MODEL

#include "network.h"
#include "core.h"
#include "lock.h"
#include "subsecond_time.h"
#ifdef BUS_USE_QUEUE_MODEL
#include "queue_model.h"
#else
#include "contention_model.h"
#endif

class NetworkModelBusGlobal
{
   public:
      Lock _lock;
      const ComponentBandwidth _bandwidth; //< Bits per cycle
      #ifdef BUS_USE_QUEUE_MODEL
      QueueModel* _queue_model;
      #else
      ContentionModel _contention_model;
      #endif

      UInt64 _num_packets;
      UInt64 _num_packets_delayed;
      UInt64 _num_bytes;
      SubsecondTime _time_used;
      SubsecondTime _total_delay;

      NetworkModelBusGlobal();
      ~NetworkModelBusGlobal();
      SubsecondTime useBus(SubsecondTime t_start, UInt32 length);
};

class NetworkModelBus : public NetworkModel
{
   static NetworkModelBusGlobal* _bus;

   private:
      bool _enabled;
      bool _mcp_detour;

      bool accountPacket(const NetPacket &pkt);
   public:
      NetworkModelBus(Network *net);
      ~NetworkModelBus() { }

      void routePacket(const NetPacket &pkt, std::vector<Hop> &nextHops);

      void processReceivedPacket(NetPacket& pkt);

      void outputSummary(std::ostream &out);

      void enable()
      { _enabled = true; }

      void disable()
      { _enabled = false; }
};

#endif /* NETWORK_MODEL_BUS_H */
