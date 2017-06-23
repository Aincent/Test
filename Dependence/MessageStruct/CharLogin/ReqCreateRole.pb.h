// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: CharLogin/ReqCreateRole.proto

#ifndef PROTOBUF_CharLogin_2fReqCreateRole_2eproto__INCLUDED
#define PROTOBUF_CharLogin_2fReqCreateRole_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2005000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace CreateCharator {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_CharLogin_2fReqCreateRole_2eproto();
void protobuf_AssignDesc_CharLogin_2fReqCreateRole_2eproto();
void protobuf_ShutdownFile_CharLogin_2fReqCreateRole_2eproto();

class DBCreateCharator;

// ===================================================================

class DBCreateCharator : public ::google::protobuf::Message {
 public:
  DBCreateCharator();
  virtual ~DBCreateCharator();

  DBCreateCharator(const DBCreateCharator& from);

  inline DBCreateCharator& operator=(const DBCreateCharator& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const DBCreateCharator& default_instance();

  void Swap(DBCreateCharator* other);

  // implements Message ----------------------------------------------

  DBCreateCharator* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const DBCreateCharator& from);
  void MergeFrom(const DBCreateCharator& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:

  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required string openID = 1;
  inline bool has_openid() const;
  inline void clear_openid();
  static const int kOpenIDFieldNumber = 1;
  inline const ::std::string& openid() const;
  inline void set_openid(const ::std::string& value);
  inline void set_openid(const char* value);
  inline void set_openid(const char* value, size_t size);
  inline ::std::string* mutable_openid();
  inline ::std::string* release_openid();
  inline void set_allocated_openid(::std::string* openid);

  // required string name = 2;
  inline bool has_name() const;
  inline void clear_name();
  static const int kNameFieldNumber = 2;
  inline const ::std::string& name() const;
  inline void set_name(const ::std::string& value);
  inline void set_name(const char* value);
  inline void set_name(const char* value, size_t size);
  inline ::std::string* mutable_name();
  inline ::std::string* release_name();
  inline void set_allocated_name(::std::string* name);

  // required int32 head = 3;
  inline bool has_head() const;
  inline void clear_head();
  static const int kHeadFieldNumber = 3;
  inline ::google::protobuf::int32 head() const;
  inline void set_head(::google::protobuf::int32 value);

  // required int64 mapid = 4;
  inline bool has_mapid() const;
  inline void clear_mapid();
  static const int kMapidFieldNumber = 4;
  inline ::google::protobuf::int64 mapid() const;
  inline void set_mapid(::google::protobuf::int64 value);

  // optional string openKey = 5;
  inline bool has_openkey() const;
  inline void clear_openkey();
  static const int kOpenKeyFieldNumber = 5;
  inline const ::std::string& openkey() const;
  inline void set_openkey(const ::std::string& value);
  inline void set_openkey(const char* value);
  inline void set_openkey(const char* value, size_t size);
  inline ::std::string* mutable_openkey();
  inline ::std::string* release_openkey();
  inline void set_allocated_openkey(::std::string* openkey);

  // optional string pf = 6;
  inline bool has_pf() const;
  inline void clear_pf();
  static const int kPfFieldNumber = 6;
  inline const ::std::string& pf() const;
  inline void set_pf(const ::std::string& value);
  inline void set_pf(const char* value);
  inline void set_pf(const char* value, size_t size);
  inline ::std::string* mutable_pf();
  inline ::std::string* release_pf();
  inline void set_allocated_pf(::std::string* pf);

  // optional string pfKey = 7;
  inline bool has_pfkey() const;
  inline void clear_pfkey();
  static const int kPfKeyFieldNumber = 7;
  inline const ::std::string& pfkey() const;
  inline void set_pfkey(const ::std::string& value);
  inline void set_pfkey(const char* value);
  inline void set_pfkey(const char* value, size_t size);
  inline ::std::string* mutable_pfkey();
  inline ::std::string* release_pfkey();
  inline void set_allocated_pfkey(::std::string* pfkey);

  // optional int32 serverId = 8;
  inline bool has_serverid() const;
  inline void clear_serverid();
  static const int kServerIdFieldNumber = 8;
  inline ::google::protobuf::int32 serverid() const;
  inline void set_serverid(::google::protobuf::int32 value);

  // optional int32 clientType = 9;
  inline bool has_clienttype() const;
  inline void clear_clienttype();
  static const int kClientTypeFieldNumber = 9;
  inline ::google::protobuf::int32 clienttype() const;
  inline void set_clienttype(::google::protobuf::int32 value);

  // optional string source = 10;
  inline bool has_source() const;
  inline void clear_source();
  static const int kSourceFieldNumber = 10;
  inline const ::std::string& source() const;
  inline void set_source(const ::std::string& value);
  inline void set_source(const char* value);
  inline void set_source(const char* value, size_t size);
  inline ::std::string* mutable_source();
  inline ::std::string* release_source();
  inline void set_allocated_source(::std::string* source);

  // @@protoc_insertion_point(class_scope:CreateCharator.DBCreateCharator)
 private:
  inline void set_has_openid();
  inline void clear_has_openid();
  inline void set_has_name();
  inline void clear_has_name();
  inline void set_has_head();
  inline void clear_has_head();
  inline void set_has_mapid();
  inline void clear_has_mapid();
  inline void set_has_openkey();
  inline void clear_has_openkey();
  inline void set_has_pf();
  inline void clear_has_pf();
  inline void set_has_pfkey();
  inline void clear_has_pfkey();
  inline void set_has_serverid();
  inline void clear_has_serverid();
  inline void set_has_clienttype();
  inline void clear_has_clienttype();
  inline void set_has_source();
  inline void clear_has_source();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* openid_;
  ::std::string* name_;
  ::google::protobuf::int64 mapid_;
  ::std::string* openkey_;
  ::google::protobuf::int32 head_;
  ::google::protobuf::int32 serverid_;
  ::std::string* pf_;
  ::std::string* pfkey_;
  ::std::string* source_;
  ::google::protobuf::int32 clienttype_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(10 + 31) / 32];

  friend void  protobuf_AddDesc_CharLogin_2fReqCreateRole_2eproto();
  friend void protobuf_AssignDesc_CharLogin_2fReqCreateRole_2eproto();
  friend void protobuf_ShutdownFile_CharLogin_2fReqCreateRole_2eproto();

  void InitAsDefaultInstance();
  static DBCreateCharator* default_instance_;
};
// ===================================================================


// ===================================================================

// DBCreateCharator

// required string openID = 1;
inline bool DBCreateCharator::has_openid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void DBCreateCharator::set_has_openid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void DBCreateCharator::clear_has_openid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void DBCreateCharator::clear_openid() {
  if (openid_ != &::google::protobuf::internal::kEmptyString) {
    openid_->clear();
  }
  clear_has_openid();
}
inline const ::std::string& DBCreateCharator::openid() const {
  return *openid_;
}
inline void DBCreateCharator::set_openid(const ::std::string& value) {
  set_has_openid();
  if (openid_ == &::google::protobuf::internal::kEmptyString) {
    openid_ = new ::std::string;
  }
  openid_->assign(value);
}
inline void DBCreateCharator::set_openid(const char* value) {
  set_has_openid();
  if (openid_ == &::google::protobuf::internal::kEmptyString) {
    openid_ = new ::std::string;
  }
  openid_->assign(value);
}
inline void DBCreateCharator::set_openid(const char* value, size_t size) {
  set_has_openid();
  if (openid_ == &::google::protobuf::internal::kEmptyString) {
    openid_ = new ::std::string;
  }
  openid_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DBCreateCharator::mutable_openid() {
  set_has_openid();
  if (openid_ == &::google::protobuf::internal::kEmptyString) {
    openid_ = new ::std::string;
  }
  return openid_;
}
inline ::std::string* DBCreateCharator::release_openid() {
  clear_has_openid();
  if (openid_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = openid_;
    openid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void DBCreateCharator::set_allocated_openid(::std::string* openid) {
  if (openid_ != &::google::protobuf::internal::kEmptyString) {
    delete openid_;
  }
  if (openid) {
    set_has_openid();
    openid_ = openid;
  } else {
    clear_has_openid();
    openid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required string name = 2;
inline bool DBCreateCharator::has_name() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void DBCreateCharator::set_has_name() {
  _has_bits_[0] |= 0x00000002u;
}
inline void DBCreateCharator::clear_has_name() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void DBCreateCharator::clear_name() {
  if (name_ != &::google::protobuf::internal::kEmptyString) {
    name_->clear();
  }
  clear_has_name();
}
inline const ::std::string& DBCreateCharator::name() const {
  return *name_;
}
inline void DBCreateCharator::set_name(const ::std::string& value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void DBCreateCharator::set_name(const char* value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void DBCreateCharator::set_name(const char* value, size_t size) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DBCreateCharator::mutable_name() {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  return name_;
}
inline ::std::string* DBCreateCharator::release_name() {
  clear_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = name_;
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void DBCreateCharator::set_allocated_name(::std::string* name) {
  if (name_ != &::google::protobuf::internal::kEmptyString) {
    delete name_;
  }
  if (name) {
    set_has_name();
    name_ = name;
  } else {
    clear_has_name();
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required int32 head = 3;
inline bool DBCreateCharator::has_head() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void DBCreateCharator::set_has_head() {
  _has_bits_[0] |= 0x00000004u;
}
inline void DBCreateCharator::clear_has_head() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void DBCreateCharator::clear_head() {
  head_ = 0;
  clear_has_head();
}
inline ::google::protobuf::int32 DBCreateCharator::head() const {
  return head_;
}
inline void DBCreateCharator::set_head(::google::protobuf::int32 value) {
  set_has_head();
  head_ = value;
}

// required int64 mapid = 4;
inline bool DBCreateCharator::has_mapid() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void DBCreateCharator::set_has_mapid() {
  _has_bits_[0] |= 0x00000008u;
}
inline void DBCreateCharator::clear_has_mapid() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void DBCreateCharator::clear_mapid() {
  mapid_ = GOOGLE_LONGLONG(0);
  clear_has_mapid();
}
inline ::google::protobuf::int64 DBCreateCharator::mapid() const {
  return mapid_;
}
inline void DBCreateCharator::set_mapid(::google::protobuf::int64 value) {
  set_has_mapid();
  mapid_ = value;
}

// optional string openKey = 5;
inline bool DBCreateCharator::has_openkey() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void DBCreateCharator::set_has_openkey() {
  _has_bits_[0] |= 0x00000010u;
}
inline void DBCreateCharator::clear_has_openkey() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void DBCreateCharator::clear_openkey() {
  if (openkey_ != &::google::protobuf::internal::kEmptyString) {
    openkey_->clear();
  }
  clear_has_openkey();
}
inline const ::std::string& DBCreateCharator::openkey() const {
  return *openkey_;
}
inline void DBCreateCharator::set_openkey(const ::std::string& value) {
  set_has_openkey();
  if (openkey_ == &::google::protobuf::internal::kEmptyString) {
    openkey_ = new ::std::string;
  }
  openkey_->assign(value);
}
inline void DBCreateCharator::set_openkey(const char* value) {
  set_has_openkey();
  if (openkey_ == &::google::protobuf::internal::kEmptyString) {
    openkey_ = new ::std::string;
  }
  openkey_->assign(value);
}
inline void DBCreateCharator::set_openkey(const char* value, size_t size) {
  set_has_openkey();
  if (openkey_ == &::google::protobuf::internal::kEmptyString) {
    openkey_ = new ::std::string;
  }
  openkey_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DBCreateCharator::mutable_openkey() {
  set_has_openkey();
  if (openkey_ == &::google::protobuf::internal::kEmptyString) {
    openkey_ = new ::std::string;
  }
  return openkey_;
}
inline ::std::string* DBCreateCharator::release_openkey() {
  clear_has_openkey();
  if (openkey_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = openkey_;
    openkey_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void DBCreateCharator::set_allocated_openkey(::std::string* openkey) {
  if (openkey_ != &::google::protobuf::internal::kEmptyString) {
    delete openkey_;
  }
  if (openkey) {
    set_has_openkey();
    openkey_ = openkey;
  } else {
    clear_has_openkey();
    openkey_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional string pf = 6;
inline bool DBCreateCharator::has_pf() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void DBCreateCharator::set_has_pf() {
  _has_bits_[0] |= 0x00000020u;
}
inline void DBCreateCharator::clear_has_pf() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void DBCreateCharator::clear_pf() {
  if (pf_ != &::google::protobuf::internal::kEmptyString) {
    pf_->clear();
  }
  clear_has_pf();
}
inline const ::std::string& DBCreateCharator::pf() const {
  return *pf_;
}
inline void DBCreateCharator::set_pf(const ::std::string& value) {
  set_has_pf();
  if (pf_ == &::google::protobuf::internal::kEmptyString) {
    pf_ = new ::std::string;
  }
  pf_->assign(value);
}
inline void DBCreateCharator::set_pf(const char* value) {
  set_has_pf();
  if (pf_ == &::google::protobuf::internal::kEmptyString) {
    pf_ = new ::std::string;
  }
  pf_->assign(value);
}
inline void DBCreateCharator::set_pf(const char* value, size_t size) {
  set_has_pf();
  if (pf_ == &::google::protobuf::internal::kEmptyString) {
    pf_ = new ::std::string;
  }
  pf_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DBCreateCharator::mutable_pf() {
  set_has_pf();
  if (pf_ == &::google::protobuf::internal::kEmptyString) {
    pf_ = new ::std::string;
  }
  return pf_;
}
inline ::std::string* DBCreateCharator::release_pf() {
  clear_has_pf();
  if (pf_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = pf_;
    pf_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void DBCreateCharator::set_allocated_pf(::std::string* pf) {
  if (pf_ != &::google::protobuf::internal::kEmptyString) {
    delete pf_;
  }
  if (pf) {
    set_has_pf();
    pf_ = pf;
  } else {
    clear_has_pf();
    pf_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional string pfKey = 7;
inline bool DBCreateCharator::has_pfkey() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void DBCreateCharator::set_has_pfkey() {
  _has_bits_[0] |= 0x00000040u;
}
inline void DBCreateCharator::clear_has_pfkey() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void DBCreateCharator::clear_pfkey() {
  if (pfkey_ != &::google::protobuf::internal::kEmptyString) {
    pfkey_->clear();
  }
  clear_has_pfkey();
}
inline const ::std::string& DBCreateCharator::pfkey() const {
  return *pfkey_;
}
inline void DBCreateCharator::set_pfkey(const ::std::string& value) {
  set_has_pfkey();
  if (pfkey_ == &::google::protobuf::internal::kEmptyString) {
    pfkey_ = new ::std::string;
  }
  pfkey_->assign(value);
}
inline void DBCreateCharator::set_pfkey(const char* value) {
  set_has_pfkey();
  if (pfkey_ == &::google::protobuf::internal::kEmptyString) {
    pfkey_ = new ::std::string;
  }
  pfkey_->assign(value);
}
inline void DBCreateCharator::set_pfkey(const char* value, size_t size) {
  set_has_pfkey();
  if (pfkey_ == &::google::protobuf::internal::kEmptyString) {
    pfkey_ = new ::std::string;
  }
  pfkey_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DBCreateCharator::mutable_pfkey() {
  set_has_pfkey();
  if (pfkey_ == &::google::protobuf::internal::kEmptyString) {
    pfkey_ = new ::std::string;
  }
  return pfkey_;
}
inline ::std::string* DBCreateCharator::release_pfkey() {
  clear_has_pfkey();
  if (pfkey_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = pfkey_;
    pfkey_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void DBCreateCharator::set_allocated_pfkey(::std::string* pfkey) {
  if (pfkey_ != &::google::protobuf::internal::kEmptyString) {
    delete pfkey_;
  }
  if (pfkey) {
    set_has_pfkey();
    pfkey_ = pfkey;
  } else {
    clear_has_pfkey();
    pfkey_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional int32 serverId = 8;
inline bool DBCreateCharator::has_serverid() const {
  return (_has_bits_[0] & 0x00000080u) != 0;
}
inline void DBCreateCharator::set_has_serverid() {
  _has_bits_[0] |= 0x00000080u;
}
inline void DBCreateCharator::clear_has_serverid() {
  _has_bits_[0] &= ~0x00000080u;
}
inline void DBCreateCharator::clear_serverid() {
  serverid_ = 0;
  clear_has_serverid();
}
inline ::google::protobuf::int32 DBCreateCharator::serverid() const {
  return serverid_;
}
inline void DBCreateCharator::set_serverid(::google::protobuf::int32 value) {
  set_has_serverid();
  serverid_ = value;
}

// optional int32 clientType = 9;
inline bool DBCreateCharator::has_clienttype() const {
  return (_has_bits_[0] & 0x00000100u) != 0;
}
inline void DBCreateCharator::set_has_clienttype() {
  _has_bits_[0] |= 0x00000100u;
}
inline void DBCreateCharator::clear_has_clienttype() {
  _has_bits_[0] &= ~0x00000100u;
}
inline void DBCreateCharator::clear_clienttype() {
  clienttype_ = 0;
  clear_has_clienttype();
}
inline ::google::protobuf::int32 DBCreateCharator::clienttype() const {
  return clienttype_;
}
inline void DBCreateCharator::set_clienttype(::google::protobuf::int32 value) {
  set_has_clienttype();
  clienttype_ = value;
}

// optional string source = 10;
inline bool DBCreateCharator::has_source() const {
  return (_has_bits_[0] & 0x00000200u) != 0;
}
inline void DBCreateCharator::set_has_source() {
  _has_bits_[0] |= 0x00000200u;
}
inline void DBCreateCharator::clear_has_source() {
  _has_bits_[0] &= ~0x00000200u;
}
inline void DBCreateCharator::clear_source() {
  if (source_ != &::google::protobuf::internal::kEmptyString) {
    source_->clear();
  }
  clear_has_source();
}
inline const ::std::string& DBCreateCharator::source() const {
  return *source_;
}
inline void DBCreateCharator::set_source(const ::std::string& value) {
  set_has_source();
  if (source_ == &::google::protobuf::internal::kEmptyString) {
    source_ = new ::std::string;
  }
  source_->assign(value);
}
inline void DBCreateCharator::set_source(const char* value) {
  set_has_source();
  if (source_ == &::google::protobuf::internal::kEmptyString) {
    source_ = new ::std::string;
  }
  source_->assign(value);
}
inline void DBCreateCharator::set_source(const char* value, size_t size) {
  set_has_source();
  if (source_ == &::google::protobuf::internal::kEmptyString) {
    source_ = new ::std::string;
  }
  source_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* DBCreateCharator::mutable_source() {
  set_has_source();
  if (source_ == &::google::protobuf::internal::kEmptyString) {
    source_ = new ::std::string;
  }
  return source_;
}
inline ::std::string* DBCreateCharator::release_source() {
  clear_has_source();
  if (source_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = source_;
    source_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void DBCreateCharator::set_allocated_source(::std::string* source) {
  if (source_ != &::google::protobuf::internal::kEmptyString) {
    delete source_;
  }
  if (source) {
    set_has_source();
    source_ = source;
  } else {
    clear_has_source();
    source_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace CreateCharator

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_CharLogin_2fReqCreateRole_2eproto__INCLUDED
