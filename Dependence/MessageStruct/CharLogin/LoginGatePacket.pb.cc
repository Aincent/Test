// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: CharLogin/LoginGatePacket.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "CharLogin/LoginGatePacket.pb.h"

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

namespace ClientLogin {

namespace {

const ::google::protobuf::Descriptor* LoginGateInfo_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  LoginGateInfo_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_CharLogin_2fLoginGatePacket_2eproto() {
  protobuf_AddDesc_CharLogin_2fLoginGatePacket_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "CharLogin/LoginGatePacket.proto");
  GOOGLE_CHECK(file != NULL);
  LoginGateInfo_descriptor_ = file->message_type(0);
  static const int LoginGateInfo_offsets_[4] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LoginGateInfo, charid_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LoginGateInfo, ip_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LoginGateInfo, port_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LoginGateInfo, gateid_),
  };
  LoginGateInfo_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      LoginGateInfo_descriptor_,
      LoginGateInfo::default_instance_,
      LoginGateInfo_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LoginGateInfo, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(LoginGateInfo, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(LoginGateInfo));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_CharLogin_2fLoginGatePacket_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    LoginGateInfo_descriptor_, &LoginGateInfo::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_CharLogin_2fLoginGatePacket_2eproto() {
  delete LoginGateInfo::default_instance_;
  delete LoginGateInfo_reflection_;
}

void protobuf_AddDesc_CharLogin_2fLoginGatePacket_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\037CharLogin/LoginGatePacket.proto\022\013Clien"
    "tLogin\"I\n\rLoginGateInfo\022\016\n\006charID\030\001 \002(\003\022"
    "\n\n\002IP\030\002 \001(\t\022\014\n\004port\030\003 \001(\005\022\016\n\006gateID\030\004 \001("
    "\005", 121);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "CharLogin/LoginGatePacket.proto", &protobuf_RegisterTypes);
  LoginGateInfo::default_instance_ = new LoginGateInfo();
  LoginGateInfo::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_CharLogin_2fLoginGatePacket_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_CharLogin_2fLoginGatePacket_2eproto {
  StaticDescriptorInitializer_CharLogin_2fLoginGatePacket_2eproto() {
    protobuf_AddDesc_CharLogin_2fLoginGatePacket_2eproto();
  }
} static_descriptor_initializer_CharLogin_2fLoginGatePacket_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int LoginGateInfo::kCharIDFieldNumber;
const int LoginGateInfo::kIPFieldNumber;
const int LoginGateInfo::kPortFieldNumber;
const int LoginGateInfo::kGateIDFieldNumber;
#endif  // !_MSC_VER

LoginGateInfo::LoginGateInfo()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void LoginGateInfo::InitAsDefaultInstance() {
}

LoginGateInfo::LoginGateInfo(const LoginGateInfo& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void LoginGateInfo::SharedCtor() {
  _cached_size_ = 0;
  charid_ = GOOGLE_LONGLONG(0);
  ip_ = const_cast< ::std::string*>(&::google::protobuf::internal::kEmptyString);
  port_ = 0;
  gateid_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

LoginGateInfo::~LoginGateInfo() {
  SharedDtor();
}

void LoginGateInfo::SharedDtor() {
  if (ip_ != &::google::protobuf::internal::kEmptyString) {
    delete ip_;
  }
  if (this != default_instance_) {
  }
}

void LoginGateInfo::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* LoginGateInfo::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return LoginGateInfo_descriptor_;
}

const LoginGateInfo& LoginGateInfo::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_CharLogin_2fLoginGatePacket_2eproto();
  return *default_instance_;
}

LoginGateInfo* LoginGateInfo::default_instance_ = NULL;

LoginGateInfo* LoginGateInfo::New() const {
  return new LoginGateInfo;
}

void LoginGateInfo::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    charid_ = GOOGLE_LONGLONG(0);
    if (has_ip()) {
      if (ip_ != &::google::protobuf::internal::kEmptyString) {
        ip_->clear();
      }
    }
    port_ = 0;
    gateid_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool LoginGateInfo::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int64 charID = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &charid_)));
          set_has_charid();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_IP;
        break;
      }

      // optional string IP = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_IP:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_ip()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->ip().data(), this->ip().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_port;
        break;
      }

      // optional int32 port = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_port:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &port_)));
          set_has_port();
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_gateID;
        break;
      }

      // optional int32 gateID = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_gateID:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &gateid_)));
          set_has_gateid();
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

void LoginGateInfo::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int64 charID = 1;
  if (has_charid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(1, this->charid(), output);
  }

  // optional string IP = 2;
  if (has_ip()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->ip().data(), this->ip().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->ip(), output);
  }

  // optional int32 port = 3;
  if (has_port()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->port(), output);
  }

  // optional int32 gateID = 4;
  if (has_gateid()) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(4, this->gateid(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* LoginGateInfo::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int64 charID = 1;
  if (has_charid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(1, this->charid(), target);
  }

  // optional string IP = 2;
  if (has_ip()) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->ip().data(), this->ip().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        2, this->ip(), target);
  }

  // optional int32 port = 3;
  if (has_port()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->port(), target);
  }

  // optional int32 gateID = 4;
  if (has_gateid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(4, this->gateid(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int LoginGateInfo::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int64 charID = 1;
    if (has_charid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->charid());
    }

    // optional string IP = 2;
    if (has_ip()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->ip());
    }

    // optional int32 port = 3;
    if (has_port()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->port());
    }

    // optional int32 gateID = 4;
    if (has_gateid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->gateid());
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

void LoginGateInfo::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const LoginGateInfo* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const LoginGateInfo*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void LoginGateInfo::MergeFrom(const LoginGateInfo& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_charid()) {
      set_charid(from.charid());
    }
    if (from.has_ip()) {
      set_ip(from.ip());
    }
    if (from.has_port()) {
      set_port(from.port());
    }
    if (from.has_gateid()) {
      set_gateid(from.gateid());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void LoginGateInfo::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void LoginGateInfo::CopyFrom(const LoginGateInfo& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool LoginGateInfo::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  return true;
}

void LoginGateInfo::Swap(LoginGateInfo* other) {
  if (other != this) {
    std::swap(charid_, other->charid_);
    std::swap(ip_, other->ip_);
    std::swap(port_, other->port_);
    std::swap(gateid_, other->gateid_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata LoginGateInfo::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = LoginGateInfo_descriptor_;
  metadata.reflection = LoginGateInfo_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace ClientLogin

// @@protoc_insertion_point(global_scope)