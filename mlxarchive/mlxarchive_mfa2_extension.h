/*                  - Mellanox Confidential and Proprietary -
 *
 *  Copyright (C) Jan 2013, Mellanox Technologies Ltd.  ALL RIGHTS RESERVED.
 *
 *  Except as specifically permitted herein, no portion of the information,
 *  including but not limited to object code and source code, may be reproduced,
 *  modified, distributed, republished or otherwise exploited in any form or by
 *  any means for any purpose without the prior written permission of Mellanox
 *  Technologies Ltd. Use of software subject to the terms and conditions
 *  detailed in the file "LICENSE.txt".
 *
 * mlxarchive_mfa2_extension.h
 *
 *  Created on: March 23, 2017
 *      Author: Ahmad Soboh
 */

#ifndef MLXARCHIVE_MFA2_EXTENSION_H_
#define MLXARCHIVE_MFA2_EXTENSION_H_

#include <string>
#include <vector>

#include <compatibility.h>
#include <tools_layouts/tools_open_layouts.h>

#include "mlxarchive_mfa2_element.h"

using namespace std;

namespace mfa2 {
class Extension : protected Element{
protected:
    enum ExtensionType {
        VersionExtensionType,
        SecurityInfoExtensionType,
        ComponentPointerExtensionType,
        SHA256ExtensionType,
        DescriptorsSHA256ExtensionType,
        CommentExtensionType,
        PSIDExtensionType,
        SourceFileNameExtensionType
    };

    MFA2Type extensionTypeToMFA2Type(ExtensionType type);

protected:
    /*class Digest {
    protected:
        vector<u_int8_t> _digest;
        const static u_int32_t _length = 0x20;
    public:
        void setDigest(vector<u_int8_t> digest);
    };*/

public:
    Extension(u_int8_t vesrion, ExtensionType type, u_int32_t length);

    virtual void pack(vector<u_int8_t>& buff) const = 0;

    virtual ~Extension  () {};
};

class VersionExtension : Extension {
private:
    u_int8_t _major;
    u_int8_t _subMinor;
    u_int16_t _minor;
    u_int8_t _day;
    u_int8_t _month;
    u_int16_t _year;
public:
    const static u_int8_t  ELEMENT_VERSION = 0x0;
    const static u_int32_t LENGTH  = TOOLS_OPEN_VERSION_SIZE;

    VersionExtension(const string& version, const string& date);
    VersionExtension(const u_int16_t* version, const u_int16_t* date);
    void pack(vector<u_int8_t>& buff) const;
};

/*class SecurityInfoExtension : Extension {

private:
    string _uuid;
    SecurityMode _securityMode;

public:
    SecurityInfoExtension(string uuid, SecurityMode mode);

};*/

class ComponentPointerExtension : Extension {
private:
    u_int16_t _componentIndex;
    u_int8_t  _storageId; //TODO: use enums
    u_int32_t _storageAddress;
public:
    const static u_int8_t  ELEMENT_VERSION = 0x0;
    const static u_int32_t LENGTH  = TOOLS_OPEN_COMPONENT_PTR_SIZE;

    explicit ComponentPointerExtension(u_int16_t componentIndex) :
                Extension(ELEMENT_VERSION, ComponentPointerExtensionType, LENGTH),
                _componentIndex(componentIndex),
                _storageId(0x1),
                _storageAddress(0x0){};

    void pack(vector<u_int8_t>& buff) const;
};

class SHA256Extension : Extension {
public:
    enum SHA256Scope {
        SHA256Scope_Descriptors,
        SHA256Scope_All
    };
    const static u_int8_t  ELEMENT_VERSION = 0x0;
    const static u_int32_t LENGTH  = 0x20;

    explicit SHA256Extension(enum SHA256Scope scope);
    void setDigest(vector<u_int8_t> digest);
    void pack(vector<u_int8_t>& buff) const;
private:
    vector<u_int8_t> _digest;

    ExtensionType scopeToExtensionType(enum SHA256Scope);
};

/*class DescriptorsSHA256Extension : Extension, Extension::Digest {
private:
    const static u_int8_t  ELEMENT_VERSION = 0x0;
    const static u_int32_t LENGTH  = Extension::Digest::_length;
public:
    DescriptorsSHA256Extension() :
        Extension(ELEMENT_VERSION, DescriptorsSHA256ExtensionType, LENGTH) {};
    void pack(vector<u_int8_t>& buff) const;
};*/

class StringExtension : public Extension {
private:
    string _str;
public:
    StringExtension(u_int8_t vesrion, ExtensionType type, string str) :
        Extension(vesrion, type, str.length()),
        _str(str) {};
    u_int32_t length() const;
    void pack(vector<u_int8_t>& buff) const;
};

inline u_int32_t StringExtension::length() const
{
    return _str.length();
}

class CommentExtension : public StringExtension {
private:
    const static u_int8_t  ELEMENT_VERSION = 0x0;
public:
    explicit CommentExtension(string comment) :
        StringExtension(ELEMENT_VERSION, CommentExtensionType, comment) {};
};

class PSIDExtension : public StringExtension {
private:
    const static u_int8_t  ELEMENT_VERSION = 0x0;
public:
    explicit PSIDExtension(string psid) :
        StringExtension(ELEMENT_VERSION, PSIDExtensionType, psid) {};
};

class SourceFileNameExtension : public StringExtension {
private:
    const static u_int8_t  ELEMENT_VERSION = 0x0;
public:
    explicit SourceFileNameExtension(string src) :
        StringExtension(ELEMENT_VERSION, SourceFileNameExtensionType, src) {};
};
}
#endif