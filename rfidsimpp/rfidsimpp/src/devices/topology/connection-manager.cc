#include <devices/topology/connection-manager.h>
#include <devices/topology/connection-manager-signals.h>
#include <devices/control/device-control-unit.h>
#include <world/mobility/mobility-unit-base.h>
#include <radio/medium/medium.h>
#include <common/module-access.h>

using namespace omnetpp;

namespace rfidsim {

simsignal_t ConnectionManager::CONNECTION_CREATED_SIGNAL_ID =
        registerSignal(ConnectionCreated::NAME);

simsignal_t ConnectionManager::CONNECTION_LOST_SIGNAL_ID =
        registerSignal(ConnectionLost::NAME);

ConnectionManager::Role ConnectionManager::getDeviceRole(cModule *device)
{
  bool is_reader = isRFIDReader(device);
  bool is_tag = isRFIDTag(device);
  if (is_reader)
    return ROLE_READER;
  else if (is_tag)
    return ROLE_TAG;
  else
    throw cRuntimeError("module <%p> is not an RFID Reader or Tag", device);
}

ConnectionManager::~ConnectionManager()
{
  for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
    i->second->unsubscribe(i->first, this);
}

void ConnectionManager::initialize()
{
  new_devices.clear();
  devices.clear();
  roles.clear();
  roles[ROLE_READER] = std::set<int>();
  roles[ROLE_TAG] = std::set<int>();

  auto sys = getSystemModule();
  subscriptions.clear();
  subscriptions[MobilityUnit::POSITION_UPDATED_SIGNAL_ID] = sys;
  subscriptions[DeviceControlUnit::DEVICE_CREATED_SIGNAL_ID] = sys;
  subscriptions[DeviceControlUnit::DEVICE_DESTROYED_SIGNAL_ID] = sys;
  for (auto i = subscriptions.begin(); i != subscriptions.end(); ++i)
    i->second->subscribe(i->first, this);
}

void ConnectionManager::receiveSignal(cComponent *source, simsignal_t signal_id,
                                      cObject *obj, cObject *details)
{
  if (signal_id == MobilityUnit::POSITION_UPDATED_SIGNAL_ID)
  {
    auto signal_ = static_cast<DevicePositionUpdated*>(obj);
    processDevicePositionUpdate(*signal_);
  }
  else if (signal_id == DeviceControlUnit::DEVICE_CREATED_SIGNAL_ID)
  {
    auto signal_ = static_cast<DeviceCreated*>(obj);
    processDeviceCreated(*signal_);
  }
  else if (signal_id == DeviceControlUnit::DEVICE_DESTROYED_SIGNAL_ID)
  {
    auto signal_ = static_cast<DeviceDestroyed*>(obj);
    processDeviceDestroyed(*signal_);
  }
}

void ConnectionManager::processDevicePositionUpdate(
        const DevicePositionUpdated& upd)
{
  //
  // Ignore the update if it doesn't provide position information
  // (coordination system updates are not used by the connection manager)
  //
  if (!upd.next_position && !upd.prev_position)
    return;

  if (new_devices.count(upd.device_id))
  {
    const Vector3& pos = upd.next_position ? *(upd.next_position)
            : *(upd.prev_position);
    setDevicePosition(upd.device_id, pos);
  }
  else if (upd.next_position)
  {
    setDevicePosition(upd.device_id, *(upd.next_position));
  }
}

void ConnectionManager::processDeviceCreated(const DeviceCreated& upd)
{
  addNewDevice(upd.device_id, upd.device);
}

void ConnectionManager::processDeviceDestroyed(const DeviceDestroyed& upd)
{
  removeDevice(upd.device_id);
}

void ConnectionManager::addNewDevice(int id, cModule *module)
{
  if (new_devices.count(id) > 0 || devices.count(id) > 0)
    throw cRuntimeError("DeviceID=%d already registered", id);
  new_devices[id] = module;
}

void ConnectionManager::setDevicePosition(int id, const Vector3& pos)
{
  auto new_device_iter = new_devices.find(id);
  if (new_device_iter != new_devices.end())
  {
    auto device = new_device_iter->second;
    auto role = getDeviceRole(device);
    Record record(id, role, device, pos);
    devices[id] = record;
    roles[role].insert(id);
    new_devices.erase(id);
  }
  else
  {
    auto device_iter = devices.find(id);
    if (device_iter != devices.end())
    {
      device_iter->second.position = pos;
    }
  }
  updateConnections(id);
}

void ConnectionManager::updateConnections(int device_id)
{
  auto device_iter = devices.find(device_id);
  if (device_iter == devices.end())
    return;

  Record& device_record = device_iter->second;
  Role peer_role = device_record.role == ROLE_READER ? ROLE_TAG : ROLE_READER;
  const std::set<int>& all_possible_peers = roles[peer_role];
  std::list<int> new_peers;
  std::list<int> removed_peers;

  //
  // Inspect distances to all devices with different role:
  // - if the distance is smaller than connection radius and devices are not
  //   connected, create a new connection;
  // - if the distance is greater that disconnect radius and devices are
  //   connected, destroy the connection.
  //
  // Since signals are processed in the emit() call stack to avoid possible
  // bugs when other signals come to ConnectionManager during emit() processing,
  // we don't create/destroy connections directly here. Instead, updated
  // peers' ids are recorded into the corresponding lists (new_peers,
  // removed_peers) and are processed afterwards.
  //
  for (auto peer_id: all_possible_peers)
  {
    Record& peer_record = devices[peer_id];
    double d = device_record.position.getDistanceTo(peer_record.position);
    bool connected = device_record.peers.count(peer_id) > 0;

    if (d <= connect_distance && !connected)
    {
      device_record.peers.insert(peer_id);
      peer_record.peers.insert(device_id);
      new_peers.push_back(peer_id);
    }
    else if (d >= disconnect_distance && connected)
    {
      device_record.peers.erase(peer_id);
      peer_record.peers.erase(device_id);
      removed_peers.push_back(peer_id);
    }
  }

  //
  // Create and destroy connections. To tune performance, inspect the device
  // role and determine which one is a reader and which one is a tag once.
  //
  if (device_record.role == ROLE_READER)
  {
    for (auto peer_id: new_peers)
      emitConnectionCreated(device_id, peer_id);
    for (auto peer_id: removed_peers)
      emitConnectionLost(device_id, peer_id);
  }
  else if (device_record.role == ROLE_TAG)
  {
    for (auto peer_id: new_peers)
      emitConnectionCreated(peer_id, device_id);
    for (auto peer_id: removed_peers)
      emitConnectionLost(peer_id, device_id);
  }
}

void ConnectionManager::removeDevice(int device_id)
{
  auto new_device_iter = new_devices.find(device_id);
  if (new_device_iter != new_devices.end())
  {
    new_devices.erase(new_device_iter);
    ASSERT(devices.count(device_id) == 0);
    return;
  }

  auto device_iter = devices.find(device_id);
  if (device_iter == devices.end())
    return;

  Record& record = device_iter->second;
  std::list<int> old_peers(record.peers.begin(), record.peers.end());
  for (int peer_id: record.peers)
    devices[peer_id].peers.erase(device_id);

  auto role = record.role;
  devices.erase(device_iter);
  roles[role].erase(device_id);

  if (role == ROLE_READER)
  {
    for (auto peer_id: old_peers)
      emitConnectionLost(device_id, peer_id);
  }
  else if (role == ROLE_TAG)
  {
    for (auto peer_id: old_peers)
      emitConnectionLost(peer_id, device_id);
  }
}

void ConnectionManager::emitConnectionCreated(int reader_id, int tag_id)
{
  ConnectionCreated signal_(reader_id, tag_id);
  emit(CONNECTION_CREATED_SIGNAL_ID, &signal_);
}

void ConnectionManager::emitConnectionLost(int reader_id, int tag_id)
{
  ConnectionLost signal_(reader_id, tag_id);
  emit(CONNECTION_LOST_SIGNAL_ID, &signal_);
}

}
