// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Meridians/meridians.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "Meridians/meridians.pb.h"

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

namespace Meridians {

namespace {

const ::google::protobuf::Descriptor* OpenMeridians_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  OpenMeridians_reflection_ = NULL;
const ::google::protobuf::Descriptor* RetOpenMeridians_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  RetOpenMeridians_reflection_ = NULL;
const ::google::protobuf::Descriptor* AskOpenMeridians_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  AskOpenMeridians_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_Meridians_2fmeridians_2eproto() {
  protobuf_AddDesc_Meridians_2fmeridians_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "Meridians/meridians.proto");
  GOOGLE_CHECK(file != NULL);
  OpenMeridians_descriptor_ = file->message_type(0);
  static const int OpenMeridians_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(OpenMeridians, acupoint_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(OpenMeridians, isgolden_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(OpenMeridians, count_),
  };
  OpenMeridians_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      OpenMeridians_descriptor_,
      OpenMeridians::default_instance_,
      OpenMeridians_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(OpenMeridians, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(OpenMeridians, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(OpenMeridians));
  RetOpenMeridians_descriptor_ = file->message_type(1);
  static const int RetOpenMeridians_offsets_[4] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RetOpenMeridians, errnum_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RetOpenMeridians, newacupoint_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RetOpenMeridians, flag_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RetOpenMeridians, type_),
  };
  RetOpenMeridians_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      RetOpenMeridians_descriptor_,
      RetOpenMeridians::default_instance_,
      RetOpenMeridians_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RetOpenMeridians, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(RetOpenMeridians, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(RetOpenMeridians));
  AskOpenMeridians_descriptor_ = file->message_type(2);
  static const int AskOpenMeridians_offsets_[3] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AskOpenMeridians, charname_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AskOpenMeridians, acupoint_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AskOpenMeridians, addexp_),
  };
  AskOpenMeridians_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      AskOpenMeridians_descriptor_,
      AskOpenMeridians::default_instance_,
      AskOpenMeridians_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AskOpenMeridians, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(AskOpenMeridians, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(AskOpenMeridians));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_Meridians_2fmeridians_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    OpenMeridians_descriptor_, &OpenMeridians::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    RetOpenMeridians_descriptor_, &RetOpenMeridians::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    AskOpenMeridians_descriptor_, &AskOpenMeridians::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_Meridians_2fmeridians_2eproto() {
  delete OpenMeridians::default_instance_;
  delete OpenMeridians_reflection_;
  delete RetOpenMeridians::default_instance_;
  delete RetOpenMeridians_reflection_;
  delete AskOpenMeridians::default_instance_;
  delete AskOpenMeridians_reflection_;
}

void protobuf_AddDesc_Meridians_2fmeridians_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\031Meridians/meridians.proto\022\tMeridians\"B"
    "\n\rOpenMeridians\022\020\n\010acupoint\030\001 \002(\005\022\020\n\010isG"
    "olden\030\002 \001(\010\022\r\n\005count\030\003 \001(\005\"S\n\020RetOpenMer"
    "idians\022\016\n\006errNum\030\001 \002(\005\022\023\n\013newAcupoint\030\002 "
    "\001(\005\022\014\n\004flag\030\003 \001(\005\022\014\n\004type\030\004 \001(\005\"F\n\020AskOp"
    "enMeridians\022\020\n\010charname\030\001 \002(\t\022\020\n\010acupoin"
    "t\030\002 \002(\005\022\016\n\006addexp\030\003 \002(\005", 263);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "Meridians/meridians.proto", &protobuf_RegisterTypes);
  OpenMeridians::default_instance_ = new OpenMeridians();
  RetOpenMeridians::default_instance_ = new RetOpenMeridians();
  AskOpenMeridians::default_instance_ = new AskOpenMeridians();
  OpenMeridians::default_instance_->InitAsDefaultInstance();
  RetOpenMeridians::default_instance_->InitAsDefaultInstance();
  AskOpenMeridians::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_Meridians_2fmeridians_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_Meridians_2fmeridians_2eproto {
  StaticDescriptorInitializer_Meridians_2fmeridians_2eproto() {
    protobuf_AddDesc_Meridians_2fmeridians_2eproto();
  }
} static_descriptor_initializer_Meridians_2fmeridians_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int OpenMeridians::kAcupointFieldNumber;
const int OpenMeridians::kIsGoldenFieldNumber;
const int OpenMeridians::kCountFieldNumber;
#endif  // !_MSC_VER

OpenMeridians::OpenMeridians()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void OpenMeridians::InitAsDefaultInstance() {
}

OpenMeridians::OpenMeridians(const OpenMeridians& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void OpenMeridians::SharedCtor() {
  _cached_size_ = 0;
  acupoint_ = 0;
  isgolden_ = false;
  count_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

OpenMeridians::~OpenMeridians() {
  SharedDtor();
}

void OpenMeridians::SharedDtor() {
  if (this != default_instance_) {
  }
}

void OpenMeridians::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* OpenMeridians::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return OpenMeridians_descriptor_;
}

const OpenMeridians& OpenMeridians::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_Meridians_2fmeridians_2eproto();
  return *default_instance_;
}

OpenMeridians* OpenMeridians::default_instance_ = NULL;

OpenMeridians* OpenMeridians::New() const {
  return new OpenMeridians;
}

void OpenMeridians::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    acupoint_ = 0;
    isgolden_ = false;
    count_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool OpenMeridians::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 acupoint = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &acupoint_)));
          set_has_acupoint();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_isGolden;
        break;
      }

      // optional bool isGolden = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_isGolden:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &isgolden_)));
          set_has_isgolden();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_count;
        break;
      }

      // optional int32 count = 3;
      case 3: {
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

void OpenMeridians::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int32 acupoint = 1;
  if (has_acupoint()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->acupoint(), output);
  }

  // optional bool isGolden = 2;
  if (has_isgolden()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(2, this->isgolden(), output);
  }

  // optional int32 count = 3;
  if (has_count()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->count(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* OpenMeridians::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int32 acupoint = 1;
  if (has_acupoint()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->acupoint(), target);
  }

  // optional bool isGolden = 2;
  if (has_isgolden()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(2, this->isgolden(), target);
  }

  // optional int32 count = 3;
  if (has_count()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->count(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int OpenMeridians::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 acupoint = 1;
    if (has_acupoint()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->acupoint());
    }

    // optional bool isGolden = 2;
    if (has_isgolden()) {
      total_size += 1 + 1;
    }

    // optional int32 count = 3;
    if (has_count()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->count());
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

void OpenMeridians::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const OpenMeridians* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const OpenMeridians*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void OpenMeridians::MergeFrom(const OpenMeridians& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_acupoint()) {
      set_acupoint(from.acupoint());
    }
    if (from.has_isgolden()) {
      set_isgolden(from.isgolden());
    }
    if (from.has_count()) {
      set_count(from.count());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void OpenMeridians::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void OpenMeridians::CopyFrom(const OpenMeridians& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool OpenMeridians::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void OpenMeridians::Swap(OpenMeridians* other) {
  if (other != this) {
    std::swap(acupoint_, other->acupoint_);
    std::swap(isgolden_, other->isgolden_);
    std::swap(count_, other->count_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata OpenMeridians::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = OpenMeridians_descriptor_;
  metadata.reflection = OpenMeridians_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int RetOpenMeridians::kErrNumFieldNumber;
const int RetOpenMeridians::kNewAcupointFieldNumber;
const int RetOpenMeridians::kFlagFieldNumber;
const int RetOpenMeridians::kTypeFieldNumber;
#endif  // !_MSC_VER

RetOpenMeridians::RetOpenMeridians()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void RetOpenMeridians::InitAsDefaultInstance() {
}

RetOpenMeridians::RetOpenMeridians(const RetOpenMeridians& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void RetOpenMeridians::SharedCtor() {
  _cached_size_ = 0;
  errnum_ = 0;
  newacupoint_ = 0;
  flag_ = 0;
  type_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

RetOpenMeridians::~RetOpenMeridians() {
  SharedDtor();
}

void RetOpenMeridians::SharedDtor() {
  if (this != default_instance_) {
  }
}

void RetOpenMeridians::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* RetOpenMeridians::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return RetOpenMeridians_descriptor_;
}

const RetOpenMeridians& RetOpenMeridians::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_Meridians_2fmeridians_2eproto();
  return *default_instance_;
}

RetOpenMeridians* RetOpenMeridians::default_instance_ = NULL;

RetOpenMeridians* RetOpenMeridians::New() const {
  return new RetOpenMeridians;
}

void RetOpenMeridians::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    errnum_ = 0;
    newacupoint_ = 0;
    flag_ = 0;
    type_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool RetOpenMeridians::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 errNum = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &errnum_)));
          set_has_errnum();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_newAcupoint;
        break;
      }

      // optional int32 newAcupoint = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_newAcupoint:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &newacupoint_)));
          set_has_newacupoint();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_flag;
        break;
      }

      // optional int32 flag = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_flag:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &flag_)));
          set_has_flag();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_type;
        break;
      }

      // optional int32 type = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_type:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &type_)));
          set_has_type();
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

void RetOpenMeridians::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int32 errNum = 1;
  if (has_errnum()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->errnum(), output);
  }

  // optional int32 newAcupoint = 2;
  if (has_newacupoint()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->newacupoint(), output);
  }

  // optional int32 flag = 3;
  if (has_flag()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->flag(), output);
  }

  // optional int32 type = 4;
  if (has_type()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(4, this->type(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* RetOpenMeridians::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int32 errNum = 1;
  if (has_errnum()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->errnum(), target);
  }

  // optional int32 newAcupoint = 2;
  if (has_newacupoint()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->newacupoint(), target);
  }

  // optional int32 flag = 3;
  if (has_flag()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->flag(), target);
  }

  // optional int32 type = 4;
  if (has_type()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(4, this->type(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int RetOpenMeridians::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 errNum = 1;
    if (has_errnum()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->errnum());
    }

    // optional int32 newAcupoint = 2;
    if (has_newacupoint()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->newacupoint());
    }

    // optional int32 flag = 3;
    if (has_flag()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->flag());
    }

    // optional int32 type = 4;
    if (has_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->type());
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

void RetOpenMeridians::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const RetOpenMeridians* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const RetOpenMeridians*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void RetOpenMeridians::MergeFrom(const RetOpenMeridians& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_errnum()) {
      set_errnum(from.errnum());
    }
    if (from.has_newacupoint()) {
      set_newacupoint(from.newacupoint());
    }
    if (from.has_flag()) {
      set_flag(from.flag());
    }
    if (from.has_type()) {
      set_type(from.type());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void RetOpenMeridians::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void RetOpenMeridians::CopyFrom(const RetOpenMeridians& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool RetOpenMeridians::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void RetOpenMeridians::Swap(RetOpenMeridians* other) {
  if (other != this) {
    std::swap(errnum_, other->errnum_);
    std::swap(newacupoint_, other->newacupoint_);
    std::swap(flag_, other->flag_);
    std::swap(type_, other->type_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata RetOpenMeridians::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = RetOpenMeridians_descriptor_;
  metadata.reflection = RetOpenMeridians_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int AskOpenMeridians::kCharnameFieldNumber;
const int AskOpenMeridians::kAcupointFieldNumber;
const int AskOpenMeridians::kAddexpFieldNumber;
#endif  // !_MSC_VER

AskOpenMeridians::AskOpenMeridians()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void AskOpenMeridians::InitAsDefaultInstance() {
}

AskOpenMeridians::AskOpenMeridians(const AskOpenMeridians& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void AskOpenMeridians::SharedCtor() {
  _cached_size_ = 0;
  charname_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  acupoint_ = 0;
  addexp_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

AskOpenMeridians::~AskOpenMeridians() {
  SharedDtor();
}

void AskOpenMeridians::SharedDtor() {
  if (charname_ != &::google::protobuf::internal::kEmptyString) {
    delete charname_;
  }
  if (this != default_instance_) {
  }
}

void AskOpenMeridians::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* AskOpenMeridians::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return AskOpenMeridians_descriptor_;
}

const AskOpenMeridians& AskOpenMeridians::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_Meridians_2fmeridians_2eproto();
  return *default_instance_;
}

AskOpenMeridians* AskOpenMeridians::default_instance_ = NULL;

AskOpenMeridians* AskOpenMeridians::New() const {
  return new AskOpenMeridians;
}

void AskOpenMeridians::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_charname()) {
      if (charname_ != &::google::protobuf::internal::kEmptyString) {
        charname_->clear();
      }
    }
    acupoint_ = 0;
    addexp_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool AskOpenMeridians::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string charname = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_charname()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->charname().data(), this->charname().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_acupoint;
        break;
      }

      // required int32 acupoint = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_acupoint:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &acupoint_)));
          set_has_acupoint();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_addexp;
        break;
      }

      // required int32 addexp = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_addexp:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &addexp_)));
          set_has_addexp();
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

void AskOpenMeridians::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string charname = 1;
  if (has_charname()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->charname().data(), this->charname().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->charname(), output);
  }

  // required int32 acupoint = 2;
  if (has_acupoint()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->acupoint(), output);
  }

  // required int32 addexp = 3;
  if (has_addexp()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->addexp(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* AskOpenMeridians::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required string charname = 1;
  if (has_charname()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->charname().data(), this->charname().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->charname(), target);
  }

  // required int32 acupoint = 2;
  if (has_acupoint()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->acupoint(), target);
  }

  // required int32 addexp = 3;
  if (has_addexp()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->addexp(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int AskOpenMeridians::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string charname = 1;
    if (has_charname()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->charname());
    }

    // required int32 acupoint = 2;
    if (has_acupoint()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->acupoint());
    }

    // required int32 addexp = 3;
    if (has_addexp()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->addexp());
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

void AskOpenMeridians::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const AskOpenMeridians* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const AskOpenMeridians*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void AskOpenMeridians::MergeFrom(const AskOpenMeridians& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_charname()) {
      set_charname(from.charname());
    }
    if (from.has_acupoint()) {
      set_acupoint(from.acupoint());
    }
    if (from.has_addexp()) {
      set_addexp(from.addexp());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void AskOpenMeridians::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void AskOpenMeridians::CopyFrom(const AskOpenMeridians& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool AskOpenMeridians::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000007) != 0x00000007) return false;

  return true;
}

void AskOpenMeridians::Swap(AskOpenMeridians* other) {
  if (other != this) {
    std::swap(charname_, other->charname_);
    std::swap(acupoint_, other->acupoint_);
    std::swap(addexp_, other->addexp_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata AskOpenMeridians::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = AskOpenMeridians_descriptor_;
  metadata.reflection = AskOpenMeridians_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace Meridians

// @@protoc_insertion_point(global_scope)