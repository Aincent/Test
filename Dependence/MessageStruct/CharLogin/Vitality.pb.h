// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: CharLogin/Vitality.proto

#ifndef PROTOBUF_CharLogin_2fVitality_2eproto__INCLUDED
#define PROTOBUF_CharLogin_2fVitality_2eproto__INCLUDED

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

namespace VitalityProto {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_CharLogin_2fVitality_2eproto();
void protobuf_AssignDesc_CharLogin_2fVitality_2eproto();
void protobuf_ShutdownFile_CharLogin_2fVitality_2eproto();

class ProtoVitalityInfo;
class ProtoVitalityItem;
class ProtoVitalitySim;

// ===================================================================

class ProtoVitalityInfo : public ::google::protobuf::Message {
 public:
  ProtoVitalityInfo();
  virtual ~ProtoVitalityInfo();

  ProtoVitalityInfo(const ProtoVitalityInfo& from);

  inline ProtoVitalityInfo& operator=(const ProtoVitalityInfo& from) {
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
  static const ProtoVitalityInfo& default_instance();

  void Swap(ProtoVitalityInfo* other);

  // implements Message ----------------------------------------------

  ProtoVitalityInfo* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ProtoVitalityInfo& from);
  void MergeFrom(const ProtoVitalityInfo& from);
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

  // repeated .VitalityProto.ProtoVitalityItem item = 1;
  inline int item_size() const;
  inline void clear_item();
  static const int kItemFieldNumber = 1;
  inline const ::VitalityProto::ProtoVitalityItem& item(int index) const;
  inline ::VitalityProto::ProtoVitalityItem* mutable_item(int index);
  inline ::VitalityProto::ProtoVitalityItem* add_item();
  inline const ::google::protobuf::RepeatedPtrField< ::VitalityProto::ProtoVitalityItem >&
      item() const;
  inline ::google::protobuf::RepeatedPtrField< ::VitalityProto::ProtoVitalityItem >*
      mutable_item();

  // repeated bool award = 2;
  inline int award_size() const;
  inline void clear_award();
  static const int kAwardFieldNumber = 2;
  inline bool award(int index) const;
  inline void set_award(int index, bool value);
  inline void add_award(bool value);
  inline const ::google::protobuf::RepeatedField< bool >&
      award() const;
  inline ::google::protobuf::RepeatedField< bool >*
      mutable_award();

  // optional int32 score = 3;
  inline bool has_score() const;
  inline void clear_score();
  static const int kScoreFieldNumber = 3;
  inline ::google::protobuf::int32 score() const;
  inline void set_score(::google::protobuf::int32 value);

  // optional int64 time = 4;
  inline bool has_time() const;
  inline void clear_time();
  static const int kTimeFieldNumber = 4;
  inline ::google::protobuf::int64 time() const;
  inline void set_time(::google::protobuf::int64 value);

  // @@protoc_insertion_point(class_scope:VitalityProto.ProtoVitalityInfo)
 private:
  inline void set_has_score();
  inline void clear_has_score();
  inline void set_has_time();
  inline void clear_has_time();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedPtrField< ::VitalityProto::ProtoVitalityItem > item_;
  ::google::protobuf::RepeatedField< bool > award_;
  ::google::protobuf::int64 time_;
  ::google::protobuf::int32 score_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];

  friend void  protobuf_AddDesc_CharLogin_2fVitality_2eproto();
  friend void protobuf_AssignDesc_CharLogin_2fVitality_2eproto();
  friend void protobuf_ShutdownFile_CharLogin_2fVitality_2eproto();

  void InitAsDefaultInstance();
  static ProtoVitalityInfo* default_instance_;
};
// -------------------------------------------------------------------

class ProtoVitalityItem : public ::google::protobuf::Message {
 public:
  ProtoVitalityItem();
  virtual ~ProtoVitalityItem();

  ProtoVitalityItem(const ProtoVitalityItem& from);

  inline ProtoVitalityItem& operator=(const ProtoVitalityItem& from) {
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
  static const ProtoVitalityItem& default_instance();

  void Swap(ProtoVitalityItem* other);

  // implements Message ----------------------------------------------

  ProtoVitalityItem* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ProtoVitalityItem& from);
  void MergeFrom(const ProtoVitalityItem& from);
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

  // required int32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);

  // required int32 count = 2;
  inline bool has_count() const;
  inline void clear_count();
  static const int kCountFieldNumber = 2;
  inline ::google::protobuf::int32 count() const;
  inline void set_count(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:VitalityProto.ProtoVitalityItem)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_count();
  inline void clear_has_count();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 id_;
  ::google::protobuf::int32 count_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_CharLogin_2fVitality_2eproto();
  friend void protobuf_AssignDesc_CharLogin_2fVitality_2eproto();
  friend void protobuf_ShutdownFile_CharLogin_2fVitality_2eproto();

  void InitAsDefaultInstance();
  static ProtoVitalityItem* default_instance_;
};
// -------------------------------------------------------------------

class ProtoVitalitySim : public ::google::protobuf::Message {
 public:
  ProtoVitalitySim();
  virtual ~ProtoVitalitySim();

  ProtoVitalitySim(const ProtoVitalitySim& from);

  inline ProtoVitalitySim& operator=(const ProtoVitalitySim& from) {
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
  static const ProtoVitalitySim& default_instance();

  void Swap(ProtoVitalitySim* other);

  // implements Message ----------------------------------------------

  ProtoVitalitySim* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const ProtoVitalitySim& from);
  void MergeFrom(const ProtoVitalitySim& from);
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

  // required int32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);

  // required int32 count = 2;
  inline bool has_count() const;
  inline void clear_count();
  static const int kCountFieldNumber = 2;
  inline ::google::protobuf::int32 count() const;
  inline void set_count(::google::protobuf::int32 value);

  // required int32 score = 3;
  inline bool has_score() const;
  inline void clear_score();
  static const int kScoreFieldNumber = 3;
  inline ::google::protobuf::int32 score() const;
  inline void set_score(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:VitalityProto.ProtoVitalitySim)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_count();
  inline void clear_has_count();
  inline void set_has_score();
  inline void clear_has_score();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 id_;
  ::google::protobuf::int32 count_;
  ::google::protobuf::int32 score_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_CharLogin_2fVitality_2eproto();
  friend void protobuf_AssignDesc_CharLogin_2fVitality_2eproto();
  friend void protobuf_ShutdownFile_CharLogin_2fVitality_2eproto();

  void InitAsDefaultInstance();
  static ProtoVitalitySim* default_instance_;
};
// ===================================================================


// ===================================================================

// ProtoVitalityInfo

// repeated .VitalityProto.ProtoVitalityItem item = 1;
inline int ProtoVitalityInfo::item_size() const {
  return item_.size();
}
inline void ProtoVitalityInfo::clear_item() {
  item_.Clear();
}
inline const ::VitalityProto::ProtoVitalityItem& ProtoVitalityInfo::item(int index) const {
  return item_.Get(index);
}
inline ::VitalityProto::ProtoVitalityItem* ProtoVitalityInfo::mutable_item(int index) {
  return item_.Mutable(index);
}
inline ::VitalityProto::ProtoVitalityItem* ProtoVitalityInfo::add_item() {
  return item_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::VitalityProto::ProtoVitalityItem >&
ProtoVitalityInfo::item() const {
  return item_;
}
inline ::google::protobuf::RepeatedPtrField< ::VitalityProto::ProtoVitalityItem >*
ProtoVitalityInfo::mutable_item() {
  return &item_;
}

// repeated bool award = 2;
inline int ProtoVitalityInfo::award_size() const {
  return award_.size();
}
inline void ProtoVitalityInfo::clear_award() {
  award_.Clear();
}
inline bool ProtoVitalityInfo::award(int index) const {
  return award_.Get(index);
}
inline void ProtoVitalityInfo::set_award(int index, bool value) {
  award_.Set(index, value);
}
inline void ProtoVitalityInfo::add_award(bool value) {
  award_.Add(value);
}
inline const ::google::protobuf::RepeatedField< bool >&
ProtoVitalityInfo::award() const {
  return award_;
}
inline ::google::protobuf::RepeatedField< bool >*
ProtoVitalityInfo::mutable_award() {
  return &award_;
}

// optional int32 score = 3;
inline bool ProtoVitalityInfo::has_score() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ProtoVitalityInfo::set_has_score() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ProtoVitalityInfo::clear_has_score() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ProtoVitalityInfo::clear_score() {
  score_ = 0;
  clear_has_score();
}
inline ::google::protobuf::int32 ProtoVitalityInfo::score() const {
  return score_;
}
inline void ProtoVitalityInfo::set_score(::google::protobuf::int32 value) {
  set_has_score();
  score_ = value;
}

// optional int64 time = 4;
inline bool ProtoVitalityInfo::has_time() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void ProtoVitalityInfo::set_has_time() {
  _has_bits_[0] |= 0x00000008u;
}
inline void ProtoVitalityInfo::clear_has_time() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void ProtoVitalityInfo::clear_time() {
  time_ = GOOGLE_LONGLONG(0);
  clear_has_time();
}
inline ::google::protobuf::int64 ProtoVitalityInfo::time() const {
  return time_;
}
inline void ProtoVitalityInfo::set_time(::google::protobuf::int64 value) {
  set_has_time();
  time_ = value;
}

// -------------------------------------------------------------------

// ProtoVitalityItem

// required int32 id = 1;
inline bool ProtoVitalityItem::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ProtoVitalityItem::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ProtoVitalityItem::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ProtoVitalityItem::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 ProtoVitalityItem::id() const {
  return id_;
}
inline void ProtoVitalityItem::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
}

// required int32 count = 2;
inline bool ProtoVitalityItem::has_count() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ProtoVitalityItem::set_has_count() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ProtoVitalityItem::clear_has_count() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ProtoVitalityItem::clear_count() {
  count_ = 0;
  clear_has_count();
}
inline ::google::protobuf::int32 ProtoVitalityItem::count() const {
  return count_;
}
inline void ProtoVitalityItem::set_count(::google::protobuf::int32 value) {
  set_has_count();
  count_ = value;
}

// -------------------------------------------------------------------

// ProtoVitalitySim

// required int32 id = 1;
inline bool ProtoVitalitySim::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void ProtoVitalitySim::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void ProtoVitalitySim::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void ProtoVitalitySim::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 ProtoVitalitySim::id() const {
  return id_;
}
inline void ProtoVitalitySim::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
}

// required int32 count = 2;
inline bool ProtoVitalitySim::has_count() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void ProtoVitalitySim::set_has_count() {
  _has_bits_[0] |= 0x00000002u;
}
inline void ProtoVitalitySim::clear_has_count() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void ProtoVitalitySim::clear_count() {
  count_ = 0;
  clear_has_count();
}
inline ::google::protobuf::int32 ProtoVitalitySim::count() const {
  return count_;
}
inline void ProtoVitalitySim::set_count(::google::protobuf::int32 value) {
  set_has_count();
  count_ = value;
}

// required int32 score = 3;
inline bool ProtoVitalitySim::has_score() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void ProtoVitalitySim::set_has_score() {
  _has_bits_[0] |= 0x00000004u;
}
inline void ProtoVitalitySim::clear_has_score() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void ProtoVitalitySim::clear_score() {
  score_ = 0;
  clear_has_score();
}
inline ::google::protobuf::int32 ProtoVitalitySim::score() const {
  return score_;
}
inline void ProtoVitalitySim::set_score(::google::protobuf::int32 value) {
  set_has_score();
  score_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace VitalityProto

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_CharLogin_2fVitality_2eproto__INCLUDED
