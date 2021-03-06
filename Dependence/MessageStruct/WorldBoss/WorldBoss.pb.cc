// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: WorldBoss/WorldBoss.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "WorldBoss/WorldBoss.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace WorldBoss {

namespace {

const ::google::protobuf::Descriptor* WorldBossInfoList_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  WorldBossInfoList_reflection_ = NULL;
const ::google::protobuf::Descriptor* SingleWorldBossInfo_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  SingleWorldBossInfo_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_WorldBoss_2fWorldBoss_2eproto() {
  protobuf_AddDesc_WorldBoss_2fWorldBoss_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "WorldBoss/WorldBoss.proto");
  GOOGLE_CHECK(file != NULL);
  WorldBossInfoList_descriptor_ = file->message_type(0);
  static const int WorldBossInfoList_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(WorldBossInfoList, boosinfolist_),
  };
  WorldBossInfoList_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      WorldBossInfoList_descriptor_,
      WorldBossInfoList::default_instance_,
      WorldBossInfoList_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(WorldBossInfoList, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(WorldBossInfoList, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(WorldBossInfoList));
  SingleWorldBossInfo_descriptor_ = file->message_type(1);
  static const int SingleWorldBossInfo_offsets_[7] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SingleWorldBossInfo, bosskey_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SingleWorldBossInfo, isactive_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SingleWorldBossInfo, bossid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SingleWorldBossInfo, reflushtime_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SingleWorldBossInfo, playerid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SingleWorldBossInfo, playername_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SingleWorldBossInfo, viplevel_),
  };
  SingleWorldBossInfo_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      SingleWorldBossInfo_descriptor_,
      SingleWorldBossInfo::default_instance_,
      SingleWorldBossInfo_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SingleWorldBossInfo, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(SingleWorldBossInfo, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(SingleWorldBossInfo));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_WorldBoss_2fWorldBoss_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    WorldBossInfoList_descriptor_, &WorldBossInfoList::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    SingleWorldBossInfo_descriptor_, &SingleWorldBossInfo::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_WorldBoss_2fWorldBoss_2eproto() {
  delete WorldBossInfoList::default_instance_;
  delete WorldBossInfoList_reflection_;
  delete SingleWorldBossInfo::default_instance_;
  delete SingleWorldBossInfo_reflection_;
}

void protobuf_AddDesc_WorldBoss_2fWorldBoss_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\031WorldBoss/WorldBoss.proto\022\tWorldBoss\"I"
    "\n\021WorldBossInfoList\0224\n\014boosInfoList\030\001 \003("
    "\0132\036.WorldBoss.SingleWorldBossInfo\"\225\001\n\023Si"
    "ngleWorldBossInfo\022\017\n\007bossKey\030\001 \001(\005\022\020\n\010is"
    "Active\030\002 \001(\010\022\016\n\006bossId\030\003 \001(\003\022\023\n\013reflushT"
    "ime\030\004 \001(\003\022\020\n\010playerID\030\005 \001(\003\022\022\n\nplayerNam"
    "e\030\006 \001(\014\022\020\n\010vipLevel\030\007 \001(\005", 265);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "WorldBoss/WorldBoss.proto", &protobuf_RegisterTypes);
  WorldBossInfoList::default_instance_ = new WorldBossInfoList();
  SingleWorldBossInfo::default_instance_ = new SingleWorldBossInfo();
  WorldBossInfoList::default_instance_->InitAsDefaultInstance();
  SingleWorldBossInfo::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_WorldBoss_2fWorldBoss_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_WorldBoss_2fWorldBoss_2eproto {
  StaticDescriptorInitializer_WorldBoss_2fWorldBoss_2eproto() {
    protobuf_AddDesc_WorldBoss_2fWorldBoss_2eproto();
  }
} static_descriptor_initializer_WorldBoss_2fWorldBoss_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int WorldBossInfoList::kBoosInfoListFieldNumber;
#endif  // !_MSC_VER

WorldBossInfoList::WorldBossInfoList()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void WorldBossInfoList::InitAsDefaultInstance() {
}

WorldBossInfoList::WorldBossInfoList(const WorldBossInfoList& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void WorldBossInfoList::SharedCtor() {
  _cached_size_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

WorldBossInfoList::~WorldBossInfoList() {
  SharedDtor();
}

void WorldBossInfoList::SharedDtor() {
  if (this != default_instance_) {
  }
}

void WorldBossInfoList::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* WorldBossInfoList::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return WorldBossInfoList_descriptor_;
}

const WorldBossInfoList& WorldBossInfoList::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_WorldBoss_2fWorldBoss_2eproto();
  return *default_instance_;
}

WorldBossInfoList* WorldBossInfoList::default_instance_ = NULL;

WorldBossInfoList* WorldBossInfoList::New() const {
  return new WorldBossInfoList;
}

void WorldBossInfoList::Clear() {
  boosinfolist_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool WorldBossInfoList::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated .WorldBoss.SingleWorldBossInfo boosInfoList = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_boosInfoList:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_boosinfolist()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(10)) goto parse_boosInfoList;
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void WorldBossInfoList::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // repeated .WorldBoss.SingleWorldBossInfo boosInfoList = 1;
  for (int i = 0; i < this->boosinfolist_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->boosinfolist(i), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* WorldBossInfoList::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // repeated .WorldBoss.SingleWorldBossInfo boosInfoList = 1;
  for (int i = 0; i < this->boosinfolist_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->boosinfolist(i), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int WorldBossInfoList::ByteSize() const {
  int total_size = 0;

  // repeated .WorldBoss.SingleWorldBossInfo boosInfoList = 1;
  total_size += 1 * this->boosinfolist_size();
  for (int i = 0; i < this->boosinfolist_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->boosinfolist(i));
  }

  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void WorldBossInfoList::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const WorldBossInfoList* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const WorldBossInfoList*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void WorldBossInfoList::MergeFrom(const WorldBossInfoList& from) {
  GOOGLE_CHECK_NE(&from, this);
  boosinfolist_.MergeFrom(from.boosinfolist_);
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void WorldBossInfoList::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void WorldBossInfoList::CopyFrom(const WorldBossInfoList& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool WorldBossInfoList::IsInitialized() const {

  return true;
}

void WorldBossInfoList::Swap(WorldBossInfoList* other) {
  if (other != this) {
    boosinfolist_.Swap(&other->boosinfolist_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata WorldBossInfoList::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = WorldBossInfoList_descriptor_;
  metadata.reflection = WorldBossInfoList_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int SingleWorldBossInfo::kBossKeyFieldNumber;
const int SingleWorldBossInfo::kIsActiveFieldNumber;
const int SingleWorldBossInfo::kBossIdFieldNumber;
const int SingleWorldBossInfo::kReflushTimeFieldNumber;
const int SingleWorldBossInfo::kPlayerIDFieldNumber;
const int SingleWorldBossInfo::kPlayerNameFieldNumber;
const int SingleWorldBossInfo::kVipLevelFieldNumber;
#endif  // !_MSC_VER

SingleWorldBossInfo::SingleWorldBossInfo()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void SingleWorldBossInfo::InitAsDefaultInstance() {
}

SingleWorldBossInfo::SingleWorldBossInfo(const SingleWorldBossInfo& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void SingleWorldBossInfo::SharedCtor() {
  _cached_size_ = 0;
  bosskey_ = 0;
  isactive_ = false;
  bossid_ = GOOGLE_LONGLONG(0);
  reflushtime_ = GOOGLE_LONGLONG(0);
  playerid_ = GOOGLE_LONGLONG(0);
  playername_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  viplevel_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

SingleWorldBossInfo::~SingleWorldBossInfo() {
  SharedDtor();
}

void SingleWorldBossInfo::SharedDtor() {
  if (playername_ != &::google::protobuf::internal::kEmptyString) {
    delete playername_;
  }
  if (this != default_instance_) {
  }
}

void SingleWorldBossInfo::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* SingleWorldBossInfo::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return SingleWorldBossInfo_descriptor_;
}

const SingleWorldBossInfo& SingleWorldBossInfo::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_WorldBoss_2fWorldBoss_2eproto();
  return *default_instance_;
}

SingleWorldBossInfo* SingleWorldBossInfo::default_instance_ = NULL;

SingleWorldBossInfo* SingleWorldBossInfo::New() const {
  return new SingleWorldBossInfo;
}

void SingleWorldBossInfo::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    bosskey_ = 0;
    isactive_ = false;
    bossid_ = GOOGLE_LONGLONG(0);
    reflushtime_ = GOOGLE_LONGLONG(0);
    playerid_ = GOOGLE_LONGLONG(0);
    if (has_playername()) {
      if (playername_ != &::google::protobuf::internal::kEmptyString) {
        playername_->clear();
      }
    }
    viplevel_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool SingleWorldBossInfo::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int32 bossKey = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &bosskey_)));
          set_has_bosskey();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_isActive;
        break;
      }

      // optional bool isActive = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_isActive:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &isactive_)));
          set_has_isactive();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_bossId;
        break;
      }

      // optional int64 bossId = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_bossId:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &bossid_)));
          set_has_bossid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_reflushTime;
        break;
      }

      // optional int64 reflushTime = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_reflushTime:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &reflushtime_)));
          set_has_reflushtime();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(40)) goto parse_playerID;
        break;
      }

      // optional int64 playerID = 5;
      case 5: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_playerID:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &playerid_)));
          set_has_playerid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(50)) goto parse_playerName;
        break;
      }

      // optional bytes playerName = 6;
      case 6: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_playerName:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_playername()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(56)) goto parse_vipLevel;
        break;
      }

      // optional int32 vipLevel = 7;
      case 7: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_vipLevel:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &viplevel_)));
          set_has_viplevel();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }

      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void SingleWorldBossInfo::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional int32 bossKey = 1;
  if (has_bosskey()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->bosskey(), output);
  }

  // optional bool isActive = 2;
  if (has_isactive()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(2, this->isactive(), output);
  }

  // optional int64 bossId = 3;
  if (has_bossid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(3, this->bossid(), output);
  }

  // optional int64 reflushTime = 4;
  if (has_reflushtime()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(4, this->reflushtime(), output);
  }

  // optional int64 playerID = 5;
  if (has_playerid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(5, this->playerid(), output);
  }

  // optional bytes playerName = 6;
  if (has_playername()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytes(
      6, this->playername(), output);
  }

  // optional int32 vipLevel = 7;
  if (has_viplevel()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(7, this->viplevel(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* SingleWorldBossInfo::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional int32 bossKey = 1;
  if (has_bosskey()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->bosskey(), target);
  }

  // optional bool isActive = 2;
  if (has_isactive()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(2, this->isactive(), target);
  }

  // optional int64 bossId = 3;
  if (has_bossid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(3, this->bossid(), target);
  }

  // optional int64 reflushTime = 4;
  if (has_reflushtime()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(4, this->reflushtime(), target);
  }

  // optional int64 playerID = 5;
  if (has_playerid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(5, this->playerid(), target);
  }

  // optional bytes playerName = 6;
  if (has_playername()) {
    target =
      ::google::protobuf::internal::WireFormatLite::WriteBytesToArray(
        6, this->playername(), target);
  }

  // optional int32 vipLevel = 7;
  if (has_viplevel()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(7, this->viplevel(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int SingleWorldBossInfo::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int32 bossKey = 1;
    if (has_bosskey()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->bosskey());
    }

    // optional bool isActive = 2;
    if (has_isactive()) {
      total_size += 1 + 1;
    }

    // optional int64 bossId = 3;
    if (has_bossid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->bossid());
    }

    // optional int64 reflushTime = 4;
    if (has_reflushtime()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->reflushtime());
    }

    // optional int64 playerID = 5;
    if (has_playerid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->playerid());
    }

    // optional bytes playerName = 6;
    if (has_playername()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->playername());
    }

    // optional int32 vipLevel = 7;
    if (has_viplevel()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->viplevel());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void SingleWorldBossInfo::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const SingleWorldBossInfo* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const SingleWorldBossInfo*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void SingleWorldBossInfo::MergeFrom(const SingleWorldBossInfo& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_bosskey()) {
      set_bosskey(from.bosskey());
    }
    if (from.has_isactive()) {
      set_isactive(from.isactive());
    }
    if (from.has_bossid()) {
      set_bossid(from.bossid());
    }
    if (from.has_reflushtime()) {
      set_reflushtime(from.reflushtime());
    }
    if (from.has_playerid()) {
      set_playerid(from.playerid());
    }
    if (from.has_playername()) {
      set_playername(from.playername());
    }
    if (from.has_viplevel()) {
      set_viplevel(from.viplevel());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void SingleWorldBossInfo::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void SingleWorldBossInfo::CopyFrom(const SingleWorldBossInfo& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool SingleWorldBossInfo::IsInitialized() const {

  return true;
}

void SingleWorldBossInfo::Swap(SingleWorldBossInfo* other) {
  if (other != this) {
    std::swap(bosskey_, other->bosskey_);
    std::swap(isactive_, other->isactive_);
    std::swap(bossid_, other->bossid_);
    std::swap(reflushtime_, other->reflushtime_);
    std::swap(playerid_, other->playerid_);
    std::swap(playername_, other->playername_);
    std::swap(viplevel_, other->viplevel_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata SingleWorldBossInfo::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = SingleWorldBossInfo_descriptor_;
  metadata.reflection = SingleWorldBossInfo_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace WorldBoss

// @@protoc_insertion_point(global_scope)
