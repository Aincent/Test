// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: AntiAddiction/AntiAddictionProto.proto

#ifndef PROTOBUF_AntiAddiction_2fAntiAddictionProto_2eproto__INCLUDED
#define PROTOBUF_AntiAddiction_2fAntiAddictionProto_2eproto__INCLUDED

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

namespace AntiAddictionProto {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_AntiAddiction_2fAntiAddictionProto_2eproto();
void protobuf_AssignDesc_AntiAddiction_2fAntiAddictionProto_2eproto();
void protobuf_ShutdownFile_AntiAddiction_2fAntiAddictionProto_2eproto();

class ReqBindAntiAddictionID;
class RetBindAntiAddictionID;
class ToDBBindAntiAddictionID;

// ===================================================================

class ReqBindAntiAddictionID : public ::google::protobuf::Message {
 public:
  ReqBindAntiAddictionID();
  virtual ~ReqBindAntiAddictionID();

  ReqBindAntiAddictionID(const ReqBindAntiAddictionID& from);

  inline ReqBindAntiAddictionID& operator=(const ReqBindAntiAddictionID& from) {
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
  static const ReqBindAntiAddictionID& default_instance();

  void Swap(ReqBindAntiAddictionID* other);

  // implements Message ----------------------------------------------

  ReqBindAntiAddictionID* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ReqBindAntiAddictionID& from);
  void MergeFrom(const ReqBindAntiAddictionID& from);
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

  // required string realname = 1;
  inline bool has_realname() const;
  inline void clear_realname();
  static const int kRealnameFieldNumber = 1;
  inline const ::std::string& realname() const;
  inline void set_realname(const ::std::string& value);
  inline void set_realname(const char* value);
  inline void set_realname(const char* value, size_t size);
  inline ::std::string* mutable_realname();
  inline ::std::string* release_realname();
  inline void set_allocated_realname(::std::string* realname);

  // required string antiaddictionid = 2;
  inline bool has_antiaddictionid() const;
  inline void clear_antiaddictionid();
  static const int kAntiaddictionidFieldNumber = 2;
  inline const ::std::string& antiaddictionid() const;
  inline void set_antiaddictionid(const ::std::string& value);
  inline void set_antiaddictionid(const char* value);
  inline void set_antiaddictionid(const char* value, size_t size);
  inline ::std::string* mutable_antiaddictionid();
  inline ::std::string* release_antiaddictionid();
  inline void set_allocated_antiaddictionid(::std::string* antiaddictionid);

  // @@protoc_insertion_point(class_scope:AntiAddictionProto.ReqBindAntiAddictionID)
 private:
  inline void set_has_realname();
  inline void clear_has_realname();
  inline void set_has_antiaddictionid();
  inline void clear_has_antiaddictionid();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* realname_;
  ::std::string* antiaddictionid_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_AntiAddiction_2fAntiAddictionProto_2eproto();
  friend void protobuf_AssignDesc_AntiAddiction_2fAntiAddictionProto_2eproto();
  friend void protobuf_ShutdownFile_AntiAddiction_2fAntiAddictionProto_2eproto();

  void InitAsDefaultInstance();
  static ReqBindAntiAddictionID* default_instance_;
};
// -------------------------------------------------------------------

class RetBindAntiAddictionID : public ::google::protobuf::Message {
 public:
  RetBindAntiAddictionID();
  virtual ~RetBindAntiAddictionID();

  RetBindAntiAddictionID(const RetBindAntiAddictionID& from);

  inline RetBindAntiAddictionID& operator=(const RetBindAntiAddictionID& from) {
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
  static const RetBindAntiAddictionID& default_instance();

  void Swap(RetBindAntiAddictionID* other);

  // implements Message ----------------------------------------------

  RetBindAntiAddictionID* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const RetBindAntiAddictionID& from);
  void MergeFrom(const RetBindAntiAddictionID& from);
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

  // required int32 error = 1;
  inline bool has_error() const;
  inline void clear_error();
  static const int kErrorFieldNumber = 1;
  inline ::google::protobuf::int32 error() const;
  inline void set_error(::google::protobuf::int32 value);

  // optional int32 state = 2;
  inline bool has_state() const;
  inline void clear_state();
  static const int kStateFieldNumber = 2;
  inline ::google::protobuf::int32 state() const;
  inline void set_state(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:AntiAddictionProto.RetBindAntiAddictionID)
 private:
  inline void set_has_error();
  inline void clear_has_error();
  inline void set_has_state();
  inline void clear_has_state();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 error_;
  ::google::protobuf::int32 state_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_AntiAddiction_2fAntiAddictionProto_2eproto();
  friend void protobuf_AssignDesc_AntiAddiction_2fAntiAddictionProto_2eproto();
  friend void protobuf_ShutdownFile_AntiAddiction_2fAntiAddictionProto_2eproto();

  void InitAsDefaultInstance();
  static RetBindAntiAddictionID* default_instance_;
};
// -------------------------------------------------------------------

class ToDBBindAntiAddictionID : public ::google::protobuf::Message {
 public:
  ToDBBindAntiAddictionID();
  virtual ~ToDBBindAntiAddictionID();

  ToDBBindAntiAddictionID(const ToDBBindAntiAddictionID& from);

  inline ToDBBindAntiAddictionID& operator=(const ToDBBindAntiAddictionID& from) {
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
  static const ToDBBindAntiAddictionID& default_instance();

  void Swap(ToDBBindAntiAddictionID* other);

  // implements Message ----------------------------------------------

  ToDBBindAntiAddictionID* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ToDBBindAntiAddictionID& from);
  void MergeFrom(const ToDBBindAntiAddictionID& from);
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

  // required int64 charid = 1;
  inline bool has_charid() const;
  inline void clear_charid();
  static const int kCharidFieldNumber = 1;
  inline ::google::protobuf::int64 charid() const;
  inline void set_charid(::google::protobuf::int64 value);

  // required int32 antiaddictionstate = 2;
  inline bool has_antiaddictionstate() const;
  inline void clear_antiaddictionstate();
  static const int kAntiaddictionstateFieldNumber = 2;
  inline ::google::protobuf::int32 antiaddictionstate() const;
  inline void set_antiaddictionstate(::google::protobuf::int32 value);

  // required string realname = 3;
  inline bool has_realname() const;
  inline void clear_realname();
  static const int kRealnameFieldNumber = 3;
  inline const ::std::string& realname() const;
  inline void set_realname(const ::std::string& value);
  inline void set_realname(const char* value);
  inline void set_realname(const char* value, size_t size);
  inline ::std::string* mutable_realname();
  inline ::std::string* release_realname();
  inline void set_allocated_realname(::std::string* realname);

  // required string antiaddictionid = 4;
  inline bool has_antiaddictionid() const;
  inline void clear_antiaddictionid();
  static const int kAntiaddictionidFieldNumber = 4;
  inline const ::std::string& antiaddictionid() const;
  inline void set_antiaddictionid(const ::std::string& value);
  inline void set_antiaddictionid(const char* value);
  inline void set_antiaddictionid(const char* value, size_t size);
  inline ::std::string* mutable_antiaddictionid();
  inline ::std::string* release_antiaddictionid();
  inline void set_allocated_antiaddictionid(::std::string* antiaddictionid);

  // @@protoc_insertion_point(class_scope:AntiAddictionProto.ToDBBindAntiAddictionID)
 private:
  inline void set_has_charid();
  inline void clear_has_charid();
  inline void set_has_antiaddictionstate();
  inline void clear_has_antiaddictionstate();
  inline void set_has_realname();
  inline void clear_has_realname();
  inline void set_has_antiaddictionid();
  inline void clear_has_antiaddictionid();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int64 charid_;
  ::std::string* realname_;
  ::std::string* antiaddictionid_;
  ::google::protobuf::int32 antiaddictionstate_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];

  friend void  protobuf_AddDesc_AntiAddiction_2fAntiAddictionProto_2eproto();
  friend void protobuf_AssignDesc_AntiAddiction_2fAntiAddictionProto_2eproto();
  friend void protobuf_ShutdownFile_AntiAddiction_2fAntiAddictionProto_2eproto();

  void InitAsDefaultInstance();
  static ToDBBindAntiAddictionID* default_instance_;
};
// ===================================================================


// ===================================================================

// ReqBindAntiAddictionID

// required string realname = 1;
inline bool ReqBindAntiAddictionID::has_realname() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ReqBindAntiAddictionID::set_has_realname() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ReqBindAntiAddictionID::clear_has_realname() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ReqBindAntiAddictionID::clear_realname() {
  if (realname_ != &::google::protobuf::internal::kEmptyString) {
    realname_->clear();
  }
  clear_has_realname();
}
inline const ::std::string& ReqBindAntiAddictionID::realname() const {
  return *realname_;
}
inline void ReqBindAntiAddictionID::set_realname(const ::std::string& value) {
  set_has_realname();
  if (realname_ == &::google::protobuf::internal::kEmptyString) {
    realname_ = new ::std::string;
  }
  realname_->assign(value);
}
inline void ReqBindAntiAddictionID::set_realname(const char* value) {
  set_has_realname();
  if (realname_ == &::google::protobuf::internal::kEmptyString) {
    realname_ = new ::std::string;
  }
  realname_->assign(value);
}
inline void ReqBindAntiAddictionID::set_realname(const char* value, size_t size) {
  set_has_realname();
  if (realname_ == &::google::protobuf::internal::kEmptyString) {
    realname_ = new ::std::string;
  }
  realname_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ReqBindAntiAddictionID::mutable_realname() {
  set_has_realname();
  if (realname_ == &::google::protobuf::internal::kEmptyString) {
    realname_ = new ::std::string;
  }
  return realname_;
}
inline ::std::string* ReqBindAntiAddictionID::release_realname() {
  clear_has_realname();
  if (realname_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = realname_;
    realname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void ReqBindAntiAddictionID::set_allocated_realname(::std::string* realname) {
  if (realname_ != &::google::protobuf::internal::kEmptyString) {
    delete realname_;
  }
  if (realname) {
    set_has_realname();
    realname_ = realname;
  } else {
    clear_has_realname();
    realname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required string antiaddictionid = 2;
inline bool ReqBindAntiAddictionID::has_antiaddictionid() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ReqBindAntiAddictionID::set_has_antiaddictionid() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ReqBindAntiAddictionID::clear_has_antiaddictionid() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ReqBindAntiAddictionID::clear_antiaddictionid() {
  if (antiaddictionid_ != &::google::protobuf::internal::kEmptyString) {
    antiaddictionid_->clear();
  }
  clear_has_antiaddictionid();
}
inline const ::std::string& ReqBindAntiAddictionID::antiaddictionid() const {
  return *antiaddictionid_;
}
inline void ReqBindAntiAddictionID::set_antiaddictionid(const ::std::string& value) {
  set_has_antiaddictionid();
  if (antiaddictionid_ == &::google::protobuf::internal::kEmptyString) {
    antiaddictionid_ = new ::std::string;
  }
  antiaddictionid_->assign(value);
}
inline void ReqBindAntiAddictionID::set_antiaddictionid(const char* value) {
  set_has_antiaddictionid();
  if (antiaddictionid_ == &::google::protobuf::internal::kEmptyString) {
    antiaddictionid_ = new ::std::string;
  }
  antiaddictionid_->assign(value);
}
inline void ReqBindAntiAddictionID::set_antiaddictionid(const char* value, size_t size) {
  set_has_antiaddictionid();
  if (antiaddictionid_ == &::google::protobuf::internal::kEmptyString) {
    antiaddictionid_ = new ::std::string;
  }
  antiaddictionid_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ReqBindAntiAddictionID::mutable_antiaddictionid() {
  set_has_antiaddictionid();
  if (antiaddictionid_ == &::google::protobuf::internal::kEmptyString) {
    antiaddictionid_ = new ::std::string;
  }
  return antiaddictionid_;
}
inline ::std::string* ReqBindAntiAddictionID::release_antiaddictionid() {
  clear_has_antiaddictionid();
  if (antiaddictionid_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = antiaddictionid_;
    antiaddictionid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void ReqBindAntiAddictionID::set_allocated_antiaddictionid(::std::string* antiaddictionid) {
  if (antiaddictionid_ != &::google::protobuf::internal::kEmptyString) {
    delete antiaddictionid_;
  }
  if (antiaddictionid) {
    set_has_antiaddictionid();
    antiaddictionid_ = antiaddictionid;
  } else {
    clear_has_antiaddictionid();
    antiaddictionid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// -------------------------------------------------------------------

// RetBindAntiAddictionID

// required int32 error = 1;
inline bool RetBindAntiAddictionID::has_error() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void RetBindAntiAddictionID::set_has_error() {
  _has_bits_[0] |= 0x00000001u;
}
inline void RetBindAntiAddictionID::clear_has_error() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void RetBindAntiAddictionID::clear_error() {
  error_ = 0;
  clear_has_error();
}
inline ::google::protobuf::int32 RetBindAntiAddictionID::error() const {
  return error_;
}
inline void RetBindAntiAddictionID::set_error(::google::protobuf::int32 value) {
  set_has_error();
  error_ = value;
}

// optional int32 state = 2;
inline bool RetBindAntiAddictionID::has_state() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void RetBindAntiAddictionID::set_has_state() {
  _has_bits_[0] |= 0x00000002u;
}
inline void RetBindAntiAddictionID::clear_has_state() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void RetBindAntiAddictionID::clear_state() {
  state_ = 0;
  clear_has_state();
}
inline ::google::protobuf::int32 RetBindAntiAddictionID::state() const {
  return state_;
}
inline void RetBindAntiAddictionID::set_state(::google::protobuf::int32 value) {
  set_has_state();
  state_ = value;
}

// -------------------------------------------------------------------

// ToDBBindAntiAddictionID

// required int64 charid = 1;
inline bool ToDBBindAntiAddictionID::has_charid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ToDBBindAntiAddictionID::set_has_charid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ToDBBindAntiAddictionID::clear_has_charid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ToDBBindAntiAddictionID::clear_charid() {
  charid_ = GOOGLE_LONGLONG(0);
  clear_has_charid();
}
inline ::google::protobuf::int64 ToDBBindAntiAddictionID::charid() const {
  return charid_;
}
inline void ToDBBindAntiAddictionID::set_charid(::google::protobuf::int64 value) {
  set_has_charid();
  charid_ = value;
}

// required int32 antiaddictionstate = 2;
inline bool ToDBBindAntiAddictionID::has_antiaddictionstate() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ToDBBindAntiAddictionID::set_has_antiaddictionstate() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ToDBBindAntiAddictionID::clear_has_antiaddictionstate() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ToDBBindAntiAddictionID::clear_antiaddictionstate() {
  antiaddictionstate_ = 0;
  clear_has_antiaddictionstate();
}
inline ::google::protobuf::int32 ToDBBindAntiAddictionID::antiaddictionstate() const {
  return antiaddictionstate_;
}
inline void ToDBBindAntiAddictionID::set_antiaddictionstate(::google::protobuf::int32 value) {
  set_has_antiaddictionstate();
  antiaddictionstate_ = value;
}

// required string realname = 3;
inline bool ToDBBindAntiAddictionID::has_realname() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ToDBBindAntiAddictionID::set_has_realname() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ToDBBindAntiAddictionID::clear_has_realname() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ToDBBindAntiAddictionID::clear_realname() {
  if (realname_ != &::google::protobuf::internal::kEmptyString) {
    realname_->clear();
  }
  clear_has_realname();
}
inline const ::std::string& ToDBBindAntiAddictionID::realname() const {
  return *realname_;
}
inline void ToDBBindAntiAddictionID::set_realname(const ::std::string& value) {
  set_has_realname();
  if (realname_ == &::google::protobuf::internal::kEmptyString) {
    realname_ = new ::std::string;
  }
  realname_->assign(value);
}
inline void ToDBBindAntiAddictionID::set_realname(const char* value) {
  set_has_realname();
  if (realname_ == &::google::protobuf::internal::kEmptyString) {
    realname_ = new ::std::string;
  }
  realname_->assign(value);
}
inline void ToDBBindAntiAddictionID::set_realname(const char* value, size_t size) {
  set_has_realname();
  if (realname_ == &::google::protobuf::internal::kEmptyString) {
    realname_ = new ::std::string;
  }
  realname_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ToDBBindAntiAddictionID::mutable_realname() {
  set_has_realname();
  if (realname_ == &::google::protobuf::internal::kEmptyString) {
    realname_ = new ::std::string;
  }
  return realname_;
}
inline ::std::string* ToDBBindAntiAddictionID::release_realname() {
  clear_has_realname();
  if (realname_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = realname_;
    realname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void ToDBBindAntiAddictionID::set_allocated_realname(::std::string* realname) {
  if (realname_ != &::google::protobuf::internal::kEmptyString) {
    delete realname_;
  }
  if (realname) {
    set_has_realname();
    realname_ = realname;
  } else {
    clear_has_realname();
    realname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required string antiaddictionid = 4;
inline bool ToDBBindAntiAddictionID::has_antiaddictionid() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ToDBBindAntiAddictionID::set_has_antiaddictionid() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ToDBBindAntiAddictionID::clear_has_antiaddictionid() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void ToDBBindAntiAddictionID::clear_antiaddictionid() {
  if (antiaddictionid_ != &::google::protobuf::internal::kEmptyString) {
    antiaddictionid_->clear();
  }
  clear_has_antiaddictionid();
}
inline const ::std::string& ToDBBindAntiAddictionID::antiaddictionid() const {
  return *antiaddictionid_;
}
inline void ToDBBindAntiAddictionID::set_antiaddictionid(const ::std::string& value) {
  set_has_antiaddictionid();
  if (antiaddictionid_ == &::google::protobuf::internal::kEmptyString) {
    antiaddictionid_ = new ::std::string;
  }
  antiaddictionid_->assign(value);
}
inline void ToDBBindAntiAddictionID::set_antiaddictionid(const char* value) {
  set_has_antiaddictionid();
  if (antiaddictionid_ == &::google::protobuf::internal::kEmptyString) {
    antiaddictionid_ = new ::std::string;
  }
  antiaddictionid_->assign(value);
}
inline void ToDBBindAntiAddictionID::set_antiaddictionid(const char* value, size_t size) {
  set_has_antiaddictionid();
  if (antiaddictionid_ == &::google::protobuf::internal::kEmptyString) {
    antiaddictionid_ = new ::std::string;
  }
  antiaddictionid_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* ToDBBindAntiAddictionID::mutable_antiaddictionid() {
  set_has_antiaddictionid();
  if (antiaddictionid_ == &::google::protobuf::internal::kEmptyString) {
    antiaddictionid_ = new ::std::string;
  }
  return antiaddictionid_;
}
inline ::std::string* ToDBBindAntiAddictionID::release_antiaddictionid() {
  clear_has_antiaddictionid();
  if (antiaddictionid_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = antiaddictionid_;
    antiaddictionid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void ToDBBindAntiAddictionID::set_allocated_antiaddictionid(::std::string* antiaddictionid) {
  if (antiaddictionid_ != &::google::protobuf::internal::kEmptyString) {
    delete antiaddictionid_;
  }
  if (antiaddictionid) {
    set_has_antiaddictionid();
    antiaddictionid_ = antiaddictionid;
  } else {
    clear_has_antiaddictionid();
    antiaddictionid_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace AntiAddictionProto

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_AntiAddiction_2fAntiAddictionProto_2eproto__INCLUDED
