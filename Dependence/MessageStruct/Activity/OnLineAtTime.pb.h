// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Activity/OnLineAtTime.proto

#ifndef PROTOBUF_Activity_2fOnLineAtTime_2eproto__INCLUDED
#define PROTOBUF_Activity_2fOnLineAtTime_2eproto__INCLUDED

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

namespace OnLineAtTimeInfo {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_Activity_2fOnLineAtTime_2eproto();
void protobuf_AssignDesc_Activity_2fOnLineAtTime_2eproto();
void protobuf_ShutdownFile_Activity_2fOnLineAtTime_2eproto();

class AtTimeInfo;
class ReqOnLineAtTimeInfo;
class RetOnLineAtTimeInfo;

// ===================================================================

class AtTimeInfo : public ::google::protobuf::Message {
 public:
  AtTimeInfo();
  virtual ~AtTimeInfo();

  AtTimeInfo(const AtTimeInfo& from);

  inline AtTimeInfo& operator=(const AtTimeInfo& from) {
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
  static const AtTimeInfo& default_instance();

  void Swap(AtTimeInfo* other);

  // implements Message ----------------------------------------------

  AtTimeInfo* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const AtTimeInfo& from);
  void MergeFrom(const AtTimeInfo& from);
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

  // required int32 hour = 1;
  inline bool has_hour() const;
  inline void clear_hour();
  static const int kHourFieldNumber = 1;
  inline ::google::protobuf::int32 hour() const;
  inline void set_hour(::google::protobuf::int32 value);

  // required int32 minute = 2;
  inline bool has_minute() const;
  inline void clear_minute();
  static const int kMinuteFieldNumber = 2;
  inline ::google::protobuf::int32 minute() const;
  inline void set_minute(::google::protobuf::int32 value);

  // repeated int64 itemIDs = 3;
  inline int itemids_size() const;
  inline void clear_itemids();
  static const int kItemIDsFieldNumber = 3;
  inline ::google::protobuf::int64 itemids(int index) const;
  inline void set_itemids(int index, ::google::protobuf::int64 value);
  inline void add_itemids(::google::protobuf::int64 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::int64 >&
      itemids() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::int64 >*
      mutable_itemids();

  // repeated int32 itemNums = 4;
  inline int itemnums_size() const;
  inline void clear_itemnums();
  static const int kItemNumsFieldNumber = 4;
  inline ::google::protobuf::int32 itemnums(int index) const;
  inline void set_itemnums(int index, ::google::protobuf::int32 value);
  inline void add_itemnums(::google::protobuf::int32 value);
  inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
      itemnums() const;
  inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
      mutable_itemnums();

  // @@protoc_insertion_point(class_scope:OnLineAtTimeInfo.AtTimeInfo)
 private:
  inline void set_has_hour();
  inline void clear_has_hour();
  inline void set_has_minute();
  inline void clear_has_minute();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 hour_;
  ::google::protobuf::int32 minute_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int64 > itemids_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 > itemnums_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];

  friend void  protobuf_AddDesc_Activity_2fOnLineAtTime_2eproto();
  friend void protobuf_AssignDesc_Activity_2fOnLineAtTime_2eproto();
  friend void protobuf_ShutdownFile_Activity_2fOnLineAtTime_2eproto();

  void InitAsDefaultInstance();
  static AtTimeInfo* default_instance_;
};
// -------------------------------------------------------------------

class ReqOnLineAtTimeInfo : public ::google::protobuf::Message {
 public:
  ReqOnLineAtTimeInfo();
  virtual ~ReqOnLineAtTimeInfo();

  ReqOnLineAtTimeInfo(const ReqOnLineAtTimeInfo& from);

  inline ReqOnLineAtTimeInfo& operator=(const ReqOnLineAtTimeInfo& from) {
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
  static const ReqOnLineAtTimeInfo& default_instance();

  void Swap(ReqOnLineAtTimeInfo* other);

  // implements Message ----------------------------------------------

  ReqOnLineAtTimeInfo* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ReqOnLineAtTimeInfo& from);
  void MergeFrom(const ReqOnLineAtTimeInfo& from);
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

  // @@protoc_insertion_point(class_scope:OnLineAtTimeInfo.ReqOnLineAtTimeInfo)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;


  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[1];

  friend void  protobuf_AddDesc_Activity_2fOnLineAtTime_2eproto();
  friend void protobuf_AssignDesc_Activity_2fOnLineAtTime_2eproto();
  friend void protobuf_ShutdownFile_Activity_2fOnLineAtTime_2eproto();

  void InitAsDefaultInstance();
  static ReqOnLineAtTimeInfo* default_instance_;
};
// -------------------------------------------------------------------

class RetOnLineAtTimeInfo : public ::google::protobuf::Message {
 public:
  RetOnLineAtTimeInfo();
  virtual ~RetOnLineAtTimeInfo();

  RetOnLineAtTimeInfo(const RetOnLineAtTimeInfo& from);

  inline RetOnLineAtTimeInfo& operator=(const RetOnLineAtTimeInfo& from) {
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
  static const RetOnLineAtTimeInfo& default_instance();

  void Swap(RetOnLineAtTimeInfo* other);

  // implements Message ----------------------------------------------

  RetOnLineAtTimeInfo* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const RetOnLineAtTimeInfo& from);
  void MergeFrom(const RetOnLineAtTimeInfo& from);
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

  // optional int64 beginTime = 2;
  inline bool has_begintime() const;
  inline void clear_begintime();
  static const int kBeginTimeFieldNumber = 2;
  inline ::google::protobuf::int64 begintime() const;
  inline void set_begintime(::google::protobuf::int64 value);

  // optional int64 endTime = 3;
  inline bool has_endtime() const;
  inline void clear_endtime();
  static const int kEndTimeFieldNumber = 3;
  inline ::google::protobuf::int64 endtime() const;
  inline void set_endtime(::google::protobuf::int64 value);

  // repeated .OnLineAtTimeInfo.AtTimeInfo atTimeList = 4;
  inline int attimelist_size() const;
  inline void clear_attimelist();
  static const int kAtTimeListFieldNumber = 4;
  inline const ::OnLineAtTimeInfo::AtTimeInfo& attimelist(int index) const;
  inline ::OnLineAtTimeInfo::AtTimeInfo* mutable_attimelist(int index);
  inline ::OnLineAtTimeInfo::AtTimeInfo* add_attimelist();
  inline const ::google::protobuf::RepeatedPtrField< ::OnLineAtTimeInfo::AtTimeInfo >&
      attimelist() const;
  inline ::google::protobuf::RepeatedPtrField< ::OnLineAtTimeInfo::AtTimeInfo >*
      mutable_attimelist();

  // @@protoc_insertion_point(class_scope:OnLineAtTimeInfo.RetOnLineAtTimeInfo)
 private:
  inline void set_has_ret();
  inline void clear_has_ret();
  inline void set_has_begintime();
  inline void clear_has_begintime();
  inline void set_has_endtime();
  inline void clear_has_endtime();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int64 begintime_;
  ::google::protobuf::int64 endtime_;
  ::google::protobuf::RepeatedPtrField< ::OnLineAtTimeInfo::AtTimeInfo > attimelist_;
  ::google::protobuf::int32 ret_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];

  friend void  protobuf_AddDesc_Activity_2fOnLineAtTime_2eproto();
  friend void protobuf_AssignDesc_Activity_2fOnLineAtTime_2eproto();
  friend void protobuf_ShutdownFile_Activity_2fOnLineAtTime_2eproto();

  void InitAsDefaultInstance();
  static RetOnLineAtTimeInfo* default_instance_;
};
// ===================================================================


// ===================================================================

// AtTimeInfo

// required int32 hour = 1;
inline bool AtTimeInfo::has_hour() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void AtTimeInfo::set_has_hour() {
  _has_bits_[0] |= 0x00000001u;
}
inline void AtTimeInfo::clear_has_hour() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void AtTimeInfo::clear_hour() {
  hour_ = 0;
  clear_has_hour();
}
inline ::google::protobuf::int32 AtTimeInfo::hour() const {
  return hour_;
}
inline void AtTimeInfo::set_hour(::google::protobuf::int32 value) {
  set_has_hour();
  hour_ = value;
}

// required int32 minute = 2;
inline bool AtTimeInfo::has_minute() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void AtTimeInfo::set_has_minute() {
  _has_bits_[0] |= 0x00000002u;
}
inline void AtTimeInfo::clear_has_minute() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void AtTimeInfo::clear_minute() {
  minute_ = 0;
  clear_has_minute();
}
inline ::google::protobuf::int32 AtTimeInfo::minute() const {
  return minute_;
}
inline void AtTimeInfo::set_minute(::google::protobuf::int32 value) {
  set_has_minute();
  minute_ = value;
}

// repeated int64 itemIDs = 3;
inline int AtTimeInfo::itemids_size() const {
  return itemids_.size();
}
inline void AtTimeInfo::clear_itemids() {
  itemids_.Clear();
}
inline ::google::protobuf::int64 AtTimeInfo::itemids(int index) const {
  return itemids_.Get(index);
}
inline void AtTimeInfo::set_itemids(int index, ::google::protobuf::int64 value) {
  itemids_.Set(index, value);
}
inline void AtTimeInfo::add_itemids(::google::protobuf::int64 value) {
  itemids_.Add(value);
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int64 >&
AtTimeInfo::itemids() const {
  return itemids_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int64 >*
AtTimeInfo::mutable_itemids() {
  return &itemids_;
}

// repeated int32 itemNums = 4;
inline int AtTimeInfo::itemnums_size() const {
  return itemnums_.size();
}
inline void AtTimeInfo::clear_itemnums() {
  itemnums_.Clear();
}
inline ::google::protobuf::int32 AtTimeInfo::itemnums(int index) const {
  return itemnums_.Get(index);
}
inline void AtTimeInfo::set_itemnums(int index, ::google::protobuf::int32 value) {
  itemnums_.Set(index, value);
}
inline void AtTimeInfo::add_itemnums(::google::protobuf::int32 value) {
  itemnums_.Add(value);
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
AtTimeInfo::itemnums() const {
  return itemnums_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
AtTimeInfo::mutable_itemnums() {
  return &itemnums_;
}

// -------------------------------------------------------------------

// ReqOnLineAtTimeInfo

// -------------------------------------------------------------------

// RetOnLineAtTimeInfo

// required int32 ret = 1;
inline bool RetOnLineAtTimeInfo::has_ret() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void RetOnLineAtTimeInfo::set_has_ret() {
  _has_bits_[0] |= 0x00000001u;
}
inline void RetOnLineAtTimeInfo::clear_has_ret() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void RetOnLineAtTimeInfo::clear_ret() {
  ret_ = 0;
  clear_has_ret();
}
inline ::google::protobuf::int32 RetOnLineAtTimeInfo::ret() const {
  return ret_;
}
inline void RetOnLineAtTimeInfo::set_ret(::google::protobuf::int32 value) {
  set_has_ret();
  ret_ = value;
}

// optional int64 beginTime = 2;
inline bool RetOnLineAtTimeInfo::has_begintime() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void RetOnLineAtTimeInfo::set_has_begintime() {
  _has_bits_[0] |= 0x00000002u;
}
inline void RetOnLineAtTimeInfo::clear_has_begintime() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void RetOnLineAtTimeInfo::clear_begintime() {
  begintime_ = GOOGLE_LONGLONG(0);
  clear_has_begintime();
}
inline ::google::protobuf::int64 RetOnLineAtTimeInfo::begintime() const {
  return begintime_;
}
inline void RetOnLineAtTimeInfo::set_begintime(::google::protobuf::int64 value) {
  set_has_begintime();
  begintime_ = value;
}

// optional int64 endTime = 3;
inline bool RetOnLineAtTimeInfo::has_endtime() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void RetOnLineAtTimeInfo::set_has_endtime() {
  _has_bits_[0] |= 0x00000004u;
}
inline void RetOnLineAtTimeInfo::clear_has_endtime() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void RetOnLineAtTimeInfo::clear_endtime() {
  endtime_ = GOOGLE_LONGLONG(0);
  clear_has_endtime();
}
inline ::google::protobuf::int64 RetOnLineAtTimeInfo::endtime() const {
  return endtime_;
}
inline void RetOnLineAtTimeInfo::set_endtime(::google::protobuf::int64 value) {
  set_has_endtime();
  endtime_ = value;
}

// repeated .OnLineAtTimeInfo.AtTimeInfo atTimeList = 4;
inline int RetOnLineAtTimeInfo::attimelist_size() const {
  return attimelist_.size();
}
inline void RetOnLineAtTimeInfo::clear_attimelist() {
  attimelist_.Clear();
}
inline const ::OnLineAtTimeInfo::AtTimeInfo& RetOnLineAtTimeInfo::attimelist(int index) const {
  return attimelist_.Get(index);
}
inline ::OnLineAtTimeInfo::AtTimeInfo* RetOnLineAtTimeInfo::mutable_attimelist(int index) {
  return attimelist_.Mutable(index);
}
inline ::OnLineAtTimeInfo::AtTimeInfo* RetOnLineAtTimeInfo::add_attimelist() {
  return attimelist_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::OnLineAtTimeInfo::AtTimeInfo >&
RetOnLineAtTimeInfo::attimelist() const {
  return attimelist_;
}
inline ::google::protobuf::RepeatedPtrField< ::OnLineAtTimeInfo::AtTimeInfo >*
RetOnLineAtTimeInfo::mutable_attimelist() {
  return &attimelist_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace OnLineAtTimeInfo

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Activity_2fOnLineAtTime_2eproto__INCLUDED
