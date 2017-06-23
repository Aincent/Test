// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Meridians/meridians.proto

#ifndef PROTOBUF_Meridians_2fmeridians_2eproto__INCLUDED
#define PROTOBUF_Meridians_2fmeridians_2eproto__INCLUDED

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

namespace Meridians {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_Meridians_2fmeridians_2eproto();
void protobuf_AssignDesc_Meridians_2fmeridians_2eproto();
void protobuf_ShutdownFile_Meridians_2fmeridians_2eproto();

class OpenMeridians;
class RetOpenMeridians;
class AskOpenMeridians;

// ===================================================================

class OpenMeridians : public ::google::protobuf::Message {
 public:
  OpenMeridians();
  virtual ~OpenMeridians();

  OpenMeridians(const OpenMeridians& from);

  inline OpenMeridians& operator=(const OpenMeridians& from) {
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
  static const OpenMeridians& default_instance();

  void Swap(OpenMeridians* other);

  // implements Message ----------------------------------------------

  OpenMeridians* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const OpenMeridians& from);
  void MergeFrom(const OpenMeridians& from);
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

  // required int32 acupoint = 1;
  inline bool has_acupoint() const;
  inline void clear_acupoint();
  static const int kAcupointFieldNumber = 1;
  inline ::google::protobuf::int32 acupoint() const;
  inline void set_acupoint(::google::protobuf::int32 value);

  // optional bool isGolden = 2;
  inline bool has_isgolden() const;
  inline void clear_isgolden();
  static const int kIsGoldenFieldNumber = 2;
  inline bool isgolden() const;
  inline void set_isgolden(bool value);

  // optional int32 count = 3;
  inline bool has_count() const;
  inline void clear_count();
  static const int kCountFieldNumber = 3;
  inline ::google::protobuf::int32 count() const;
  inline void set_count(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:Meridians.OpenMeridians)
 private:
  inline void set_has_acupoint();
  inline void clear_has_acupoint();
  inline void set_has_isgolden();
  inline void clear_has_isgolden();
  inline void set_has_count();
  inline void clear_has_count();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 acupoint_;
  bool isgolden_;
  ::google::protobuf::int32 count_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_Meridians_2fmeridians_2eproto();
  friend void protobuf_AssignDesc_Meridians_2fmeridians_2eproto();
  friend void protobuf_ShutdownFile_Meridians_2fmeridians_2eproto();

  void InitAsDefaultInstance();
  static OpenMeridians* default_instance_;
};
// -------------------------------------------------------------------

class RetOpenMeridians : public ::google::protobuf::Message {
 public:
  RetOpenMeridians();
  virtual ~RetOpenMeridians();

  RetOpenMeridians(const RetOpenMeridians& from);

  inline RetOpenMeridians& operator=(const RetOpenMeridians& from) {
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
  static const RetOpenMeridians& default_instance();

  void Swap(RetOpenMeridians* other);

  // implements Message ----------------------------------------------

  RetOpenMeridians* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const RetOpenMeridians& from);
  void MergeFrom(const RetOpenMeridians& from);
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

  // required int32 errNum = 1;
  inline bool has_errnum() const;
  inline void clear_errnum();
  static const int kErrNumFieldNumber = 1;
  inline ::google::protobuf::int32 errnum() const;
  inline void set_errnum(::google::protobuf::int32 value);

  // optional int32 newAcupoint = 2;
  inline bool has_newacupoint() const;
  inline void clear_newacupoint();
  static const int kNewAcupointFieldNumber = 2;
  inline ::google::protobuf::int32 newacupoint() const;
  inline void set_newacupoint(::google::protobuf::int32 value);

  // optional int32 flag = 3;
  inline bool has_flag() const;
  inline void clear_flag();
  static const int kFlagFieldNumber = 3;
  inline ::google::protobuf::int32 flag() const;
  inline void set_flag(::google::protobuf::int32 value);

  // optional int32 type = 4;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 4;
  inline ::google::protobuf::int32 type() const;
  inline void set_type(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:Meridians.RetOpenMeridians)
 private:
  inline void set_has_errnum();
  inline void clear_has_errnum();
  inline void set_has_newacupoint();
  inline void clear_has_newacupoint();
  inline void set_has_flag();
  inline void clear_has_flag();
  inline void set_has_type();
  inline void clear_has_type();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 errnum_;
  ::google::protobuf::int32 newacupoint_;
  ::google::protobuf::int32 flag_;
  ::google::protobuf::int32 type_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];

  friend void  protobuf_AddDesc_Meridians_2fmeridians_2eproto();
  friend void protobuf_AssignDesc_Meridians_2fmeridians_2eproto();
  friend void protobuf_ShutdownFile_Meridians_2fmeridians_2eproto();

  void InitAsDefaultInstance();
  static RetOpenMeridians* default_instance_;
};
// -------------------------------------------------------------------

class AskOpenMeridians : public ::google::protobuf::Message {
 public:
  AskOpenMeridians();
  virtual ~AskOpenMeridians();

  AskOpenMeridians(const AskOpenMeridians& from);

  inline AskOpenMeridians& operator=(const AskOpenMeridians& from) {
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
  static const AskOpenMeridians& default_instance();

  void Swap(AskOpenMeridians* other);

  // implements Message ----------------------------------------------

  AskOpenMeridians* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const AskOpenMeridians& from);
  void MergeFrom(const AskOpenMeridians& from);
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

  // required string charname = 1;
  inline bool has_charname() const;
  inline void clear_charname();
  static const int kCharnameFieldNumber = 1;
  inline const ::std::string& charname() const;
  inline void set_charname(const ::std::string& value);
  inline void set_charname(const char* value);
  inline void set_charname(const char* value, size_t size);
  inline ::std::string* mutable_charname();
  inline ::std::string* release_charname();
  inline void set_allocated_charname(::std::string* charname);

  // required int32 acupoint = 2;
  inline bool has_acupoint() const;
  inline void clear_acupoint();
  static const int kAcupointFieldNumber = 2;
  inline ::google::protobuf::int32 acupoint() const;
  inline void set_acupoint(::google::protobuf::int32 value);

  // required int32 addexp = 3;
  inline bool has_addexp() const;
  inline void clear_addexp();
  static const int kAddexpFieldNumber = 3;
  inline ::google::protobuf::int32 addexp() const;
  inline void set_addexp(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:Meridians.AskOpenMeridians)
 private:
  inline void set_has_charname();
  inline void clear_has_charname();
  inline void set_has_acupoint();
  inline void clear_has_acupoint();
  inline void set_has_addexp();
  inline void clear_has_addexp();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::std::string* charname_;
  ::google::protobuf::int32 acupoint_;
  ::google::protobuf::int32 addexp_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_Meridians_2fmeridians_2eproto();
  friend void protobuf_AssignDesc_Meridians_2fmeridians_2eproto();
  friend void protobuf_ShutdownFile_Meridians_2fmeridians_2eproto();

  void InitAsDefaultInstance();
  static AskOpenMeridians* default_instance_;
};
// ===================================================================


// ===================================================================

// OpenMeridians

// required int32 acupoint = 1;
inline bool OpenMeridians::has_acupoint() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void OpenMeridians::set_has_acupoint() {
  _has_bits_[0] |= 0x00000001u;
}
inline void OpenMeridians::clear_has_acupoint() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void OpenMeridians::clear_acupoint() {
  acupoint_ = 0;
  clear_has_acupoint();
}
inline ::google::protobuf::int32 OpenMeridians::acupoint() const {
  return acupoint_;
}
inline void OpenMeridians::set_acupoint(::google::protobuf::int32 value) {
  set_has_acupoint();
  acupoint_ = value;
}

// optional bool isGolden = 2;
inline bool OpenMeridians::has_isgolden() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void OpenMeridians::set_has_isgolden() {
  _has_bits_[0] |= 0x00000002u;
}
inline void OpenMeridians::clear_has_isgolden() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void OpenMeridians::clear_isgolden() {
  isgolden_ = false;
  clear_has_isgolden();
}
inline bool OpenMeridians::isgolden() const {
  return isgolden_;
}
inline void OpenMeridians::set_isgolden(bool value) {
  set_has_isgolden();
  isgolden_ = value;
}

// optional int32 count = 3;
inline bool OpenMeridians::has_count() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void OpenMeridians::set_has_count() {
  _has_bits_[0] |= 0x00000004u;
}
inline void OpenMeridians::clear_has_count() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void OpenMeridians::clear_count() {
  count_ = 0;
  clear_has_count();
}
inline ::google::protobuf::int32 OpenMeridians::count() const {
  return count_;
}
inline void OpenMeridians::set_count(::google::protobuf::int32 value) {
  set_has_count();
  count_ = value;
}

// -------------------------------------------------------------------

// RetOpenMeridians

// required int32 errNum = 1;
inline bool RetOpenMeridians::has_errnum() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void RetOpenMeridians::set_has_errnum() {
  _has_bits_[0] |= 0x00000001u;
}
inline void RetOpenMeridians::clear_has_errnum() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void RetOpenMeridians::clear_errnum() {
  errnum_ = 0;
  clear_has_errnum();
}
inline ::google::protobuf::int32 RetOpenMeridians::errnum() const {
  return errnum_;
}
inline void RetOpenMeridians::set_errnum(::google::protobuf::int32 value) {
  set_has_errnum();
  errnum_ = value;
}

// optional int32 newAcupoint = 2;
inline bool RetOpenMeridians::has_newacupoint() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void RetOpenMeridians::set_has_newacupoint() {
  _has_bits_[0] |= 0x00000002u;
}
inline void RetOpenMeridians::clear_has_newacupoint() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void RetOpenMeridians::clear_newacupoint() {
  newacupoint_ = 0;
  clear_has_newacupoint();
}
inline ::google::protobuf::int32 RetOpenMeridians::newacupoint() const {
  return newacupoint_;
}
inline void RetOpenMeridians::set_newacupoint(::google::protobuf::int32 value) {
  set_has_newacupoint();
  newacupoint_ = value;
}

// optional int32 flag = 3;
inline bool RetOpenMeridians::has_flag() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void RetOpenMeridians::set_has_flag() {
  _has_bits_[0] |= 0x00000004u;
}
inline void RetOpenMeridians::clear_has_flag() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void RetOpenMeridians::clear_flag() {
  flag_ = 0;
  clear_has_flag();
}
inline ::google::protobuf::int32 RetOpenMeridians::flag() const {
  return flag_;
}
inline void RetOpenMeridians::set_flag(::google::protobuf::int32 value) {
  set_has_flag();
  flag_ = value;
}

// optional int32 type = 4;
inline bool RetOpenMeridians::has_type() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void RetOpenMeridians::set_has_type() {
  _has_bits_[0] |= 0x00000008u;
}
inline void RetOpenMeridians::clear_has_type() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void RetOpenMeridians::clear_type() {
  type_ = 0;
  clear_has_type();
}
inline ::google::protobuf::int32 RetOpenMeridians::type() const {
  return type_;
}
inline void RetOpenMeridians::set_type(::google::protobuf::int32 value) {
  set_has_type();
  type_ = value;
}

// -------------------------------------------------------------------

// AskOpenMeridians

// required string charname = 1;
inline bool AskOpenMeridians::has_charname() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void AskOpenMeridians::set_has_charname() {
  _has_bits_[0] |= 0x00000001u;
}
inline void AskOpenMeridians::clear_has_charname() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void AskOpenMeridians::clear_charname() {
  if (charname_ != &::google::protobuf::internal::kEmptyString) {
    charname_->clear();
  }
  clear_has_charname();
}
inline const ::std::string& AskOpenMeridians::charname() const {
  return *charname_;
}
inline void AskOpenMeridians::set_charname(const ::std::string& value) {
  set_has_charname();
  if (charname_ == &::google::protobuf::internal::kEmptyString) {
    charname_ = new ::std::string;
  }
  charname_->assign(value);
}
inline void AskOpenMeridians::set_charname(const char* value) {
  set_has_charname();
  if (charname_ == &::google::protobuf::internal::kEmptyString) {
    charname_ = new ::std::string;
  }
  charname_->assign(value);
}
inline void AskOpenMeridians::set_charname(const char* value, size_t size) {
  set_has_charname();
  if (charname_ == &::google::protobuf::internal::kEmptyString) {
    charname_ = new ::std::string;
  }
  charname_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* AskOpenMeridians::mutable_charname() {
  set_has_charname();
  if (charname_ == &::google::protobuf::internal::kEmptyString) {
    charname_ = new ::std::string;
  }
  return charname_;
}
inline ::std::string* AskOpenMeridians::release_charname() {
  clear_has_charname();
  if (charname_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = charname_;
    charname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void AskOpenMeridians::set_allocated_charname(::std::string* charname) {
  if (charname_ != &::google::protobuf::internal::kEmptyString) {
    delete charname_;
  }
  if (charname) {
    set_has_charname();
    charname_ = charname;
  } else {
    clear_has_charname();
    charname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// required int32 acupoint = 2;
inline bool AskOpenMeridians::has_acupoint() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void AskOpenMeridians::set_has_acupoint() {
  _has_bits_[0] |= 0x00000002u;
}
inline void AskOpenMeridians::clear_has_acupoint() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void AskOpenMeridians::clear_acupoint() {
  acupoint_ = 0;
  clear_has_acupoint();
}
inline ::google::protobuf::int32 AskOpenMeridians::acupoint() const {
  return acupoint_;
}
inline void AskOpenMeridians::set_acupoint(::google::protobuf::int32 value) {
  set_has_acupoint();
  acupoint_ = value;
}

// required int32 addexp = 3;
inline bool AskOpenMeridians::has_addexp() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void AskOpenMeridians::set_has_addexp() {
  _has_bits_[0] |= 0x00000004u;
}
inline void AskOpenMeridians::clear_has_addexp() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void AskOpenMeridians::clear_addexp() {
  addexp_ = 0;
  clear_has_addexp();
}
inline ::google::protobuf::int32 AskOpenMeridians::addexp() const {
  return addexp_;
}
inline void AskOpenMeridians::set_addexp(::google::protobuf::int32 value) {
  set_has_addexp();
  addexp_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace Meridians

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Meridians_2fmeridians_2eproto__INCLUDED