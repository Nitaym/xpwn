#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "dmg.h"

void flipUDIFChecksum(UDIFChecksum* o, char out) {
  int i;

  FLIPENDIAN(o->type);
  
  if(out) {
    for(i = 0; i < o->size; i++) {
      FLIPENDIAN(o->data[i]);
    }
    FLIPENDIAN(o->size);
  } else {
    FLIPENDIAN(o->size);
    for(i = 0; i < o->size; i++) {
      FLIPENDIAN(o->data[i]);
    }
  }
}

void readUDIFChecksum(FILE* file, UDIFChecksum* o) {
  int i;
  
  o->type = readUInt32(file);
  o->size = readUInt32(file);
  
  ASSERT(o->size == 0x20, "readUDIFChecksum");
  
  for(i = 0; i < o->size; i++) {
    o->data[i] = readUInt32(file);
  }
}

void writeUDIFChecksum(FILE* file, UDIFChecksum* o) {
  int i;
  
  writeUInt32(file, o->type);
  writeUInt32(file, o->size);  
  
  for(i = 0; i < o->size; i++) {
    writeUInt32(file, o->data[i]);
  }
}

void readUDIFID(FILE* file, UDIFID* o) {
  o->data4 = readUInt32(file); FLIPENDIAN(o->data4);
  o->data3 = readUInt32(file); FLIPENDIAN(o->data3);
  o->data2 = readUInt32(file); FLIPENDIAN(o->data2);
  o->data1 = readUInt32(file); FLIPENDIAN(o->data1);
}

void writeUDIFID(FILE* file, UDIFID* o) {
  FLIPENDIAN(o->data4); writeUInt32(file, o->data4); FLIPENDIAN(o->data4);
  FLIPENDIAN(o->data3); writeUInt32(file, o->data3); FLIPENDIAN(o->data3);
  FLIPENDIAN(o->data2); writeUInt32(file, o->data2); FLIPENDIAN(o->data2);
  FLIPENDIAN(o->data1); writeUInt32(file, o->data1); FLIPENDIAN(o->data1);
}

void readUDIFResourceFile(FILE* file, UDIFResourceFile* o) {
  o->fUDIFSignature = readUInt32(file);
  o->fUDIFVersion = readUInt32(file);
  o->fUDIFHeaderSize = readUInt32(file);
  o->fUDIFFlags = readUInt32(file);
  
  o->fUDIFRunningDataForkOffset = readUInt64(file);
  o->fUDIFDataForkOffset = readUInt64(file);
  o->fUDIFDataForkLength = readUInt64(file);
  o->fUDIFRsrcForkOffset = readUInt64(file);
  o->fUDIFRsrcForkLength = readUInt64(file);
  
  o->fUDIFSegmentNumber = readUInt32(file);
  o->fUDIFSegmentCount = readUInt32(file);
  readUDIFID(file, &(o->fUDIFSegmentID));
  
  readUDIFChecksum(file, &(o->fUDIFDataForkChecksum));
  
  o->fUDIFXMLOffset = readUInt64(file);
  o->fUDIFXMLLength = readUInt64(file);
  
  ASSERT(fread(&(o->reserved1), 0x78, 1, file) == 1, "fread");
  
  readUDIFChecksum(file, &(o->fUDIFMasterChecksum));
  
  o->fUDIFImageVariant = readUInt32(file);
  o->fUDIFSectorCount = readUInt64(file);
  
  o->reserved2 = readUInt32(file);
  o->reserved3 = readUInt32(file);
  o->reserved4 = readUInt32(file);
}

void writeUDIFResourceFile(FILE* file, UDIFResourceFile* o) {
  writeUInt32(file, o->fUDIFSignature);
  writeUInt32(file, o->fUDIFVersion);
  writeUInt32(file, o->fUDIFHeaderSize);
  writeUInt32(file, o->fUDIFFlags);
  
  writeUInt64(file, o->fUDIFRunningDataForkOffset);
  writeUInt64(file, o->fUDIFDataForkOffset);
  writeUInt64(file, o->fUDIFDataForkLength);
  writeUInt64(file, o->fUDIFRsrcForkOffset);
  writeUInt64(file, o->fUDIFRsrcForkLength);
  
  writeUInt32(file, o->fUDIFSegmentNumber);
  writeUInt32(file, o->fUDIFSegmentCount);
  writeUDIFID(file, &(o->fUDIFSegmentID));
  
  writeUDIFChecksum(file, &(o->fUDIFDataForkChecksum));
  
  writeUInt64(file, o->fUDIFXMLOffset);
  writeUInt64(file, o->fUDIFXMLLength);
  
  ASSERT(fwrite(&(o->reserved1), 0x78, 1, file) == 1, "fwrite");
  
  writeUDIFChecksum(file, &(o->fUDIFMasterChecksum));
  
  writeUInt32(file, o->fUDIFImageVariant);
  writeUInt64(file, o->fUDIFSectorCount);
  
  writeUInt32(file, o->reserved2);
  writeUInt32(file, o->reserved3);
  writeUInt32(file, o->reserved4);
}
