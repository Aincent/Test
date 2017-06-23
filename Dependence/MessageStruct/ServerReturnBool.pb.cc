// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ServerReturnBool.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "ServerReturnBool.pb.h"

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

namespace ServerReturn {

namespace {

const ::google::protobuf::Descriptor* ServerRetBool_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ServerRetBool_reflection_ = NULL;
const ::google::protobuf::Descriptor* ServerRetBoolInt_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ServerRetBoolInt_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_ServerReturnBool_2eproto() {
  protobuf_AddDesc_ServerReturnBool_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "ServerReturnBool.proto");
  GOOGLE_CHECK(file != NULL);
  ServerRetBool_descriptor_ = file->message_type(0);
  static const int ServerRetBool_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ServerRetBool, ret_),
  };
  ServerRetBool_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ServerRetBool_descriptor_,
      ServerRetBool::default_instance_,
      ServerRetBool_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ServerRetBool, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ServerRetBool, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ServerRetBool));
  ServerRetBoolInt_descriptor_ = file->message_type(1);
  static const int ServerRetBoolInt_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ServerRetBoolInt, retf_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ServerRetBoolInt, rets_),
  };
  ServerRetBoolInt_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ServerRetBoolInt_descriptor_,
      ServerRetBoolInt::default_instance_,
      ServerRetBoolInt_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ServerRetBoolInt, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ServerRetBoolInt, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ServerRetBoolInt));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_ServerReturnBool_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ServerRetBool_descriptor_, &ServerRetBool::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ServerRetBoolInt_descriptor_, &ServerRetBoolInt::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_ServerReturnBool_2eproto() {
  delete ServerRetBool::default_instance_;
  delete ServerRetBool_reflection_;
  delete ServerRetBoolInt::default_instance_;
  delete ServerRetBoolInt_reflection_;
}

void protobuf_AddDesc_ServerReturnBool_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\026ServerReturnBool.proto\022\014ServerReturn\"\034"
    "\n\rServerRetBool\022\013\n\003ret\030\001 \002(\010\".\n\020ServerRe"
    "tBoolInt\022\014\n\004retf\030\001 \002(\010\022\014\n\004rets\030\002 \002(\005", 116);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "ServerReturnBool.proto", &protobuf_RegisterTypes);
  ServerRetBool::default_instance_ = new ServerRetBool();
  ServerRetBoolInt::default_instance_ = new ServerRetBoolInt();
  ServerRetBool::default_instance_->InitAsDefaultInstance();
  ServerRetBoolInt::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_ServerReturnBool_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_ServerReturnBool_2eproto {
  StaticDescriptorInitializer_ServerReturnBool_2eproto() {
    protobuf_AddDesc_ServerReturnBool_2eproto();
  }
} static_descriptor_initializer_ServerReturnBool_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int ServerRetBool::kRetFieldNumber;
#endif  // !_MSC_VER

ServerRetBool::ServerRetBool()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void ServerRetBool::InitAsDefaultInstance() {
}

ServerRetBool::ServerRetBool(const ServerRetBool& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void ServerRetBool::SharedCtor() {
  _cached_size_ = 0;
  ret_ = false;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ServerRetBool::~ServerRetBool() {
  SharedDtor();
}

void ServerRetBool::SharedDtor() {
  if (this != default_instance_) {
  }
}

void ServerRetBool::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ServerRetBool::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ServerRetBool_descriptor_;
}

const ServerRetBool& ServerRetBool::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_ServerReturnBool_2eproto();
  return *default_instance_;
}

ServerRetBool* ServerRetBool::default_instance_ = NULL;

ServerRetBool* ServerRetBool::New() const {
  return new ServerRetBool;
}

void ServerRetBool::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    ret_ = false;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ServerRetBool::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required bool ret = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &ret_)));
          set_has_ret();
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

void ServerRetBool::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required bool ret = 1;
  if (has_ret()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(1, this->ret(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* ServerRetBool::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required bool ret = 1;
  if (has_ret()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(1, this->ret(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int ServerRetBool::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required bool ret = 1;
    if (has_ret()) {
      total_size += 1 + 1;
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

void ServerRetBool::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ServerRetBool* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ServerRetBool*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ServerRetBool::MergeFrom(const ServerRetBool& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_ret()) {
      set_ret(from.ret());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ServerRetBool::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ServerRetBool::CopyFrom(const ServerRetBool& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ServerRetBool::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void ServerRetBool::Swap(ServerRetBool* other) {
  if (other != this) {
    std::swap(ret_, other->ret_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ServerRetBool::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ServerRetBool_descriptor_;
  metadata.reflection = ServerRetBool_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int ServerRetBoolInt::kRetfFieldNumber;
const int ServerRetBoolInt::kRetsFieldNumber;
#endif  // !_MSC_VER

ServerRetBoolInt::ServerRetBoolInt()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void ServerRetBoolInt::InitAsDefaultInstance() {
}

ServerRetBoolInt::ServerRetBoolInt(const ServerRetBoolInt& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void ServerRetBoolInt::SharedCtor() {
  _cached_size_ = 0;
  retf_ = false;
  rets_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ServerRetBoolInt::~ServerRetBoolInt() {
  SharedDtor();
}

void ServerRetBoolInt::SharedDtor() {
  if (this != default_instance_) {
  }
}

void ServerRetBoolInt::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ServerRetBoolInt::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ServerRetBoolInt_descriptor_;
}

const ServerRetBoolInt& ServerRetBoolInt::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_ServerReturnBool_2eproto();
  return *default_instance_;
}

ServerRetBoolInt* ServerRetBoolInt::default_instance_ = NULL;

ServerRetBoolInt* ServerRetBoolInt::New() const {
  return new ServerRetBoolInt;
}

void ServerRetBoolInt::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    retf_ = false;
    rets_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ServerRetBoolInt::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required bool retf = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &retf_)));
          set_has_retf();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_rets;
        break;
      }

      // required int32 rets = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_rets:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &rets_)));
          set_has_rets();
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

void ServerRetBoolInt::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required bool retf = 1;
  if (has_retf()) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(1, this->retf(), output);
  }

  // required int32 rets = 2;
  if (has_rets()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->rets(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* ServerRetBoolInt::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required bool retf = 1;
  if (has_retf()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(1, this->retf(), target);
  }

  // required int32 rets = 2;
  if (has_rets()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->rets(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int ServerRetBoolInt::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required bool retf = 1;
    if (has_retf()) {
      total_size += 1 + 1;
    }

    // required int32 rets = 2;
    if (has_rets()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->rets());
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

void ServerRetBoolInt::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ServerRetBoolInt* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ServerRetBoolInt*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ServerRetBoolInt::MergeFrom(const ServerRetBoolInt& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_retf()) {
      set_retf(from.retf());
    }
    if (from.has_rets()) {
      set_rets(from.rets());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ServerRetBoolInt::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ServerRetBoolInt::CopyFrom(const ServerRetBoolInt& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ServerRetBoolInt::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void ServerRetBoolInt::Swap(ServerRetBoolInt* other) {
  if (other != this) {
    std::swap(retf_, other->retf_);
    std::swap(rets_, other->rets_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ServerRetBoolInt::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ServerRetBoolInt_descriptor_;
  metadata.reflection = ServerRetBoolInt_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace ServerReturn

// @@protoc_insertion_point(global_scope)
