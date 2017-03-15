#ifndef RFIDSIMPP_COMMON_MODULE_ACCESSORS_H_
#define RFIDSIMPP_COMMON_MODULE_ACCESSORS_H_

#include <omnetpp.h>

namespace rfidsim {

extern const char *RFID_DEVICE_PROPERTY;
extern const char *THING_PROPERTY;

/**
 * Returns true if the given module has @RFIDDevice property.
 */
bool isRFIDDevice(omnetpp::cModule *module);

bool isRFIDTag(omnetpp::cModule *rfid_device);
bool isRFIDReader(omnetpp::cModule *rfid_device);

/**
 * Looks upward the hierarchy to find the RFID Device module (with @RFIDDevice
 * property set). If not found, nullptr is returned.
 */
omnetpp::cModule *findEnclosingRFIDDevice(omnetpp::cModule *module);

/**
 * Looks upward the hierarchy to find the RFID Device module (with @RFIDDevice
 * property set). If not found, cRuntimeError is thrown.
 */
omnetpp::cModule *getEnclosingRFIDDevice(omnetpp::cModule *module);

/**
 * Returns true if the given module has @Thing property
 */
bool isThing(omnetpp::cModule *module);

/**
 * Looks upward the hierarchy to find the Thing module (with @Thing property).
 * If not found, nullptr is returned.
 */
omnetpp::cModule *findEnclosingThing(omnetpp::cModule *module);

/**
 * Looks upward the hierarchy to find the Thing module (with @Thing property).
 * If not found, cRuntimeError is thrown.
 */
omnetpp::cModule *getEnclosingThing(omnetpp::cModule *module);

/**
 * Searches for a direct child of the given class T. If `required` is set,
 * an exception will be thrown if not found.
 */
template<class T>
T *findChildByType(omnetpp::cModule *module, bool required = true)
{
  if (!module)
  {
    if (required)
      throw omnetpp::cRuntimeError("module is null");
    else
      return nullptr;
  }
  for (omnetpp::cModule::SubmoduleIterator i(module); !i.end(); i++)
  {
    T *submodule = dynamic_cast<T*>(*i);
    if (submodule)
      return submodule;
  }
  if (required)
    throw omnetpp::cRuntimeError("submodule not found");
  else
    return nullptr;
}

/**
 * Checks whether parent module is upward the hierarchy upon the child.
 * If some of parameters is nullptr, returns false.
 */
bool isParentOf(omnetpp::cModule *parent, omnetpp::cModule *child);

/**
 * Searches for the module that is closest to both m1 and m2 and which is
 * upward the hierarchy for both of them.
 */
omnetpp::cModule *getClosestCommonParent(
        omnetpp::cModule *m1, omnetpp::cModule *m2);

}

/**
 * Finds a module in the module tree, given by its absolute or relative path
 * defined by 'par' parameter.
 * Returns nullptr if the 'par' parameter is empty.
 * Returns the pointer to a module of type T or throws an error if module not
 * found or type mismatch.
 *
 * This function is taken from INET framework.
 */
template<typename T>
T *findModuleFromPar(omnetpp::cPar& par, omnetpp::cModule *from,
                     bool required = true)
{
  const char *path = par.stringValue();
  if (path && *path)
  {
    omnetpp::cModule *mod = from->getModuleByPath(path);
    if (!mod)
    {
      if (required) {
        throw omnetpp::cRuntimeError(
                "Module not found on path '%s' defined by par '%s'", path,
                par.getFullPath().c_str());
      } else {
        return nullptr;
      }
    }
    T *m = dynamic_cast<T *>(mod);
    if (!m) {
      throw omnetpp::cRuntimeError(
              "Module can not cast to '%s' on path '%s' defined by par '%s'",
              omnetpp::opp_typename(typeid(T)),
              path, par.getFullPath().c_str());
    }
    return m;
  }
  return nullptr;
}

#endif
