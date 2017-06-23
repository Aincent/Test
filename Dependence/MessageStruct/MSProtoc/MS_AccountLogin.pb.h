// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: MSProtoc/MS_AccountLogin.proto

#ifndef PROTOBUF_MSProtoc_2fMS_5fAccountLogin_2eproto__INCLUDED
#define PROTOBUF_MSProtoc_2fMS_5fAccountLogin_2eproto__INCLUDED

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

namespace MS_AccountLogin {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_MSProtoc_2fMS_5fAccountLogin_2eproto();
void protobuf_AssignDesc_MSProtoc_2fMS_5fAccountLogin_2eproto();
void protobuf_ShutdownFile_MSProtoc_2fMS_5fAccountLogin_2eproto();

class AccountLogin;
class RetLogin;

// ===================================================================

class AccountLogin : public ::google::protobuf::Message {
 public:
  AccountLogin();
  virtual ~AccountLogin();

  AccountLogin(const AccountLogin& from);

  inline AccountLogin& operator=(const AccountLogin& from) {
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
  static const AccountLogin& default_instance();

  void Swap(AccountLogin* other);

  // implements Message ----------------------------------------------

  AccountLogin* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const AccountLogin& from);
  void MergeFrom(const AccountLogin& from);
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

  // required string openString = 1;
  inline bool has_openstring() const;
  inline void clear_openstring();
  static const int kOpenStringFieldNumber = 1;
  inline const ::std::string& openstring() const;
  inline void set_openstring(const ::std::string& value);
  inline void set_openstring(const char* value);
  inline void set_openstring(const char* value, size_t size);
  inline ::std::string* mutable_openstring();
  inline ::std::string* release_openstring();
  inline void set_allocated_openstring(::std::string* openstring);

  // @@protoc_insertion_point(class_scope:MS_AccountLogin.AccountLogin)
 private:
  inline void set_has_openstring();
  inline void clear_has_openstring();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* openstring_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_MSProtoc_2fMS_5fAccountLogin_2eproto();
  friend void protobuf_AssignDesc_MSProtoc_2fMS_5fAccountLogin_2eproto();
  friend void protobuf_ShutdownFile_MSProtoc_2fMS_5fAccountLogin_2eproto();

  void InitAsDefaultInstance();
  static AccountLogin* default_instance_;
};
// -------------------------------------------------------------------

class RetLogin : public ::google::protobuf::Message {
 public:
  RetLogin();
  virtual ~RetLogin();

  RetLogin(const RetLogin& from);

  inline RetLogin& operator=(const RetLogin& from) {
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
  static const RetLogin& default_instance();

  void Swap(RetLogin* other);

  // implements Message ----------------------------------------------

  RetLogin* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const RetLogin& from);
  void MergeFrom(const RetLogin& from);
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

  // required int32 ret = 1;
  inline bool has_ret() const;
  inline void clear_ret();
  static const int kRetFieldNumber = 1;
  inline ::google::protobuf::int32 ret() const;
  inline void set_ret(::google::protobuf::int32 value);

  // optional string openString = 2;
  inline bool has_openstring() const;
  inline void clear_openstring();
  static const int kOpenStringFieldNumber = 2;
  inline const ::std::string& openstring() const;
  inline void set_openstring(const ::std::string& value);
  inline void set_openstring(const char* value);
  inline void set_openstring(const char* value, size_t size);
  inline ::std::string* mutable_openstring();
  inline ::std::string* release_openstring();
  inline void set_allocated_openstring(::std::string* openstring);

  // optional string IP = 3;
  inline bool has_ip() const;
  inline void clear_ip();
  static const int kIPFieldNumber = 3;
  inline const ::std::string& ip() const;
  inline void set_ip(const ::std::string& value);
  inline void set_ip(const char* value);
  inline void set_ip(const char* value, size_t size);
  inline ::std::string* mutable_ip();
  inline ::std::string* release_ip();
  inline void set_allocated_ip(::std::string* ip);

  // optional int32 port = 4;
  inline bool has_port() const;
  inline void clear_port();
  static const int kPortFieldNumber = 4;
  inline ::google::protobuf::int32 port() const;
  inline void set_port(::google::protobuf::int32 value);

  // optional bool isRole = 5;
  inline bool has_isrole() const;
  inline void clear_isrole();
  static const int kIsRoleFieldNumber = 5;
  inline bool isrole() const;
  inline void set_isrole(bool value);

  // optional string version = 6;
  inline bool has_version() const;
  inline void clear_version();
  static const int kVersionFieldNumber = 6;
  inline const ::std::string& version() const;
  inline void set_version(const ::std::string& value);
  inline void set_version(const char* value);
  inline void set_version(const char* value, size_t size);
  inline ::std::string* mutable_version();
  inline ::std::string* release_version();
  inline void set_allocated_version(::std::string* version);

  // @@protoc_insertion_point(class_scope:MS_AccountLogin.RetLogin)
 private:
  inline void set_has_ret();
  inline void clear_has_ret();
  inline void set_has_openstring();
  inline void clear_has_openstring();
  inline void set_has_ip();
  inline void clear_has_ip();
  inline void set_has_port();
  inline void clear_has_port();
  inline void set_has_isrole();
  inline void clear_has_isrole();
  inline void set_has_version();
  inline void clear_has_version();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* openstring_;
  ::google::protobuf::int32 ret_;
  ::google::protobuf::int32 port_;
  ::std::string* ip_;
  ::std::string* version_;
  bool isrole_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(6 + 31) / 32];

  friend void  protobuf_AddDesc_MSProtoc_2fMS_5fAccountLogin_2eproto();
  friend void protobuf_AssignDesc_MSProtoc_2fMS_5fAccountLogin_2eproto();
  friend void protobuf_ShutdownFile_MSProtoc_2fMS_5fAccountLogin_2eproto();

  void InitAsDefaultInstance();
  static RetLogin* default_instance_;
};
// ===================================================================


// ===================================================================

// AccountLogin

// required string openString = 1;
inline bool AccountLogin::has_openstring() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void AccountLogin::set_has_openstring() {
  _has_bits_[0] |= 0x00000001u;
}
inline void AccountLogin::clear_has_openstring() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void AccountLogin::clear_openstring() {
  if (openstring_ != &::google::protobuf::internal::kEmptyString) {
    openstring_->clear();
  }
  clear_has_openstring();
}
inline const ::std::string& AccountLogin::openstring() const {
  return *openstring_;
}
inline void AccountLogin::set_openstring(const ::std::string& value) {
  set_has_openstring();
  if (openstring_ == &::google::protobuf::internal::kEmptyString) {
    openstring_ = new ::std::string;
  }
  openstring_->assign(value);
}
inline void AccountLogin::set_openstring(const char* value) {
  set_has_openstring();
  if (openstring_ == &::google::protobuf::internal::kEmptyString) {
    openstring_ = new ::std::string;
  }
  openstring_->assign(value);
}
inline void AccountLogin::set_openstring(const char* value, size_t size) {
  set_has_openstring();
  if (openstring_ == &::google::protobuf::internal::kEmptyString) {
    openstring_ = new ::std::string;
  }
  openstring_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* AccountLogin::mutable_openstring() {
  set_has_openstring();
  if (openstring_ == &::google::protobuf::internal::kEmptyString) {
    openstring_ = new ::std::string;
  }
  return openstring_;
}
inline ::std::string* AccountLogin::release_openstring() {
  clear_has_openstring();
  if (openstring_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = openstring_;
    openstring_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void AccountLogin::set_allocated_openstring(::std::string* openstring) {
  if (openstring_ != &::google::protobuf::internal::kEmptyString) {
    delete openstring_;
  }
  if (openstring) {
    set_has_openstring();
    openstring_ = openstring;
  } else {
    clear_has_openstring();
    openstring_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// -------------------------------------------------------------------

// RetLogin

// required int32 ret = 1;
inline bool RetLogin::has_ret() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void RetLogin::set_has_ret() {
  _has_bits_[0] |= 0x00000001u;
}
inline void RetLogin::clear_has_ret() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void RetLogin::clear_ret() {
  ret_ = 0;
  clear_has_ret();
}
inline ::google::protobuf::int32 RetLogin::ret() const {
  return ret_;
}
inline void RetLogin::set_ret(::google::protobuf::int32 value) {
  set_has_ret();
  ret_ = value;
}

// optional string openString = 2;
inline bool RetLogin::has_openstring() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void RetLogin::set_has_openstring() {
  _has_bits_[0] |= 0x00000002u;
}
inline void RetLogin::clear_has_openstring() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void RetLogin::clear_openstring() {
  if (openstring_ != &::google::protobuf::internal::kEmptyString) {
    openstring_->clear();
  }
  clear_has_openstring();
}
inline const ::std::string& RetLogin::openstring() const {
  return *openstring_;
}
inline void RetLogin::set_openstring(const ::std::string& value) {
  set_has_openstring();
  if (openstring_ == &::google::protobuf::internal::kEmptyString) {
    openstring_ = new ::std::string;
  }
  openstring_->assign(value);
}
inline void RetLogin::set_openstring(const char* value) {
  set_has_openstring();
  if (openstring_ == &::google::protobuf::internal::kEmptyString) {
    openstring_ = new ::std::string;
  }
  openstring_->assign(value);
}
inline void RetLogin::set_openstring(const char* value, size_t size) {
  set_has_openstring();
  if (openstring_ == &::google::protobuf::internal::kEmptyString) {
    openstring_ = new ::std::string;
  }
  openstring_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* RetLogin::mutable_openstring() {
  set_has_openstring();
  if (openstring_ == &::google::protobuf::internal::kEmptyString) {
    openstring_ = new ::std::string;
  }
  return openstring_;
}
inline ::std::string* RetLogin::release_openstring() {
  clear_has_openstring();
  if (openstring_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = openstring_;
    openstring_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void RetLogin::set_allocated_openstring(::std::string* openstring) {
  if (openstring_ != &::google::protobuf::internal::kEmptyString) {
    delete openstring_;
  }
  if (openstring) {
    set_has_openstring();
    openstring_ = openstring;
  } else {
    clear_has_openstring();
    openstring_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional string IP = 3;
inline bool RetLogin::has_ip() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void RetLogin::set_has_ip() {
  _has_bits_[0] |= 0x00000004u;
}
inline void RetLogin::clear_has_ip() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void RetLogin::clear_ip() {
  if (ip_ != &::google::protobuf::internal::kEmptyString) {
    ip_->clear();
  }
  clear_has_ip();
}
inline const ::std::string& RetLogin::ip() const {
  return *ip_;
}
inline void RetLogin::set_ip(const ::std::string& value) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(value);
}
inline void RetLogin::set_ip(const char* value) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(value);
}
inline void RetLogin::set_ip(const char* value, size_t size) {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  ip_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* RetLogin::mutable_ip() {
  set_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    ip_ = new ::std::string;
  }
  return ip_;
}
inline ::std::string* RetLogin::release_ip() {
  clear_has_ip();
  if (ip_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = ip_;
    ip_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void RetLogin::set_allocated_ip(::std::string* ip) {
  if (ip_ != &::google::protobuf::internal::kEmptyString) {
    delete ip_;
  }
  if (ip) {
    set_has_ip();
    ip_ = ip;
  } else {
    clear_has_ip();
    ip_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional int32 port = 4;
inline bool RetLogin::has_port() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void RetLogin::set_has_port() {
  _has_bits_[0] |= 0x00000008u;
}
inline void RetLogin::clear_has_port() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void RetLogin::clear_port() {
  port_ = 0;
  clear_has_port();
}
inline ::google::protobuf::int32 RetLogin::port() const {
  return port_;
}
inline void RetLogin::set_port(::google::protobuf::int32 value) {
  set_has_port();
  port_ = value;
}

// optional bool isRole = 5;
inline bool RetLogin::has_isrole() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void RetLogin::set_has_isrole() {
  _has_bits_[0] |= 0x00000010u;
}
inline void RetLogin::clear_has_isrole() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void RetLogin::clear_isrole() {
  isrole_ = false;
  clear_has_isrole();
}
inline bool RetLogin::isrole() const {
  return isrole_;
}
inline void RetLogin::set_isrole(bool value) {
  set_has_isrole();
  isrole_ = value;
}

// optional string version = 6;
inline bool RetLogin::has_version() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void RetLogin::set_has_version() {
  _has_bits_[0] |= 0x00000020u;
}
inline void RetLogin::clear_has_version() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void RetLogin::clear_version() {
  if (version_ != &::google::protobuf::internal::kEmptyString) {
    version_->clear();
  }
  clear_has_version();
}
inline const ::std::string& RetLogin::version() const {
  return *version_;
}
inline void RetLogin::set_version(const ::std::string& value) {
  set_has_version();
  if (version_ == &::google::protobuf::internal::kEmptyString) {
    version_ = new ::std::string;
  }
  version_->assign(value);
}
inline void RetLogin::set_version(const char* value) {
  set_has_version();
  if (version_ == &::google::protobuf::internal::kEmptyString) {
    version_ = new ::std::string;
  }
  version_->assign(value);
}
inline void RetLogin::set_version(const char* value, size_t size) {
  set_has_version();
  if (version_ == &::google::protobuf::internal::kEmptyString) {
    version_ = new ::std::string;
  }
  version_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* RetLogin::mutable_version() {
  set_has_version();
  if (version_ == &::google::protobuf::internal::kEmptyString) {
    version_ = new ::std::string;
  }
  return version_;
}
inline ::std::string* RetLogin::release_version() {
  clear_has_version();
  if (version_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = version_;
    version_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void RetLogin::set_allocated_version(::std::string* version) {
  if (version_ != &::google::protobuf::internal::kEmptyString) {
    delete version_;
  }
  if (version) {
    set_has_version();
    version_ = version;
  } else {
    clear_has_version();
    version_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace MS_AccountLogin

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_MSProtoc_2fMS_5fAccountLogin_2eproto__INCLUDED
