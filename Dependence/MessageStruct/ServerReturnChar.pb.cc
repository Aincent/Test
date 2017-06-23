// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: ServerReturnChar.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "ServerReturnChar.pb.h"

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

const ::google::protobuf::Descriptor* ServerRetChar_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  ServerRetChar_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_ServerReturnChar_2eproto() {
  protobuf_AddDesc_ServerReturnChar_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "ServerReturnChar.proto");
  GOOGLE_CHECK(file != NULL);
  ServerRetChar_descriptor_ = file->message_type(0);
  static const int ServerRetChar_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ServerRetChar, ret_),
  };
  ServerRetChar_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      ServerRetChar_descriptor_,
      ServerRetChar::default_instance_,
      ServerRetChar_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ServerRetChar, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(ServerRetChar, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(ServerRetChar));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_ServerReturnChar_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    ServerRetChar_descriptor_, &ServerRetChar::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_ServerReturnChar_2eproto() {
  delete ServerRetChar::default_instance_;
  delete ServerRetChar_reflection_;
}

void protobuf_AddDesc_ServerReturnChar_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\026ServerReturnChar.proto\022\014ServerReturn\"\034"
    "\n\rServerRetChar\022\013\n\003ret\030\001 \002(\t", 68);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "ServerReturnChar.proto", &protobuf_RegisterTypes);
  ServerRetChar::default_instance_ = new ServerRetChar();
  ServerRetChar::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_ServerReturnChar_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_ServerReturnChar_2eproto {
  StaticDescriptorInitializer_ServerReturnChar_2eproto() {
    protobuf_AddDesc_ServerReturnChar_2eproto();
  }
} static_descriptor_initializer_ServerReturnChar_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int ServerRetChar::kRetFieldNumber;
#endif  // !_MSC_VER

ServerRetChar::ServerRetChar()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void ServerRetChar::InitAsDefaultInstance() {
}

ServerRetChar::ServerRetChar(const ServerRetChar& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void ServerRetChar::SharedCtor() {
  _cached_size_ = 0;
  ret_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

ServerRetChar::~ServerRetChar() {
  SharedDtor();
}

void ServerRetChar::SharedDtor() {
  if (ret_ != &::google::protobuf::internal::kEmptyString) {
    delete ret_;
  }
  if (this != default_instance_) {
  }
}

void ServerRetChar::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* ServerRetChar::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return ServerRetChar_descriptor_;
}

const ServerRetChar& ServerRetChar::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_ServerReturnChar_2eproto();
  return *default_instance_;
}

ServerRetChar* ServerRetChar::default_instance_ = NULL;

ServerRetChar* ServerRetChar::New() const {
  return new ServerRetChar;
}

void ServerRetChar::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (has_ret()) {
      if (ret_ != &::google::protobuf::internal::kEmptyString) {
        ret_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool ServerRetChar::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string ret = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_ret()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->ret().data(), this->ret().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
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

void ServerRetChar::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string ret = 1;
  if (has_ret()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->ret().data(), this->ret().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->ret(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* ServerRetChar::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required string ret = 1;
  if (has_ret()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->ret().data(), this->ret().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->ret(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int ServerRetChar::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string ret = 1;
    if (has_ret()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->ret());
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

void ServerRetChar::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const ServerRetChar* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const ServerRetChar*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void ServerRetChar::MergeFrom(const ServerRetChar& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_ret()) {
      set_ret(from.ret());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void ServerRetChar::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void ServerRetChar::CopyFrom(const ServerRetChar& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool ServerRetChar::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void ServerRetChar::Swap(ServerRetChar* other) {
  if (other != this) {
    std::swap(ret_, other->ret_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata ServerRetChar::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = ServerRetChar_descriptor_;
  metadata.reflection = ServerRetChar_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace ServerReturn

// @@protoc_insertion_point(global_scope)
