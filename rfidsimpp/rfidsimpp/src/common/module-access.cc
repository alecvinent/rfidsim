#include <common/module-access.h>
#include <functional>

using namespace omnetpp;

namespace rfidsim {

const char *RFID_DEVICE_PROPERTY = "RFIDDevice";
const char *THING_PROPERTY = "Thing";

inline bool _isModuleWithProperty(cModule *module, const char *property)
{
  if (!module)
    return false;
  cProperties *properties = module->getProperties();
  return properties && properties->getAsBool(property);
}

bool isRFIDDevice(cModule *module)
{
  return _isModuleWithProperty(module, RFID_DEVICE_PROPERTY);
}

bool isThing(cModule *module)
{
  return _isModuleWithProperty(module, THING_PROPERTY);
}

inline bool _isRFIDDeviceType(cModule *module, const char *type)
{
  if (isRFIDDevice(module))
  {
    auto properties = module->getProperties();
    auto rfid_prop = properties->get(RFID_DEVICE_PROPERTY);
    std::string rfid_device_type = rfid_prop->getValue("");
    return rfid_device_type == type;
  }
  else
  {
    return false;
  }
}

bool isRFIDTag(cModule *rfid_device)
{
  return _isRFIDDeviceType(rfid_device, "tag");
}

bool isRFIDReader(cModule *rfid_device)
{
  return _isRFIDDeviceType(rfid_device, "reader");
}

inline omnetpp::cModule *_findEnclosingModule(
        cModule *module, std::function<bool(cModule*)> test)
{
  while (module && !test(module))
  {
    module = module->getParentModule();
  }
  return module;
}

cModule *findEnclosingRFIDDevice(cModule *module)
{
  return _findEnclosingModule(module, isRFIDDevice);
}

cModule *findEnclosingThing(cModule *module)
{
  return _findEnclosingModule(module, isThing);
}

inline cModule *_getEnclosingModule(
        cModule *module, std::function<cModule*(cModule*)> find,
        const char *property_name)
{
  auto result = find(module);
  if (!result)
    throw cRuntimeError("module %s not contained in @%s module",
                        (module ? module->getFullPath().c_str() : "(null)"),
                        property_name);
  return result;
}

cModule *getEnclosingRFIDDevice(cModule *module)
{
  return _getEnclosingModule(module, findEnclosingRFIDDevice,
                             RFID_DEVICE_PROPERTY);
}

cModule *getEnclosingThing(cModule *module)
{
  return _getEnclosingModule(module, findEnclosingThing, THING_PROPERTY);
}

bool isParentOf(cModule *parent, cModule *child)
{
  if (!parent || !child)
    return false;
  auto p = child;
  while (p && p != parent)
  {
    p = p->getParentModule();
  }
  return p == parent ? true : false;
}

cModule *getClosestCommonParent(
        omnetpp::cModule *m1, omnetpp::cModule *m2)
{
  if (!m1 || !m2)
    throw cRuntimeError("m1=<%p>, m2=<%p>", m1, m2);
  auto parent1 = m1;
  while (parent1 && !isParentOf(parent1, m2))
  {
    parent1 = parent1->getParentModule();
  }
  ASSERT(parent1 != nullptr);  // otherwise system module isn't a parent for m2
  return parent1;
}

}
