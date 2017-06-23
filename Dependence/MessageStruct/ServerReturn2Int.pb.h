// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ServerReturn2Int.proto

#ifndef PROTOBUF_ServerReturn2Int_2eproto__INCLUDED
#define PROTOBUF_ServerReturn2Int_2eproto__INCLUDED

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

namespace ServerReturn {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_ServerReturn2Int_2eproto();
void protobuf_AssignDesc_ServerReturn2Int_2eproto();
void protobuf_ShutdownFile_ServerReturn2Int_2eproto();

class ServerDoubleInt;
class ServerDoubleRetInt;
class ServerDoubleUint32;
class ServerDoubleUint32Array;

// ===================================================================

class ServerDoubleInt : public ::google::protobuf::Message {
 public:
  ServerDoubleInt();
  virtual ~ServerDoubleInt();

  ServerDoubleInt(const ServerDoubleInt& from);

  inline ServerDoubleInt& operator=(const ServerDoubleInt& from) {
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
  static const ServerDoubleInt& default_instance();

  void Swap(ServerDoubleInt* other);

  // implements Message ----------------------------------------------

  ServerDoubleInt* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ServerDoubleInt& from);
  void MergeFrom(const ServerDoubleInt& from);
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

  // required int64 retf = 1;
  inline bool has_retf() const;
  inline void clear_retf();
  static const int kRetfFieldNumber = 1;
  inline ::google::protobuf::int64 retf() const;
  inline void set_retf(::google::protobuf::int64 value);

  // optional int64 rets = 2;
  inline bool has_rets() const;
  inline void clear_rets();
  static const int kRetsFieldNumber = 2;
  inline ::google::protobuf::int64 rets() const;
  inline void set_rets(::google::protobuf::int64 value);

  // @@protoc_insertion_point(class_scope:ServerReturn.ServerDoubleInt)
 private:
  inline void set_has_retf();
  inline void clear_has_retf();
  inline void set_has_rets();
  inline void clear_has_rets();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int64 retf_;
  ::google::protobuf::int64 rets_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_ServerReturn2Int_2eproto();
  friend void protobuf_AssignDesc_ServerReturn2Int_2eproto();
  friend void protobuf_ShutdownFile_ServerReturn2Int_2eproto();

  void InitAsDefaultInstance();
  static ServerDoubleInt* default_instance_;
};
// -------------------------------------------------------------------

class ServerDoubleRetInt : public ::google::protobuf::Message {
 public:
  ServerDoubleRetInt();
  virtual ~ServerDoubleRetInt();

  ServerDoubleRetInt(const ServerDoubleRetInt& from);

  inline ServerDoubleRetInt& operator=(const ServerDoubleRetInt& from) {
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
  static const ServerDoubleRetInt& default_instance();

  void Swap(ServerDoubleRetInt* other);

  // implements Message ----------------------------------------------

  ServerDoubleRetInt* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ServerDoubleRetInt& from);
  void MergeFrom(const ServerDoubleRetInt& from);
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

  // required int32 retf = 1;
  inline bool has_retf() const;
  inline void clear_retf();
  static const int kRetfFieldNumber = 1;
  inline ::google::protobuf::int32 retf() const;
  inline void set_retf(::google::protobuf::int32 value);

  // optional int64 rets = 2;
  inline bool has_rets() const;
  inline void clear_rets();
  static const int kRetsFieldNumber = 2;
  inline ::google::protobuf::int64 rets() const;
  inline void set_rets(::google::protobuf::int64 value);

  // @@protoc_insertion_point(class_scope:ServerReturn.ServerDoubleRetInt)
 private:
  inline void set_has_retf();
  inline void clear_has_retf();
  inline void set_has_rets();
  inline void clear_has_rets();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int64 rets_;
  ::google::protobuf::int32 retf_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_ServerReturn2Int_2eproto();
  friend void protobuf_AssignDesc_ServerReturn2Int_2eproto();
  friend void protobuf_ShutdownFile_ServerReturn2Int_2eproto();

  void InitAsDefaultInstance();
  static ServerDoubleRetInt* default_instance_;
};
// -------------------------------------------------------------------

class ServerDoubleUint32 : public ::google::protobuf::Message {
 public:
  ServerDoubleUint32();
  virtual ~ServerDoubleUint32();

  ServerDoubleUint32(const ServerDoubleUint32& from);

  inline ServerDoubleUint32& operator=(const ServerDoubleUint32& from) {
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
  static const ServerDoubleUint32& default_instance();

  void Swap(ServerDoubleUint32* other);

  // implements Message ----------------------------------------------

  ServerDoubleUint32* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ServerDoubleUint32& from);
  void MergeFrom(const ServerDoubleUint32& from);
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

  // required int32 retf = 1;
  inline bool has_retf() const;
  inline void clear_retf();
  static const int kRetfFieldNumber = 1;
  inline ::google::protobuf::int32 retf() const;
  inline void set_retf(::google::protobuf::int32 value);

  // optional int32 rets = 2;
  inline bool has_rets() const;
  inline void clear_rets();
  static const int kRetsFieldNumber = 2;
  inline ::google::protobuf::int32 rets() const;
  inline void set_rets(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:ServerReturn.ServerDoubleUint32)
 private:
  inline void set_has_retf();
  inline void clear_has_retf();
  inline void set_has_rets();
  inline void clear_has_rets();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 retf_;
  ::google::protobuf::int32 rets_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_ServerReturn2Int_2eproto();
  friend void protobuf_AssignDesc_ServerReturn2Int_2eproto();
  friend void protobuf_ShutdownFile_ServerReturn2Int_2eproto();

  void InitAsDefaultInstance();
  static ServerDoubleUint32* default_instance_;
};
// -------------------------------------------------------------------

class ServerDoubleUint32Array : public ::google::protobuf::Message {
 public:
  ServerDoubleUint32Array();
  virtual ~ServerDoubleUint32Array();

  ServerDoubleUint32Array(const ServerDoubleUint32Array& from);

  inline ServerDoubleUint32Array& operator=(const ServerDoubleUint32Array& from) {
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
  static const ServerDoubleUint32Array& default_instance();

  void Swap(ServerDoubleUint32Array* other);

  // implements Message ----------------------------------------------

  ServerDoubleUint32Array* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ServerDoubleUint32Array& from);
  void MergeFrom(const ServerDoubleUint32Array& from);
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

  // repeated .ServerReturn.ServerDoubleUint32 ret = 1;
  inline int ret_size() const;
  inline void clear_ret();
  static const int kRetFieldNumber = 1;
  inline const ::ServerReturn::ServerDoubleUint32& ret(int index) const;
  inline ::ServerReturn::ServerDoubleUint32* mutable_ret(int index);
  inline ::ServerReturn::ServerDoubleUint32* add_ret();
  inline const ::google::protobuf::RepeatedPtrField< ::ServerReturn::ServerDoubleUint32 >&
      ret() const;
  inline ::google::protobuf::RepeatedPtrField< ::ServerReturn::ServerDoubleUint32 >*
      mutable_ret();

  // @@protoc_insertion_point(class_scope:ServerReturn.ServerDoubleUint32Array)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedPtrField< ::ServerReturn::ServerDoubleUint32 > ret_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_ServerReturn2Int_2eproto();
  friend void protobuf_AssignDesc_ServerReturn2Int_2eproto();
  friend void protobuf_ShutdownFile_ServerReturn2Int_2eproto();

  void InitAsDefaultInstance();
  static ServerDoubleUint32Array* default_instance_;
};
// ===================================================================


// ===================================================================

// ServerDoubleInt

// required int64 retf = 1;
inline bool ServerDoubleInt::has_retf() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ServerDoubleInt::set_has_retf() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ServerDoubleInt::clear_has_retf() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ServerDoubleInt::clear_retf() {
  retf_ = GOOGLE_LONGLONG(0);
  clear_has_retf();
}
inline ::google::protobuf::int64 ServerDoubleInt::retf() const {
  return retf_;
}
inline void ServerDoubleInt::set_retf(::google::protobuf::int64 value) {
  set_has_retf();
  retf_ = value;
}

// optional int64 rets = 2;
inline bool ServerDoubleInt::has_rets() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ServerDoubleInt::set_has_rets() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ServerDoubleInt::clear_has_rets() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ServerDoubleInt::clear_rets() {
  rets_ = GOOGLE_LONGLONG(0);
  clear_has_rets();
}
inline ::google::protobuf::int64 ServerDoubleInt::rets() const {
  return rets_;
}
inline void ServerDoubleInt::set_rets(::google::protobuf::int64 value) {
  set_has_rets();
  rets_ = value;
}

// -------------------------------------------------------------------

// ServerDoubleRetInt

// required int32 retf = 1;
inline bool ServerDoubleRetInt::has_retf() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ServerDoubleRetInt::set_has_retf() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ServerDoubleRetInt::clear_has_retf() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ServerDoubleRetInt::clear_retf() {
  retf_ = 0;
  clear_has_retf();
}
inline ::google::protobuf::int32 ServerDoubleRetInt::retf() const {
  return retf_;
}
inline void ServerDoubleRetInt::set_retf(::google::protobuf::int32 value) {
  set_has_retf();
  retf_ = value;
}

// optional int64 rets = 2;
inline bool ServerDoubleRetInt::has_rets() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ServerDoubleRetInt::set_has_rets() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ServerDoubleRetInt::clear_has_rets() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ServerDoubleRetInt::clear_rets() {
  rets_ = GOOGLE_LONGLONG(0);
  clear_has_rets();
}
inline ::google::protobuf::int64 ServerDoubleRetInt::rets() const {
  return rets_;
}
inline void ServerDoubleRetInt::set_rets(::google::protobuf::int64 value) {
  set_has_rets();
  rets_ = value;
}

// -------------------------------------------------------------------

// ServerDoubleUint32

// required int32 retf = 1;
inline bool ServerDoubleUint32::has_retf() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ServerDoubleUint32::set_has_retf() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ServerDoubleUint32::clear_has_retf() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ServerDoubleUint32::clear_retf() {
  retf_ = 0;
  clear_has_retf();
}
inline ::google::protobuf::int32 ServerDoubleUint32::retf() const {
  return retf_;
}
inline void ServerDoubleUint32::set_retf(::google::protobuf::int32 value) {
  set_has_retf();
  retf_ = value;
}

// optional int32 rets = 2;
inline bool ServerDoubleUint32::has_rets() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ServerDoubleUint32::set_has_rets() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ServerDoubleUint32::clear_has_rets() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ServerDoubleUint32::clear_rets() {
  rets_ = 0;
  clear_has_rets();
}
inline ::google::protobuf::int32 ServerDoubleUint32::rets() const {
  return rets_;
}
inline void ServerDoubleUint32::set_rets(::google::protobuf::int32 value) {
  set_has_rets();
  rets_ = value;
}

// -------------------------------------------------------------------

// ServerDoubleUint32Array

// repeated .ServerReturn.ServerDoubleUint32 ret = 1;
inline int ServerDoubleUint32Array::ret_size() const {
  return ret_.size();
}
inline void ServerDoubleUint32Array::clear_ret() {
  ret_.Clear();
}
inline const ::ServerReturn::ServerDoubleUint32& ServerDoubleUint32Array::ret(int index) const {
  return ret_.Get(index);
}
inline ::ServerReturn::ServerDoubleUint32* ServerDoubleUint32Array::mutable_ret(int index) {
  return ret_.Mutable(index);
}
inline ::ServerReturn::ServerDoubleUint32* ServerDoubleUint32Array::add_ret() {
  return ret_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::ServerReturn::ServerDoubleUint32 >&
ServerDoubleUint32Array::ret() const {
  return ret_;
}
inline ::google::protobuf::RepeatedPtrField< ::ServerReturn::ServerDoubleUint32 >*
ServerDoubleUint32Array::mutable_ret() {
  return &ret_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace ServerReturn

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_ServerReturn2Int_2eproto__INCLUDED
