#ifndef RFIDSIMPP_DEVICES_TOPOLOGY_CONNECTION_MANAGER_H_
#define RFIDSIMPP_DEVICES_TOPOLOGY_CONNECTION_MANAGER_H_

#include <omnetpp.h>
#include <devices/control/device-control-signals.h>
#include <world/mobility/mobility-signals.h>
#include <devices/control/device-control-signals.h>

namespace rfidsim {

class ConnectionManager : public omnetpp::cSimpleModule,
                          public omnetpp::cListener {
 public:
  static omnetpp::simsignal_t CONNECTION_CREATED_SIGNAL_ID;
  static omnetpp::simsignal_t CONNECTION_LOST_SIGNAL_ID;

  enum Role { ROLE_READER, ROLE_TAG };

  static Role getDeviceRole(omnetpp::cModule *device);

  virtual ~ConnectionManager();

 protected:
  virtual void initialize();
  virtual void receiveSignal(
          omnetpp::cComponent *source, omnetpp::simsignal_t signal_id,
          omnetpp::cObject *obj, omnetpp::cObject *details);

  virtual void processDevicePositionUpdate(const DevicePositionUpdated& upd);
  virtual void processDeviceCreated(const DeviceCreated& upd);
  virtual void processDeviceDestroyed(const DeviceDestroyed& upd);

 private:
  void addNewDevice(int device_id, omnetpp::cModule *device_module);
  void setDevicePosition(int device_id, const Vector3& position);
  void updateConnections(int device_id);
  void removeDevice(int device_id);

  void emitConnectionCreated(int reader_id, int tag_id);
  void emitConnectionLost(int reader_id, int tag_id);

 private:
  double connect_distance = 0.0;
  double disconnect_distance = 0.0;

  std::map<int, omnetpp::cModule*> new_devices;

  struct Record {
    int device_id;
    Role role;
    omnetpp::cModule *device;
    Vector3 position;
    std::set<int> peers;

    Record() {}

    Record(int device_id, Role role, omnetpp::cModule *device, Vector3 position)
      : device_id(device_id), role(role), device(device), position(position) {}
  };

  std::map<int, Record> devices;
  std::map<Role, std::set<int>> roles;

  std::map<omnetpp::simsignal_t, omnetpp::cModule*> subscriptions;
};

}

#endif
