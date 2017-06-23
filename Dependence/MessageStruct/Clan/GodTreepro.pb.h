// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Clan/GodTreepro.proto

#ifndef PROTOBUF_Clan_2fGodTreepro_2eproto__INCLUDED
#define PROTOBUF_Clan_2fGodTreepro_2eproto__INCLUDED

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

namespace GodTreepro {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_Clan_2fGodTreepro_2eproto();
void protobuf_AssignDesc_Clan_2fGodTreepro_2eproto();
void protobuf_ShutdownFile_Clan_2fGodTreepro_2eproto();

class OneItem;
class GodTreeData;
class SubmitItemReturn;
class CollectData;
class GodTreeDBData;
class GmBuckleItem;

// ===================================================================

class OneItem : public ::google::protobuf::Message {
 public:
  OneItem();
  virtual ~OneItem();

  OneItem(const OneItem& from);

  inline OneItem& operator=(const OneItem& from) {
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
  static const OneItem& default_instance();

  void Swap(OneItem* other);

  // implements Message ----------------------------------------------

  OneItem* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const OneItem& from);
  void MergeFrom(const OneItem& from);
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

  // optional int32 id = 1;
  inline bool has_id() const;
  inline void clear_id();
  static const int kIdFieldNumber = 1;
  inline ::google::protobuf::int32 id() const;
  inline void set_id(::google::protobuf::int32 value);

  // optional int32 counts = 2;
  inline bool has_counts() const;
  inline void clear_counts();
  static const int kCountsFieldNumber = 2;
  inline ::google::protobuf::int32 counts() const;
  inline void set_counts(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:GodTreepro.OneItem)
 private:
  inline void set_has_id();
  inline void clear_has_id();
  inline void set_has_counts();
  inline void clear_has_counts();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 id_;
  ::google::protobuf::int32 counts_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_Clan_2fGodTreepro_2eproto();
  friend void protobuf_AssignDesc_Clan_2fGodTreepro_2eproto();
  friend void protobuf_ShutdownFile_Clan_2fGodTreepro_2eproto();

  void InitAsDefaultInstance();
  static OneItem* default_instance_;
};
// -------------------------------------------------------------------

class GodTreeData : public ::google::protobuf::Message {
 public:
  GodTreeData();
  virtual ~GodTreeData();

  GodTreeData(const GodTreeData& from);

  inline GodTreeData& operator=(const GodTreeData& from) {
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
  static const GodTreeData& default_instance();

  void Swap(GodTreeData* other);

  // implements Message ----------------------------------------------

  GodTreeData* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const GodTreeData& from);
  void MergeFrom(const GodTreeData& from);
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

  // repeated .GodTreepro.OneItem items = 1;
  inline int items_size() const;
  inline void clear_items();
  static const int kItemsFieldNumber = 1;
  inline const ::GodTreepro::OneItem& items(int index) const;
  inline ::GodTreepro::OneItem* mutable_items(int index);
  inline ::GodTreepro::OneItem* add_items();
  inline const ::google::protobuf::RepeatedPtrField< ::GodTreepro::OneItem >&
      items() const;
  inline ::google::protobuf::RepeatedPtrField< ::GodTreepro::OneItem >*
      mutable_items();

  // optional int32 curExp = 2;
  inline bool has_curexp() const;
  inline void clear_curexp();
  static const int kCurExpFieldNumber = 2;
  inline ::google::protobuf::int32 curexp() const;
  inline void set_curexp(::google::protobuf::int32 value);

  // optional int32 level = 3;
  inline bool has_level() const;
  inline void clear_level();
  static const int kLevelFieldNumber = 3;
  inline ::google::protobuf::int32 level() const;
  inline void set_level(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:GodTreepro.GodTreeData)
 private:
  inline void set_has_curexp();
  inline void clear_has_curexp();
  inline void set_has_level();
  inline void clear_has_level();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedPtrField< ::GodTreepro::OneItem > items_;
  ::google::protobuf::int32 curexp_;
  ::google::protobuf::int32 level_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_Clan_2fGodTreepro_2eproto();
  friend void protobuf_AssignDesc_Clan_2fGodTreepro_2eproto();
  friend void protobuf_ShutdownFile_Clan_2fGodTreepro_2eproto();

  void InitAsDefaultInstance();
  static GodTreeData* default_instance_;
};
// -------------------------------------------------------------------

class SubmitItemReturn : public ::google::protobuf::Message {
 public:
  SubmitItemReturn();
  virtual ~SubmitItemReturn();

  SubmitItemReturn(const SubmitItemReturn& from);

  inline SubmitItemReturn& operator=(const SubmitItemReturn& from) {
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
  static const SubmitItemReturn& default_instance();

  void Swap(SubmitItemReturn* other);

  // implements Message ----------------------------------------------

  SubmitItemReturn* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const SubmitItemReturn& from);
  void MergeFrom(const SubmitItemReturn& from);
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

  // optional int32 error = 1;
  inline bool has_error() const;
  inline void clear_error();
  static const int kErrorFieldNumber = 1;
  inline ::google::protobuf::int32 error() const;
  inline void set_error(::google::protobuf::int32 value);

  // optional .GodTreepro.GodTreeData data = 2;
  inline bool has_data() const;
  inline void clear_data();
  static const int kDataFieldNumber = 2;
  inline const ::GodTreepro::GodTreeData& data() const;
  inline ::GodTreepro::GodTreeData* mutable_data();
  inline ::GodTreepro::GodTreeData* release_data();
  inline void set_allocated_data(::GodTreepro::GodTreeData* data);

  // @@protoc_insertion_point(class_scope:GodTreepro.SubmitItemReturn)
 private:
  inline void set_has_error();
  inline void clear_has_error();
  inline void set_has_data();
  inline void clear_has_data();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::GodTreepro::GodTreeData* data_;
  ::google::protobuf::int32 error_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_Clan_2fGodTreepro_2eproto();
  friend void protobuf_AssignDesc_Clan_2fGodTreepro_2eproto();
  friend void protobuf_ShutdownFile_Clan_2fGodTreepro_2eproto();

  void InitAsDefaultInstance();
  static SubmitItemReturn* default_instance_;
};
// -------------------------------------------------------------------

class CollectData : public ::google::protobuf::Message {
 public:
  CollectData();
  virtual ~CollectData();

  CollectData(const CollectData& from);

  inline CollectData& operator=(const CollectData& from) {
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
  static const CollectData& default_instance();

  void Swap(CollectData* other);

  // implements Message ----------------------------------------------

  CollectData* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const CollectData& from);
  void MergeFrom(const CollectData& from);
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

  // optional int32 itemID = 1;
  inline bool has_itemid() const;
  inline void clear_itemid();
  static const int kItemIDFieldNumber = 1;
  inline ::google::protobuf::int32 itemid() const;
  inline void set_itemid(::google::protobuf::int32 value);

  // optional int32 counts = 2;
  inline bool has_counts() const;
  inline void clear_counts();
  static const int kCountsFieldNumber = 2;
  inline ::google::protobuf::int32 counts() const;
  inline void set_counts(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:GodTreepro.CollectData)
 private:
  inline void set_has_itemid();
  inline void clear_has_itemid();
  inline void set_has_counts();
  inline void clear_has_counts();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 itemid_;
  ::google::protobuf::int32 counts_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_Clan_2fGodTreepro_2eproto();
  friend void protobuf_AssignDesc_Clan_2fGodTreepro_2eproto();
  friend void protobuf_ShutdownFile_Clan_2fGodTreepro_2eproto();

  void InitAsDefaultInstance();
  static CollectData* default_instance_;
};
// -------------------------------------------------------------------

class GodTreeDBData : public ::google::protobuf::Message {
 public:
  GodTreeDBData();
  virtual ~GodTreeDBData();

  GodTreeDBData(const GodTreeDBData& from);

  inline GodTreeDBData& operator=(const GodTreeDBData& from) {
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
  static const GodTreeDBData& default_instance();

  void Swap(GodTreeDBData* other);

  // implements Message ----------------------------------------------

  GodTreeDBData* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const GodTreeDBData& from);
  void MergeFrom(const GodTreeDBData& from);
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

  // repeated .GodTreepro.CollectData collect = 1;
  inline int collect_size() const;
  inline void clear_collect();
  static const int kCollectFieldNumber = 1;
  inline const ::GodTreepro::CollectData& collect(int index) const;
  inline ::GodTreepro::CollectData* mutable_collect(int index);
  inline ::GodTreepro::CollectData* add_collect();
  inline const ::google::protobuf::RepeatedPtrField< ::GodTreepro::CollectData >&
      collect() const;
  inline ::google::protobuf::RepeatedPtrField< ::GodTreepro::CollectData >*
      mutable_collect();

  // optional int32 exp = 2;
  inline bool has_exp() const;
  inline void clear_exp();
  static const int kExpFieldNumber = 2;
  inline ::google::protobuf::int32 exp() const;
  inline void set_exp(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:GodTreepro.GodTreeDBData)
 private:
  inline void set_has_exp();
  inline void clear_has_exp();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedPtrField< ::GodTreepro::CollectData > collect_;
  ::google::protobuf::int32 exp_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];

  friend void  protobuf_AddDesc_Clan_2fGodTreepro_2eproto();
  friend void protobuf_AssignDesc_Clan_2fGodTreepro_2eproto();
  friend void protobuf_ShutdownFile_Clan_2fGodTreepro_2eproto();

  void InitAsDefaultInstance();
  static GodTreeDBData* default_instance_;
};
// -------------------------------------------------------------------

class GmBuckleItem : public ::google::protobuf::Message {
 public:
  GmBuckleItem();
  virtual ~GmBuckleItem();

  GmBuckleItem(const GmBuckleItem& from);

  inline GmBuckleItem& operator=(const GmBuckleItem& from) {
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
  static const GmBuckleItem& default_instance();

  void Swap(GmBuckleItem* other);

  // implements Message ----------------------------------------------

  GmBuckleItem* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const GmBuckleItem& from);
  void MergeFrom(const GmBuckleItem& from);
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

  // repeated .GodTreepro.OneItem items = 1;
  inline int items_size() const;
  inline void clear_items();
  static const int kItemsFieldNumber = 1;
  inline const ::GodTreepro::OneItem& items(int index) const;
  inline ::GodTreepro::OneItem* mutable_items(int index);
  inline ::GodTreepro::OneItem* add_items();
  inline const ::google::protobuf::RepeatedPtrField< ::GodTreepro::OneItem >&
      items() const;
  inline ::google::protobuf::RepeatedPtrField< ::GodTreepro::OneItem >*
      mutable_items();

  // optional int32 level = 2;
  inline bool has_level() const;
  inline void clear_level();
  static const int kLevelFieldNumber = 2;
  inline ::google::protobuf::int32 level() const;
  inline void set_level(::google::protobuf::int32 value);

  // optional int32 error = 3;
  inline bool has_error() const;
  inline void clear_error();
  static const int kErrorFieldNumber = 3;
  inline ::google::protobuf::int32 error() const;
  inline void set_error(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:GodTreepro.GmBuckleItem)
 private:
  inline void set_has_level();
  inline void clear_has_level();
  inline void set_has_error();
  inline void clear_has_error();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedPtrField< ::GodTreepro::OneItem > items_;
  ::google::protobuf::int32 level_;
  ::google::protobuf::int32 error_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];

  friend void  protobuf_AddDesc_Clan_2fGodTreepro_2eproto();
  friend void protobuf_AssignDesc_Clan_2fGodTreepro_2eproto();
  friend void protobuf_ShutdownFile_Clan_2fGodTreepro_2eproto();

  void InitAsDefaultInstance();
  static GmBuckleItem* default_instance_;
};
// ===================================================================


// ===================================================================

// OneItem

// optional int32 id = 1;
inline bool OneItem::has_id() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void OneItem::set_has_id() {
  _has_bits_[0] |= 0x00000001u;
}
inline void OneItem::clear_has_id() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void OneItem::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 OneItem::id() const {
  return id_;
}
inline void OneItem::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
}

// optional int32 counts = 2;
inline bool OneItem::has_counts() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void OneItem::set_has_counts() {
  _has_bits_[0] |= 0x00000002u;
}
inline void OneItem::clear_has_counts() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void OneItem::clear_counts() {
  counts_ = 0;
  clear_has_counts();
}
inline ::google::protobuf::int32 OneItem::counts() const {
  return counts_;
}
inline void OneItem::set_counts(::google::protobuf::int32 value) {
  set_has_counts();
  counts_ = value;
}

// -------------------------------------------------------------------

// GodTreeData

// repeated .GodTreepro.OneItem items = 1;
inline int GodTreeData::items_size() const {
  return items_.size();
}
inline void GodTreeData::clear_items() {
  items_.Clear();
}
inline const ::GodTreepro::OneItem& GodTreeData::items(int index) const {
  return items_.Get(index);
}
inline ::GodTreepro::OneItem* GodTreeData::mutable_items(int index) {
  return items_.Mutable(index);
}
inline ::GodTreepro::OneItem* GodTreeData::add_items() {
  return items_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::GodTreepro::OneItem >&
GodTreeData::items() const {
  return items_;
}
inline ::google::protobuf::RepeatedPtrField< ::GodTreepro::OneItem >*
GodTreeData::mutable_items() {
  return &items_;
}

// optional int32 curExp = 2;
inline bool GodTreeData::has_curexp() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void GodTreeData::set_has_curexp() {
  _has_bits_[0] |= 0x00000002u;
}
inline void GodTreeData::clear_has_curexp() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void GodTreeData::clear_curexp() {
  curexp_ = 0;
  clear_has_curexp();
}
inline ::google::protobuf::int32 GodTreeData::curexp() const {
  return curexp_;
}
inline void GodTreeData::set_curexp(::google::protobuf::int32 value) {
  set_has_curexp();
  curexp_ = value;
}

// optional int32 level = 3;
inline bool GodTreeData::has_level() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void GodTreeData::set_has_level() {
  _has_bits_[0] |= 0x00000004u;
}
inline void GodTreeData::clear_has_level() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void GodTreeData::clear_level() {
  level_ = 0;
  clear_has_level();
}
inline ::google::protobuf::int32 GodTreeData::level() const {
  return level_;
}
inline void GodTreeData::set_level(::google::protobuf::int32 value) {
  set_has_level();
  level_ = value;
}

// -------------------------------------------------------------------

// SubmitItemReturn

// optional int32 error = 1;
inline bool SubmitItemReturn::has_error() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void SubmitItemReturn::set_has_error() {
  _has_bits_[0] |= 0x00000001u;
}
inline void SubmitItemReturn::clear_has_error() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void SubmitItemReturn::clear_error() {
  error_ = 0;
  clear_has_error();
}
inline ::google::protobuf::int32 SubmitItemReturn::error() const {
  return error_;
}
inline void SubmitItemReturn::set_error(::google::protobuf::int32 value) {
  set_has_error();
  error_ = value;
}

// optional .GodTreepro.GodTreeData data = 2;
inline bool SubmitItemReturn::has_data() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void SubmitItemReturn::set_has_data() {
  _has_bits_[0] |= 0x00000002u;
}
inline void SubmitItemReturn::clear_has_data() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void SubmitItemReturn::clear_data() {
  if (data_ != NULL) data_->::GodTreepro::GodTreeData::Clear();
  clear_has_data();
}
inline const ::GodTreepro::GodTreeData& SubmitItemReturn::data() const {
  return data_ != NULL ? *data_ : *default_instance_->data_;
}
inline ::GodTreepro::GodTreeData* SubmitItemReturn::mutable_data() {
  set_has_data();
  if (data_ == NULL) data_ = new ::GodTreepro::GodTreeData;
  return data_;
}
inline ::GodTreepro::GodTreeData* SubmitItemReturn::release_data() {
  clear_has_data();
  ::GodTreepro::GodTreeData* temp = data_;
  data_ = NULL;
  return temp;
}
inline void SubmitItemReturn::set_allocated_data(::GodTreepro::GodTreeData* data) {
  delete data_;
  data_ = data;
  if (data) {
    set_has_data();
  } else {
    clear_has_data();
  }
}

// -------------------------------------------------------------------

// CollectData

// optional int32 itemID = 1;
inline bool CollectData::has_itemid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void CollectData::set_has_itemid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void CollectData::clear_has_itemid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void CollectData::clear_itemid() {
  itemid_ = 0;
  clear_has_itemid();
}
inline ::google::protobuf::int32 CollectData::itemid() const {
  return itemid_;
}
inline void CollectData::set_itemid(::google::protobuf::int32 value) {
  set_has_itemid();
  itemid_ = value;
}

// optional int32 counts = 2;
inline bool CollectData::has_counts() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void CollectData::set_has_counts() {
  _has_bits_[0] |= 0x00000002u;
}
inline void CollectData::clear_has_counts() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void CollectData::clear_counts() {
  counts_ = 0;
  clear_has_counts();
}
inline ::google::protobuf::int32 CollectData::counts() const {
  return counts_;
}
inline void CollectData::set_counts(::google::protobuf::int32 value) {
  set_has_counts();
  counts_ = value;
}

// -------------------------------------------------------------------

// GodTreeDBData

// repeated .GodTreepro.CollectData collect = 1;
inline int GodTreeDBData::collect_size() const {
  return collect_.size();
}
inline void GodTreeDBData::clear_collect() {
  collect_.Clear();
}
inline const ::GodTreepro::CollectData& GodTreeDBData::collect(int index) const {
  return collect_.Get(index);
}
inline ::GodTreepro::CollectData* GodTreeDBData::mutable_collect(int index) {
  return collect_.Mutable(index);
}
inline ::GodTreepro::CollectData* GodTreeDBData::add_collect() {
  return collect_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::GodTreepro::CollectData >&
GodTreeDBData::collect() const {
  return collect_;
}
inline ::google::protobuf::RepeatedPtrField< ::GodTreepro::CollectData >*
GodTreeDBData::mutable_collect() {
  return &collect_;
}

// optional int32 exp = 2;
inline bool GodTreeDBData::has_exp() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void GodTreeDBData::set_has_exp() {
  _has_bits_[0] |= 0x00000002u;
}
inline void GodTreeDBData::clear_has_exp() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void GodTreeDBData::clear_exp() {
  exp_ = 0;
  clear_has_exp();
}
inline ::google::protobuf::int32 GodTreeDBData::exp() const {
  return exp_;
}
inline void GodTreeDBData::set_exp(::google::protobuf::int32 value) {
  set_has_exp();
  exp_ = value;
}

// -------------------------------------------------------------------

// GmBuckleItem

// repeated .GodTreepro.OneItem items = 1;
inline int GmBuckleItem::items_size() const {
  return items_.size();
}
inline void GmBuckleItem::clear_items() {
  items_.Clear();
}
inline const ::GodTreepro::OneItem& GmBuckleItem::items(int index) const {
  return items_.Get(index);
}
inline ::GodTreepro::OneItem* GmBuckleItem::mutable_items(int index) {
  return items_.Mutable(index);
}
inline ::GodTreepro::OneItem* GmBuckleItem::add_items() {
  return items_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::GodTreepro::OneItem >&
GmBuckleItem::items() const {
  return items_;
}
inline ::google::protobuf::RepeatedPtrField< ::GodTreepro::OneItem >*
GmBuckleItem::mutable_items() {
  return &items_;
}

// optional int32 level = 2;
inline bool GmBuckleItem::has_level() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void GmBuckleItem::set_has_level() {
  _has_bits_[0] |= 0x00000002u;
}
inline void GmBuckleItem::clear_has_level() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void GmBuckleItem::clear_level() {
  level_ = 0;
  clear_has_level();
}
inline ::google::protobuf::int32 GmBuckleItem::level() const {
  return level_;
}
inline void GmBuckleItem::set_level(::google::protobuf::int32 value) {
  set_has_level();
  level_ = value;
}

// optional int32 error = 3;
inline bool GmBuckleItem::has_error() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void GmBuckleItem::set_has_error() {
  _has_bits_[0] |= 0x00000004u;
}
inline void GmBuckleItem::clear_has_error() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void GmBuckleItem::clear_error() {
  error_ = 0;
  clear_has_error();
}
inline ::google::protobuf::int32 GmBuckleItem::error() const {
  return error_;
}
inline void GmBuckleItem::set_error(::google::protobuf::int32 value) {
  set_has_error();
  error_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace GodTreepro

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_Clan_2fGodTreepro_2eproto__INCLUDED
