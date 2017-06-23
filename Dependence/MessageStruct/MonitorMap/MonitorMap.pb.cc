// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: MonitorMap/MonitorMap.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "MonitorMap/MonitorMap.pb.h"

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

namespace MonitorMap {

namespace {

const ::google::protobuf::Descriptor* mapInfo_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  mapInfo_reflection_ = NULL;
const ::google::protobuf::Descriptor* mapItem_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  mapItem_reflection_ = NULL;
const ::google::protobuf::Descriptor* mapPlayerInfo_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  mapPlayerInfo_reflection_ = NULL;
const ::google::protobuf::Descriptor* mapPlayerItem_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  mapPlayerItem_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_MonitorMap_2fMonitorMap_2eproto() {
  protobuf_AddDesc_MonitorMap_2fMonitorMap_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "MonitorMap/MonitorMap.proto");
  GOOGLE_CHECK(file != NULL);
  mapInfo_descriptor_ = file->message_type(0);
  static const int mapInfo_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(mapInfo, item_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(mapInfo, count_),
  };
  mapInfo_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      mapInfo_descriptor_,
      mapInfo::default_instance_,
      mapInfo_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(mapInfo, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(mapInfo, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(mapInfo));
  mapItem_descriptor_ = file->message_type(1);
  static const int mapItem_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(mapItem, mapid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(mapItem, num_),
  };
  mapItem_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      mapItem_descriptor_,
      mapItem::default_instance_,
      mapItem_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(mapItem, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(mapItem, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(mapItem));
  mapPlayerInfo_descriptor_ = file->message_type(2);
  static const int mapPlayerInfo_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(mapPlayerInfo, item_),
  };
  mapPlayerInfo_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      mapPlayerInfo_descriptor_,
      mapPlayerInfo::default_instance_,
      mapPlayerInfo_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(mapPlayerInfo, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(mapPlayerInfo, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(mapPlayerInfo));
  mapPlayerItem_descriptor_ = file->message_type(3);
  static const int mapPlayerItem_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(mapPlayerItem, ip_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(mapPlayerItem, id_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(mapPlayerItem, state_),
  };
  mapPlayerItem_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      mapPlayerItem_descriptor_,
      mapPlayerItem::default_instance_,
      mapPlayerItem_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(mapPlayerItem, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(mapPlayerItem, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(mapPlayerItem));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_MonitorMap_2fMonitorMap_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    mapInfo_descriptor_, &mapInfo::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    mapItem_descriptor_, &mapItem::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    mapPlayerInfo_descriptor_, &mapPlayerInfo::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    mapPlayerItem_descriptor_, &mapPlayerItem::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_MonitorMap_2fMonitorMap_2eproto() {
  delete mapInfo::default_instance_;
  delete mapInfo_reflection_;
  delete mapItem::default_instance_;
  delete mapItem_reflection_;
  delete mapPlayerInfo::default_instance_;
  delete mapPlayerInfo_reflection_;
  delete mapPlayerItem::default_instance_;
  delete mapPlayerItem_reflection_;
}

void protobuf_AddDesc_MonitorMap_2fMonitorMap_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\033MonitorMap/MonitorMap.proto\022\nMonitorMa"
    "p\";\n\007mapInfo\022!\n\004item\030\001 \003(\0132\023.MonitorMap."
    "mapItem\022\r\n\005count\030\002 \002(\005\"%\n\007mapItem\022\r\n\005map"
    "id\030\001 \002(\005\022\013\n\003num\030\002 \002(\005\"8\n\rmapPlayerInfo\022\'"
    "\n\004item\030\001 \003(\0132\031.MonitorMap.mapPlayerItem\""
    "6\n\rmapPlayerItem\022\n\n\002ip\030\001 \002(\t\022\n\n\002id\030\002 \002(\005"
    "\022\r\n\005state\030\003 \002(\005", 255);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "MonitorMap/MonitorMap.proto", &protobuf_RegisterTypes);
  mapInfo::default_instance_ = new mapInfo();
  mapItem::default_instance_ = new mapItem();
  mapPlayerInfo::default_instance_ = new mapPlayerInfo();
  mapPlayerItem::default_instance_ = new mapPlayerItem();
  mapInfo::default_instance_->InitAsDefaultInstance();
  mapItem::default_instance_->InitAsDefaultInstance();
  mapPlayerInfo::default_instance_->InitAsDefaultInstance();
  mapPlayerItem::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_MonitorMap_2fMonitorMap_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_MonitorMap_2fMonitorMap_2eproto {
  StaticDescriptorInitializer_MonitorMap_2fMonitorMap_2eproto() {
    protobuf_AddDesc_MonitorMap_2fMonitorMap_2eproto();
  }
} static_descriptor_initializer_MonitorMap_2fMonitorMap_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int mapInfo::kItemFieldNumber;
const int mapInfo::kCountFieldNumber;
#endif  // !_MSC_VER

mapInfo::mapInfo()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void mapInfo::InitAsDefaultInstance() {
}

mapInfo::mapInfo(const mapInfo& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void mapInfo::SharedCtor() {
  _cached_size_ = 0;
  count_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

mapInfo::~mapInfo() {
  SharedDtor();
}

void mapInfo::SharedDtor() {
  if (this != default_instance_) {
  }
}

void mapInfo::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* mapInfo::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return mapInfo_descriptor_;
}

const mapInfo& mapInfo::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_MonitorMap_2fMonitorMap_2eproto();
  return *default_instance_;
}

mapInfo* mapInfo::default_instance_ = NULL;

mapInfo* mapInfo::New() const {
  return new mapInfo;
}

void mapInfo::Clear() {
  if (_has_bits_[1 / 32] & (0xffu << (1 % 32))) {
    count_ = 0;
  }
  item_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool mapInfo::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated .MonitorMap.mapItem item = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_item:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_item()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(10)) goto parse_item;
        if (input->ExpectTag(16)) goto parse_count;
        break;
      }

      // required int32 count = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_count:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &count_)));
          set_has_count();
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

void mapInfo::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // repeated .MonitorMap.mapItem item = 1;
  for (int i = 0; i < this->item_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->item(i), output);
  }

  // required int32 count = 2;
  if (has_count()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->count(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* mapInfo::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // repeated .MonitorMap.mapItem item = 1;
  for (int i = 0; i < this->item_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->item(i), target);
  }

  // required int32 count = 2;
  if (has_count()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->count(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int mapInfo::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[1 / 32] & (0xffu << (1 % 32))) {
    // required int32 count = 2;
    if (has_count()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->count());
    }

  }
  // repeated .MonitorMap.mapItem item = 1;
  total_size += 1 * this->item_size();
  for (int i = 0; i < this->item_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->item(i));
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

void mapInfo::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const mapInfo* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const mapInfo*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void mapInfo::MergeFrom(const mapInfo& from) {
  GOOGLE_CHECK_NE(&from, this);
  item_.MergeFrom(from.item_);
  if (from._has_bits_[1 / 32] & (0xffu << (1 % 32))) {
    if (from.has_count()) {
      set_count(from.count());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void mapInfo::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void mapInfo::CopyFrom(const mapInfo& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool mapInfo::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000002) != 0x00000002) return false;

  for (int i = 0; i < item_size(); i++) {
    if (!this->item(i).IsInitialized()) return false;
  }
  return true;
}

void mapInfo::Swap(mapInfo* other) {
  if (other != this) {
    item_.Swap(&other->item_);
    std::swap(count_, other->count_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata mapInfo::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = mapInfo_descriptor_;
  metadata.reflection = mapInfo_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int mapItem::kMapidFieldNumber;
const int mapItem::kNumFieldNumber;
#endif  // !_MSC_VER

mapItem::mapItem()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void mapItem::InitAsDefaultInstance() {
}

mapItem::mapItem(const mapItem& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void mapItem::SharedCtor() {
  _cached_size_ = 0;
  mapid_ = 0;
  num_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

mapItem::~mapItem() {
  SharedDtor();
}

void mapItem::SharedDtor() {
  if (this != default_instance_) {
  }
}

void mapItem::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* mapItem::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return mapItem_descriptor_;
}

const mapItem& mapItem::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_MonitorMap_2fMonitorMap_2eproto();
  return *default_instance_;
}

mapItem* mapItem::default_instance_ = NULL;

mapItem* mapItem::New() const {
  return new mapItem;
}

void mapItem::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    mapid_ = 0;
    num_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool mapItem::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 mapid = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &mapid_)));
          set_has_mapid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_num;
        break;
      }

      // required int32 num = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_num:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &num_)));
          set_has_num();
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

void mapItem::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int32 mapid = 1;
  if (has_mapid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->mapid(), output);
  }

  // required int32 num = 2;
  if (has_num()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->num(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* mapItem::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int32 mapid = 1;
  if (has_mapid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->mapid(), target);
  }

  // required int32 num = 2;
  if (has_num()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->num(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int mapItem::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 mapid = 1;
    if (has_mapid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->mapid());
    }

    // required int32 num = 2;
    if (has_num()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->num());
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

void mapItem::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const mapItem* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const mapItem*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void mapItem::MergeFrom(const mapItem& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_mapid()) {
      set_mapid(from.mapid());
    }
    if (from.has_num()) {
      set_num(from.num());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void mapItem::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void mapItem::CopyFrom(const mapItem& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool mapItem::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void mapItem::Swap(mapItem* other) {
  if (other != this) {
    std::swap(mapid_, other->mapid_);
    std::swap(num_, other->num_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata mapItem::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = mapItem_descriptor_;
  metadata.reflection = mapItem_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int mapPlayerInfo::kItemFieldNumber;
#endif  // !_MSC_VER

mapPlayerInfo::mapPlayerInfo()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void mapPlayerInfo::InitAsDefaultInstance() {
}

mapPlayerInfo::mapPlayerInfo(const mapPlayerInfo& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void mapPlayerInfo::SharedCtor() {
  _cached_size_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

mapPlayerInfo::~mapPlayerInfo() {
  SharedDtor();
}

void mapPlayerInfo::SharedDtor() {
  if (this != default_instance_) {
  }
}

void mapPlayerInfo::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* mapPlayerInfo::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return mapPlayerInfo_descriptor_;
}

const mapPlayerInfo& mapPlayerInfo::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_MonitorMap_2fMonitorMap_2eproto();
  return *default_instance_;
}

mapPlayerInfo* mapPlayerInfo::default_instance_ = NULL;

mapPlayerInfo* mapPlayerInfo::New() const {
  return new mapPlayerInfo;
}

void mapPlayerInfo::Clear() {
  item_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool mapPlayerInfo::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated .MonitorMap.mapPlayerItem item = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_item:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_item()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(10)) goto parse_item;
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

void mapPlayerInfo::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // repeated .MonitorMap.mapPlayerItem item = 1;
  for (int i = 0; i < this->item_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->item(i), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* mapPlayerInfo::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // repeated .MonitorMap.mapPlayerItem item = 1;
  for (int i = 0; i < this->item_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->item(i), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int mapPlayerInfo::ByteSize() const {
  int total_size = 0;

  // repeated .MonitorMap.mapPlayerItem item = 1;
  total_size += 1 * this->item_size();
  for (int i = 0; i < this->item_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->item(i));
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

void mapPlayerInfo::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const mapPlayerInfo* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const mapPlayerInfo*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void mapPlayerInfo::MergeFrom(const mapPlayerInfo& from) {
  GOOGLE_CHECK_NE(&from, this);
  item_.MergeFrom(from.item_);
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void mapPlayerInfo::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void mapPlayerInfo::CopyFrom(const mapPlayerInfo& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool mapPlayerInfo::IsInitialized() const {

  for (int i = 0; i < item_size(); i++) {
    if (!this->item(i).IsInitialized()) return false;
  }
  return true;
}

void mapPlayerInfo::Swap(mapPlayerInfo* other) {
  if (other != this) {
    item_.Swap(&other->item_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata mapPlayerInfo::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = mapPlayerInfo_descriptor_;
  metadata.reflection = mapPlayerInfo_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int mapPlayerItem::kIpFieldNumber;
const int mapPlayerItem::kIdFieldNumber;
const int mapPlayerItem::kStateFieldNumber;
#endif  // !_MSC_VER

mapPlayerItem::mapPlayerItem()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void mapPlayerItem::InitAsDefaultInstance() {
}

mapPlayerItem::mapPlayerItem(const mapPlayerItem& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void mapPlayerItem::SharedCtor() {
  _cached_size_ = 0;
  ip_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  id_ = 0;
  state_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

mapPlayerItem::~mapPlayerItem() {
  SharedDtor();
}

void mapPlayerItem::SharedDtor() {
  if (ip_ != &::google::protobuf::internal::kEmptyString) {
    delete ip_;
  }
  if (this != default_instance_) {
  }
}

void mapPlayerItem::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* mapPlayerItem::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return mapPlayerItem_descriptor_;
}

const mapPlayerItem& mapPlayerItem::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_MonitorMap_2fMonitorMap_2eproto();
  return *default_instance_;
}

mapPlayerItem* mapPlayerItem::default_instance_ = NULL;

mapPlayerItem* mapPlayerItem::New() const {
  return new mapPlayerItem;
}

void mapPlayerItem::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_ip()) {
      if (ip_ != &::google::protobuf::internal::kEmptyString) {
        ip_->clear();
      }
    }
    id_ = 0;
    state_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool mapPlayerItem::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string ip = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_ip()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->ip().data(), this->ip().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_id;
        break;
      }

      // required int32 id = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_id:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &id_)));
          set_has_id();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_state;
        break;
      }

      // required int32 state = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_state:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &state_)));
          set_has_state();
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

void mapPlayerItem::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string ip = 1;
  if (has_ip()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->ip().data(), this->ip().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->ip(), output);
  }

  // required int32 id = 2;
  if (has_id()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->id(), output);
  }

  // required int32 state = 3;
  if (has_state()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->state(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* mapPlayerItem::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required string ip = 1;
  if (has_ip()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->ip().data(), this->ip().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->ip(), target);
  }

  // required int32 id = 2;
  if (has_id()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->id(), target);
  }

  // required int32 state = 3;
  if (has_state()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->state(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int mapPlayerItem::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string ip = 1;
    if (has_ip()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->ip());
    }

    // required int32 id = 2;
    if (has_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->id());
    }

    // required int32 state = 3;
    if (has_state()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->state());
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

void mapPlayerItem::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const mapPlayerItem* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const mapPlayerItem*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void mapPlayerItem::MergeFrom(const mapPlayerItem& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_ip()) {
      set_ip(from.ip());
    }
    if (from.has_id()) {
      set_id(from.id());
    }
    if (from.has_state()) {
      set_state(from.state());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void mapPlayerItem::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void mapPlayerItem::CopyFrom(const mapPlayerItem& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool mapPlayerItem::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000007) != 0x00000007) return false;

  return true;
}

void mapPlayerItem::Swap(mapPlayerItem* other) {
  if (other != this) {
    std::swap(ip_, other->ip_);
    std::swap(id_, other->id_);
    std::swap(state_, other->state_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata mapPlayerItem::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = mapPlayerItem_descriptor_;
  metadata.reflection = mapPlayerItem_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace MonitorMap

// @@protoc_insertion_point(global_scope)
