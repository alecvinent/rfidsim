#ifndef RFIDSIMPP_PROTOCOL_EPCSTD_DATA_TYPES_H_
#define RFIDSIMPP_PROTOCOL_EPCSTD_DATA_TYPES_H_

#include <protocol/epcstd-data-types_m.h>

namespace rfidsim {
namespace epcstd {

const char *str(CommandType v);
const char *str(TagEncoding v);
const char *str(DivideRatio v);
const char *str(Sel v);
const char *str(Session v);
const char *str(InventoryFlag v);
const char *str(MemoryBank v);

int getSymbolsPerBit(TagEncoding v);
double getDivideRatioValue(DivideRatio v);
double getBLF(omnetpp::simtime_t trcal, DivideRatio dr);
double getTagBitRate(omnetpp::simtime_t trcal, DivideRatio dr, TagEncoding m);

}}

#endif
