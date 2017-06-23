// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: WorldBoss/WorldBoss.proto

#ifndef PROTOBUF_WorldBoss_2fWorldBoss_2eproto__INCLUDED
#define PROTOBUF_WorldBoss_2fWorldBoss_2eproto__INCLUDED

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

namespace WorldBoss {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_WorldBoss_2fWorldBoss_2eproto();
void protobuf_AssignDesc_WorldBoss_2fWorldBoss_2eproto();
void protobuf_ShutdownFile_WorldBoss_2fWorldBoss_2eproto();

class WorldBossInfoList;
class SingleWorldBossInfo;

// ===================================================================

class WorldBossInfoList : public ::google::protobuf::Message {
 public:
  WorldBossInfoList();
  virtual ~WorldBossInfoList();

  WorldBossInfoList(const WorldBossInfoList& from);

  inline WorldBossInfoList& operator=(const WorldBossInfoList& from) {
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
  static const WorldBossInfoList& default_instance();

  void Swap(WorldBossInfoList* other);

  // implements Message ----------------------------------------------

  WorldBossInfoList* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const WorldBossInfoList& from);
  void MergeFrom(const WorldBossInfoList& from);
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

  // repeated .WorldBoss.SingleWorldBossInfo boosInfoList = 1;
  inline int boosinfolist_size() const;
  inline void clear_boosinfolist();
  static const int kBoosInfoListFieldNumber = 1;
  inline const ::WorldBoss::SingleWorldBossInfo& boosinfolist(int index) const;
  inline ::WorldBoss::SingleWorldBossInfo* mutable_boosinfolist(int index);
  inline ::WorldBoss::SingleWorldBossInfo* add_boosinfolist();
  inline const ::google::protobuf::RepeatedPtrField< ::WorldBoss::SingleWorldBossInfo >&
      boosinfolist() const;
  inline ::google::protobuf::RepeatedPtrField< ::WorldBoss::SingleWorldBossInfo >*
      mutable_boosinfolist();

  // @@protoc_insertion_point(class_scope:WorldBoss.WorldBossInfoList)
 private:

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::RepeatedPtrField< ::WorldBoss::SingleWorldBossInfo > boosinfolist_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];

  friend void  protobuf_AddDesc_WorldBoss_2fWorldBoss_2eproto();
  friend void protobuf_AssignDesc_WorldBoss_2fWorldBoss_2eproto();
  friend void protobuf_ShutdownFile_WorldBoss_2fWorldBoss_2eproto();

  void InitAsDefaultInstance();
  static WorldBossInfoList* default_instance_;
};
// -------------------------------------------------------------------

class SingleWorldBossInfo : public ::google::protobuf::Message {
 public:
  SingleWorldBossInfo();
  virtual ~SingleWorldBossInfo();

  SingleWorldBossInfo(const SingleWorldBossInfo& from);

  inline SingleWorldBossInfo& operator=(const SingleWorldBossInfo& from) {
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
  static const SingleWorldBossInfo& default_instance();

  void Swap(SingleWorldBossInfo* other);

  // implements Message ----------------------------------------------

  SingleWorldBossInfo* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const SingleWorldBossInfo& from);
  void MergeFrom(const SingleWorldBossInfo& from);
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

  // optional int32 bossKey = 1;
  inline bool has_bosskey() const;
  inline void clear_bosskey();
  static const int kBossKeyFieldNumber = 1;
  inline ::google::protobuf::int32 bosskey() const;
  inline void set_bosskey(::google::protobuf::int32 value);

  // optional bool isActive = 2;
  inline bool has_isactive() const;
  inline void clear_isactive();
  static const int kIsActiveFieldNumber = 2;
  inline bool isactive() const;
  inline void set_isactive(bool value);

  // optional int64 bossId = 3;
  inline bool has_bossid() const;
  inline void clear_bossid();
  static const int kBossIdFieldNumber = 3;
  inline ::google::protobuf::int64 bossid() const;
  inline void set_bossid(::google::protobuf::int64 value);

  // optional int64 reflushTime = 4;
  inline bool has_reflushtime() const;
  inline void clear_reflushtime();
  static const int kReflushTimeFieldNumber = 4;
  inline ::google::protobuf::int64 reflushtime() const;
  inline void set_reflushtime(::google::protobuf::int64 value);

  // optional int64 playerID = 5;
  inline bool has_playerid() const;
  inline void clear_playerid();
  static const int kPlayerIDFieldNumber = 5;
  inline ::google::protobuf::int64 playerid() const;
  inline void set_playerid(::google::protobuf::int64 value);

  // optional bytes playerName = 6;
  inline bool has_playername() const;
  inline void clear_playername();
  static const int kPlayerNameFieldNumber = 6;
  inline const ::std::string& playername() const;
  inline void set_playername(const ::std::string& value);
  inline void set_playername(const char* value);
  inline void set_playername(const void* value, size_t size);
  inline ::std::string* mutable_playername();
  inline ::std::string* release_playername();
  inline void set_allocated_playername(::std::string* playername);

  // optional int32 vipLevel = 7;
  inline bool has_viplevel() const;
  inline void clear_viplevel();
  static const int kVipLevelFieldNumber = 7;
  inline ::google::protobuf::int32 viplevel() const;
  inline void set_viplevel(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:WorldBoss.SingleWorldBossInfo)
 private:
  inline void set_has_bosskey();
  inline void clear_has_bosskey();
  inline void set_has_isactive();
  inline void clear_has_isactive();
  inline void set_has_bossid();
  inline void clear_has_bossid();
  inline void set_has_reflushtime();
  inline void clear_has_reflushtime();
  inline void set_has_playerid();
  inline void clear_has_playerid();
  inline void set_has_playername();
  inline void clear_has_playername();
  inline void set_has_viplevel();
  inline void clear_has_viplevel();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::int32 bosskey_;
  bool isactive_;
  ::google::protobuf::int64 bossid_;
  ::google::protobuf::int64 reflushtime_;
  ::google::protobuf::int64 playerid_;
  ::std::string* playername_;
  ::google::protobuf::int32 viplevel_;

  mutable int _cached_size_;
  ::google::protobuf::uint32 _has_bits_[(7 + 31) / 32];

  friend void  protobuf_AddDesc_WorldBoss_2fWorldBoss_2eproto();
  friend void protobuf_AssignDesc_WorldBoss_2fWorldBoss_2eproto();
  friend void protobuf_ShutdownFile_WorldBoss_2fWorldBoss_2eproto();

  void InitAsDefaultInstance();
  static SingleWorldBossInfo* default_instance_;
};
// ===================================================================


// ===================================================================

// WorldBossInfoList

// repeated .WorldBoss.SingleWorldBossInfo boosInfoList = 1;
inline int WorldBossInfoList::boosinfolist_size() const {
  return boosinfolist_.size();
}
inline void WorldBossInfoList::clear_boosinfolist() {
  boosinfolist_.Clear();
}
inline const ::WorldBoss::SingleWorldBossInfo& WorldBossInfoList::boosinfolist(int index) const {
  return boosinfolist_.Get(index);
}
inline ::WorldBoss::SingleWorldBossInfo* WorldBossInfoList::mutable_boosinfolist(int index) {
  return boosinfolist_.Mutable(index);
}
inline ::WorldBoss::SingleWorldBossInfo* WorldBossInfoList::add_boosinfolist() {
  return boosinfolist_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::WorldBoss::SingleWorldBossInfo >&
WorldBossInfoList::boosinfolist() const {
  return boosinfolist_;
}
inline ::google::protobuf::RepeatedPtrField< ::WorldBoss::SingleWorldBossInfo >*
WorldBossInfoList::mutable_boosinfolist() {
  return &boosinfolist_;
}

// -------------------------------------------------------------------

// SingleWorldBossInfo

// optional int32 bossKey = 1;
inline bool SingleWorldBossInfo::has_bosskey() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void SingleWorldBossInfo::set_has_bosskey() {
  _has_bits_[0] |= 0x00000001u;
}
inline void SingleWorldBossInfo::clear_has_bosskey() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void SingleWorldBossInfo::clear_bosskey() {
  bosskey_ = 0;
  clear_has_bosskey();
}
inline ::google::protobuf::int32 SingleWorldBossInfo::bosskey() const {
  return bosskey_;
}
inline void SingleWorldBossInfo::set_bosskey(::google::protobuf::int32 value) {
  set_has_bosskey();
  bosskey_ = value;
}

// optional bool isActive = 2;
inline bool SingleWorldBossInfo::has_isactive() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void SingleWorldBossInfo::set_has_isactive() {
  _has_bits_[0] |= 0x00000002u;
}
inline void SingleWorldBossInfo::clear_has_isactive() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void SingleWorldBossInfo::clear_isactive() {
  isactive_ = false;
  clear_has_isactive();
}
inline bool SingleWorldBossInfo::isactive() const {
  return isactive_;
}
inline void SingleWorldBossInfo::set_isactive(bool value) {
  set_has_isactive();
  isactive_ = value;
}

// optional int64 bossId = 3;
inline bool SingleWorldBossInfo::has_bossid() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void SingleWorldBossInfo::set_has_bossid() {
  _has_bits_[0] |= 0x00000004u;
}
inline void SingleWorldBossInfo::clear_has_bossid() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void SingleWorldBossInfo::clear_bossid() {
  bossid_ = GOOGLE_LONGLONG(0);
  clear_has_bossid();
}
inline ::google::protobuf::int64 SingleWorldBossInfo::bossid() const {
  return bossid_;
}
inline void SingleWorldBossInfo::set_bossid(::google::protobuf::int64 value) {
  set_has_bossid();
  bossid_ = value;
}

// optional int64 reflushTime = 4;
inline bool SingleWorldBossInfo::has_reflushtime() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void SingleWorldBossInfo::set_has_reflushtime() {
  _has_bits_[0] |= 0x00000008u;
}
inline void SingleWorldBossInfo::clear_has_reflushtime() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void SingleWorldBossInfo::clear_reflushtime() {
  reflushtime_ = GOOGLE_LONGLONG(0);
  clear_has_reflushtime();
}
inline ::google::protobuf::int64 SingleWorldBossInfo::reflushtime() const {
  return reflushtime_;
}
inline void SingleWorldBossInfo::set_reflushtime(::google::protobuf::int64 value) {
  set_has_reflushtime();
  reflushtime_ = value;
}

// optional int64 playerID = 5;
inline bool SingleWorldBossInfo::has_playerid() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void SingleWorldBossInfo::set_has_playerid() {
  _has_bits_[0] |= 0x00000010u;
}
inline void SingleWorldBossInfo::clear_has_playerid() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void SingleWorldBossInfo::clear_playerid() {
  playerid_ = GOOGLE_LONGLONG(0);
  clear_has_playerid();
}
inline ::google::protobuf::int64 SingleWorldBossInfo::playerid() const {
  return playerid_;
}
inline void SingleWorldBossInfo::set_playerid(::google::protobuf::int64 value) {
  set_has_playerid();
  playerid_ = value;
}

// optional bytes playerName = 6;
inline bool SingleWorldBossInfo::has_playername() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void SingleWorldBossInfo::set_has_playername() {
  _has_bits_[0] |= 0x00000020u;
}
inline void SingleWorldBossInfo::clear_has_playername() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void SingleWorldBossInfo::clear_playername() {
  if (playername_ != &::google::protobuf::internal::kEmptyString) {
    playername_->clear();
  }
  clear_has_playername();
}
inline const ::std::string& SingleWorldBossInfo::playername() const {
  return *playername_;
}
inline void SingleWorldBossInfo::set_playername(const ::std::string& value) {
  set_has_playername();
  if (playername_ == &::google::protobuf::internal::kEmptyString) {
    playername_ = new ::std::string;
  }
  playername_->assign(value);
}
inline void SingleWorldBossInfo::set_playername(const char* value) {
  set_has_playername();
  if (playername_ == &::google::protobuf::internal::kEmptyString) {
    playername_ = new ::std::string;
  }
  playername_->assign(value);
}
inline void SingleWorldBossInfo::set_playername(const void* value, size_t size) {
  set_has_playername();
  if (playername_ == &::google::protobuf::internal::kEmptyString) {
    playername_ = new ::std::string;
  }
  playername_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* SingleWorldBossInfo::mutable_playername() {
  set_has_playername();
  if (playername_ == &::google::protobuf::internal::kEmptyString) {
    playername_ = new ::std::string;
  }
  return playername_;
}
inline ::std::string* SingleWorldBossInfo::release_playername() {
  clear_has_playername();
  if (playername_ == &::google::protobuf::internal::kEmptyString) {
    return NULL;
  } else {
    ::std::string* temp = playername_;
    playername_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
    return temp;
  }
}
inline void SingleWorldBossInfo::set_allocated_playername(::std::string* playername) {
  if (playername_ != &::google::protobuf::internal::kEmptyString) {
    delete playername_;
  }
  if (playername) {
    set_has_playername();
    playername_ = playername;
  } else {
    clear_has_playername();
    playername_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  }
}

// optional int32 vipLevel = 7;
inline bool SingleWorldBossInfo::has_viplevel() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void SingleWorldBossInfo::set_has_viplevel() {
  _has_bits_[0] |= 0x00000040u;
}
inline void SingleWorldBossInfo::clear_has_viplevel() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void SingleWorldBossInfo::clear_viplevel() {
  viplevel_ = 0;
  clear_has_viplevel();
}
inline ::google::protobuf::int32 SingleWorldBossInfo::viplevel() const {
  return viplevel_;
}
inline void SingleWorldBossInfo::set_viplevel(::google::protobuf::int32 value) {
  set_has_viplevel();
  viplevel_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace WorldBoss

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_WorldBoss_2fWorldBoss_2eproto__INCLUDED
