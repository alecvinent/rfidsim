#include <logic/reader-logic-layer-signals.h>
#include <common/functions.h>

using namespace omnetpp;

namespace rfidsim {

Register_Class(TagReadData);
Register_Class(NewRound);
Register_Class(NewSlot);

const char *TagReadData::NAME = "TagReadData";
const char *NewRound::NAME = "NewRound";
const char *NewSlot::NAME = "NewSlot";

std::string TagReadData::info() const
{
  char buf[256];
  snprintf(buf, sizeof(buf),
           "%s {ReaderID=%d, TagID=%d, EPC=%s, TID=%s, Round=%lu, Slot=%u}",
           NAME, reader_id, tag_id, (epc ? toHexString(*epc).c_str() : "-"),
           (tid ? toHexString(*tid).c_str() : "-"), round_index, slot_index);
  return buf;
}

std::string NewRound::info() const
{
  char buf[64];
  snprintf(buf, sizeof(buf), "%s {ReaderID=%d, Round=%lu, SlotsNum=%u}", NAME,
           reader_id, round_index, slots_number);
  return buf;
}

std::string NewSlot::info() const
{
  char buf[64];
  snprintf(buf, sizeof(buf), "%s {ReaderID=%d, Round=%lu, Slot=%u}", NAME,
           reader_id, round_index, slot_index);
  return buf;
}

}
