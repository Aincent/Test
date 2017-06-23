// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: MSProtoc/MS_OpreationSych.proto

#ifndef PROTOBUF_MSProtoc_2fMS_5fOpreationSych_2eproto__INCLUDED
#define PROTOBUF_MSProtoc_2fMS_5fOpreationSych_2eproto__INCLUDED

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

namespace MS_SychProto {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_MSProtoc_2fMS_5fOpreationSych_2eproto();
void protobuf_AssignDesc_MSProtoc_2fMS_5fOpreationSych_2eproto();
void protobuf_ShutdownFile_MSProtoc_2fMS_5fOpreationSych_2eproto();

class SychManagerThig;
class SychManagerData;
class SychGMThig;

// ===================================================================

class SychManagerThig : public ::google::protobuf::Message {
 public:
  SychManagerThig();
  virtual ~SychManagerThig();

  SychManagerThig(const SychManagerThig& from);

  inline SychManagerThig& operator=(const SychManagerThig& from) {
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
  static const SychManagerThig& default_instance();

  void Swap(SychManagerThig* other);

  // implements Message ----------------------------------------------

  SychManagerThig* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const SychManagerThig& from);
  void MergeFrom(const SychManagerThig& from);
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

  // required string name = 1;
  inline bool has_name() const;
  inline void clear_name();
  static const int kNameFieldNumber = 1;
  inline const ::std::string& name() const;
  inline void set_name(const ::std::string& value);
  inline void set_name(const char* value);
  inline void set_name(const char* value, size_t size);
  inline ::std::string* mutable_name();
  inline ::std::string* release_name();
  inline void set_allocated_name(::std::string* name);

  // optional .MS_SychProto.SychManagerData data = 2;
  inline bool has_data() const;
  inline void clear_data();
  static const int kDataFieldNumber = 2;
  inline const ::MS_SychProto::SychManagerData& data() const;
  inline ::MS_SychProto::SychManagerData* mutable_data();
  inline ::MS_SychProto::SychManagerData* release_data();
  inline void set_allocated_data(::MS_SychProto::SychManagerData* data);

  // @@protoc_insertion_point(class_scope:MS_SychProto.SychManagerThig)
 private:
  inline void set_has_name();
  inline void clear_has_name();
  inline void set_has_data();
  inline void clear_has_data();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* name_;
  ::MS_SychProto::SychManagerData* data_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_MSProtoc_2fMS_5fOpreationSych_2eproto();
  friend void protobuf_AssignDesc_MSProtoc_2fMS_5fOpreationSych_2eproto();
  friend void protobuf_ShutdownFile_MSProtoc_2fMS_5fOpreationSych_2eproto();

  void InitAsDefaultInstance();
  static SychManagerThig* default_instance_;
};
// -------------------------------------------------------------------

class SychManagerData : public ::google::protobuf::Message {
 public:
  SychManagerData();
  virtual ~SychManagerData();

  SychManagerData(const SychManagerData& from);

  inline SychManagerData& operator=(const SychManagerData& from) {
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
  static const SychManagerData& default_instance();

  void Swap(SychManagerData* other);

  // implements Message ----------------------------------------------

  SychManagerData* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const SychManagerData& from);
  void MergeFrom(const SychManagerData& from);
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

  // required int64 nextTime = 1;
  inline bool has_nexttime() const;
  inline void clear_nexttime();
  static const int kNextTimeFieldNumber = 1;
  inline ::google::protobuf::int64 nexttime() const;
  inline void set_nexttime(::google::protobuf::int64 value);

  // required string sayText = 2;
  inline bool has_saytext() const;
  inline void clear_saytext();
  static const int kSayTextFieldNumber = 2;
  inline const ::std::string& saytext() const;
  inline void set_saytext(const ::std::string& value);
  inline void set_saytext(const char* value);
  inline void set_saytext(const char* value, size_t size);
  inline ::std::string* mutable_saytext();
  inline ::std::string* release_saytext();
  inline void set_allocated_saytext(::std::string* saytext);

  // required int32 type = 3;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 3;
  inline ::google::protobuf::int32 type() const;
  inline void set_type(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:MS_SychProto.SychManagerData)
 private:
  inline void set_has_nexttime();
  inline void clear_has_nexttime();
  inline void set_has_saytext();
  inline void clear_has_saytext();
  inline void set_has_type();
  inline void clear_has_type();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int64 nexttime_;
  ::std::string* saytext_;
  ::google::protobuf::int32 type_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_MSProtoc_2fMS_5fOpreationSych_2eproto();
  friend void protobuf_AssignDesc_MSProtoc_2fMS_5fOpreationSych_2eproto();
  friend void protobuf_ShutdownFile_MSProtoc_2fMS_5fOpreationSych_2eproto();

  void InitAsDefaultInstance();
  static SychManagerData* default_instance_;
};
// -------------------------------------------------------------------

class SychGMThig : public ::google::protobuf::Message {
 public:
  SychGMThig();
  virtual ~SychGMThig();

  SychGMThig(const SychGMThig& from);

  inline SychGMThig& operator=(const SychGMThig& from) {
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
  static const SychGMThig& default_instance();

  void Swap(SychGMThig* other);

  // implements Message ----------------------------------------------

  SychGMThig* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const SychGMThig& from);
  void MergeFrom(const SychGMThig& from);
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

  // optional .MS_SychProto.SychManagerData data = 2;
  inline bool has_data() const;
  inline void clear_data();
  static const int kDataFieldNumber = 2;
  inline const ::MS_SychProto::SychManagerData& data() const;
  inline ::MS_SychProto::SychManagerData* mutable_data();
  inline ::MS_SychProto::SychManagerData* release_data();
  inline void set_allocated_data(::MS_SychProto::SychManagerData* data);

  // @@protoc_insertion_point(class_scope:MS_SychProto.SychGMThig)
 private:
  inline void set_has_charid();
  inline void clear_has_charid();
  inline void set_has_data();
  inline void clear_has_data();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int64 charid_;
  ::MS_SychProto::SychManagerData* data_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_MSProtoc_2fMS_5fOpreationSych_2eproto();
  friend void protobuf_AssignDesc_MSProtoc_2fMS_5fOpreationSych_2eproto();
  friend void protobuf_ShutdownFile_MSProtoc_2fMS_5fOpreationSych_2eproto();

  void InitAsDefaultInstance();
  static SychGMThig* default_instance_;
};
// ===================================================================


// ===================================================================

// SychManagerThig

// required string name = 1;
inline bool SychManagerThig::has_name() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void SychManagerThig::set_has_name() {
  _has_bits_[0] |= 0x00000001u;
}
inline void SychManagerThig::clear_has_name() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void SychManagerThig::clear_name() {
  if (name_ != &::google::protobuf::internal::kEmptyString) {
    name_->clear();
  }
  clear_has_name();
}
inline const ::std::string& SychManagerThig::name() const {
  return *name_;
}
inline void SychManagerThig::set_name(const ::std::string& value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void SychManagerThig::set_name(const char* value) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(value);
}
inline void SychManagerThig::set_name(const char* value, size_t size) {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* SychManagerThig::mutable_name() {
  set_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    name_ = new ::std::string;
  }
  return name_;
}
inline ::std::string* SychManagerThig::release_name() {
  clear_has_name();
  if (name_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = name_;
    name_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void SychManagerThig::set_allocated_name(::std::string* name) {
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

// optional .MS_SychProto.SychManagerData data = 2;
inline bool SychManagerThig::has_data() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void SychManagerThig::set_has_data() {
  _has_bits_[0] |= 0x00000002u;
}
inline void SychManagerThig::clear_has_data() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void SychManagerThig::clear_data() {
  if (data_ != NULL) data_->::MS_SychProto::SychManagerData::Clear();
  clear_has_data();
}
inline const ::MS_SychProto::SychManagerData& SychManagerThig::data() const {
  return data_ != NULL ? *data_ : *default_instance_->data_;
}
inline ::MS_SychProto::SychManagerData* SychManagerThig::mutable_data() {
  set_has_data();
  if (data_ == NULL) data_ = new ::MS_SychProto::SychManagerData;
  return data_;
}
inline ::MS_SychProto::SychManagerData* SychManagerThig::release_data() {
  clear_has_data();
  ::MS_SychProto::SychManagerData* temp = data_;
  data_ = NULL;
  return temp;
}
inline void SychManagerThig::set_allocated_data(::MS_SychProto::SychManagerData* data) {
  delete data_;
  data_ = data;
  if (data) {
    set_has_data();
  } else {
    clear_has_data();
  }
}

// -------------------------------------------------------------------

// SychManagerData

// required int64 nextTime = 1;
inline bool SychManagerData::has_nexttime() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void SychManagerData::set_has_nexttime() {
  _has_bits_[0] |= 0x00000001u;
}
inline void SychManagerData::clear_has_nexttime() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void SychManagerData::clear_nexttime() {
  nexttime_ = GOOGLE_LONGLONG(0);
  clear_has_nexttime();
}
inline ::google::protobuf::int64 SychManagerData::nexttime() const {
  return nexttime_;
}
inline void SychManagerData::set_nexttime(::google::protobuf::int64 value) {
  set_has_nexttime();
  nexttime_ = value;
}

// required string sayText = 2;
inline bool SychManagerData::has_saytext() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void SychManagerData::set_has_saytext() {
  _has_bits_[0] |= 0x00000002u;
}
inline void SychManagerData::clear_has_saytext() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void SychManagerData::clear_saytext() {
  if (saytext_ != &::google::protobuf::internal::kEmptyString) {
    saytext_->clear();
  }
  clear_has_saytext();
}
inline const ::std::string& SychManagerData::saytext() const {
  return *saytext_;
}
inline void SychManagerData::set_saytext(const ::std::string& value) {
  set_has_saytext();
  if (saytext_ == &::google::protobuf::internal::kEmptyString) {
    saytext_ = new ::std::string;
  }
  saytext_->assign(value);
}
inline void SychManagerData::set_saytext(const char* value) {
  set_has_saytext();
  if (saytext_ == &::google::protobuf::internal::kEmptyString) {
    saytext_ = new ::std::string;
  }
  saytext_->assign(value);
}
inline void SychManagerData::set_saytext(const char* value, size_t size) {
  set_has_saytext();
  if (saytext_ == &::google::protobuf::internal::kEmptyString) {
    saytext_ = new ::std::string;
  }
  saytext_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* SychManagerData::mutable_saytext() {
  set_has_saytext();
  if (saytext_ == &::google::protobuf::internal::kEmptyString) {
    saytext_ = new ::std::string;
  }
  return saytext_;
}
inline ::std::string* SychManagerData::release_saytext() {
  clear_has_saytext();
  if (saytext_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = saytext_;
    saytext_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void SychManagerData::set_allocated_saytext(::std::string* saytext) {
  if (saytext_ != &::google::protobuf::internal::kEmptyString) {
    delete saytext_;
  }
  if (saytext) {
    set_has_saytext();
    saytext_ = saytext;
  } else {
    clear_has_saytext();
    saytext_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required int32 type = 3;
inline bool SychManagerData::has_type() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void SychManagerData::set_has_type() {
  _has_bits_[0] |= 0x00000004u;
}
inline void SychManagerData::clear_has_type() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void SychManagerData::clear_type() {
  type_ = 0;
  clear_has_type();
}
inline ::google::protobuf::int32 SychManagerData::type() const {
  return type_;
}
inline void SychManagerData::set_type(::google::protobuf::int32 value) {
  set_has_type();
  type_ = value;
}

// -------------------------------------------------------------------

// SychGMThig

// required int64 charid = 1;
inline bool SychGMThig::has_charid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void SychGMThig::set_has_charid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void SychGMThig::clear_has_charid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void SychGMThig::clear_charid() {
  charid_ = GOOGLE_LONGLONG(0);
  clear_has_charid();
}
inline ::google::protobuf::int64 SychGMThig::charid() const {
  return charid_;
}
inline void SychGMThig::set_charid(::google::protobuf::int64 value) {
  set_has_charid();
  charid_ = value;
}

// optional .MS_SychProto.SychManagerData data = 2;
inline bool SychGMThig::has_data() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void SychGMThig::set_has_data() {
  _has_bits_[0] |= 0x00000002u;
}
inline void SychGMThig::clear_has_data() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void SychGMThig::clear_data() {
  if (data_ != NULL) data_->::MS_SychProto::SychManagerData::Clear();
  clear_has_data();
}
inline const ::MS_SychProto::SychManagerData& SychGMThig::data() const {
  return data_ != NULL ? *data_ : *default_instance_->data_;
}
inline ::MS_SychProto::SychManagerData* SychGMThig::mutable_data() {
  set_has_data();
  if (data_ == NULL) data_ = new ::MS_SychProto::SychManagerData;
  return data_;
}
inline ::MS_SychProto::SychManagerData* SychGMThig::release_data() {
  clear_has_data();
  ::MS_SychProto::SychManagerData* temp = data_;
  data_ = NULL;
  return temp;
}
inline void SychGMThig::set_allocated_data(::MS_SychProto::SychManagerData* data) {
  delete data_;
  data_ = data;
  if (data) {
    set_has_data();
  } else {
    clear_has_data();
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace MS_SychProto

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_MSProtoc_2fMS_5fOpreationSych_2eproto__INCLUDED
