// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ChristmasActivity/NewYearDBPro.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "ChristmasActivity/NewYearDBPro.pb.h"

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

namespace NewYearDBPro {

namespace {

const ::google::protobuf::Descriptor* LimitPlayerData_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  LimitPlayerData_reflection_ = NULL;
const ::google::protobuf::Descriptor* NewYearLimitData_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  NewYearLimitData_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_ChristmasActivity_2fNewYearDBPro_2eproto() {
  protobuf_AddDesc_ChristmasActivity_2fNewYearDBPro_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "ChristmasActivity/NewYearDBPro.proto");
  GOOGLE_CHECK(file != NULL);
  LimitPlayerData_descriptor_ = file->message_type(0);
  static const int LimitPlayerData_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LimitPlayerData, playerid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LimitPlayerData, counts_),
  };
  LimitPlayerData_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      LimitPlayerData_descriptor_,
      LimitPlayerData::default_instance_,
      LimitPlayerData_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LimitPlayerData, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LimitPlayerData, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(LimitPlayerData));
  NewYearLimitData_descriptor_ = file->message_type(1);
  static const int NewYearLimitData_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(NewYearLimitData, datas_),
  };
  NewYearLimitData_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      NewYearLimitData_descriptor_,
      NewYearLimitData::default_instance_,
      NewYearLimitData_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(NewYearLimitData, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(NewYearLimitData, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(NewYearLimitData));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_ChristmasActivity_2fNewYearDBPro_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    LimitPlayerData_descriptor_, &LimitPlayerData::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    NewYearLimitData_descriptor_, &NewYearLimitData::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_ChristmasActivity_2fNewYearDBPro_2eproto() {
  delete LimitPlayerData::default_instance_;
  delete LimitPlayerData_reflection_;
  delete NewYearLimitData::default_instance_;
  delete NewYearLimitData_reflection_;
}

void protobuf_AddDesc_ChristmasActivity_2fNewYearDBPro_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n$ChristmasActivity/NewYearDBPro.proto\022\014"
    "NewYearDBPro\"3\n\017LimitPlayerData\022\020\n\010playe"
    "rID\030\001 \001(\003\022\016\n\006counts\030\002 \001(\005\"@\n\020NewYearLimi"
    "tData\022,\n\005datas\030\001 \003(\0132\035.NewYearDBPro.Limi"
    "tPlayerData", 171);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "ChristmasActivity/NewYearDBPro.proto", &protobuf_RegisterTypes);
  LimitPlayerData::default_instance_ = new LimitPlayerData();
  NewYearLimitData::default_instance_ = new NewYearLimitData();
  LimitPlayerData::default_instance_->InitAsDefaultInstance();
  NewYearLimitData::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_ChristmasActivity_2fNewYearDBPro_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_ChristmasActivity_2fNewYearDBPro_2eproto {
  StaticDescriptorInitializer_ChristmasActivity_2fNewYearDBPro_2eproto() {
    protobuf_AddDesc_ChristmasActivity_2fNewYearDBPro_2eproto();
  }
} static_descriptor_initializer_ChristmasActivity_2fNewYearDBPro_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int LimitPlayerData::kPlayerIDFieldNumber;
const int LimitPlayerData::kCountsFieldNumber;
#endif  // !_MSC_VER

LimitPlayerData::LimitPlayerData()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void LimitPlayerData::InitAsDefaultInstance() {
}

LimitPlayerData::LimitPlayerData(const LimitPlayerData& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void LimitPlayerData::SharedCtor() {
  _cached_size_ = 0;
  playerid_ = GOOGLE_LONGLONG(0);
  counts_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

LimitPlayerData::~LimitPlayerData() {
  SharedDtor();
}

void LimitPlayerData::SharedDtor() {
  if (this != default_instance_) {
  }
}

void LimitPlayerData::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* LimitPlayerData::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return LimitPlayerData_descriptor_;
}

const LimitPlayerData& LimitPlayerData::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_ChristmasActivity_2fNewYearDBPro_2eproto();
  return *default_instance_;
}

LimitPlayerData* LimitPlayerData::default_instance_ = NULL;

LimitPlayerData* LimitPlayerData::New() const {
  return new LimitPlayerData;
}

void LimitPlayerData::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    playerid_ = GOOGLE_LONGLONG(0);
    counts_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool LimitPlayerData::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int64 playerID = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &playerid_)));
          set_has_playerid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_counts;
        break;
      }

      // optional int32 counts = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_counts:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &counts_)));
          set_has_counts();
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

void LimitPlayerData::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional int64 playerID = 1;
  if (has_playerid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(1, this->playerid(), output);
  }

  // optional int32 counts = 2;
  if (has_counts()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->counts(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* LimitPlayerData::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional int64 playerID = 1;
  if (has_playerid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(1, this->playerid(), target);
  }

  // optional int32 counts = 2;
  if (has_counts()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->counts(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int LimitPlayerData::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int64 playerID = 1;
    if (has_playerid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->playerid());
    }

    // optional int32 counts = 2;
    if (has_counts()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->counts());
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

void LimitPlayerData::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const LimitPlayerData* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const LimitPlayerData*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void LimitPlayerData::MergeFrom(const LimitPlayerData& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_playerid()) {
      set_playerid(from.playerid());
    }
    if (from.has_counts()) {
      set_counts(from.counts());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void LimitPlayerData::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void LimitPlayerData::CopyFrom(const LimitPlayerData& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool LimitPlayerData::IsInitialized() const {

  return true;
}

void LimitPlayerData::Swap(LimitPlayerData* other) {
  if (other != this) {
    std::swap(playerid_, other->playerid_);
    std::swap(counts_, other->counts_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata LimitPlayerData::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = LimitPlayerData_descriptor_;
  metadata.reflection = LimitPlayerData_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int NewYearLimitData::kDatasFieldNumber;
#endif  // !_MSC_VER

NewYearLimitData::NewYearLimitData()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void NewYearLimitData::InitAsDefaultInstance() {
}

NewYearLimitData::NewYearLimitData(const NewYearLimitData& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void NewYearLimitData::SharedCtor() {
  _cached_size_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

NewYearLimitData::~NewYearLimitData() {
  SharedDtor();
}

void NewYearLimitData::SharedDtor() {
  if (this != default_instance_) {
  }
}

void NewYearLimitData::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* NewYearLimitData::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return NewYearLimitData_descriptor_;
}

const NewYearLimitData& NewYearLimitData::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_ChristmasActivity_2fNewYearDBPro_2eproto();
  return *default_instance_;
}

NewYearLimitData* NewYearLimitData::default_instance_ = NULL;

NewYearLimitData* NewYearLimitData::New() const {
  return new NewYearLimitData;
}

void NewYearLimitData::Clear() {
  datas_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool NewYearLimitData::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // repeated .NewYearDBPro.LimitPlayerData datas = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_datas:
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
                input, add_datas()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(10)) goto parse_datas;
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

void NewYearLimitData::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // repeated .NewYearDBPro.LimitPlayerData datas = 1;
  for (int i = 0; i < this->datas_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->datas(i), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* NewYearLimitData::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // repeated .NewYearDBPro.LimitPlayerData datas = 1;
  for (int i = 0; i < this->datas_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->datas(i), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int NewYearLimitData::ByteSize() const {
  int total_size = 0;

  // repeated .NewYearDBPro.LimitPlayerData datas = 1;
  total_size += 1 * this->datas_size();
  for (int i = 0; i < this->datas_size(); i++) {
    total_size +=
      ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
        this->datas(i));
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

void NewYearLimitData::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const NewYearLimitData* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const NewYearLimitData*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void NewYearLimitData::MergeFrom(const NewYearLimitData& from) {
  GOOGLE_CHECK_NE(&from, this);
  datas_.MergeFrom(from.datas_);
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void NewYearLimitData::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void NewYearLimitData::CopyFrom(const NewYearLimitData& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool NewYearLimitData::IsInitialized() const {

  return true;
}

void NewYearLimitData::Swap(NewYearLimitData* other) {
  if (other != this) {
    datas_.Swap(&other->datas_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata NewYearLimitData::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = NewYearLimitData_descriptor_;
  metadata.reflection = NewYearLimitData_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace NewYearDBPro

// @@protoc_insertion_point(global_scope)
