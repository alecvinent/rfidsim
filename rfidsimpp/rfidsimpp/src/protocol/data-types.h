#ifndef RFIDSIMPP_PROTOCOL_DATA_TYPES_H_
#define RFIDSIMPP_PROTOCOL_DATA_TYPES_H_

#include <protocol/data-types_m.h>

namespace rfidsim {

const char *strTagEncoding(TagEncoding v);
int getSymbolsPerBit(TagEncoding v);

const char *strDivideRatio(DivideRatio v);
double getDivideRationValue(DivideRatio v);

double getBLF(omnetpp::simtime_t trcal, DivideRatio dr);

double getTagBitRate(omnetpp::simtime_t trcal, DivideRatio dr, TagEncoding m);

}

#endif
