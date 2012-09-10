//
// Generated file, do not edit! Created by opp_msgc 4.2 from src/node/application/newSyncProtocol/syncPacket.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "syncPacket_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(syncPacket);

syncPacket::syncPacket(const char *name, int kind) : ApplicationPacket(name,kind)
{
    this->trsTime_var = 0;
    this->rcvTime_var = 0;
    this->Emax_var = 0;
    this->maxDrift_var = 0;
    this->Tsync_var = 0;
}

syncPacket::syncPacket(const syncPacket& other) : ApplicationPacket(other)
{
    copy(other);
}

syncPacket::~syncPacket()
{
}

syncPacket& syncPacket::operator=(const syncPacket& other)
{
    if (this==&other) return *this;
    ApplicationPacket::operator=(other);
    copy(other);
    return *this;
}

void syncPacket::copy(const syncPacket& other)
{
    this->trsTime_var = other.trsTime_var;
    this->rcvTime_var = other.rcvTime_var;
    this->Emax_var = other.Emax_var;
    this->maxDrift_var = other.maxDrift_var;
    this->Tsync_var = other.Tsync_var;
}

void syncPacket::parsimPack(cCommBuffer *b)
{
    ApplicationPacket::parsimPack(b);
    doPacking(b,this->trsTime_var);
    doPacking(b,this->rcvTime_var);
    doPacking(b,this->Emax_var);
    doPacking(b,this->maxDrift_var);
    doPacking(b,this->Tsync_var);
}

void syncPacket::parsimUnpack(cCommBuffer *b)
{
    ApplicationPacket::parsimUnpack(b);
    doUnpacking(b,this->trsTime_var);
    doUnpacking(b,this->rcvTime_var);
    doUnpacking(b,this->Emax_var);
    doUnpacking(b,this->maxDrift_var);
    doUnpacking(b,this->Tsync_var);
}

simtime_t syncPacket::getTrsTime() const
{
    return trsTime_var;
}

void syncPacket::setTrsTime(simtime_t trsTime)
{
    this->trsTime_var = trsTime;
}

simtime_t syncPacket::getRcvTime() const
{
    return rcvTime_var;
}

void syncPacket::setRcvTime(simtime_t rcvTime)
{
    this->rcvTime_var = rcvTime;
}

simtime_t syncPacket::getEmax() const
{
    return Emax_var;
}

void syncPacket::setEmax(simtime_t Emax)
{
    this->Emax_var = Emax;
}

double syncPacket::getMaxDrift() const
{
    return maxDrift_var;
}

void syncPacket::setMaxDrift(double maxDrift)
{
    this->maxDrift_var = maxDrift;
}

simtime_t syncPacket::getTsync() const
{
    return Tsync_var;
}

void syncPacket::setTsync(simtime_t Tsync)
{
    this->Tsync_var = Tsync;
}

class syncPacketDescriptor : public cClassDescriptor
{
  public:
    syncPacketDescriptor();
    virtual ~syncPacketDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(syncPacketDescriptor);

syncPacketDescriptor::syncPacketDescriptor() : cClassDescriptor("syncPacket", "ApplicationPacket")
{
}

syncPacketDescriptor::~syncPacketDescriptor()
{
}

bool syncPacketDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<syncPacket *>(obj)!=NULL;
}

const char *syncPacketDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int syncPacketDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount(object) : 5;
}

unsigned int syncPacketDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *syncPacketDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "trsTime",
        "rcvTime",
        "Emax",
        "maxDrift",
        "Tsync",
    };
    return (field>=0 && field<5) ? fieldNames[field] : NULL;
}

int syncPacketDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "trsTime")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "rcvTime")==0) return base+1;
    if (fieldName[0]=='E' && strcmp(fieldName, "Emax")==0) return base+2;
    if (fieldName[0]=='m' && strcmp(fieldName, "maxDrift")==0) return base+3;
    if (fieldName[0]=='T' && strcmp(fieldName, "Tsync")==0) return base+4;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *syncPacketDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "simtime_t",
        "simtime_t",
        "simtime_t",
        "double",
        "simtime_t",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : NULL;
}

const char *syncPacketDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int syncPacketDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    syncPacket *pp = (syncPacket *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string syncPacketDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    syncPacket *pp = (syncPacket *)object; (void)pp;
    switch (field) {
        case 0: return double2string(pp->getTrsTime());
        case 1: return double2string(pp->getRcvTime());
        case 2: return double2string(pp->getEmax());
        case 3: return double2string(pp->getMaxDrift());
        case 4: return double2string(pp->getTsync());
        default: return "";
    }
}

bool syncPacketDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    syncPacket *pp = (syncPacket *)object; (void)pp;
    switch (field) {
        case 0: pp->setTrsTime(string2double(value)); return true;
        case 1: pp->setRcvTime(string2double(value)); return true;
        case 2: pp->setEmax(string2double(value)); return true;
        case 3: pp->setMaxDrift(string2double(value)); return true;
        case 4: pp->setTsync(string2double(value)); return true;
        default: return false;
    }
}

const char *syncPacketDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<5) ? fieldStructNames[field] : NULL;
}

void *syncPacketDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    syncPacket *pp = (syncPacket *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


