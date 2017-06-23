// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: DestinationProto.proto

#ifndef PROTOBUF_DestinationProto_2eproto__INCLUDED
#define PROTOBUF_DestinationProto_2eproto__INCLUDED

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
#include "ServerReturn3Int.pb.h"
#include "ServerReturnInt.pb.h"
// @@protoc_insertion_point(includes)

namespace DestinationProto {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_DestinationProto_2eproto();
void protobuf_AssignDesc_DestinationProto_2eproto();
void protobuf_ShutdownFile_DestinationProto_2eproto();

class ReqLookForDestinationType;
class RetLookForDestinationType;
class ReqGetPrestigePrize;
class SimDestinationComplete;
class ReqGetHaveRewardDestination;
class RetGetHaveRewardDestination;

// ===================================================================

class ReqLookForDestinationType : public ::google::protobuf::Message {
 public:
  ReqLookForDestinationType();
  virtual ~ReqLookForDestinationType();

  ReqLookForDestinationType(const ReqLookForDestinationType& from);

  inline ReqLookForDestinationType& operator=(const ReqLookForDestinationType& from) {
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
  static const ReqLookForDestinationType& default_instance();

  void Swap(ReqLookForDestinationType* other);

  // implements Message ----------------------------------------------

  ReqLookForDestinationType* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ReqLookForDestinationType& from);
  void MergeFrom(const ReqLookForDestinationType& from);
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

  // required int32 type = 1;
  inline bool has_type() const;
  inline void clear_type();
  static const int kTypeFieldNumber = 1;
  inline ::google::protobuf::int32 type() const;
  inline void set_type(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:DestinationProto.ReqLookForDestinationType)
 private:
  inline void set_has_type();
  inline void clear_has_type();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 type_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_DestinationProto_2eproto();
  friend void protobuf_AssignDesc_DestinationProto_2eproto();
  friend void protobuf_ShutdownFile_DestinationProto_2eproto();

  void InitAsDefaultInstance();
  static ReqLookForDestinationType* default_instance_;
};
// -------------------------------------------------------------------

class RetLookForDestinationType : public ::google::protobuf::Message {
 public:
  RetLookForDestinationType();
  virtual ~RetLookForDestinationType();

  RetLookForDestinationType(const RetLookForDestinationType& from);

  inline RetLookForDestinationType& operator=(const RetLookForDestinationType& from) {
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
  static const RetLookForDestinationType& default_instance();

  void Swap(RetLookForDestinationType* other);

  // implements Message ----------------------------------------------

  RetLookForDestinationType* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const RetLookForDestinationType& from);
  void MergeFrom(const RetLookForDestinationType& from);
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

  // repeated .ServerReturn.ServerThreeInt DestinationInfo = 1;
  inline int destinationinfo_size() const;
  inline void clear_destinationinfo();
  static const int kDestinationInfoFieldNumber = 1;
  inline const ::ServerReturn::ServerThreeInt& destinationinfo(int index) const;
  inline ::ServerReturn::ServerThreeInt* mutable_destinationinfo(int index);
  inline ::ServerReturn::ServerThreeInt* add_destinationinfo();
  inline const ::google::protobuf::RepeatedPtrField< ::ServerReturn::ServerThreeInt >&
      destinationinfo() const;
  inline ::google::protobuf::RepeatedPtrField< ::ServerReturn::ServerThreeInt >*
      mutable_destinationinfo();

  // @@protoc_insertion_point(class_scope:DestinationProto.RetLookForDestinationType)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedPtrField< ::ServerReturn::ServerThreeInt > destinationinfo_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_DestinationProto_2eproto();
  friend void protobuf_AssignDesc_DestinationProto_2eproto();
  friend void protobuf_ShutdownFile_DestinationProto_2eproto();

  void InitAsDefaultInstance();
  static RetLookForDestinationType* default_instance_;
};
// -------------------------------------------------------------------

class ReqGetPrestigePrize : public ::google::protobuf::Message {
 public:
  ReqGetPrestigePrize();
  virtual ~ReqGetPrestigePrize();

  ReqGetPrestigePrize(const ReqGetPrestigePrize& from);

  inline ReqGetPrestigePrize& operator=(const ReqGetPrestigePrize& from) {
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
  static const ReqGetPrestigePrize& default_instance();

  void Swap(ReqGetPrestigePrize* other);

  // implements Message ----------------------------------------------

  ReqGetPrestigePrize* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ReqGetPrestigePrize& from);
  void MergeFrom(const ReqGetPrestigePrize& from);
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

  // required int32 destinationID = 1;
  inline bool has_destinationid() const;
  inline void clear_destinationid();
  static const int kDestinationIDFieldNumber = 1;
  inline ::google::protobuf::int32 destinationid() const;
  inline void set_destinationid(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:DestinationProto.ReqGetPrestigePrize)
 private:
  inline void set_has_destinationid();
  inline void clear_has_destinationid();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 destinationid_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_DestinationProto_2eproto();
  friend void protobuf_AssignDesc_DestinationProto_2eproto();
  friend void protobuf_ShutdownFile_DestinationProto_2eproto();

  void InitAsDefaultInstance();
  static ReqGetPrestigePrize* default_instance_;
};
// -------------------------------------------------------------------

class SimDestinationComplete : public ::google::protobuf::Message {
 public:
  SimDestinationComplete();
  virtual ~SimDestinationComplete();

  SimDestinationComplete(const SimDestinationComplete& from);

  inline SimDestinationComplete& operator=(const SimDestinationComplete& from) {
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
  static const SimDestinationComplete& default_instance();

  void Swap(SimDestinationComplete* other);

  // implements Message ----------------------------------------------

  SimDestinationComplete* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const SimDestinationComplete& from);
  void MergeFrom(const SimDestinationComplete& from);
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

  // required int32 completeID = 1;
  inline bool has_completeid() const;
  inline void clear_completeid();
  static const int kCompleteIDFieldNumber = 1;
  inline ::google::protobuf::int32 completeid() const;
  inline void set_completeid(::google::protobuf::int32 value);

  // required int32 completeType = 2;
  inline bool has_completetype() const;
  inline void clear_completetype();
  static const int kCompleteTypeFieldNumber = 2;
  inline ::google::protobuf::int32 completetype() const;
  inline void set_completetype(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:DestinationProto.SimDestinationComplete)
 private:
  inline void set_has_completeid();
  inline void clear_has_completeid();
  inline void set_has_completetype();
  inline void clear_has_completetype();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 completeid_;
  ::google::protobuf::int32 completetype_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_DestinationProto_2eproto();
  friend void protobuf_AssignDesc_DestinationProto_2eproto();
  friend void protobuf_ShutdownFile_DestinationProto_2eproto();

  void InitAsDefaultInstance();
  static SimDestinationComplete* default_instance_;
};
// -------------------------------------------------------------------

class ReqGetHaveRewardDestination : public ::google::protobuf::Message {
 public:
  ReqGetHaveRewardDestination();
  virtual ~ReqGetHaveRewardDestination();

  ReqGetHaveRewardDestination(const ReqGetHaveRewardDestination& from);

  inline ReqGetHaveRewardDestination& operator=(const ReqGetHaveRewardDestination& from) {
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
  static const ReqGetHaveRewardDestination& default_instance();

  void Swap(ReqGetHaveRewardDestination* other);

  // implements Message ----------------------------------------------

  ReqGetHaveRewardDestination* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ReqGetHaveRewardDestination& from);
  void MergeFrom(const ReqGetHaveRewardDestination& from);
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

  // @@protoc_insertion_point(class_scope:DestinationProto.ReqGetHaveRewardDestination)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;


  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[1];

  friend void  protobuf_AddDesc_DestinationProto_2eproto();
  friend void protobuf_AssignDesc_DestinationProto_2eproto();
  friend void protobuf_ShutdownFile_DestinationProto_2eproto();

  void InitAsDefaultInstance();
  static ReqGetHaveRewardDestination* default_instance_;
};
// -------------------------------------------------------------------

class RetGetHaveRewardDestination : public ::google::protobuf::Message {
 public:
  RetGetHaveRewardDestination();
  virtual ~RetGetHaveRewardDestination();

  RetGetHaveRewardDestination(const RetGetHaveRewardDestination& from);

  inline RetGetHaveRewardDestination& operator=(const RetGetHaveRewardDestination& from) {
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
  static const RetGetHaveRewardDestination& default_instance();

  void Swap(RetGetHaveRewardDestination* other);

  // implements Message ----------------------------------------------

  RetGetHaveRewardDestination* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const RetGetHaveRewardDestination& from);
  void MergeFrom(const RetGetHaveRewardDestination& from);
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

  // repeated int32 destinationId = 1;
  inline int destinationid_size() const;
  inline void clear_destinationid();
  static const int kDestinationIdFieldNumber = 1;
  inline ::google::protobuf::int32 destinationid(int index) const;
  inline void set_destinationid(int index, ::google::protobuf::int32 value);
  inline void add_destinationid(::google::protobuf::int32 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
      destinationid() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
      mutable_destinationid();

  // @@protoc_insertion_point(class_scope:DestinationProto.RetGetHaveRewardDestination)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedField< ::google::protobuf::int32 > destinationid_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_DestinationProto_2eproto();
  friend void protobuf_AssignDesc_DestinationProto_2eproto();
  friend void protobuf_ShutdownFile_DestinationProto_2eproto();

  void InitAsDefaultInstance();
  static RetGetHaveRewardDestination* default_instance_;
};
// ===================================================================


// ===================================================================

// ReqLookForDestinationType

// required int32 type = 1;
inline bool ReqLookForDestinationType::has_type() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ReqLookForDestinationType::set_has_type() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ReqLookForDestinationType::clear_has_type() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ReqLookForDestinationType::clear_type() {
  type_ = 0;
  clear_has_type();
}
inline ::google::protobuf::int32 ReqLookForDestinationType::type() const {
  return type_;
}
inline void ReqLookForDestinationType::set_type(::google::protobuf::int32 value) {
  set_has_type();
  type_ = value;
}

// -------------------------------------------------------------------

// RetLookForDestinationType

// repeated .ServerReturn.ServerThreeInt DestinationInfo = 1;
inline int RetLookForDestinationType::destinationinfo_size() const {
  return destinationinfo_.size();
}
inline void RetLookForDestinationType::clear_destinationinfo() {
  destinationinfo_.Clear();
}
inline const ::ServerReturn::ServerThreeInt& RetLookForDestinationType::destinationinfo(int index) const {
  return destinationinfo_.Get(index);
}
inline ::ServerReturn::ServerThreeInt* RetLookForDestinationType::mutable_destinationinfo(int index) {
  return destinationinfo_.Mutable(index);
}
inline ::ServerReturn::ServerThreeInt* RetLookForDestinationType::add_destinationinfo() {
  return destinationinfo_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::ServerReturn::ServerThreeInt >&
RetLookForDestinationType::destinationinfo() const {
  return destinationinfo_;
}
inline ::google::protobuf::RepeatedPtrField< ::ServerReturn::ServerThreeInt >*
RetLookForDestinationType::mutable_destinationinfo() {
  return &destinationinfo_;
}

// -------------------------------------------------------------------

// ReqGetPrestigePrize

// required int32 destinationID = 1;
inline bool ReqGetPrestigePrize::has_destinationid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ReqGetPrestigePrize::set_has_destinationid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ReqGetPrestigePrize::clear_has_destinationid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ReqGetPrestigePrize::clear_destinationid() {
  destinationid_ = 0;
  clear_has_destinationid();
}
inline ::google::protobuf::int32 ReqGetPrestigePrize::destinationid() const {
  return destinationid_;
}
inline void ReqGetPrestigePrize::set_destinationid(::google::protobuf::int32 value) {
  set_has_destinationid();
  destinationid_ = value;
}

// -------------------------------------------------------------------

// SimDestinationComplete

// required int32 completeID = 1;
inline bool SimDestinationComplete::has_completeid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void SimDestinationComplete::set_has_completeid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void SimDestinationComplete::clear_has_completeid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void SimDestinationComplete::clear_completeid() {
  completeid_ = 0;
  clear_has_completeid();
}
inline ::google::protobuf::int32 SimDestinationComplete::completeid() const {
  return completeid_;
}
inline void SimDestinationComplete::set_completeid(::google::protobuf::int32 value) {
  set_has_completeid();
  completeid_ = value;
}

// required int32 completeType = 2;
inline bool SimDestinationComplete::has_completetype() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void SimDestinationComplete::set_has_completetype() {
  _has_bits_[0] |= 0x00000002u;
}
inline void SimDestinationComplete::clear_has_completetype() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void SimDestinationComplete::clear_completetype() {
  completetype_ = 0;
  clear_has_completetype();
}
inline ::google::protobuf::int32 SimDestinationComplete::completetype() const {
  return completetype_;
}
inline void SimDestinationComplete::set_completetype(::google::protobuf::int32 value) {
  set_has_completetype();
  completetype_ = value;
}

// -------------------------------------------------------------------

// ReqGetHaveRewardDestination

// -------------------------------------------------------------------

// RetGetHaveRewardDestination

// repeated int32 destinationId = 1;
inline int RetGetHaveRewardDestination::destinationid_size() const {
  return destinationid_.size();
}
inline void RetGetHaveRewardDestination::clear_destinationid() {
  destinationid_.Clear();
}
inline ::google::protobuf::int32 RetGetHaveRewardDestination::destinationid(int index) const {
  return destinationid_.Get(index);
}
inline void RetGetHaveRewardDestination::set_destinationid(int index, ::google::protobuf::int32 value) {
  destinationid_.Set(index, value);
}
inline void RetGetHaveRewardDestination::add_destinationid(::google::protobuf::int32 value) {
  destinationid_.Add(value);
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
RetGetHaveRewardDestination::destinationid() const {
  return destinationid_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
RetGetHaveRewardDestination::mutable_destinationid() {
  return &destinationid_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace DestinationProto

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_DestinationProto_2eproto__INCLUDED
