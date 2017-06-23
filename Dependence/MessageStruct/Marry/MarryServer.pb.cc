// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Marry/MarryServer.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "Marry/MarryServer.pb.h"

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

namespace MarryServer {

namespace {

const ::google::protobuf::Descriptor* MarrySuccess_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  MarrySuccess_reflection_ = NULL;
const ::google::protobuf::Descriptor* MarryChangeMap_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  MarryChangeMap_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_Marry_2fMarryServer_2eproto() {
  protobuf_AddDesc_Marry_2fMarryServer_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "Marry/MarryServer.proto");
  GOOGLE_CHECK(file != NULL);
  MarrySuccess_descriptor_ = file->message_type(0);
  static const int MarrySuccess_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MarrySuccess, otherplayerid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MarrySuccess, othername_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MarrySuccess, selfplayerid_),
  };
  MarrySuccess_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      MarrySuccess_descriptor_,
      MarrySuccess::default_instance_,
      MarrySuccess_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MarrySuccess, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MarrySuccess, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(MarrySuccess));
  MarryChangeMap_descriptor_ = file->message_type(1);
  static const int MarryChangeMap_offsets_[4] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MarryChangeMap, mapid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MarryChangeMap, xpos_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MarryChangeMap, ypos_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MarryChangeMap, error_),
  };
  MarryChangeMap_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      MarryChangeMap_descriptor_,
      MarryChangeMap::default_instance_,
      MarryChangeMap_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MarryChangeMap, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(MarryChangeMap, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(MarryChangeMap));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_Marry_2fMarryServer_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    MarrySuccess_descriptor_, &MarrySuccess::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    MarryChangeMap_descriptor_, &MarryChangeMap::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_Marry_2fMarryServer_2eproto() {
  delete MarrySuccess::default_instance_;
  delete MarrySuccess_reflection_;
  delete MarryChangeMap::default_instance_;
  delete MarryChangeMap_reflection_;
}

void protobuf_AddDesc_Marry_2fMarryServer_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\027Marry/MarryServer.proto\022\013MarryServer\"N"
    "\n\014MarrySuccess\022\025\n\rotherPlayerID\030\001 \001(\003\022\021\n"
    "\totherName\030\002 \001(\014\022\024\n\014selfPlayerID\030\003 \001(\003\"J"
    "\n\016MarryChangeMap\022\r\n\005mapID\030\001 \001(\003\022\014\n\004xPos\030"
    "\002 \001(\005\022\014\n\004yPos\030\003 \001(\005\022\r\n\005error\030\004 \001(\005", 194);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "Marry/MarryServer.proto", &protobuf_RegisterTypes);
  MarrySuccess::default_instance_ = new MarrySuccess();
  MarryChangeMap::default_instance_ = new MarryChangeMap();
  MarrySuccess::default_instance_->InitAsDefaultInstance();
  MarryChangeMap::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_Marry_2fMarryServer_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_Marry_2fMarryServer_2eproto {
  StaticDescriptorInitializer_Marry_2fMarryServer_2eproto() {
    protobuf_AddDesc_Marry_2fMarryServer_2eproto();
  }
} static_descriptor_initializer_Marry_2fMarryServer_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int MarrySuccess::kOtherPlayerIDFieldNumber;
const int MarrySuccess::kOtherNameFieldNumber;
const int MarrySuccess::kSelfPlayerIDFieldNumber;
#endif  // !_MSC_VER

MarrySuccess::MarrySuccess()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void MarrySuccess::InitAsDefaultInstance() {
}

MarrySuccess::MarrySuccess(const MarrySuccess& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void MarrySuccess::SharedCtor() {
  _cached_size_ = 0;
  otherplayerid_ = GOOGLE_LONGLONG(0);
  othername_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  selfplayerid_ = GOOGLE_LONGLONG(0);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

MarrySuccess::~MarrySuccess() {
  SharedDtor();
}

void MarrySuccess::SharedDtor() {
  if (othername_ != &::google::protobuf::internal::kEmptyString) {
    delete othername_;
  }
  if (this != default_instance_) {
  }
}

void MarrySuccess::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* MarrySuccess::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return MarrySuccess_descriptor_;
}

const MarrySuccess& MarrySuccess::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_Marry_2fMarryServer_2eproto();
  return *default_instance_;
}

MarrySuccess* MarrySuccess::default_instance_ = NULL;

MarrySuccess* MarrySuccess::New() const {
  return new MarrySuccess;
}

void MarrySuccess::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    otherplayerid_ = GOOGLE_LONGLONG(0);
    if (has_othername()) {
      if (othername_ != &::google::protobuf::internal::kEmptyString) {
        othername_->clear();
      }
    }
    selfplayerid_ = GOOGLE_LONGLONG(0);
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool MarrySuccess::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int64 otherPlayerID = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &otherplayerid_)));
          set_has_otherplayerid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_otherName;
        break;
      }

      // optional bytes otherName = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_otherName:
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_othername()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_selfPlayerID;
        break;
      }

      // optional int64 selfPlayerID = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_selfPlayerID:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &selfplayerid_)));
          set_has_selfplayerid();
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

void MarrySuccess::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional int64 otherPlayerID = 1;
  if (has_otherplayerid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(1, this->otherplayerid(), output);
  }

  // optional bytes otherName = 2;
  if (has_othername()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytes(
      2, this->othername(), output);
  }

  // optional int64 selfPlayerID = 3;
  if (has_selfplayerid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(3, this->selfplayerid(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* MarrySuccess::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional int64 otherPlayerID = 1;
  if (has_otherplayerid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(1, this->otherplayerid(), target);
  }

  // optional bytes otherName = 2;
  if (has_othername()) {
    target =
      ::google::protobuf::internal::WireFormatLite::WriteBytesToArray(
        2, this->othername(), target);
  }

  // optional int64 selfPlayerID = 3;
  if (has_selfplayerid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(3, this->selfplayerid(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int MarrySuccess::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int64 otherPlayerID = 1;
    if (has_otherplayerid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->otherplayerid());
    }

    // optional bytes otherName = 2;
    if (has_othername()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->othername());
    }

    // optional int64 selfPlayerID = 3;
    if (has_selfplayerid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->selfplayerid());
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

void MarrySuccess::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const MarrySuccess* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const MarrySuccess*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void MarrySuccess::MergeFrom(const MarrySuccess& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_otherplayerid()) {
      set_otherplayerid(from.otherplayerid());
    }
    if (from.has_othername()) {
      set_othername(from.othername());
    }
    if (from.has_selfplayerid()) {
      set_selfplayerid(from.selfplayerid());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void MarrySuccess::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void MarrySuccess::CopyFrom(const MarrySuccess& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool MarrySuccess::IsInitialized() const {

  return true;
}

void MarrySuccess::Swap(MarrySuccess* other) {
  if (other != this) {
    std::swap(otherplayerid_, other->otherplayerid_);
    std::swap(othername_, other->othername_);
    std::swap(selfplayerid_, other->selfplayerid_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata MarrySuccess::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = MarrySuccess_descriptor_;
  metadata.reflection = MarrySuccess_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int MarryChangeMap::kMapIDFieldNumber;
const int MarryChangeMap::kXPosFieldNumber;
const int MarryChangeMap::kYPosFieldNumber;
const int MarryChangeMap::kErrorFieldNumber;
#endif  // !_MSC_VER

MarryChangeMap::MarryChangeMap()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void MarryChangeMap::InitAsDefaultInstance() {
}

MarryChangeMap::MarryChangeMap(const MarryChangeMap& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void MarryChangeMap::SharedCtor() {
  _cached_size_ = 0;
  mapid_ = GOOGLE_LONGLONG(0);
  xpos_ = 0;
  ypos_ = 0;
  error_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

MarryChangeMap::~MarryChangeMap() {
  SharedDtor();
}

void MarryChangeMap::SharedDtor() {
  if (this != default_instance_) {
  }
}

void MarryChangeMap::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* MarryChangeMap::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return MarryChangeMap_descriptor_;
}

const MarryChangeMap& MarryChangeMap::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_Marry_2fMarryServer_2eproto();
  return *default_instance_;
}

MarryChangeMap* MarryChangeMap::default_instance_ = NULL;

MarryChangeMap* MarryChangeMap::New() const {
  return new MarryChangeMap;
}

void MarryChangeMap::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    mapid_ = GOOGLE_LONGLONG(0);
    xpos_ = 0;
    ypos_ = 0;
    error_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool MarryChangeMap::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int64 mapID = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &mapid_)));
          set_has_mapid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_xPos;
        break;
      }

      // optional int32 xPos = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_xPos:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &xpos_)));
          set_has_xpos();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_yPos;
        break;
      }

      // optional int32 yPos = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_yPos:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &ypos_)));
          set_has_ypos();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_error;
        break;
      }

      // optional int32 error = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_error:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &error_)));
          set_has_error();
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

void MarryChangeMap::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional int64 mapID = 1;
  if (has_mapid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(1, this->mapid(), output);
  }

  // optional int32 xPos = 2;
  if (has_xpos()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->xpos(), output);
  }

  // optional int32 yPos = 3;
  if (has_ypos()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->ypos(), output);
  }

  // optional int32 error = 4;
  if (has_error()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(4, this->error(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* MarryChangeMap::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional int64 mapID = 1;
  if (has_mapid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(1, this->mapid(), target);
  }

  // optional int32 xPos = 2;
  if (has_xpos()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->xpos(), target);
  }

  // optional int32 yPos = 3;
  if (has_ypos()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->ypos(), target);
  }

  // optional int32 error = 4;
  if (has_error()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(4, this->error(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int MarryChangeMap::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int64 mapID = 1;
    if (has_mapid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->mapid());
    }

    // optional int32 xPos = 2;
    if (has_xpos()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->xpos());
    }

    // optional int32 yPos = 3;
    if (has_ypos()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->ypos());
    }

    // optional int32 error = 4;
    if (has_error()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->error());
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

void MarryChangeMap::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const MarryChangeMap* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const MarryChangeMap*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void MarryChangeMap::MergeFrom(const MarryChangeMap& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_mapid()) {
      set_mapid(from.mapid());
    }
    if (from.has_xpos()) {
      set_xpos(from.xpos());
    }
    if (from.has_ypos()) {
      set_ypos(from.ypos());
    }
    if (from.has_error()) {
      set_error(from.error());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void MarryChangeMap::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void MarryChangeMap::CopyFrom(const MarryChangeMap& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool MarryChangeMap::IsInitialized() const {

  return true;
}

void MarryChangeMap::Swap(MarryChangeMap* other) {
  if (other != this) {
    std::swap(mapid_, other->mapid_);
    std::swap(xpos_, other->xpos_);
    std::swap(ypos_, other->ypos_);
    std::swap(error_, other->error_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata MarryChangeMap::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = MarryChangeMap_descriptor_;
  metadata.reflection = MarryChangeMap_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace MarryServer

// @@protoc_insertion_point(global_scope)
